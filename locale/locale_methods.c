/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Kirti Velankar <kirtig@yahoo-inc.com>  			  		  |
   +----------------------------------------------------------------------+
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unicode/ustring.h>
#include <unicode/udata.h>
#include <unicode/putil.h>

#include "php_intl.h"
#include "locale.h"
#include "locale_class.h"
#include "locale_methods.h"
#include "intl_convert.h"

#include <zend_API.h>
#include <zend.h>
#include <php.h>

ZEND_EXTERN_MODULE_GLOBALS( intl )


//Sizes required for the strings "variant15" , "extlang11", "private12" etc.
static const char* 	SEPARATOR= "_";
static const char* 	SEPARATOR1= "-";
static const char* 	DELIMITER= "-_";
static const char* 	EXTLANG_PREFIX= "a";
static const char* 	PRIVATE_PREFIX= "x";
static const char* 	DISP_NAME= "name";

static const int 	MAX_NO_VARIANT = 15;
static const int 	MAX_NO_EXTLANG = 3;
static const int 	MAX_NO_PRIVATE = 15;
static const int 	MAX_NO_LOOKUP_LANG_TAG = 100;

//Sizes required for the strings "variant15" , "extlang3", "private12" etc.
static const int 	VARIANT_KEYNAME_LEN = 11;
static const int 	EXTLANG_KEYNAME_LEN = 10;
static const int 	PRIVATE_KEYNAME_LEN = 11;

/* Based on IANA registry at the time of writing this code
*
*/
static const char * const LOC_GRANDFATHERED[] = {
	"art-lojban",	"i-klingon",	"i-lux",		"i-navajo",	
	"no-bok",		"no-nyn",
	"cel-gaulish",	"en-GB-oed",	"i-ami", 		"i-bnn",	
	"i-default",	"i-enochian",	"i-mingo",		"i-pwn",
	"i-tao", 		"i-tay",		"i-tsu",		"sgn-BE-fr",	
	"sgn-BE-nl",	"sgn-CH-de", 	"zh-cmn", 		"zh-cmn-Hans",
	"zh-cmn-Hant",	"zh-gan" ,		"zh-guoyu", 	"zh-hakka",	
	"zh-min",		"zh-min-nan", 	"zh-wuu", 		"zh-xiang",	
	"zh-yue"
};

/* Based on IANA registry at the time of writing this code
*  This array lists the preferred values for the grandfathered tags if applicable
*  This is in sync with the array LOC_GRANDFATHERED	 
*  e.g. the offsets of the grandfathered tags match the offset of the preferred  value
*/
static const int 	LOC_PREFERRED_GRANDFATHERED_LEN = 6;
static const char * const LOC_PREFERRED_GRANDFATHERED[]  = {
	"jbo",			"tlh",			"lb",			"nv",
	"nb",			"nn"
};

/*returns TRUE if a is an ID separator FALSE otherwise*/
#define isIDSeparator(a) (a == '_' || a == '-')
#define isKeywordSeparator(a) (a == '@' )
#define isEndOfTag(a) (a == '\0' )

#define isPrefixLetter(a) ((a=='x')||(a=='X')||(a=='i')||(a=='I'))

/*returns TRUE if one of the special prefixes is here (s=string)
  'x-' or 'i-' */
#define isIDPrefix(s) (isPrefixLetter(s[0])&&isIDSeparator(s[1]))
#define isKeywordPrefix(s) ( isKeywordSeparator(s[0]) )

/* Dot terminates it because of POSIX form  where dot precedes the codepage
 * except for variant
 */
#define isTerminator(a)  ((a==0)||(a=='.')||(a=='@'))


/*{{{  return the offset of 'key' in the array 'list'.  
 * returns -1 if not present
 */
static int16_t findOffset(const char* const* list, const char* key)
{
    const char* const* anchor = list;

	while (*list) {
		if (strcmp(key, *list) == 0) {
			return (int16_t)(list - anchor);
		}
		list++;
	}

    return -1;
}
/*}}}*/

/*{{{ returns preferred replacement if applicable
 */
static const char* 
getCorrectGrandfatheredLang(const char* oldID ){
	int32_t offset = findOffset(LOC_GRANDFATHERED, oldID);
    if (offset >= 0  && offset < LOC_PREFERRED_GRANDFATHERED_LEN ) {
        return (LOC_PREFERRED_GRANDFATHERED[offset]);
    }
    return oldID;
}

/*}}}*/

/* {{{ proto static string Locale::getDefault(  )
 * Gets the default locale 
 }}} */
/* {{{ proto static string locale_get_default( )
 * Gets the default locale 
 */
PHP_NAMED_FUNCTION( zif_locale_get_default){
	char*  		locale_name = NULL;

	if(UG(default_locale) && (strcmp("", UG(default_locale))!=0) ) {
		locale_name = estrdup( UG (default_locale) ); 
	}else{
		locale_name = (char*) uloc_getDefault();
		UG(default_locale) = estrdup( locale_name);
	}

	RETVAL_STRINGL( locale_name, strlen(locale_name), TRUE );
}

/* }}} */

/* {{{ proto static string Locale::setDefault( string $locale )
* sets the default locale
/*}}} */
/* {{{ proto static string locale_set_default( string $locale )
* sets the default locale
*/
PHP_NAMED_FUNCTION(  zif_locale_set_default){
	char* locale_name = NULL;
	int   len=0;	

	// Parse parameters.
	if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC,  "s",
		&locale_name ,&len ) == FAILURE)
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			 "locale_set_default: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Set new value for the given attribute.
	UG( default_locale) = estrndup( locale_name , len );

	RETVAL_STRINGL( UG( default_locale), len , TRUE );
}
/* }}} */

/* {{{
* Gets the value from ICU 
* common code shared by get_primary_language,get_script or get_region or get_variant
* result = 0 if error, 1 if successful , -1 if no value
*/
static char* get_icu_value_internal( char* loc_name , char* tag_name, int* result)
{
    char*       tag_value		= NULL;
    int32_t     tag_value_len   = 0;
    int			singletonPos   	= 0;
    char*       mod_loc_name	= NULL;
	int			grOffset		= 0;

    int32_t     buflen          = 512;
    UErrorCode  status          = U_ZERO_ERROR;

	 mod_loc_name = estrndup(loc_name , strlen(loc_name));	

	if( tag_name != LOC_CANONICALIZE_TAG ){
		//Handle  grandfathered languages
			grOffset =  findOffset( LOC_GRANDFATHERED , mod_loc_name );
			if( grOffset >= 0 ){
				if( tag_name == LOC_LANG_TAG ){
					tag_value = estrdup( LOC_GRANDFATHERED[grOffset] );	
					return tag_value;
				}else{
					//Since Grandfathered , no value , do nothing , retutn NULL
					return tag_value;
				}
			}

		//Handle singletons 
			if( tag_name == LOC_LANG_TAG  && isIDPrefix(mod_loc_name) ){
					return mod_loc_name;
			}

			singletonPos = getSingletonPos( mod_loc_name );	
			if( singletonPos == 0){
					//singleton at start of script, region , variant etc.
					//or invalid singleton at start of language
					//return NULL
					return tag_value;
			}else{
				//singleton at some position except at start
				//strip off the singleton and rest of the loc_name
				*(mod_loc_name+singletonPos-1)='\0';
				
			}
	}//end of if != LOC_CANONICAL_TAG

//Proceed to ICU
    do{
        tag_value = erealloc( tag_value , buflen  );
        tag_value_len = buflen;

		if( tag_name == LOC_SCRIPT_TAG ){
			buflen = uloc_getScript ( mod_loc_name ,tag_value , tag_value_len , &status);
		}
		if( tag_name == LOC_LANG_TAG ){
			buflen = uloc_getLanguage ( mod_loc_name ,tag_value , tag_value_len , &status);
		}
		if( tag_name == LOC_REGION_TAG ){
			buflen = uloc_getCountry ( mod_loc_name ,tag_value , tag_value_len , &status);
		}
		if( tag_name == LOC_VARIANT_TAG ){
			buflen = uloc_getVariant ( mod_loc_name ,tag_value , tag_value_len , &status);
		}
		if( tag_name == LOC_CANONICALIZE_TAG ){
			buflen = uloc_canonicalize ( mod_loc_name ,tag_value , tag_value_len , &status);
		}

		if( U_FAILURE( status ) )
		{
			if( status == U_BUFFER_OVERFLOW_ERROR )
			{
				status = U_ZERO_ERROR;
        		tag_value = erealloc( tag_value , buflen  );
				continue;
			}

			//Error in retriving data
			*result = 0;
			efree( tag_value );
			return NULL;
		}

    } while( buflen > tag_value_len );

	if(  buflen ==0 ){
		//No value found
		*result = -1;
		if( tag_value ){
			efree( tag_value );
		}
		if( mod_loc_name ){
			efree( mod_loc_name);
		}
		return NULL;
	}else{
		*result = 1;
	}

	if( mod_loc_name ){
		efree( mod_loc_name);
	}
	return tag_value;
}
/* }}} */

/* {{{
* Gets the value from ICU , called when PHP  userspace function is called
* common code shared by get_primary_language,get_script or get_region or get_variant
*/
static void get_icu_value_src_php( char* tag_name, INTERNAL_FUNCTION_PARAMETERS) {

    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    char*       tag_value		= NULL;
    int         singletonPos    = 0;

    int32_t     buflen          = 512;
    UErrorCode  status          = U_ZERO_ERROR;

    int*        result    		= 0;
    char*       msg        		= NULL;
    int         msg_len    		= 50;


    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s",
        &loc_name ,&loc_name_len ) == FAILURE)
    {
		msg = erealloc( msg , msg_len);
		sprintf(msg , "locale_get_%s : unable to parse input params", tag_name );
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,  msg , 0 TSRMLS_CC );

        RETURN_FALSE;
    }

	if(loc_name_len == 0) {
		loc_name = UG(default_locale);
	}

	//Call ICU get
	tag_value = get_icu_value_internal( loc_name , tag_name , &result);

	if( tag_value){
		RETURN_STRINGL( tag_value , strlen(tag_value) , TRUE);
	}

	if( result ==0) {
		msg = erealloc( msg , msg_len);
		sprintf(msg , "locale_get_%s : unable to get locale %s", tag_name , tag_name );
		intl_error_set( NULL, status, msg , 0 TSRMLS_CC );
		efree(msg);
		RETURN_FALSE;
	}

}
/* }}} */

/* {{{
 * proto public static string Locale::getScript($locale) 
 * gets the script for the $locale 
 }}} */
/* {{{
 * proto public static string locale_get_script($locale) 
 * gets the script for the $locale 
 */
PHP_FUNCTION( locale_get_script ) {
	get_icu_value_src_php( LOC_SCRIPT_TAG , INTERNAL_FUNCTION_PARAM_PASSTHRU );
}
/* }}} */

/* {{{
 * proto public static string Locale::getRegion($locale) 
 * gets the region for the $locale 
 }}} */
/* {{{
 * proto public static string locale_get_region($locale) 
 * gets the region for the $locale 
 */
PHP_FUNCTION( locale_get_region ) {
	get_icu_value_src_php( LOC_REGION_TAG , INTERNAL_FUNCTION_PARAM_PASSTHRU );
}
/* }}} */

/* {{{
 * proto public static string Locale::getPrimaryLanguage($locale) 
 * gets the primary language for the $locale 
 }}} */
/* {{{
 * proto public static string locale_get_primary_language($locale) 
 * gets the primary language for the $locale 
 */
PHP_FUNCTION(locale_get_primary_language ) {
	get_icu_value_src_php( LOC_LANG_TAG , INTERNAL_FUNCTION_PARAM_PASSTHRU );
}
/* }}} */


/* {{{
 * common code shared by display_xyz functions to  get the value from ICU 
 }}} */
static void get_icu_disp_value_src_php( char* tag_name, INTERNAL_FUNCTION_PARAMETERS) {
    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    char*       disp_loc_name        = NULL;
    int         disp_loc_name_len    = 0;

    UChar*      disp_name      	= NULL;
    int32_t     disp_name_len  	= 0;

    char*       mod_loc_name        = NULL;
    int         mod_loc_name_len    = 0;

    int32_t     buflen          = 512;
    UErrorCode  status          = U_ZERO_ERROR;

	char*       utf8value		= NULL;
	int         utf8value_len   = 0;

  	char*       msg             = NULL;
    int         msg_len         = 50;
    int         singletonPos    = 0;
    int         grOffset    	= 0;

    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s|s",
        &loc_name, &loc_name_len , &disp_loc_name ,&disp_loc_name_len ) == FAILURE)
    {
        msg = erealloc( msg , msg_len);
        sprintf(msg , "locale_get_display_%s : unable to parse input params", tag_name );
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,  msg , 0 TSRMLS_CC );
        RETURN_FALSE;
    }

    if(loc_name_len == 0) {
        loc_name = UG(default_locale);
    }

	mod_loc_name = estrndup( loc_name , loc_name_len );
	if( tag_name != DISP_NAME ){
		//Handle  grandfathered languages
			grOffset =  findOffset( LOC_GRANDFATHERED , mod_loc_name );
			if( grOffset >= 0 ){
				if( tag_name == LOC_LANG_TAG ){
					//Return correct grandfathered language tag
					mod_loc_name = estrdup( LOC_GRANDFATHERED[grOffset] );	
				}else{
					//Since Grandfathered , no value , do nothing , retutn NULL
					RETURN_FALSE;
				}
			}

		//Handle singletons 
			if( tag_name == LOC_LANG_TAG  && isIDPrefix(mod_loc_name) ){
					//return mod_loc_name;
			}else{
				singletonPos = getSingletonPos( mod_loc_name );	
				if( singletonPos == 0){
						//singleton at start of script, region , variant etc.
						//or invalid singleton at start of language
						RETURN_FALSE;
				}else{
					//singleton at some position except at start
					//strip off the singleton and rest of the loc_name
					*(mod_loc_name+singletonPos-1)='\0';
				}
			}
	}//end of if != LOC_CANONICAL_TAG

    //Get the disp_value for the given locale
    do{
        disp_name = erealloc( disp_name , buflen  );
        disp_name_len = buflen;

        //Check if disp_loc_name passed , if not use default locale
        if( !disp_loc_name){
            disp_loc_name = UG(default_locale);
        }

		if( tag_name ==  LOC_LANG_TAG ){
			buflen = uloc_getDisplayLanguage ( mod_loc_name , disp_loc_name , disp_name , disp_name_len , &status);
		}
		if( tag_name ==  LOC_SCRIPT_TAG ){
			buflen = uloc_getDisplayScript ( mod_loc_name , disp_loc_name , disp_name , disp_name_len , &status);
		}
		if( tag_name ==  LOC_REGION_TAG ){
			buflen = uloc_getDisplayCountry ( mod_loc_name , disp_loc_name , disp_name , disp_name_len , &status);
		}
		if( tag_name ==  LOC_VARIANT_TAG ){
			buflen = uloc_getDisplayVariant ( mod_loc_name , disp_loc_name , disp_name , disp_name_len , &status);
		}
		if( tag_name ==  DISP_NAME ){
			buflen = uloc_getDisplayName ( mod_loc_name , disp_loc_name , disp_name , disp_name_len , &status);
		}

        if( U_FAILURE( status ) )
        {
            if( status == U_BUFFER_OVERFLOW_ERROR )
            {
                status = U_ZERO_ERROR;
                continue;
            }

			msg = erealloc( msg , msg_len);
			sprintf(msg , "locale_get_display_%s : unable to get locale %s", tag_name , tag_name );
			intl_error_set( NULL, status, msg , 0 TSRMLS_CC );
			efree(msg);
            efree( disp_name );
            RETURN_FALSE;
		}
    } while( buflen > disp_name_len );

    // Convert display locale name from UTF-16 to UTF-8.
    intl_convert_utf16_to_utf8( &utf8value, &utf8value_len, disp_name, buflen, &status );
    efree( disp_name );
    if( U_FAILURE( status ) )
    {
			msg = erealloc( msg , msg_len);
			sprintf(msg , "locale_get_display_%s :error converting display name for %s to UTF-8", tag_name , tag_name );
			intl_error_set( NULL, status, msg , 0 TSRMLS_CC );
			efree(msg);
        RETURN_FALSE;
    }

    RETVAL_STRINGL( utf8value, utf8value_len , FALSE);

}
/* }}} */

/* {{{
* public static string Locale::getDisplayName($locale, $in_locale = null)
* gets the name for the $locale in $in_locale or default_locale
 }}} */
/* {{{
* public static string get_display_name($locale, $in_locale = null)
* gets the name for the $locale in $in_locale or default_locale
*/
PHP_FUNCTION(locale_get_display_name) {
    get_icu_disp_value_src_php( DISP_NAME , INTERNAL_FUNCTION_PARAM_PASSTHRU );
}
/* }}} */

/* {{{
* public static string Locale::getDisplayLanguage($locale, $in_locale = null)
* gets the language for the $locale in $in_locale or default_locale
 }}} */
/* {{{
* public static string get_display_language($locale, $in_locale = null)
* gets the language for the $locale in $in_locale or default_locale
*/
PHP_FUNCTION(locale_get_display_language) {
    get_icu_disp_value_src_php( LOC_LANG_TAG , INTERNAL_FUNCTION_PARAM_PASSTHRU );
}
/* }}} */

/* {{{
* public static string Locale::getDisplayScript($locale, $in_locale = null)
* gets the script for the $locale in $in_locale or default_locale
 }}} */
/* {{{
* public static string get_display_script($locale, $in_locale = null)
* gets the script for the $locale in $in_locale or default_locale
*/
PHP_FUNCTION(locale_get_display_script) {
    get_icu_disp_value_src_php( LOC_SCRIPT_TAG , INTERNAL_FUNCTION_PARAM_PASSTHRU );
}
/* }}} */

/* {{{
* public static string Locale::getDisplayRegion($locale, $in_locale = null)
* gets the region for the $locale in $in_locale or default_locale
 }}} */
/* {{{
* public static string get_display_region($locale, $in_locale = null)
* gets the region for the $locale in $in_locale or default_locale
*/
PHP_FUNCTION(locale_get_display_region) {
    get_icu_disp_value_src_php( LOC_REGION_TAG , INTERNAL_FUNCTION_PARAM_PASSTHRU );
}
/* }}} */

/* {{{
* public static string Locale::getDisplayVariant($locale, $in_locale = null)
* gets the variant for the $locale in $in_locale or default_locale
 }}} */
/* {{{
* public static string get_display_variant($locale, $in_locale = null)
* gets the variant for the $locale in $in_locale or default_locale
*/
PHP_FUNCTION(locale_get_display_variant) {
    get_icu_disp_value_src_php( LOC_VARIANT_TAG , INTERNAL_FUNCTION_PARAM_PASSTHRU );
}
/* }}} */

     /* {{{
     * proto static string[] getKeywords(string $locale) {
     * return an associative array containing keyword-value
     * pairs for this locale. The keys are keys to the array (doh!)
     * }}}*/
     /* {{{
     * proto static string[] locale_get_keywords(string $locale) {
     * return an associative array containing keyword-value
     * pairs for this locale. The keys are keys to the array (doh!)
     */ 

PHP_FUNCTION( locale_get_keywords )
{
    UEnumeration*     e        = NULL;
    UErrorCode        status   = U_ZERO_ERROR;

	const char*	 	kw_key        = NULL;
    int32_t         kw_key_len    = 0;

    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

/* 
	ICU expects the buffer to be allocated  before calling the function 
	and so the buffer size has been explicitly specified 
	ICU uloc.h #define 	ULOC_KEYWORD_AND_VALUES_CAPACITY   100 
	hence the kw_value buffer size is 100
*/
    //char        kw_value[100];
	char*	 	kw_value        = NULL;
    int32_t     kw_value_len    = 100;

    int32_t     buflen          = 512;

    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s",
        &loc_name, &loc_name_len ) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_get_keywords: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    if(loc_name_len == 0) {
        loc_name = UG(default_locale);
    }

	//Get the keywords
    e = uloc_openKeywords( loc_name, &status );
    if( e != NULL )
    {
		// Traverse it, filling the return array.
    	array_init( return_value );

        kw_value = erealloc( kw_value , kw_value_len  );

    	while( ( kw_key = uenum_next( e, &kw_key_len, &status ) ) != NULL ){

			//Get the keyword value for each keyword
			kw_value_len=uloc_getKeywordValue( loc_name,kw_key, kw_value, kw_value_len ,  &status );
			if (status == U_BUFFER_OVERFLOW_ERROR) {
				status = U_ZERO_ERROR;
				kw_value = erealloc( kw_value , kw_value_len);
				kw_value_len=uloc_getKeywordValue( loc_name,kw_key, kw_value, kw_value_len ,  &status );
			}
			if (status == FAILURE) {
        		intl_error_set( NULL, FAILURE, "locale_get_keywords: Error encountered while getting the keyword  value for the  keyword", 0 TSRMLS_CC );
        		RETURN_FALSE;
			}

			//Add the obtained keyword name and value pair to the return-array
			if( kw_value ){
       			add_assoc_stringl( return_value, kw_key, kw_value , kw_value_len ,TRUE );
			}//end of if

		} //end of while
	}//end of if e!=NULL

	efree(kw_value);
    uenum_close( e );
}
/* }}} */

 /* {{{
 * proto static string Locale::canonicalize($locale) {}
 * @return string the canonicalized locale 
 * }}} */
 /* {{{
 * proto static string locale_canonicalize(Locale $loc, string $locale) {}
 * @param string $locale	The locale string to canonicalize
 */
PHP_FUNCTION(locale_canonicalize){
	get_icu_value_src_php( LOC_CANONICALIZE_TAG , INTERNAL_FUNCTION_PARAM_PASSTHRU );
}
/* }}} */


/* {{{ append_key_value 
* Internal function which is called from locale_compose
* gets the value for the key_name and appends to the loc_name
* returns 1 if successful , -1 if not found , 0 not a string
*/
static int append_key_value(char* loc_name, zval* hash_arr, char* key_name ){
	int		result = -1;
	zval**	ele_value		= NULL;

	if( zend_hash_find( hash_arr , key_name , strlen(key_name) + 1 ,(void **)&ele_value ) == SUCCESS ){
		if( Z_TYPE_PP(ele_value)!= IS_STRING ){
			//element value is not a string 
			intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
				 "append_key_value: array element is not a string ", 0 TSRMLS_CC );
		}else{
			if(key_name != LOC_LANG_TAG && key_name != LOC_GRANDFATHERED_LANG_TAG ){
				strcat( loc_name , SEPARATOR);
			}
			strncat( loc_name , Z_STRVAL_PP(ele_value) , Z_STRLEN_PP(ele_value) );	
			result = 1;
		}
	}

	return result;

}
/* }}} */

/* {{{ append_prefix , appends the prefix needed
* e.g. private adds 'x'
*/
static void add_prefix(char* loc_name , char* key_name){
	if( strncmp(key_name , LOC_PRIVATE_TAG , 7) == 0 ){
		strcat( loc_name , SEPARATOR);
		strcat( loc_name , PRIVATE_PREFIX);
	}
}
/* }}} */

/* {{{ append_multiple_key_values 
* Internal function which is called from locale_compose
* gets the multiple values for the key_name and appends to the loc_name
* used for 'variant','extlang','private' 
* returns 1 if successful , -1 if not found , 0 not a string
*/
static int append_multiple_key_values(char* loc_name, zval* hash_arr, char* key_name ){
	int		result = -1;
	zval**	ele_value		= NULL;

	int 	i = 0;
	int 	isFirstSubtag = 0;
	int 	max_value = 0;
	char*   cur_key_name = NULL;

	//Variant
	if( zend_hash_find( hash_arr , key_name , strlen(key_name) + 1 ,(void **)&ele_value ) == SUCCESS ){
		if( Z_TYPE_PP(ele_value)!= IS_STRING ){
			//key_name is not a string 
			result = 0;
		}
		add_prefix( loc_name , key_name);

		strcat( loc_name , SEPARATOR);
		strncat( loc_name , Z_STRVAL_PP(ele_value) , Z_STRLEN_PP(ele_value) );	
	}else{
		//Decide the max_value
		if( key_name == LOC_VARIANT_TAG){
			max_value  = MAX_NO_VARIANT;
		}
		if( key_name == LOC_EXTLANG_TAG){
			max_value  = MAX_NO_EXTLANG;
		}
		if( key_name == LOC_PRIVATE_TAG){
			max_value  = MAX_NO_PRIVATE;
		}

		//Multiple variant values as variant0, variant1 ,variant2
		cur_key_name = (char*)ecalloc( 25,  25);
		isFirstSubtag = 0;
		for( i=0 ; i< max_value; i++ ){  
			sprintf( cur_key_name , "%s%d", key_name , i);	
			if( zend_hash_find( hash_arr , cur_key_name , strlen(cur_key_name) + 1,(void **)&ele_value ) == SUCCESS ){
				if( Z_TYPE_PP(ele_value)!= IS_STRING ){
					//variant is not a string 
					result = 0;
				}
				if (isFirstSubtag++ == 0){
					add_prefix( loc_name , cur_key_name);
				}
				strcat( loc_name , SEPARATOR);
				strncat( loc_name , Z_STRVAL_PP(ele_value) , Z_STRLEN_PP(ele_value) );	
			}
		}//end of for 
		efree(cur_key_name);
	}//end of else

	return result;
}
/* }}} */

/* {{{
* proto static string Locale::composeLocale($array) {}
* Creates a locale by combining the parts of locale-ID passed	
* }}} */
/* {{{
* proto static string compose_locale($array) {}
* Creates a locale by combining the parts of locale-ID passed	
*/
PHP_FUNCTION(locale_compose){
    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    int32_t     buflen          = 512;

	zval*		arr				= NULL;

	zval*		hash_arr		= NULL;
	zval**		ele_value		= NULL;

	//to get the value for the key for multiple occurring parts
	//e.g. variant0 , variant1 etc. so key_name will be variant0 then variant1
    char*       key_name 		= NULL;
    int         key_name_len    = 0;

	int i=0;
	int isGrandfathered=0;

	UErrorCode	status			= U_ZERO_ERROR;
    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "a",
		&arr) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_compose: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    if(loc_name_len == 0) {
        loc_name = UG(default_locale);
    }


	//MAKE_STD_ZVAL(hash_arr);
	hash_arr = Z_ARRVAL_P( arr );

	if( !hash_arr || zend_hash_num_elements( hash_arr ) == 0 )
		RETURN_FALSE;
	
	//Allocate memory
    loc_name = (char*)ecalloc( 512,  sizeof(char));
    loc_name_len = buflen;

	//Check for grandfathered first
	isGrandfathered = append_key_value( loc_name , hash_arr ,  LOC_GRANDFATHERED_LANG_TAG );	
	if( isGrandfathered ==1 ){
		//RETVAL_STRINGL( loc_name , strlen(loc_name) , FALSE);
		RETURN_STRINGL( loc_name , strlen(loc_name) , FALSE);
	}

	//Not grandfathered
	i = append_key_value( loc_name , hash_arr , LOC_LANG_TAG );	
	if( i == 0 ){
		RETURN_FALSE;
	}
	if( i == -1 ){
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_compose: parameter array does not contain 'language' tag.", 0 TSRMLS_CC );
		RETURN_FALSE;
	}
	if(append_multiple_key_values( loc_name , hash_arr , LOC_EXTLANG_TAG ) ==0 )
		RETURN_FALSE;
	if(append_key_value( loc_name , hash_arr , LOC_SCRIPT_TAG ) ==0 )
		RETURN_FALSE;
	if(append_key_value( loc_name , hash_arr , LOC_REGION_TAG ) ==0 )
		RETURN_FALSE;
		
	if(append_multiple_key_values( loc_name , hash_arr , LOC_VARIANT_TAG ) ==0 )
		RETURN_FALSE;
	if(append_multiple_key_values( loc_name , hash_arr , LOC_PRIVATE_TAG ) ==0 )
		RETURN_FALSE;

    RETVAL_STRINGL( loc_name , strlen(loc_name) , FALSE);
}
/* }}} */


/*{{{
* Parses the locale and returns private subtags  if existing
* else returns NULL
* e.g. for locale='en_US-x-prv1-prv2-prv3'
* returns 'prv1-prv2-prv3'
*/
static char* get_private_subtags(char* loc_name){
	char* 	result =NULL;
	int 	singletonPos = 0;
	int 	len =0; 
	char* 	mod_loc_name =NULL;

	if( loc_name && (len = strlen(loc_name)>0 ) ){
		mod_loc_name = loc_name ; 
		len   = strlen(mod_loc_name);
		while( (singletonPos = getSingletonPos(mod_loc_name))!= -1){

			if( singletonPos!=-1){ 
				if( (*(mod_loc_name+singletonPos)=='x') || (*(mod_loc_name+singletonPos)=='X') ){		
					//private subtag start found
					if( singletonPos + 2 ==  len){
						//loc_name ends with '-x-' ; return  NULL
					}
					else{
						result = mod_loc_name + singletonPos +2;
					}
					break;
				}
				else{
					if( singletonPos + 1 >=  len){
						//String end
						break;
					}else{
						//modify the string
						mod_loc_name = mod_loc_name + singletonPos +1;
						len = strlen(mod_loc_name);
					}
				}
			}

		}//end of while
	}
	
	return result;
}
/* }}} */

/* {{{ code used by locale_parse
*/
static int add_array_entry(char* loc_name, zval* hash_arr, char* key_name ){
    char*       key_value        	= NULL;
    char*       cur_key_name        = NULL;
    char*       token        		= NULL;

    int32_t     buflen          	= 512;
	int*		result				= 0;
	int 		cur_result  		= 0;
	int 		cnt  				= 0;


	if( key_name == LOC_PRIVATE_TAG ){
		key_value = get_private_subtags( loc_name );
		result = 1;
	}else{
		key_value = get_icu_value_internal( loc_name , key_name , &result);
	}
	if( key_name == LOC_VARIANT_TAG ||  key_name == LOC_PRIVATE_TAG ){
		if( result > 0 && key_value){
			//Tokenize on the "_" or "-" 
			token = strtok( key_value , DELIMITER );	
			cur_key_name = (char*)ecalloc( 25,  25);
			sprintf( cur_key_name , "%s%d", key_name , cnt++);	
			add_assoc_stringl( hash_arr, cur_key_name , token , strlen(token) ,TRUE );
			//tokenize on the "_" or "-" and stop  at singleton if any		
			while( (token = strtok(NULL , DELIMITER)) && (strlen(token)>1) ){
				sprintf( cur_key_name , "%s%d", key_name , cnt++);	
				add_assoc_stringl( hash_arr, cur_key_name , token , strlen(token) ,TRUE );
			}
		}
	}else{
		if( result == 1 ){
			add_assoc_stringl( hash_arr, key_name , key_value , strlen(key_value) ,TRUE );
			cur_result = 1;
		}
	}
	
	return cur_result;
}

/* {{{
* proto static array Locale::parseLocale($locale) {}
* parses a locale-id into an array the different parts of it
 }}} */
/* {{{
* proto static array parse_locale($locale) {}
* parses a locale-id into an array the different parts of it
*/
PHP_FUNCTION(locale_parse){
    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    int32_t     buflen          	= 512;

    char*       token        = NULL;
    char*       saved_ptr    	=0;

    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s",
        &loc_name, &loc_name_len ) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_parse: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    if(loc_name_len == 0) {
        loc_name = UG(default_locale);
    }

	array_init( return_value );
	add_array_entry( loc_name , return_value , LOC_LANG_TAG );
	add_array_entry( loc_name , return_value , LOC_SCRIPT_TAG );
	add_array_entry( loc_name , return_value , LOC_REGION_TAG );
	add_array_entry( loc_name , return_value , LOC_VARIANT_TAG );
	add_array_entry( loc_name , return_value , LOC_PRIVATE_TAG );

}
/* }}} */

/* {{{ proto static array Locale::getAllVariants($locale)
* gets an array containing the list of variants, or null
 }}} */
/* {{{ proto static array locale_get_all_variants($locale)
* gets an array containing the list of variants, or null
*/
PHP_FUNCTION(locale_get_all_variants){
    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    int32_t     buflen          	= 512;
	int*		result				= 0;
	char*		token				= NULL;
	char*		variant				= NULL;

    intl_error_reset( NULL TSRMLS_CC );
	
    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s",
        &loc_name, &loc_name_len ) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_parse: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    if(loc_name_len == 0) {
        loc_name = UG(default_locale);
    }


	array_init( return_value );

	//If the locale is grandfathered , stop , no variants
	if( findOffset( LOC_GRANDFATHERED , loc_name ) >=  0 ){ 
		//("Grandfathered Tag. No variants.");
	}
	else {	
	//Call ICU variant
		variant = get_icu_value_internal( loc_name , LOC_VARIANT_TAG , &result);
		if( result > 0 && variant){
			//Tokenize on the "_" or "-" 
			token = strtok( variant , DELIMITER );	
			add_next_index_stringl( return_value, token , strlen(token) ,TRUE );
			//tokenize on the "_" or "-" and stop  at singleton if any		
			while( (token = strtok(NULL , DELIMITER)) && (strlen(token)>1) ){
 				add_next_index_stringl( return_value, token , strlen(token) ,TRUE );
			}
		}
	}
			

}
/* }}} */

/*{{{
* Converts to lower case and also replaces all hyphuns with the underscore
*/
static char* strToMatch(char* str){
    char* result = NULL;
    int len =0;
    if( (!str) || strlen(str) ==0){
        return NULL;
    }else{
        len = strlen(str);
        result  = ecalloc( 1,strlen(str)+1 );
        while( (*str)!='\0' ){
			if( *str == '-' ){
            	*result =  '_';
			}else{
				*result = tolower(*str);
			}
            str++;
            result++;
		}
    }
    *result='\0';
    return(result-len );
}
/* }}} */

/*{{{
* code used by locale_filter_maatches
*/
static void filter_matches_internal( INTERNAL_FUNCTION_PARAMETERS) {
    char*       lang_tag        = NULL;
    int         lang_tag_len    = 0;
    char*       loc_range        = NULL;
    int         loc_range_len    = 0;

    int32_t     buflen          	= 512;
	int*		result				= 0;
	int			token				= 0;
	char*		chrcheck			= NULL;

    char*       can_lang_tag        = NULL;
    int         can_lang_tag_len    = 0;
    char*       can_loc_range        = NULL;
    int         can_loc_range_len    = 0;

	zend_bool 	boolCanonical 		 = NULL;	
	UErrorCode	status			= U_ZERO_ERROR;

    intl_error_reset( NULL TSRMLS_CC );
	
    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ssb",
        &lang_tag, &lang_tag_len , &loc_range , &loc_range_len , &boolCanonical) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_filter_matches: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    if(loc_range_len == 0) {
        loc_range = UG(default_locale);
    }

	if( strcmp(loc_range,"*")==0){
        RETURN_TRUE;
	}


	if( boolCanonical ){
		//canonicalize loc_range
		can_loc_range=get_icu_value_internal( loc_range , LOC_CANONICALIZE_TAG , &result);
		if( result ==0) {
			intl_error_set( NULL, status, 
				"locale_filter_matches : unable to canonicalize loc_range" , 0 TSRMLS_CC );
			RETURN_FALSE;
		}
		if( can_loc_range){
			loc_range = estrndup( can_loc_range , strlen(can_loc_range) );
		}

		//canonicalize lang_tag
		can_lang_tag = get_icu_value_internal( lang_tag , LOC_CANONICALIZE_TAG , &result);
		if( result ==0) {
			intl_error_set( NULL, status, 
				"locale_filter_matches : unable to canonicalize lang_tag" , 0 TSRMLS_CC );
			RETURN_FALSE;
		}
		if( can_lang_tag){
			lang_tag = estrndup( can_lang_tag , strlen(can_lang_tag) );
		}
	}//end of if isCanonical

	//Convert to lower case for case-insensitive comparison
	lang_tag = strToMatch( lang_tag );
	loc_range= strToMatch( loc_range );

	//check if prefix
	token 	= strstr( lang_tag , loc_range );
	
	if( token && (token==lang_tag) ){
		//check if the char. after match is SEPARATOR
		chrcheck = token + (strlen(loc_range));
		if( isIDSeparator(*chrcheck) || isEndOfTag(*chrcheck) ){ 
			RETURN_TRUE;
		}
	}

	//No prefix as loc_range 
	RETURN_FALSE;
}

PHP_FUNCTION(locale_filter_matches){
	filter_matches_internal( INTERNAL_FUNCTION_PARAM_PASSTHRU );
}

/* {{{
* returns the lookup result to lookup_loc_range_src_php 
* internal function
*/
static char* lookup_loc_range(char* loc_range, zval* hash_arr , int isCanonical){
	int		cur_arr_ind 		= 0;
	int		i 					= 0;
	int* 	result 				= 0;

	char* 	lang_tag 			= NULL;
	zval**	ele_value			= NULL;
	char*	cur_arr[MAX_NO_LOOKUP_LANG_TAG] ;

	char* 	loc_range_to_cmp 	= NULL;
	int		saved_pos 			= 0;
	UErrorCode	status			= U_ZERO_ERROR;

	loc_range = strToMatch(loc_range);	

	//convert the array to lowercase , also replace hyphuns with the underscore and store it in cur_arr
	for(zend_hash_internal_pointer_reset(hash_arr);
		zend_hash_has_more_elements(hash_arr) == SUCCESS;
		zend_hash_move_forward(hash_arr)) {
		
		if (zend_hash_get_current_data(hash_arr, (void**)&ele_value) == FAILURE) {
            /* Should never actually fail
             * since the key is known to exist. */
            continue;
        }
		if( Z_TYPE_PP(ele_value)!= IS_STRING ){
			//element value is not a string 
			intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
				 "lookup_internal_src_php: array element is not a string ", 0 TSRMLS_CC );
		}else{
			lang_tag = ecalloc( 1, Z_STRLEN_PP(ele_value) );
			lang_tag = estrndup(Z_STRVAL_PP(ele_value) , Z_STRLEN_PP(ele_value));
			cur_arr[cur_arr_ind++] = strToMatch(lang_tag);	
		}
	}

	//Canonicalize array elements
	if( isCanonical ==1 ){
		for( i=0; i< cur_arr_ind; i++ ){ 
			cur_arr[i]=get_icu_value_internal( cur_arr[i] , LOC_CANONICALIZE_TAG , &result);
			if( result ==0) {
				intl_error_set( NULL, status, 
					"locale_lookup : unable to canonicalize lang_tag" , 0 TSRMLS_CC );
				return NULL;
			}
		}
	}

	//Lookup for the lang_tag match
	saved_pos = strlen(loc_range);
	while(saved_pos!=(-1) ){
		loc_range_to_cmp = estrndup(loc_range,saved_pos+1);
		for( i=0; i< cur_arr_ind; i++ ){ 
			if( cur_arr[i] && (strcmp(loc_range_to_cmp,cur_arr[i])==0) ){	
				//Match found
				return(cur_arr[i]);
			}
		}
		saved_pos = getStrrtokenPos( loc_range , saved_pos );
	}

	return NULL;

}
/* }}} */

/* {{{
* returns the position of a singleton if present 
* returns -1 if no singleton
* strtok equivalent search for singleton
*/
static int getSingletonPos(char* str){
	int result =-1;
	int i=0;
	int len = 0;
	
	if( str && ((len=strlen(str))>0) ){
		for( i=0; i<len ; i++){
			if( isIDSeparator(*(str+i)) ){
				if( i==1){
					// string is of the form x-avy or a-prv1
					result =0;
					break;
				}else{
					//delimiter found; check for singleton 
					if( isIDSeparator(*(str+i+2)) ){
						//a singleton; so send the position of separator before singleton
						result = i+1;
						break;
					}
				}
			}
		}//end of for
		
	}
	return result;
}
/* }}} */

/* {{{
* returns the position of next token for lookup 
* or -1 if no token
* strtokr equivalent search for token in reverse direction 
*/
static int getStrrtokenPos(char* str, int savedPos){
	int result =-1;
	int i=0;
	int nextPos = 0; 	//for the next pos. of delimiter
	
	for( i=savedPos; i>=0 ;i--){
		if( isIDSeparator(*(str+i)) ){
			//delimiter found; check for singleton 
			if( isIDSeparator(*(str+i-2)) ){
				//a singleton; so send the position of token before the singleton
				result = i-3;
			}else{
				result = i-1;
			}
			break;
		}
	}
	if(result < 1){
		//Just in case inavlid locale e.g. '-x-xyz' or '-sl_Latn'
		result =-1;
	}
	return result;
}
/* }}} */

/* {{{
* returns the lookup result  
* or -1 if no token
*/
static char* lookup_internal_src_php( INTERNAL_FUNCTION_PARAMETERS) {
    char*       fallback_loc  	= NULL;
    int         fallback_loc_len= 0;
    char*       lang_tag        = NULL;
    int         lang_tag_len    = 0;
    char*       loc_range      	= NULL;
    int         loc_range_len  = 0;

    char*       can_lang_tag      	= NULL;
    int         can_lang_tag_len   	= 0;
    char*       can_loc_range      	= NULL;
    int         can_loc_range_len   = 0;

	zval*		arr				= NULL;
	zval*		hash_arr		= NULL;
	zend_bool	boolCanonical		=0;

	char*	 	result			=NULL;
	char*	 	empty_result	="";

	UErrorCode	status			= U_ZERO_ERROR;

    intl_error_reset( NULL TSRMLS_CC );
	
    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "asb|s",
		&arr,&loc_range,&loc_range_len,&boolCanonical,&fallback_loc,&fallback_loc_len) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_lookup: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    if(loc_range_len == 0) {
        loc_range = UG(default_locale);
    }


	//MAKE_STD_ZVAL(hash_arr);
	hash_arr = Z_ARRVAL_P( arr );

	if( !hash_arr || zend_hash_num_elements( hash_arr ) == 0 ){
		result = empty_result;
	}
	else{
		result = lookup_loc_range( loc_range ,hash_arr ,(boolCanonical?1:0) );
		if( !result ){
			if( fallback_loc ) {
				result = estrndup( fallback_loc , fallback_loc_len);
			}else{
				result = empty_result;
			}
		}
	}

	RETVAL_STRINGL( result, strlen(result), TRUE );
}

/* {{{
* public static function lookup(array $langtag, $locale, $default = null) {}
* Searchs the items in $langtag for the best match to the language
* range 
*/
/* {{{
* public static function lookup(array $langtag, $locale, $default = null) {}
* Searchs the items in $langtag for the best match to the language
* range 
*/
PHP_FUNCTION(locale_lookup){
	lookup_internal_src_php( INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/*}}}*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 *can_loc_len
*/
