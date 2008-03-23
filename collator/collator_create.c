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
   | Authors: Vadim Savchuk <vsavchuk@productengine.com>                  |
   |          Dmitry Lakhtyuk <dlakhtyuk@productengine.com>               |
   +----------------------------------------------------------------------+
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_intl.h"
#include "collator_class.h"
#include "collator_create.h"
#include "intl_data.h"

/* {{{ proto Collator collator_create( string $locale )
 * Create collator.
 */
PHP_FUNCTION( collator_create )
{
	char*            locale;
	int              locale_len = 0;
	zval*            object;
	Collator_object* co;

	intl_error_reset( NULL TSRMLS_CC );

	// Parse parameters.
	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s",
		&locale, &locale_len ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"collator_create: unable to parse input params", 0 TSRMLS_CC );

		RETURN_NULL();
	}

	INTL_CHECK_LOCALE_LEN(locale_len);
	// Create a Collator object and save the ICU collator into it.
	if( ( object = getThis() ) == NULL )
		object = return_value;

	if( Z_TYPE_P( object ) != IS_OBJECT )
		object_init_ex( object, Collator_ce_ptr );

	co = (Collator_object *) zend_object_store_get_object( object TSRMLS_CC );

	intl_error_reset( COLLATOR_ERROR_P( co ) TSRMLS_CC );

	if(locale_len == 0) {
		locale = UG(default_locale);
	}

	// Open ICU collator.
	co->ucoll = ucol_open( locale, COLLATOR_ERROR_CODE_P( co ) );

	if( U_FAILURE( COLLATOR_ERROR_CODE( co ) ) || co->ucoll == NULL )
	{
		intl_error_set( NULL, COLLATOR_ERROR_CODE( co ),
			"collator_create: unable to open ICU collator", 0 TSRMLS_CC );

		// Collator creation failed.
		RETURN_NULL();
	}
}
/* }}} */

/* {{{ proto Collator Collator::__construct( string $locale )
 * Collator object constructor.
 */
PHP_METHOD( Collator, __construct )
{
	char* locale     = NULL;
	int   locale_len = 0;

	COLLATOR_METHOD_INIT_VARS

	object = getThis();
	// Parse parameters.
	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s",
		&locale, &locale_len ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"__construct: unable to parse input params", 0 TSRMLS_CC );

		zval_dtor(object);
		ZVAL_NULL(object);
		RETURN_NULL();
	}

	INTL_CHECK_LOCALE_LEN_OBJ(locale_len, object);
	/* Fetch the object. */
	co  = (Collator_object*) zend_object_store_get_object( object TSRMLS_CC );

	intl_error_reset( COLLATOR_ERROR_P( co ) TSRMLS_CC );

	if(locale_len == 0) {
		locale = UG(default_locale);
	}

	// Open ICU collator.
	co->ucoll = ucol_open( locale, COLLATOR_ERROR_CODE_P( co ) );

	if( U_FAILURE( COLLATOR_ERROR_CODE( co ) ) || co->ucoll == NULL )
	{
		intl_error_set( NULL, COLLATOR_ERROR_CODE( co ),
			"__construct: unable to open ICU collator", 0 TSRMLS_CC );

		zval_dtor(object);
		ZVAL_NULL(object);
		RETURN_NULL();
	}
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
