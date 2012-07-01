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
   | Authors: Gustavo Lopes <cataphract@php.net>                          |
   +----------------------------------------------------------------------+
*/

#include "../intl_cppshims.h"

#include <unicode/calendar.h>

extern "C" {
#include "../php_intl.h"
#define USE_CALENDAR_POINTER 1
#include "../calendar/calendar_class.h"
#include <ext/date/php_date.h>
}

U_CFUNC double intl_zval_to_millis(zval *z, UErrorCode *status TSRMLS_DC)
{
	double rv = NAN;
	long lv;
	int type;

	if (U_FAILURE(*status)) {
		return NAN;
	}

	switch (Z_TYPE_P(z)) {
	case IS_STRING:
		type = is_numeric_string(Z_STRVAL_P(z), Z_STRLEN_P(z), &lv, &rv, 0);
		if (type == IS_DOUBLE) {
			rv *= U_MILLIS_PER_SECOND;
		} else if (type == IS_LONG) {
			rv = U_MILLIS_PER_SECOND * (double)lv;
		} else {
			*status = U_ILLEGAL_ARGUMENT_ERROR;
		}
		break;
	case IS_LONG:
		rv = U_MILLIS_PER_SECOND * (double)Z_LVAL_P(z);
		break;
	case IS_DOUBLE:
		rv = U_MILLIS_PER_SECOND * Z_DVAL_P(z);
		break;
	case IS_OBJECT:
		if (instanceof_function(Z_OBJCE_P(z), php_date_get_date_ce() TSRMLS_CC)) {
			zval retval = zval_used_for_init,
				 zfuncname = zval_used_for_init,
				 *ts_format_p;
#if PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION < 3
			zval ts_format = zval_used_for_init;
#endif
			int  argcount,
				 expected_type;
			
#if PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION < 3
			ZVAL_STRING(&zfuncname, "format", 0);
			ZVAL_STRINGL(&ts_format, "U", sizeof("U") - 1, 0);
			argcount = 1;
			ts_format_p = &ts_format;
			expected_type = IS_STRING;
#else
			ZVAL_STRING(&zfuncname, "getTimestamp", 0);
			argcount = 0;
			ts_format_p = NULL;
			expected_type = IS_LONG;
#endif
			
			if (call_user_function(NULL, &z, &zfuncname, &retval, argcount, &ts_format_p TSRMLS_CC) != SUCCESS
					|| Z_TYPE(retval) != expected_type) {
				*status = U_ILLEGAL_ARGUMENT_ERROR;
			} else {
#if PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION < 3
				convert_to_long(&retval);
#endif			
				rv = U_MILLIS_PER_SECOND * (double)Z_LVAL(retval);
			}
		} else if (instanceof_function(Z_OBJCE_P(z), Calendar_ce_ptr TSRMLS_CC)) {
			Calendar_object *co = (Calendar_object *)
				zend_object_store_get_object(z TSRMLS_CC );
			if (co->ucal == NULL) {
				*status = U_ILLEGAL_ARGUMENT_ERROR;
			} else {
				rv = (double)co->ucal->getTime(*status);
			}
		} else {
			/* TODO: try with cast(), get() to obtain a number */
			*status = U_ILLEGAL_ARGUMENT_ERROR;
		}
		break;
	default:
		*status = U_ILLEGAL_ARGUMENT_ERROR;
	}

	return rv;
}

