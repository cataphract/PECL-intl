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

/* {{{ proto static string Locale::getDefault(  )
 	* @return string the current runtime locale as an RFC 4646 language tag, 
    * normalized according to case mapping conventions in RFC 4646 
    * Section 2.1. }}} */
/* {{{ proto static string locale_get_default( )
    * @return string the current runtime locale as an RFC 4646 language tag, 
    * normalized according to case mapping conventions in RFC 4646 
    * Section 2.1. 
 */

PHP_NAMED_FUNCTION( zif_locale_get_default){
	char*  		locale_name = NULL;

	locale_name = (char*) uloc_getDefault();

	RETVAL_STRINGL( locale_name, strlen(locale_name), TRUE );
}
/* }}} */

/* {{{ proto static Locale Locale::setDefault( string $locale )
     * @param string $locale the locale to extract the primary language code from
     * @return string the language code associated with the language (or 
     * a grandfathered language tag). This will not be an empty 
     * value except for the root locale. Deprecated values will be 
     * mapped to modern equivalents. 
     *
     * The language subtag must always be present. Returns 'null' if an
     * error occured extracting the language subtag (as with an invalid
     * locale code).
     * }}} */
/* {{{ proto static locale_set_default( string $locale )
 	 * @param string $locale the locale to extract the primary language code from
     * @return string the language code associated with the language (or 
     * a grandfathered language tag). This will not be an empty 
     * value except for the root locale. Deprecated values will be 
     * mapped to modern equivalents. 
     *
     * The language subtag must always be present. Returns 'null' if an
     * error occured extracting the language subtag (as with an invalid
     * locale code).
*/
PHP_NAMED_FUNCTION(  zif_locale_set_default){
	char* locale_name = NULL;
	int   len=0;	
	char* saved_locale =  NULL; 		

	//LOCALE_METHOD_INIT_VARS


	// Parse parameters.
	if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC,  "s",
		&locale_name ,&len ) == FAILURE)
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			 "locale_set_default: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Set new value for the given attribute.
	if(INTL_G(current_locale)) {
		saved_locale = estrdup( INTL_G (current_locale) ); 
	}
	INTL_G( current_locale) = estrndup( locale_name , len );

	if( saved_locale) {
		efree (saved_locale);
	}
	RETVAL_STRINGL( INTL_G( current_locale), strlen(INTL_G( current_locale)), TRUE );
}

     /* }}} */

    /* {{{
     * proto static string Locale Locale::getPrimaryLanguage( string $locale )
     * proto string locale_get_primary_language( string $locale)
     * @param string $locale the locale to extract the primary language code from
     * @return string the language code associated with the language (or 
     * a grandfathered language tag). This will not be an empty 
     * value except for the root locale. Deprecated values will be 
     * mapped to modern equivalents. 
     *
     * The language subtag must always be present. Returns 'null' if an
     * error occured extracting the language subtag (as with an invalid
     * locale code).
     * }}} */
    /* {{{
     * proto string locale_get_primary_language( string $locale)
     * @param string $locale the locale to extract the primary language code from
     * @return string the language code associated with the language (or 
     * a grandfathered language tag). This will not be an empty 
     * value except for the root locale. Deprecated values will be 
     * mapped to modern equivalents. 
     *
     * The language subtag must always be present. Returns 'null' if an
     * error occured extracting the language subtag (as with an invalid
     * locale code).
     */
    PHP_FUNCTION(locale_get_primary_language) {

	char* 		loc_name 		= NULL;
	int   		loc_name_len	= 0;	

	char*		lang_name 		= NULL;
	int32_t		lang_name_len 	= 0;
	
	int32_t		buflen 			= 512;
	UErrorCode	status			= U_ZERO_ERROR;

	intl_error_reset( NULL TSRMLS_CC );

	// Parse parameters.
	if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s",
		&loc_name ,&loc_name_len ) == FAILURE)
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			 "locale_get_primary_language: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	//Get the primary  language for the given locale
	do{
		lang_name = erealloc( lang_name , buflen  );
		lang_name_len = buflen;

		buflen = uloc_getLanguage ( loc_name ,lang_name , lang_name_len , &status); 
		if( U_FAILURE( status ) )
		{
			if( status == U_BUFFER_OVERFLOW_ERROR )
			{
				status = U_ZERO_ERROR;
				continue;
			}

			intl_error_set( NULL, status,
				"locale_get_primary_language: unable to get locale primary language", 0 TSRMLS_CC );
			efree( lang_name );
			RETURN_FALSE;
		}
	} while( buflen > lang_name_len );

	RETVAL_STRINGL( lang_name , buflen , FALSE);
}
/* }}} */

    /* {{{
     * proto static string Locale Locale::getScript( )
     * @param string $locale the locale to extract the script code from
     * @return string the script subtag for the locale or the empty string
     * if the script is not assigned. Note that many locales do not
     * assign a script code.
     *
     * If no script is present, returns the empty string.
     * }}} */
    /* {{{
	 * proto static string locale_get_script( string $locale)
     * @param string $locale the locale to extract the script code from
     * @return string the script subtag for the locale or the empty string
     * if the script is not assigned. Note that many locales do not
     * assign a script code.
     *
     * If no script is present, returns the empty string.
     */
 PHP_FUNCTION( locale_get_script ) {

    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    char*       script			= NULL;
    int32_t     script_len   	= 0;

    int32_t     buflen          = 512;
    UErrorCode  status          = U_ZERO_ERROR;

    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s",
        &loc_name ,&loc_name_len ) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_get_script: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    //Get the script for the given locale
    do{
        script = erealloc( script , buflen  );
        script_len = buflen;

        buflen = uloc_getScript ( loc_name ,script , script_len , &status);
		if( U_FAILURE( status ) )
		{
			if( status == U_BUFFER_OVERFLOW_ERROR )
			{
				status = U_ZERO_ERROR;
				continue;
			}

			intl_error_set( NULL, status,
				"locale_get_script: unable to get locale script", 0 TSRMLS_CC );
			efree( script );
			RETURN_FALSE;
		}
    } while( buflen > script_len );

    RETVAL_STRINGL( script , buflen , FALSE);
}
/* }}} */

    /* {{{
     * proto public static string Locale::getRegion($locale) 
     * @param string $locale the locale to extract the region code from
     * @return string the region subtag for the locale or the empty string
     * if the region is not assigned. 
     * Valid values include both ISO 3166 country codes and UN
     * M49 region codes (which are numeric strings).
     *
     * If no region is present, returns the empty string.
     * }}}*/
     /* {{{
     * proto public static string locale_get_region($locale) 
     * @param string $locale the locale to extract the region code from
     * @return string the region subtag for the locale or the empty string
     * if the region is not assigned. 
     * Valid values include both ISO 3166 country codes and UN
     * M49 region codes (which are numeric strings).
     *
     * If no region is present, returns the empty string.
     */
PHP_FUNCTION( locale_get_region ) {

    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    char*       region			= NULL;
    int32_t     region_len   	= 0;

    int32_t     buflen          = 512;
    UErrorCode  status          = U_ZERO_ERROR;

    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s",
        &loc_name ,&loc_name_len ) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_get_region: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    //Get the region for the given locale
    do{
        region = erealloc( region , buflen  );
        region_len = buflen;

        buflen = uloc_getCountry ( loc_name ,region , region_len , &status);
		if( U_FAILURE( status ) )
		{
			if( status == U_BUFFER_OVERFLOW_ERROR )
			{
				status = U_ZERO_ERROR;
				continue;
			}

			intl_error_set( NULL, status,
				"locale_get_region: unable to get locale region", 0 TSRMLS_CC );
			efree( region );
			RETURN_FALSE;
		}
    } while( buflen > region_len );

    RETVAL_STRINGL( region , buflen , FALSE);
}
/* }}} */

    /* {{{
     * proto static string Locale::getVariant($locale) {}
     * @param string $locale the locale to extract the first variant code from
     * @return string the first variant subtag for the locale or the empty string
     * Note that there can be multiple variants. In practice
     * multiple variants are rare. Obtain the complete list of variants using
     * getAllVariants().
     *
     * If no variant is present, returns the empty string.
     * }}}*/
	 /* {{{
     * proto static string locale_get_variant($locale) {}
     * @param string $locale the locale to extract the first variant code from
     * @return string the first variant subtag for the locale or the empty string
     * Note that there can be multiple variants. In practice
     * multiple variants are rare. Obtain the complete list of variants using
     * getAllVariants().
     *
     * If no variant is present, returns the empty string.
     */
 PHP_FUNCTION(locale_get_variant) {

    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    char*       variant                  = NULL;
    int32_t     variant_len      = 0;

    int32_t     buflen          = 512;
    UErrorCode  status          = U_ZERO_ERROR;

    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s",
        &loc_name ,&loc_name_len ) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_get_variant: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    //Get the variant for the given locale
    do{
        variant = erealloc( variant , buflen  );
        variant_len = buflen;

        buflen = uloc_getVariant ( loc_name ,variant , variant_len , &status);
		if( U_FAILURE( status ) )
		{
			if( status == U_BUFFER_OVERFLOW_ERROR )
			{
				status = U_ZERO_ERROR;
				continue;
			}

			intl_error_set( NULL, status,
				"locale_get_variant: unable to get locale variant", 0 TSRMLS_CC );
			efree( variant );
			RETURN_FALSE;
		}
    } while( buflen > variant_len );

    RETVAL_STRINGL( variant , buflen , FALSE);
}
/* }}} */

    /* {{{
	* proto static string Locale::getDisplayName($locale, $in_locale = null)
    * @param string $locale - the locale to return a displayname for
	* @param [string] $in_locale - optional format locale
	*
	* @return string - display name of the locale in the format
	* appropriate for $in_locale. If $in_locale is 'null' 
	* then the default locale is used. 
	*
	* For example, if the default locale is "de", getDisplayName("de") returns
	* "Deutsch" while getDisplayName("de", "en-US") returns "German".
	* }}} */
    /* {{{
	* proto static string locale_get_display_name($locale, $in_locale = null)
    * @param string $locale - the locale to return a displayname for
	* @param [string] $in_locale - optional format locale
	*
	* @return string - display name of the locale in the format
	* appropriate for $in_locale. If $in_locale is 'null' 
	* then the default locale is used. 
	*
	* For example, if the default locale is "de", getDisplayName("de") returns
	* "Deutsch" while getDisplayName("de", "en-US") returns "German".
	*/
 PHP_FUNCTION(locale_get_display_name) {

    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    char*       disp_loc_name        = NULL;
    int         disp_loc_name_len    = 0;

    UChar*      disp_name      	= NULL;
    int32_t     disp_name_len  	= 0;

    int32_t     buflen          = 512;
    UErrorCode  status          = U_ZERO_ERROR;

	char*       utf8value		= NULL;
	int         utf8value_len   = 0;

    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ss",
        &loc_name, &loc_name_len , &disp_loc_name ,&disp_loc_name_len ) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_get_display_name: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    //Get the disp_lang for the given locale
    do{
        disp_name = erealloc( disp_name , buflen  );
        disp_name_len = buflen;

        //Check if disp_loc_name passed , if not use default locale
        if( !disp_loc_name){
            disp_loc_name =(char*) uloc_getDefault();
        }

        buflen = uloc_getDisplayName ( loc_name , disp_loc_name , disp_name , disp_name_len , &status);
        if( U_FAILURE( status ) )
        {
            if( status == U_BUFFER_OVERFLOW_ERROR )
            {
                status = U_ZERO_ERROR;
                continue;
            }

            intl_error_set( NULL, status,
                "locale_get_disp_name: unable to get locale disp_name", 0 TSRMLS_CC );
            efree( disp_name );
            RETURN_FALSE;
		}
    } while( buflen > disp_name_len );

    // Convert display locale name from UTF-16 to UTF-8.
    intl_convert_utf16_to_utf8( &utf8value, &utf8value_len, disp_name, buflen, &status );
    efree( disp_name );
    if( U_FAILURE( status ) )
    {
        intl_error_set( NULL, status,
            "locale_get_display_name: error converting display name to UTF-8", 0 TSRMLS_CC );
        RETURN_FALSE;
    }

    RETVAL_STRINGL( utf8value, utf8value_len , FALSE);

}
/* }}} */

    /* {{{
	 * public static string Locale::getDisplayLanguage($locale, $in_locale = null)
	 * Returns an appropriately localized display name for the language
	 * subtag $lang. For example, the language subtag for locale
	 * "en-US" is "en". Thus getDisplayLanguage("en-US", "en-US")
	 * would return the string "English".
	 *
	 * Language must always be present. If an error occurs returns null.
	 *
     * @param string $locale the locale to extract the language 
	 * @param string  $in_locale locale to use to display the language name
	 * @return string  display name for $lang
	 * }}}*/
     /* {{{
	 * public static function get_display_language($locale, $in_locale = null) 
	 * Returns an appropriately localized display name for the language
	 * subtag $lang. For example, the language subtag for locale
	 * "en-US" is "en". Thus getDisplayLanguage("en-US", "en-US")
	 * would return the string "English".
	 *
	 * Language must always be present. If an error occurs returns null.
	 *
     * @param string $locale the locale to extract the language 
	 * @param string  $in_locale locale to use to display the language name
	 * @return string  display name for $lang
	 */
 PHP_FUNCTION(locale_get_display_language) {

    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    char*       disp_loc_name        = NULL;
    int         disp_loc_name_len    = 0;

    UChar*      disp_lang      	= NULL;
    int32_t     disp_lang_len  	= 0;

    int32_t     buflen          = 512;
    UErrorCode  status          = U_ZERO_ERROR;

	char*       utf8value		= NULL;
	int         utf8value_len   = 0;

    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s|s",
        &loc_name, &loc_name_len , &disp_loc_name ,&disp_loc_name_len ) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_get_disp_lang: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    //Get the disp_lang for the given locale
    do{
        disp_lang = erealloc( disp_lang , buflen  );
        disp_lang_len = buflen;

		//Check if disp_loc_name passed , if not use default locale
		if( !disp_loc_name){
			disp_loc_name = (char*) uloc_getDefault();
		}
	
        buflen = uloc_getDisplayLanguage ( loc_name , disp_loc_name , disp_lang , disp_lang_len , &status);
        if( U_FAILURE( status ) )
        {
            if( status == U_BUFFER_OVERFLOW_ERROR )
            {
                status = U_ZERO_ERROR;
                continue;
            }

            intl_error_set( NULL, status,
                "locale_get_disp_lang: unable to get locale disp_lang", 0 TSRMLS_CC );
            efree( disp_lang );
            RETURN_FALSE;
		}
    } while( buflen > disp_lang_len );

	// Convert display language from UTF-16 to UTF-8.
	intl_convert_utf16_to_utf8( &utf8value, &utf8value_len, disp_lang, buflen, &status );
	efree( disp_lang );
	if( U_FAILURE( status ) )
	{
		intl_error_set( NULL, status,
			"locale_get_display_language: error converting display language to UTF-8", 0 TSRMLS_CC );
		RETURN_FALSE;
	}


    RETVAL_STRINGL( utf8value, utf8value_len , FALSE);
}
/* }}} */

	/* {{{
	 * public static string Locale::getDisplayScript($locale, $in_locale = null) {}
	 * Returns an appropriately localized display name for the script 
	 * subtag $script. For example, the script subtag for locale
	 * "zh-Hant-TW" is "Hant". Thus getDisplayScript(getScript("zh-Hant-TW"), "en-US")
	 * would return the string "Traditional Chinese".
	 *
	 * If no script is present, returns null.
	 *
     * @param string $locale the locale to extract the script from
	 * @param  string  $in_locale locale to use to display the script name
	 * @return string  display name for $script
	 * }}}*/
	/* {{{
	 * public static function get_display_script($locale, $in_locale = null) {}
	 * Returns an appropriately localized display name for the script 
	 * subtag $script. For example, the script subtag for locale
	 * "zh-Hant-TW" is "Hant". Thus getDisplayScript(getScript("zh-Hant-TW"), "en-US")
	 * would return the string "Traditional Chinese".
	 *
	 * If no script is present, returns null.
	 *
     * @param string $locale the locale to extract the script from
	 * @param  string  $in_locale locale to use to display the script name
	 * @return string  display name for $script
	 */
 PHP_FUNCTION(locale_get_display_script) {

    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    char*       disp_loc_name        = NULL;
    int         disp_loc_name_len    = 0;

    UChar*      disp_script       = NULL;
    int32_t     disp_script_len   = 0;

    int32_t     buflen          = 512;
    UErrorCode  status          = U_ZERO_ERROR;

	char*       utf8value		= NULL;
	int         utf8value_len   = 0;

    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ss",
        &loc_name, &loc_name_len , &disp_loc_name ,&disp_loc_name_len ) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_get_disp_script: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    //Get the disp_script for the given locale
    do{
        disp_script = erealloc( disp_script , buflen  );
        disp_script_len = buflen;

        //Check if disp_loc_name passed , if not use default locale
        if( !disp_loc_name){
            disp_loc_name = (char*) uloc_getDefault();
        }

        buflen = uloc_getDisplayScript ( loc_name , disp_loc_name ,  disp_script , disp_script_len , &status);
        if( U_FAILURE( status ) )
        {
            if( status == U_BUFFER_OVERFLOW_ERROR )
            {
                status = U_ZERO_ERROR;
                continue;
            }

            intl_error_set( NULL, status,
                "locale_get_disp_script: unable to get locale disp_script", 0 TSRMLS_CC );
            efree( disp_script );
            RETURN_FALSE;
		}
    } while( buflen > disp_script_len );

    // Convert display script from UTF-16 to UTF-8.
    intl_convert_utf16_to_utf8( &utf8value, &utf8value_len, disp_script, buflen, &status );
    efree( disp_script );
    if( U_FAILURE( status ) )
    {
        intl_error_set( NULL, status,
            "locale_get_display_script: error converting display script to UTF-8", 0 TSRMLS_CC );
        RETURN_FALSE;
    }


    RETVAL_STRINGL( utf8value, utf8value_len , FALSE);

}
/* }}} */

	/* {{{
	 * proto static string Locale::getDisplayRegion(string $locale, string $in_locale = null) {}
	 * Returns an appropriately localized display name for the region 
	 * subtag $region. For example, the region subtag for locale
	 * "en-US" is "US". Thus getDisplayRegion(getRegion("en-US"), "en-US")
	 * would return the string "United States".
	 *
	 * If no $region is empty or an error occurs, returns null.
	 *
     * @param string $locale the locale to extract the region from
	 * @param  string  $in_locale locale to use to display the region name
	 * @return string  display name for $region
	 * }}}*/
	/* {{{
	 * proto static string get_display_region(string $locale, string $in_locale = null) {}
	 * Returns an appropriately localized display name for the region 
	 * subtag $region. For example, the region subtag for locale
	 * "en-US" is "US". Thus getDisplayRegion(getRegion("en-US"), "en-US")
	 * would return the string "United States".
	 *
	 * If no $region is empty or an error occurs, returns null.
	 *
     * @param string $locale the locale to extract the region from
	 * @param  string  $in_locale locale to use to display the region name
	 * @return string  display name for $region
	 */
 PHP_FUNCTION(locale_get_display_region) {

    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    char*       disp_loc_name        = NULL;
    int         disp_loc_name_len    = 0;

    UChar*      disp_region       = NULL;
    int32_t     disp_region_len   = 0;

    int32_t     buflen          = 512;
    UErrorCode  status          = U_ZERO_ERROR;

	char*       utf8value		= NULL;
	int         utf8value_len   = 0;

    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ss",
        &loc_name, &loc_name_len , &disp_loc_name ,&disp_loc_name_len ) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_get_disp_region: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    //Get the disp_region for the given locale
    do{
        disp_region = erealloc( disp_region , buflen  );
        disp_region_len = buflen;

        //Check if disp_loc_name passed , if not use default locale
        if( !disp_loc_name){
            disp_loc_name = (char*) uloc_getDefault();
        }

        buflen = uloc_getDisplayCountry ( loc_name , disp_loc_name , disp_region , disp_region_len , &status);
        if( U_FAILURE( status ) )
        {
            if( status == U_BUFFER_OVERFLOW_ERROR )
            {
                status = U_ZERO_ERROR;
                continue;
            }

            intl_error_set( NULL, status,
                "locale_get_disp_region : unable to get locale disp_region", 0 TSRMLS_CC );
            efree( disp_region );
            RETURN_FALSE;
		}
    } while( buflen > disp_region_len );

    // Convert display region from UTF-16 to UTF-8.
    intl_convert_utf16_to_utf8( &utf8value, &utf8value_len, disp_region, buflen, &status );
    efree( disp_region );
    if( U_FAILURE( status ) )
    {
        intl_error_set( NULL, status,
            "locale_get_display_region: error converting display region to UTF-8", 0 TSRMLS_CC );
        RETURN_FALSE;
    }


    RETVAL_STRINGL( utf8value, utf8value_len , FALSE);

}
/* }}} */

	/* {{{
	 * proto static string Locale::getDisplayVariant(string $locale, string $in_locale = null) {}
	 * Returns an appropriately localized display name for the variant 
	 * subtag $variant. For example, the variant subtag 
	 * for locale "sl-IT-rozaj" is "rozaj". 
	 * getDisplayVariants(getVariant("sl-IT-rozaj"), "en-US") would return 
	 * the string "Resian".
	 *
	 * Note that a locale can contain multiple variants. You will need to iterate
	 * over the list of variants in order to obtain each display name.
	 *
	 * If no $variant is empty or an error occurs, returns null.
	 *
	 * @param  string  $variant   variant to get a display name for
	 * @param  string  $in_locale locale to use to display the variant's name
	 * @return string  display name for $variant
	 * }}}*/
	/* {{{
	 * proto static string get_display_variant(string $locale, string $in_locale = null) {}
	 * Returns an appropriately localized display name for the variant 
	 * subtag $variant. For example, the variant subtag 
	 * for locale "sl-IT-rozaj" is "rozaj". 
	 * getDisplayVariants(getVariant("sl-IT-rozaj"), "en-US") would return 
	 * the string "Resian".
	 *
	 * Note that a locale can contain multiple variants. You will need to iterate
	 * over the list of variants in order to obtain each display name.
	 *
	 * If no $variant is empty or an error occurs, returns null.
	 *
	 * @param  string  $variant   variant to get a display name for
	 * @param  string  $in_locale locale to use to display the variant's name
	 * @return string  display name for $variant
	 */
 PHP_FUNCTION(locale_get_display_variant) {

    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    char*       disp_loc_name        = NULL;
    int         disp_loc_name_len    = 0;

    UChar*      disp_variant       = NULL;
    int32_t     disp_variant_len   = 0;

    int32_t     buflen          = 512;
    UErrorCode  status          = U_ZERO_ERROR;

	char*       utf8value		= NULL;
	int         utf8value_len   = 0;

    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ss",
        &loc_name, &loc_name_len , &disp_loc_name ,&disp_loc_name_len ) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_get_disp_variant: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    //Get the disp_variant for the given locale
    do{
        disp_variant = erealloc( disp_variant , buflen  );
        disp_variant_len = buflen;

        //Check if disp_loc_name passed , if not use default locale
        if( !disp_loc_name){
            disp_loc_name = (char*) uloc_getDefault();
        }

        buflen = uloc_getDisplayCountry ( loc_name , disp_loc_name , disp_variant , disp_variant_len , &status);
        if( U_FAILURE( status ) )
        {
            if( status == U_BUFFER_OVERFLOW_ERROR )
            {
                status = U_ZERO_ERROR;
                continue;
            }

            intl_error_set( NULL, status,
                "locale_get_disp_variant : unable to get locale disp_variant", 0 TSRMLS_CC );
            efree( disp_variant );
            RETURN_FALSE;
		}
    } while( buflen > disp_variant_len );

    // Convert display variant from UTF-16 to UTF-8.
    intl_convert_utf16_to_utf8( &utf8value, &utf8value_len, disp_variant, buflen, &status );
    efree( disp_variant );
    if( U_FAILURE( status ) )
    {
        intl_error_set( NULL, status,
            "locale_get_display_variant: error converting display variant to UTF-8", 0 TSRMLS_CC );
        RETURN_FALSE;
    }

    RETVAL_STRINGL( utf8value, utf8value_len , FALSE);

}
/* }}} */

     /* {{{
     * proto static string[] getKeywords(string $locale) {
     * @param string $locale locale to extract the keywords from
     * @return an associative array containing keyword-value
     * pairs for this locale. The keys are keys to the array (doh!)
     * }}}*/
     /* {{{
     * proto static string[] locale_get_keywords(string $locale) {
     * @param string $locale locale to extract the keywords from
     * @return an associative array containing keyword-value
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
	 * @param string $locale	The locale string to canonicalize
	 * @return string the canonicalized locale 
     * Deprecated subtags are mapped to modern equivalents and 
	 * case is normalized according to the rules in RFC 4646. 
	 * ICU-style locale identifiers are converted into the 
	 * language tag form by replacing underscores with hyphens
	 *
	 * Note that invalid subtags are preserved. A canonicalized
	 * locale tag may not be valid.
	 * }}} */
     /* {{{
     * proto static string locale_canonicalize(Locale $loc, string $locale) {}
	 * @param string $locale	The locale string to canonicalize
	 * @return string the canonicalized locale 
     * Deprecated subtags are mapped to modern equivalents and 
	 * case is normalized according to the rules in RFC 4646. 
	 * ICU-style locale identifiers are converted into the 
	 * language tag form by replacing underscores with hyphens
	 *
	 * Note that invalid subtags are preserved. A canonicalized
	 * locale tag may not be valid.
	 */
PHP_FUNCTION(locale_canonicalize){
    char*       loc_name        = NULL;
    int         loc_name_len    = 0;

    char*       can_loc_name    = NULL;
    int32_t     can_loc_name_len 	= 0;

    int32_t     buflen          = 512;
    UErrorCode  status          = U_ZERO_ERROR;

    intl_error_reset( NULL TSRMLS_CC );

    // Parse parameters.
    if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s",
        &loc_name, &loc_name_len ) == FAILURE)
    {
        intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
             "locale_canonicalize: unable to parse input params", 0 TSRMLS_CC );

        RETURN_FALSE;
    }

    //Get the canonicalized locale for the given locale
    do{
        can_loc_name = erealloc( can_loc_name , buflen  );
        can_loc_name_len = buflen;

        buflen = uloc_canonicalize ( loc_name ,  can_loc_name , can_loc_name_len , &status);
        if( U_FAILURE( status ) )
        {
            if( status == U_BUFFER_OVERFLOW_ERROR )
            {
                status = U_ZERO_ERROR;
                continue;
            }

            intl_error_set( NULL, status,
                "locale_canonicalize : unable to get locale canonicalize", 0 TSRMLS_CC );
            efree( can_loc_name );
            RETURN_FALSE;
		}
    } while( buflen > can_loc_name_len );

    RETVAL_STRINGL( can_loc_name , buflen , FALSE);
}
/* }}} */
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 *can_loc_len
*/
