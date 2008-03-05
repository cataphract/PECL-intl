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

#include <unicode/ustring.h>
#include <unicode/ucal.h>

#include "php_intl.h"
#include "intl_convert.h"
#include "dateformat.h"
#include "dateformat_class.h"
#include "dateformat_format.h"
#include "dateformat_data.h"

#define TIMESTAMP_ARG 0
#define ARRAY_ARG 1
#define DATETIME_OBJ_ARG 2
#define BUFFER_SIZE 512

/* {{{ 
 * Internal function which calls the udat_format
*/
static void internal_format(DateFormatter_object *mfo, double timestamp , zval *return_value TSRMLS_DC){
	UChar* 	formatted =  NULL;
	int	resultlengthneeded =0 ;

	resultlengthneeded=udat_format( DATE_FORMAT_OBJECT(mfo), timestamp, NULL, resultlengthneeded, NULL, &INTL_DATA_ERROR_CODE(mfo));
	if(INTL_DATA_ERROR_CODE(mfo)==U_BUFFER_OVERFLOW_ERROR)
	{
		INTL_DATA_ERROR_CODE(mfo)=U_ZERO_ERROR;
		formatted=(UChar*)emalloc(sizeof(UChar) * resultlengthneeded); 
		udat_format( DATE_FORMAT_OBJECT(mfo), timestamp, formatted, resultlengthneeded, NULL, &INTL_DATA_ERROR_CODE(mfo));
	}

	if (formatted && U_FAILURE( INTL_DATA_ERROR_CODE(mfo) ) ) {
			efree(formatted);
	}

	INTL_METHOD_CHECK_STATUS( mfo, "Date formatting failed" );
	INTL_METHOD_RETVAL_UTF8( mfo, formatted, resultlengthneeded, 0 );
	if (formatted ) {
		efree(formatted);
	}

}
/* }}} */


/* {{{ 
 * Internal function which fetches an element from the passed array for the key_name passed 
*/
static double internal_get_arr_ele(DateFormatter_object *mfo  , HashTable* hash_arr  ,char* key_name TSRMLS_DC){
	zval**  ele_value       = NULL;
	int result = -1;

        if( zend_hash_find( hash_arr , key_name , strlen(key_name) + 1 ,(void **)&ele_value ) == SUCCESS ){
                if( Z_TYPE_PP(ele_value)!= IS_LONG ){
                        //key_value is not a string
			intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
				"datefmt_format: parameter array does not contain an integer or double element", 0 TSRMLS_CC );
                }else{
			result =  Z_LVAL_PP(ele_value);
		}
	}
	return result;
}
/* }}} */

/* {{{ 
 * Internal function which creates a UCalendar  from the passed array
*/
static void internal_create_ucal(DateFormatter_object *mfo, HashTable* hash_arr , UCalendar* pcal  TSRMLS_DC){
	int year =0;
	int month =0;
	int hour =0;
	int minute =0;
	int second =0;
	int wday =0;
	int yday =0;
	int mday =0;
	UBool isInDST = FALSE;

	//Fetch  values from the incoming array
	year = internal_get_arr_ele( mfo , hash_arr , CALENDAR_YEAR TSRMLS_CC);
	month = internal_get_arr_ele( mfo , hash_arr , CALENDAR_MON TSRMLS_CC);
	hour = internal_get_arr_ele( mfo , hash_arr , CALENDAR_HOUR TSRMLS_CC);
	minute = internal_get_arr_ele( mfo , hash_arr , CALENDAR_MIN TSRMLS_CC);
	second = internal_get_arr_ele( mfo , hash_arr , CALENDAR_SEC TSRMLS_CC);
	wday = internal_get_arr_ele( mfo , hash_arr , CALENDAR_WDAY TSRMLS_CC);
	yday = internal_get_arr_ele( mfo , hash_arr , CALENDAR_YDAY TSRMLS_CC);
	isInDST = internal_get_arr_ele( mfo , hash_arr , CALENDAR_ISDST TSRMLS_CC);
	//For the ucal_setDateTime() function , this is the 'date'  value
	mday = internal_get_arr_ele( mfo , hash_arr , CALENDAR_MDAY TSRMLS_CC);

	//set the incoming values for the calendar 	
	ucal_setDateTime( pcal, year, month  , mday , hour , minute , second , &INTL_DATA_ERROR_CODE(mfo));
	if( INTL_DATA_ERROR_CODE(mfo) != U_ZERO_ERROR){
		return;
	}
	ucal_set( pcal, UCAL_DAY_OF_WEEK , wday);
	ucal_set( pcal, UCAL_DAY_OF_YEAR , yday);
	
	//TO DO :How to set the isInDST field?Is it required to set
}


/* {{{ proto string DateFormatter::format( [mixed]int $args or array $args or DateTime $args)
 * Format the time value as a string. }}}*/
/* {{{ proto string datefmt_format( [mixed]int $args or array $args or DateTime $args)
 * Format the time value as a string. }}}*/
PHP_FUNCTION(datefmt_format) 
{
	double 		timestamp =0;
	zval*		datetime_obj = NULL;
	UCalendar*    	temp_cal ; 
	HashTable*      hash_arr        = NULL;
	zval*		zarg	= NULL;

	DATE_FORMAT_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Oz", &object, DateFormatter_ce_ptr ,&zarg ) == FAILURE )
        {
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			 "datefmt_format: unable to parse input params", 0 TSRMLS_CC );
                RETURN_FALSE;
        }


	// Fetch the object.
	DATE_FORMAT_METHOD_FETCH_OBJECT;

	switch(Z_TYPE_P(zarg) ){
		case IS_LONG:
			timestamp = Z_LVAL_P(zarg);
			break;
		case IS_ARRAY:
			hash_arr = Z_ARRVAL_P(zarg);
			if( !hash_arr || zend_hash_num_elements( hash_arr ) == 0 )
				RETURN_FALSE;
			//Create a UCalendar object from the array and then format it
			temp_cal = ucal_open(NULL, -1, NULL, UCAL_GREGORIAN, &INTL_DATA_ERROR_CODE(mfo));
			ucal_clear(temp_cal);
			INTL_METHOD_CHECK_STATUS( mfo, "datefmt_format: Date formatting failed while creating calendar from the  array" )
			internal_create_ucal( mfo ,  hash_arr , temp_cal TSRMLS_CC);
			INTL_METHOD_CHECK_STATUS( mfo, "datefmt_format: Date formatting failed while creating calendar from the  array" )
			//Fetch the timestamp from the  created UCalendar
			timestamp  = ucal_getMillis(temp_cal  , &INTL_DATA_ERROR_CODE(mfo) );
			INTL_METHOD_CHECK_STATUS( mfo, "datefmt_format: Date formatting failed" )
			break;
/*
		case IS_OBJECT:
			break;
*/
		default:
			intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
				"datefmt_format: takes either an array  or a DateTime object or an integer TimeStamp value ", 0 TSRMLS_CC );
			// drop the temporary formatter
			dateformat_data_free(&mfo->datef_data TSRMLS_CC);
			RETURN_FALSE;
	}

	internal_format( mfo, timestamp ,return_value TSRMLS_CC);
	
	// drop the temporary formatter
	dateformat_data_free(&mfo->datef_data TSRMLS_CC);
}

/* }}} */

