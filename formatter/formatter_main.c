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

#include <unicode/ustring.h>

#include "php_intl.h"
#include "formatter_class.h"
#include "intl_convert.h"

/* {{{ proto NumberFormatter NumberFormatter::create( string $locale, int style[, string $pattern ] )
 * Create formatter. }}} */
/* {{{ proto NumberFormatter numfmt_create( string $locale, int style[, string $pattern ] )
 * Create formatter.
 */
PHP_FUNCTION( numfmt_create )
{
	char*       locale;
	UChar*      pattern;
	int         locale_len = 0, style, pattern_len = 0;
	zval*       object;
	NumberFormatter_object* nfo;

	intl_error_reset( NULL TSRMLS_CC );

	// Parse parameters.
	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "sl|u",
		&locale, &locale_len, &style, &pattern, &pattern_len ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"numfmt_create: unable to parse input parameters", 0 TSRMLS_CC );

		RETURN_NULL();
	}

	// Create a NumberFormatter object and save the ICU formatter into it.
	if( ( object = getThis() ) == NULL )
		object = return_value;

	if( Z_TYPE_P( object ) != IS_OBJECT )
		object_init_ex( object, NumberFormatter_ce_ptr );

	nfo = (NumberFormatter_object *) zend_object_store_get_object( object TSRMLS_CC );

	intl_error_reset( &nfo->nf_data.error TSRMLS_CC );

	if(locale_len == 0) {
		locale = INTL_G(current_locale);
	}

	nfo->nf_data.unum = unum_open(style, pattern, pattern_len, locale, NULL, &FORMATTER_ERROR_CODE(nfo));

	if( U_FAILURE( FORMATTER_ERROR_CODE((nfo)) ) )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"numfmt_create: number formatter creation failed", 0 TSRMLS_CC );

		RETURN_NULL();
	}
}
/* }}} */

/* {{{ proto void NumberFormatter::__construct( string $locale, int style[, string $pattern ] )
 * NumberFormatter object constructor.
 */
PHP_METHOD( NumberFormatter, __construct )
{
	char*       locale;
	UChar*      pattern;
	int         locale_len = 0, style, pattern_len = 0;
	zval*       object;
	NumberFormatter_object* nfo;

	intl_error_reset( NULL TSRMLS_CC );

	// Parse parameters.
	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "sl|u",
		&locale, &locale_len, &style, &pattern, &pattern_len ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"__construct: unable to parse input params", 0 TSRMLS_CC );

		RETURN_NULL();
	}

	object = getThis();
	nfo = (NumberFormatter_object *) zend_object_store_get_object( object TSRMLS_CC );

	intl_error_reset( &nfo->nf_data.error TSRMLS_CC );

	if(locale_len == 0) {
		locale = UG(default_locale);
	}

	// Create an ICU number formatter.
	nfo->nf_data.unum = unum_open(style, pattern, pattern_len, locale, NULL, &FORMATTER_ERROR_CODE(nfo));

	if( U_FAILURE( FORMATTER_ERROR_CODE((nfo)) ) )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"__construct: number formatter creation failed", 0 TSRMLS_CC );

		RETURN_NULL();
	}
}
/* }}} */

/* {{{ proto int NumberFormatter::getErrorCode()
 * Get formatter's last error code. }}} */
/* {{{ proto int numfmt_get_error_code( NumberFormatter $nf )
 * Get formatter's last error code.
 */
PHP_FUNCTION( numfmt_get_error_code )
{
	zval*                    object  = NULL;
	NumberFormatter_object*  nfo     = NULL;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O",
		&object, NumberFormatter_ce_ptr ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"numfmt_get_error_code: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	nfo = (NumberFormatter_object *) zend_object_store_get_object( object TSRMLS_CC );

	// Return formatter's last error code.
	RETURN_LONG( FORMATTER_ERROR_CODE(nfo) );
}
/* }}} */

/* {{{ proto string NumberFormatter::getErrorMessage( )
 * Get text description for formatter's last error code. }}} */
/* {{{ proto string numfmt_get_error_message( NumberFormatter $coll )
 * Get text description for formatter's last error code.
 */
PHP_FUNCTION( numfmt_get_error_message )
{
	char*                    message = NULL;
	zval*                    object  = NULL;
	NumberFormatter_object*  nfo     = NULL;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O",
		&object, NumberFormatter_ce_ptr ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"numfmt_get_error_message: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	if(locale_len == 0) {
		locale = UG(default_locale);
	}

	// Create an ICU number formatter.
	nfo = (NumberFormatter_object *) zend_object_store_get_object( object TSRMLS_CC );

	// Return last error message.
	message = intl_error_get_message( &nfo->nf_data.error TSRMLS_CC );
	RETURN_STRING( message, 0);
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
