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

#include <unicode/unum.h>

#include "formatter_class.h"
#include "php_intl.h"
#include "formatter_data.h"
#include "formatter_format.h"
#include "formatter_parse.h"
#include "formatter_main.h"
#include "formatter_attr.h"

zend_class_entry *NumberFormatter_ce_ptr = NULL;

/////////////////////////////////////////////////////////////////////////////
// Auxiliary functions needed by objects of 'NumberFormatter' class
/////////////////////////////////////////////////////////////////////////////

/* {{{ NumberFormatter_objects_dtor */
static void NumberFormatter_object_dtor(
	void *object,
	zend_object_handle handle TSRMLS_DC )
{
	zend_objects_destroy_object( object, handle TSRMLS_CC );
}
/* }}} */

/* {{{ NumberFormatter_objects_free */
void NumberFormatter_object_free( zend_object *object TSRMLS_DC )
{
	NumberFormatter_object* nfo = (NumberFormatter_object*)object;

	zend_object_std_dtor( &nfo->zo TSRMLS_CC );

	formatter_data_free( &nfo->nf_data );

	efree( nfo );
}
/* }}} */

/* {{{ NumberFormatter_object_create */
zend_object_value NumberFormatter_object_create(
	zend_class_entry *ce TSRMLS_DC )
{
	zend_object_value    retval;
	NumberFormatter_object*     intern;

	intern = ecalloc( 1, sizeof(NumberFormatter_object) );
	formatter_data_init(&intern->nf_data);
	zend_object_std_init( &intern->zo, ce TSRMLS_CC );

	retval.handle = zend_objects_store_put(
		intern,
		NumberFormatter_object_dtor,
		(zend_objects_free_object_storage_t)NumberFormatter_object_free,
		NULL );

	retval.handlers = zend_get_std_object_handlers();

	return retval;
}
/* }}} */

/////////////////////////////////////////////////////////////////////////////
// 'NumberFormatter' class registration structures & functions
/////////////////////////////////////////////////////////////////////////////

/* {{{ NumberFormatter_class_functions
 * Every 'NumberFormatter' class method has an entry in this table
 */
static ZEND_BEGIN_ARG_INFO_EX( number_parse_arginfo, 0, 0, 1 )
	ZEND_ARG_INFO( 0, string )
	ZEND_ARG_INFO( 0, type )
	ZEND_ARG_INFO( 1, position )
ZEND_END_ARG_INFO()

static ZEND_BEGIN_ARG_INFO_EX( number_parse_currency_arginfo, 0, 0, 2 )
	ZEND_ARG_INFO( 0, string )
	ZEND_ARG_INFO( 1, currency )
	ZEND_ARG_INFO( 1, position )
ZEND_END_ARG_INFO()

static function_entry NumberFormatter_class_functions[] = {
	PHP_ME( NumberFormatter, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR )
	ZEND_FENTRY( create, ZEND_FN( numfmt_create ), NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	PHP_NAMED_FE( format, ZEND_FN( numfmt_format ), NULL )
	PHP_NAMED_FE( parse, ZEND_FN( numfmt_parse ), number_parse_arginfo )
	PHP_NAMED_FE( formatCurrency, ZEND_FN( numfmt_format_currency ), NULL )
	PHP_NAMED_FE( parseCurrency, ZEND_FN( numfmt_parse_currency ), number_parse_currency_arginfo )
	PHP_NAMED_FE( setAttribute, ZEND_FN( numfmt_set_attribute ), NULL )
	PHP_NAMED_FE( getAttribute, ZEND_FN( numfmt_get_attribute ), NULL )
	PHP_NAMED_FE( setTextAttribute, ZEND_FN( numfmt_set_text_attribute ), NULL )
	PHP_NAMED_FE( getTextAttribute, ZEND_FN( numfmt_get_text_attribute ), NULL )
	PHP_NAMED_FE( setSymbol, ZEND_FN( numfmt_set_symbol ), NULL )
	PHP_NAMED_FE( getSymbol, ZEND_FN( numfmt_get_symbol ), NULL )
	PHP_NAMED_FE( setPattern, ZEND_FN( numfmt_set_pattern ), NULL )
	PHP_NAMED_FE( getPattern, ZEND_FN( numfmt_get_pattern ), NULL )
	PHP_NAMED_FE( getLocale, ZEND_FN( numfmt_get_locale ), NULL )
	PHP_NAMED_FE( getErrorCode, ZEND_FN( numfmt_get_error_code ), NULL )
	PHP_NAMED_FE( getErrorMessage, ZEND_FN( numfmt_get_error_message ), NULL )
	{ NULL, NULL, NULL }
};
/* }}} */

/* {{{ formatter_register_class
 * Initialize 'NumberFormatter' class
 */
void formatter_register_class()
{
	zend_class_entry ce;

	// Create and register 'NumberFormatter' class.
	INIT_CLASS_ENTRY( ce, "NumberFormatter", NumberFormatter_class_functions );
	ce.create_object = NumberFormatter_object_create;
	NumberFormatter_ce_ptr = zend_register_internal_class( &ce TSRMLS_CC );

	// Declare 'NumberFormatter' class properties.
	if( !NumberFormatter_ce_ptr )
	{
		zend_error(E_ERROR, "Failed to register NumberFormatter class");
		return;
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