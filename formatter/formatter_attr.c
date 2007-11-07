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
   | Authors: Stanislav Malyshev <stas@zend.com>                          |
   +----------------------------------------------------------------------+
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_intl.h"
#include "formatter_class.h"
#include "formatter_attr.h"

#include <unicode/ustring.h>

/* {{{ proto mixed NumberFormatter::getAttribute( int $attr )
 * Get formatter attribute value. }}} */
/* {{{ proto mixed numfmt_get_attribute( NumberFormatter $nf, int $attr )
 * Get formatter attribute value.
 */
PHP_FUNCTION( numfmt_get_attribute )
{
	long attribute, value;
	FORMATTER_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol",
		&object, NumberFormatter_ce_ptr, &attribute ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"numfmt_get_attribute: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Fetch the object.
	FORMATTER_METHOD_FETCH_OBJECT;

	switch(attribute) {
		case UNUM_PARSE_INT_ONLY:
		case UNUM_GROUPING_USED:
		case UNUM_DECIMAL_ALWAYS_SHOWN:
		case UNUM_MAX_INTEGER_DIGITS:
		case UNUM_MIN_INTEGER_DIGITS:
		case UNUM_INTEGER_DIGITS:
		case UNUM_MAX_FRACTION_DIGITS:
		case UNUM_MIN_FRACTION_DIGITS:
		case UNUM_FRACTION_DIGITS:
		case UNUM_MULTIPLIER:
		case UNUM_GROUPING_SIZE:
		case UNUM_ROUNDING_MODE:
		case UNUM_FORMAT_WIDTH:
		case UNUM_PADDING_POSITION:
		case UNUM_SECONDARY_GROUPING_SIZE:
		case UNUM_SIGNIFICANT_DIGITS_USED:
		case UNUM_MIN_SIGNIFICANT_DIGITS:
		case UNUM_MAX_SIGNIFICANT_DIGITS:
		case UNUM_LENIENT_PARSE:
			value = unum_getAttribute(FORMATTER_OBJECT(nfo), attribute);
			if(value == -1) {
				INTL_DATA_ERROR_CODE(nfo) = U_UNSUPPORTED_ERROR;
			} else {
				RETVAL_LONG(value);
			}
			break;
		case UNUM_ROUNDING_INCREMENT:
		{
			double value = unum_getDoubleAttribute(FORMATTER_OBJECT(nfo), attribute);
			if(value == -1) {
				INTL_DATA_ERROR_CODE(nfo) = U_UNSUPPORTED_ERROR;
			} else {
				RETVAL_DOUBLE(value);
			}
		}
			break;
		default:
			INTL_DATA_ERROR_CODE(nfo) = U_UNSUPPORTED_ERROR;
			break;
	}

	INTL_METHOD_CHECK_STATUS( nfo, "Error getting attribute value" );
}
/* }}} */

/* {{{ proto string NumberFormatter::getTextAttribute( int $attr )
 * Get formatter attribute value. }}} */
/* {{{ proto string numfmt_get_text_attribute( NumberFormatter $nf, int $attr )
 * Get formatter attribute value.
 */
PHP_FUNCTION( numfmt_get_text_attribute )
{
	long   attribute;
	UChar  value_buf[64];
	int    value_buf_size = USIZE( value_buf );
	UChar* value  = value_buf;
	int    length = 0;
	FORMATTER_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol",
		&object, NumberFormatter_ce_ptr, &attribute ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"numfmt_get_text_attribute: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Fetch the object.
	FORMATTER_METHOD_FETCH_OBJECT;

	length = unum_getTextAttribute( FORMATTER_OBJECT(nfo), attribute, value, value_buf_size, &INTL_DATA_ERROR_CODE(nfo) );

	INTL_METHOD_CHECK_STATUS( nfo, "Error getting attribute value" );
	if( length >= value_buf_size )
	{
		++length; // to avoid U_STRING_NOT_TERMINATED_WARNING
		value = eumalloc( length );

		intl_error_reset( INTL_DATA_ERROR_P(nfo) TSRMLS_CC );
		length = unum_getTextAttribute( FORMATTER_OBJECT(nfo), attribute, value, length, &INTL_DATA_ERROR_CODE(nfo) );

		if( U_FAILURE( INTL_DATA_ERROR_CODE((nfo)) ) )
		{
			intl_error_set_code( NULL, INTL_DATA_ERROR_CODE((nfo)) TSRMLS_CC );
			intl_errors_set_custom_msg( INTL_DATA_ERROR_P(nfo), "Error getting attribute value", 0 TSRMLS_CC );
			efree( value );
			RETURN_FALSE;
		}
	}

	RETVAL_UNICODEL( value, length, ( value == value_buf ) );
}
/* }}} */

/* {{{ proto bool NumberFormatter::setAttribute( int $attr, mixed $value )
 * Get formatter attribute value. }}} */
/* {{{ proto bool numfmt_set_attribute( NumberFormatter $nf, int $attr, mixed $value )
 * Get formatter attribute value.
 */
PHP_FUNCTION( numfmt_set_attribute )
{
	long attribute;
	zval **value;
	FORMATTER_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "OlZ",
		&object, NumberFormatter_ce_ptr, &attribute, &value ) == FAILURE)
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"numfmt_set_attribute: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Fetch the object.
	FORMATTER_METHOD_FETCH_OBJECT;

	switch(attribute) {
		case UNUM_PARSE_INT_ONLY:
		case UNUM_GROUPING_USED:
		case UNUM_DECIMAL_ALWAYS_SHOWN:
		case UNUM_MAX_INTEGER_DIGITS:
		case UNUM_MIN_INTEGER_DIGITS:
		case UNUM_INTEGER_DIGITS:
		case UNUM_MAX_FRACTION_DIGITS:
		case UNUM_MIN_FRACTION_DIGITS:
		case UNUM_FRACTION_DIGITS:
		case UNUM_MULTIPLIER:
		case UNUM_GROUPING_SIZE:
		case UNUM_ROUNDING_MODE:
		case UNUM_FORMAT_WIDTH:
		case UNUM_PADDING_POSITION:
		case UNUM_SECONDARY_GROUPING_SIZE:
		case UNUM_SIGNIFICANT_DIGITS_USED:
		case UNUM_MIN_SIGNIFICANT_DIGITS:
		case UNUM_MAX_SIGNIFICANT_DIGITS:
		case UNUM_LENIENT_PARSE:
			convert_to_long_ex(value);
			unum_setAttribute(FORMATTER_OBJECT(nfo), attribute, Z_LVAL_PP(value));
			break;
		case UNUM_ROUNDING_INCREMENT:
			convert_to_double_ex(value);
			unum_setDoubleAttribute(FORMATTER_OBJECT(nfo), attribute, Z_DVAL_PP(value));
			break;
		default:
			INTL_DATA_ERROR_CODE(nfo) = U_UNSUPPORTED_ERROR;
			break;
	}

	INTL_METHOD_CHECK_STATUS( nfo, "Error setting attribute value" );

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool NumberFormatter::setTextAttribute( int $attr, string $value )
 * Get formatter attribute value. }}} */
/* {{{ proto bool numfmt_set_text_attribute( NumberFormatter $nf, int $attr, string $value )
 * Get formatter attribute value.
 */
PHP_FUNCTION( numfmt_set_text_attribute )
{
	long attribute;
	UChar *value;
	int len;
	FORMATTER_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Olu",
		&object, NumberFormatter_ce_ptr, &attribute, &value, &len ) == FAILURE)
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"numfmt_set_text_attribute: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Fetch the object.
	FORMATTER_METHOD_FETCH_OBJECT;

	// Actually set new attribute value.
	unum_setTextAttribute(FORMATTER_OBJECT(nfo), attribute, value, len, &INTL_DATA_ERROR_CODE(nfo));
	INTL_METHOD_CHECK_STATUS( nfo, "Error setting text attribute" );

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string NumberFormatter::getSymbol( int $attr )
 * Get formatter symbol value. }}} */
/* {{{ proto string numfmt_get_symbol( NumberFormatter $nf, int $attr )
 * Get formatter symbol value.
 */
PHP_FUNCTION( numfmt_get_symbol )
{
	long symbol;
	UChar value[4];
	UChar *value_buf = value;
	int length = USIZE(value);
	FORMATTER_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol",
		&object, NumberFormatter_ce_ptr, &symbol ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"numfmt_get_symbol: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Fetch the object.
	FORMATTER_METHOD_FETCH_OBJECT;

	length = unum_getSymbol(FORMATTER_OBJECT(nfo), symbol, value_buf, length, &INTL_DATA_ERROR_CODE(nfo));
	INTL_METHOD_CHECK_STATUS( nfo, "Error getting symbol value" );
	if(length >= USIZE(value)) {
		++length; // to avoid U_STRING_NOT_TERMINATED_WARNING
		value_buf = eumalloc(length);
		length = unum_getSymbol(FORMATTER_OBJECT(nfo), symbol, value_buf, length, &INTL_DATA_ERROR_CODE(nfo));
		INTL_METHOD_CHECK_STATUS( nfo, "Error getting symbol value" );
	}

	RETVAL_UNICODEL( value_buf, length, ( value_buf == value ) );
}
/* }}} */

/* {{{ proto bool NumberFormatter::setSymbol( int $attr, string $symbol )
 * Set formatter symbol value. }}} */
/* {{{ proto bool numfmt_set_symbol( NumberFormatter $nf, int $attr, string $symbol )
 * Set formatter symbol value.
 */
PHP_FUNCTION( numfmt_set_symbol )
{
	long       symbol;
	UChar*     value     = NULL;
	int        value_len = 0;
	FORMATTER_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Olu",
		&object, NumberFormatter_ce_ptr, &symbol, &value, &value_len ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"numfmt_set_symbol: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Fetch the object.
	FORMATTER_METHOD_FETCH_OBJECT;

	// Actually set the symbol.
	unum_setSymbol(FORMATTER_OBJECT(nfo), symbol, value, value_len, &INTL_DATA_ERROR_CODE(nfo));
	INTL_METHOD_CHECK_STATUS( nfo, "Error setting symbol value" );

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string NumberFormatter::getPattern( )
 * Get formatter pattern. }}} */
/* {{{ proto string numfmt_get_pattern( NumberFormatter $nf )
 * Get formatter pattern.
 */
PHP_FUNCTION( numfmt_get_pattern )
{
	UChar  value_buf[64];
	int    length = USIZE( value_buf );
	UChar* value  = value_buf;
	FORMATTER_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O",
		&object, NumberFormatter_ce_ptr ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"numfmt_get_pattern: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Fetch the object.
	FORMATTER_METHOD_FETCH_OBJECT;

	length = unum_toPattern(FORMATTER_OBJECT(nfo), 0, value, length, &INTL_DATA_ERROR_CODE(nfo));
	INTL_METHOD_CHECK_STATUS( nfo, "Error getting formatter pattern" );

	if( length >= USIZE( value_buf ) )
	{
		value = eumalloc( length );
		unum_toPattern( FORMATTER_OBJECT(nfo), 0, value_buf, length, &INTL_DATA_ERROR_CODE(nfo) );
		INTL_METHOD_CHECK_STATUS( nfo, "Error getting formatter pattern" );
	}

	RETVAL_UNICODEL( value_buf, length, ( value == value_buf ) );
}
/* }}} */

/* {{{ proto bool NumberFormatter::setPattern( string $pattern )
 * Set formatter pattern. }}} */
/* {{{ proto bool numfmt_set_pattern( NumberFormatter $nf, string $pattern )
 * Set formatter pattern.
 */
PHP_FUNCTION( numfmt_set_pattern )
{
	UChar*      value = NULL;
	int         value_len = 0;
	FORMATTER_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ou",
		&object, NumberFormatter_ce_ptr, &value, &value_len ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"numfmt_set_pattern: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Fetch the object.
	FORMATTER_METHOD_FETCH_OBJECT;

	unum_applyPattern(FORMATTER_OBJECT(nfo), 0, value, value_len, NULL, &INTL_DATA_ERROR_CODE(nfo));
	INTL_METHOD_CHECK_STATUS( nfo, "Error setting symbol value" );

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string NumberFormatter::getLocale([int type])
 * Get formatter locale. }}} */
/* {{{ proto string numfmt_get_locale( NumberFormatter $nf[, int type] )
 * Get formatter locale.
 */
PHP_FUNCTION( numfmt_get_locale )
{
	long type = ULOC_ACTUAL_LOCALE;
	char* loc;
	FORMATTER_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O|l",
		&object, NumberFormatter_ce_ptr, &type ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"numfmt_get_locale: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Fetch the object.
	FORMATTER_METHOD_FETCH_OBJECT;

	loc = (char *)unum_getLocaleByType(FORMATTER_OBJECT(nfo), type, &INTL_DATA_ERROR_CODE(nfo));
	INTL_METHOD_CHECK_STATUS( nfo, "Error getting locale" );
	RETURN_STRING(loc, 1);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
