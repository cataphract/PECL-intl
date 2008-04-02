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
   | Authors: Kirti Velankar <kirtig@yahoo-inc.com>                       |
   +----------------------------------------------------------------------+
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_intl.h"
#include "intl_convert.h"
#include "dateformat_class.h"
#include "dateformat_attr.h"

#include <unicode/ustring.h>
#include <unicode/udat.h>
#include <unicode/ucal.h>

static void internal_set_calendar(DateFormatter_object *mfo, char* timezone_id , int timezone_id_len , int calendar  ,zval* return_value TSRMLS_DC){

        int         timezone_utf16_len = 0;
        UChar*      timezone_utf16  = NULL;             //timezone_id in UTF-16

        char*       locale = NULL;
        int         locale_type =ULOC_ACTUAL_LOCALE;

        UCalendar*   ucal_obj = NULL;

        //check for the validity  of value of calendar passed
        intl_error_reset( NULL TSRMLS_CC );
        if( calendar > 1){
                intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR,
                        "datefmt_set_calendar: calendar value specified is out of valid range", 0 TSRMLS_CC);
                RETURN_FALSE;
        }

        // Convert timezone to UTF-16.
        intl_convert_utf8_to_utf16(&timezone_utf16, &timezone_utf16_len, timezone_id, timezone_id_len  , &INTL_DATA_ERROR_CODE(mfo));
        INTL_METHOD_CHECK_STATUS(mfo, "Error converting timezone to UTF-16" );


        //Get the lcoale for the dateformatter
        locale = (char *)udat_getLocaleByType(DATE_FORMAT_OBJECT(mfo), locale_type ,&INTL_DATA_ERROR_CODE(mfo));

        //Set the calendar if passed
        ucal_obj = ucal_open( timezone_utf16 , timezone_utf16_len , locale , calendar , &INTL_DATA_ERROR_CODE(mfo) );
        udat_setCalendar( DATE_FORMAT_OBJECT(mfo), ucal_obj );
        INTL_METHOD_CHECK_STATUS(mfo, "Error setting the calendar.");

        if( timezone_utf16){
                efree(timezone_utf16);
        }

}

/* {{{ proto unicode DateFormatter::getDateType( )
 * Get formatter datetype. }}} */
/* {{{ proto string datefmt_get_datetype( DateFormatter $mf )
 * Get formatter datetype.
 */
PHP_FUNCTION( datefmt_get_datetype )
{
	DATE_FORMAT_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &object, DateFormatter_ce_ptr ) == FAILURE )
	{
		intl_error_set( NULL , U_ILLEGAL_ARGUMENT_ERROR,	
			"datefmt_get_datetype: unable to parse input params", 0 TSRMLS_CC );
		RETURN_FALSE;
	}

	// Fetch the object.
	DATE_FORMAT_METHOD_FETCH_OBJECT;

	INTL_METHOD_CHECK_STATUS(mfo, "Error getting formatter datetype." );

	RETURN_LONG(mfo->date_type );
}
/* }}} */

/* {{{ proto unicode DateFormatter::getTimeType( )
 * Get formatter timetype. }}} */
/* {{{ proto string datefmt_get_timetype( DateFormatter $mf )
 * Get formatter timetype.
 */
PHP_FUNCTION( datefmt_get_timetype )
{
	DATE_FORMAT_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &object, DateFormatter_ce_ptr ) == FAILURE )
	{
		intl_error_set( NULL , U_ILLEGAL_ARGUMENT_ERROR,	
			"datefmt_get_timetype: unable to parse input params", 0 TSRMLS_CC );
		RETURN_FALSE;
	}

	// Fetch the object.
	DATE_FORMAT_METHOD_FETCH_OBJECT;

	INTL_METHOD_CHECK_STATUS(mfo, "Error getting formatter timetype." );

	RETURN_LONG(mfo->time_type );
}
/* }}} */


/* {{{ proto unicode DateFormatter::getCalendar( )
 * Get formatter calendar. }}} */
/* {{{ proto string datefmt_get_calendar( DateFormatter $mf )
 * Get formatter calendar.
 */
PHP_FUNCTION( datefmt_get_calendar )
{
	DATE_FORMAT_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &object, DateFormatter_ce_ptr ) == FAILURE )
	{
		intl_error_set( NULL , U_ILLEGAL_ARGUMENT_ERROR,	
			"datefmt_get_calendar: unable to parse input params", 0 TSRMLS_CC );
		RETURN_FALSE;
	}

	// Fetch the object.
	DATE_FORMAT_METHOD_FETCH_OBJECT;

	INTL_METHOD_CHECK_STATUS(mfo, "Error getting formatter calendar." );

	RETURN_LONG(mfo->calendar );
}
/* }}} */

/* {{{ proto unicode DateFormatter::getTimeZoneId( )
 * Get formatter timezone_id. }}} */
/* {{{ proto string datefmt_get_timezone_id( DateFormatter $mf )
 * Get formatter timezone_id.
 */
PHP_FUNCTION( datefmt_get_timezone_id )
{
	DATE_FORMAT_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &object, DateFormatter_ce_ptr ) == FAILURE )
	{
		intl_error_set( NULL , U_ILLEGAL_ARGUMENT_ERROR,	
			"datefmt_get_timezone_id: unable to parse input params", 0 TSRMLS_CC );
		RETURN_FALSE;
	}

	// Fetch the object.
	DATE_FORMAT_METHOD_FETCH_OBJECT;

	INTL_METHOD_CHECK_STATUS(mfo, "Error getting formatter timezone_id." );

	if( mfo->timezone_id ){
		RETURN_STRING((char*)mfo->timezone_id ,TRUE );
	}else{
		RETURN_NULL();
	}
}

/* {{{ proto boolean DateFormatter::setTimeZoneId( $timezone_id)
 * Set formatter timezone_id. }}} */
/* {{{ proto boolean datefmt_set_timezone_id( DateFormatter $mf ,$timezone_id)
 * Set formatter timezone_id.
 */
PHP_FUNCTION( datefmt_set_timezone_id )
{
	char* 		timezone_id 		= NULL;
	int 		timezone_id_len 	= 0;

        DATE_FORMAT_METHOD_INIT_VARS;

        // Parse parameters.
        if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &object, DateFormatter_ce_ptr ,&timezone_id , &timezone_id_len) == FAILURE )
        {
                intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
                        "datefmt_set_timezone_id: unable to parse input params", 0 TSRMLS_CC );
                RETURN_FALSE;
        }

        // Fetch the object.
        DATE_FORMAT_METHOD_FETCH_OBJECT;

	//set the timezone for the calendar
	internal_set_calendar( mfo , timezone_id , timezone_id_len , mfo->calendar ,return_value TSRMLS_CC );

	//Set the DateFormatter variable
        if( mfo->timezone_id ){
		efree(mfo->timezone_id);
	}
	mfo->timezone_id = estrndup(timezone_id , timezone_id_len);

	RETURN_TRUE;
}

/* {{{ proto string DateFormatter::getPattern( )
 * Get formatter pattern. }}} */
/* {{{ proto string datefmt_get_pattern( DateFormatter $mf )
 * Get formatter pattern.
 */
PHP_FUNCTION( datefmt_get_pattern )
{
	UChar  value_buf[64];
	int    length = USIZE( value_buf );
	UChar* value  = value_buf;
	zend_bool   is_pattern_localized =FALSE;

	DATE_FORMAT_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &object, DateFormatter_ce_ptr ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,	
			"datefmt_get_pattern: unable to parse input params", 0 TSRMLS_CC );
		RETURN_FALSE;
	}

	// Fetch the object.
	DATE_FORMAT_METHOD_FETCH_OBJECT;

	length = udat_toPattern(DATE_FORMAT_OBJECT(mfo), is_pattern_localized, value, length, &INTL_DATA_ERROR_CODE(mfo));
	if(INTL_DATA_ERROR_CODE(mfo) == U_BUFFER_OVERFLOW_ERROR && length >= USIZE( value_buf )) {
		++length; // to avoid U_STRING_NOT_TERMINATED_WARNING
		INTL_DATA_ERROR_CODE(mfo) = U_ZERO_ERROR;
		value = eumalloc(length);
		length = udat_toPattern(DATE_FORMAT_OBJECT(mfo), is_pattern_localized , value, length, &INTL_DATA_ERROR_CODE(mfo) );
		if(U_FAILURE(INTL_DATA_ERROR_CODE(mfo))) {
			efree(value);
			value = value_buf;
		}
	}
	INTL_METHOD_CHECK_STATUS(mfo, "Error getting formatter pattern" );

	INTL_METHOD_RETVAL_UTF8( mfo, value, length, ( value != value_buf ) );
}
/* }}} */

/* {{{ proto bool DateFormatter::setPattern( string $pattern )
 * Set formatter pattern. }}} */
/* {{{ proto bool datefmt_set_pattern( DateFormatter $mf, string $pattern )
 * Set formatter pattern.
 */
PHP_FUNCTION( datefmt_set_pattern )
{
	char*       value = NULL;
	int         value_len = 0;
	int         slength = 0;
	UChar*	    svalue  = NULL;
	zend_bool   is_pattern_localized =FALSE;


	DATE_FORMAT_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os",
		&object, DateFormatter_ce_ptr,  &value, &value_len ) == FAILURE )
	{
		intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR,	
			"datefmt_set_pattern: unable to parse input params", 0 TSRMLS_CC);
		RETURN_FALSE;
	}

	DATE_FORMAT_METHOD_FETCH_OBJECT;

	// Convert given pattern to UTF-16.
	intl_convert_utf8_to_utf16(&svalue, &slength, value, value_len, &INTL_DATA_ERROR_CODE(mfo));
	INTL_METHOD_CHECK_STATUS(mfo, "Error converting pattern to UTF-16" );

	udat_applyPattern(DATE_FORMAT_OBJECT(mfo), (UBool)is_pattern_localized , svalue, slength);

	efree(svalue);
	INTL_METHOD_CHECK_STATUS(mfo, "Error setting symbol value");

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string DateFormatter::getLocale()
 * Get formatter locale. }}} */
/* {{{ proto string datefmt_get_locale(DateFormatter $mf)
 * Get formatter locale.
 */
PHP_FUNCTION( datefmt_get_locale )
{
	char *loc;
	int  loc_type =ULOC_ACTUAL_LOCALE;

	DATE_FORMAT_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O|l",
		&object, DateFormatter_ce_ptr ,&loc_type) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"datefmt_get_locale: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Fetch the object.
	DATE_FORMAT_METHOD_FETCH_OBJECT;

	loc = (char *)udat_getLocaleByType(DATE_FORMAT_OBJECT(mfo), loc_type ,&INTL_DATA_ERROR_CODE(mfo));
	RETURN_STRING(loc, 1);
}
/* }}} */

/* {{{ proto string DateFormatter::isLenient()
 * Get formatter isLenient. }}} */
/* {{{ proto string datefmt_isLenient(DateFormatter $mf)
 * Get formatter locale.
 */
PHP_FUNCTION( datefmt_is_lenient )
{
	
	DATE_FORMAT_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O",
		&object, DateFormatter_ce_ptr ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"datefmt_is_lenient: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Fetch the object.
	DATE_FORMAT_METHOD_FETCH_OBJECT;

	RETVAL_BOOL(udat_isLenient(DATE_FORMAT_OBJECT(mfo)));
}
/* }}} */

/* {{{ proto string DateFormatter::setLenient()
 * Set formatter lenient. }}} */
/* {{{ proto string datefmt_setLenient(DateFormatter $mf)
 * Set formatter lenient.
 */
PHP_FUNCTION( datefmt_set_lenient )
{

        zend_bool isLenient  = FALSE;

        DATE_FORMAT_METHOD_INIT_VARS;

        // Parse parameters.
        if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ob",
                &object, DateFormatter_ce_ptr ,&isLenient ) == FAILURE )
        {
                intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
                        "datefmt_set_lenient: unable to parse input params", 0 TSRMLS_CC );

                RETURN_FALSE;
        }

        // Fetch the object.
        DATE_FORMAT_METHOD_FETCH_OBJECT;

        udat_setLenient(DATE_FORMAT_OBJECT(mfo) , (UBool)isLenient );
}
/* }}} */

/* {{{ proto bool DateFormatter::setPattern( int $calendar )
 * Set formatter calendar. }}} */
/* {{{ proto bool datefmt_set_calendar( DateFormatter $mf, int $calendar )
 * Set formatter calendar.
 */
PHP_FUNCTION( datefmt_set_calendar )
{
	long	calendar = 0;

        DATE_FORMAT_METHOD_INIT_VARS;

        // Parse parameters.
        if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol",
                &object, DateFormatter_ce_ptr, &calendar ) == FAILURE )
        {
                intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR,
                        "datefmt_set_calendar: unable to parse input params", 0 TSRMLS_CC);
                RETURN_FALSE;
        }

	//check for the validity  of value of calendar passed
	intl_error_reset( NULL TSRMLS_CC );
	if( calendar > 1){
                intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR,
                        "datefmt_set_calendar: calendar value specified is out of valid range", 0 TSRMLS_CC);
                RETURN_FALSE;
	}

        DATE_FORMAT_METHOD_FETCH_OBJECT;

	internal_set_calendar( mfo , mfo->timezone_id , strlen(mfo->timezone_id) , calendar ,return_value TSRMLS_CC );

	//Set the calendar  value in the DateFormatter object
        mfo->calendar = calendar ;

        RETURN_TRUE;
}
/* }}} */


