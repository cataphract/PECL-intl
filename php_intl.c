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
   |          Stanislav Malyshev <stas@zend.com>                          |
   +----------------------------------------------------------------------+
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_intl.h"
#include "intl_error.h"
#include "collator/collator_class.h"
#include "collator/collator.h"
#include "collator/collator_attr.h"
#include "collator/collator_compare.h"
#include "collator/collator_sort.h"
#include "collator/collator_convert.h"
#include "collator/collator_locale.h"
#include "collator/collator_create.h"
#include "collator/collator_error.h"

#include "formatter/formatter.h"
#include "formatter/formatter_class.h"
#include "formatter/formatter_attr.h"
#include "formatter/formatter_format.h"
#include "formatter/formatter_main.h"
#include "formatter/formatter_parse.h"

#include "common/common_error.h"

#include <ext/standard/info.h>

#define INTL_MODULE_VERSION "0.1a"

ZEND_DECLARE_MODULE_GLOBALS( intl )

/* {{{ Arguments info */
static
ZEND_BEGIN_ARG_INFO_EX( collator_static_0_args, 0, 0, 0 )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( collator_static_1_arg, 0, 0, 1 )
	ZEND_ARG_INFO( 0, arg1 )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( collator_static_2_args, 0, 0, 2 )
	ZEND_ARG_INFO( 0, arg1 )
	ZEND_ARG_INFO( 0, arg2 )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( collator_0_args, 0, 0, 1 )
	ZEND_ARG_OBJ_INFO( 0, object, Collator, 0 )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( collator_1_arg, 0, 0, 2 )
	ZEND_ARG_OBJ_INFO( 0, object, Collator, 0 )
	ZEND_ARG_INFO( 0, arg1 )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( collator_2_args, 0, 0, 3 )
	ZEND_ARG_OBJ_INFO( 0, object, Collator, 0 )
	ZEND_ARG_INFO( 0, arg1 )
	ZEND_ARG_INFO( 0, arg2 )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( collator_sort_args, 0, 0, 2 )
	ZEND_ARG_OBJ_INFO( 0, object, Collator, 0 )
	ZEND_ARG_ARRAY_INFO( 1, arr, 0 )
	ZEND_ARG_INFO( 0, sort_flags )
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX( numfmt_parse_arginfo, 0, 0, 2 )
	ZEND_ARG_INFO( 0, formatter )
	ZEND_ARG_INFO( 0, string )
	ZEND_ARG_INFO( 0, type )
	ZEND_ARG_INFO( 1, position )
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX( numfmt_parse_currency_arginfo, 0, 0, 3 )
	ZEND_ARG_INFO( 0, formatter )
	ZEND_ARG_INFO( 0, string )
	ZEND_ARG_INFO( 1, currency )
	ZEND_ARG_INFO( 1, position )
ZEND_END_ARG_INFO()

#define intl_0_args collator_static_0_args
#define intl_1_arg collator_static_1_arg
/* }}} */

/* {{{ intl_functions[]
 *
 * Every user visible function must have an entry in intl_functions[].
 */
zend_function_entry intl_functions[] = {

	// collator functions
	PHP_FE( collator_create, collator_static_1_arg )
	PHP_FE( collator_compare, collator_2_args )
	PHP_FE( collator_get_attribute, collator_1_arg )
	PHP_FE( collator_set_attribute, collator_2_args )
	PHP_FE( collator_get_strength, collator_0_args )
	PHP_FE( collator_set_strength, collator_1_arg )
	PHP_FE( collator_sort, collator_sort_args )
	PHP_FE( collator_sort_with_sort_keys, collator_sort_args )
	PHP_FE( collator_asort, collator_sort_args )
	PHP_FE( collator_get_locale, collator_1_arg )
	PHP_FE( collator_get_display_name, collator_static_2_args )
	PHP_FE( collator_get_available_locales, collator_static_0_args )
	PHP_FE( collator_get_error_code, collator_0_args )
	PHP_FE( collator_get_error_message, collator_0_args )

	// formatter functions
	PHP_FE( numfmt_create, NULL )
	PHP_FE( numfmt_format, NULL )
	PHP_FE( numfmt_parse, numfmt_parse_arginfo )
	PHP_FE( numfmt_format_currency, NULL )
	PHP_FE( numfmt_parse_currency, numfmt_parse_currency_arginfo )
	PHP_FE( numfmt_set_attribute, NULL )
	PHP_FE( numfmt_get_attribute, NULL )
	PHP_FE( numfmt_set_text_attribute, NULL )
	PHP_FE( numfmt_get_text_attribute, NULL )
	PHP_FE( numfmt_set_symbol, NULL )
	PHP_FE( numfmt_get_symbol, NULL )
	PHP_FE( numfmt_set_pattern, NULL )
	PHP_FE( numfmt_get_pattern, NULL )
	PHP_FE( numfmt_get_locale, NULL )
	PHP_FE( numfmt_get_error_code, NULL )
	PHP_FE( numfmt_get_error_message, NULL )

	// common functions
	PHP_FE( intl_get_error_code, intl_0_args )
	PHP_FE( intl_get_error_message, intl_0_args )
	PHP_FE( intl_is_failure, intl_1_arg )
	PHP_FE( intl_error_name, intl_1_arg )

	{ NULL, NULL, NULL }
};
/* }}} */

/* {{{ intl_module_entry
 */
zend_module_entry intl_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"intl",
	intl_functions,
	PHP_MINIT( intl ),
	PHP_MSHUTDOWN( intl ),
	PHP_RINIT( intl ),
	PHP_RSHUTDOWN( intl ),
	PHP_MINFO( intl ),
#if ZEND_MODULE_API_NO >= 20010901
	INTL_MODULE_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_INTL
ZEND_GET_MODULE( intl )
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION( intl )
{
	// Register 'Collator' PHP class
	collator_register_Collator_class();

	// Expose Collator constants to PHP scripts
	collator_register_constants( INIT_FUNC_ARGS_PASSTHRU );

	// Register 'NumberFormatter' PHP class
	formatter_register_class();

	// Expose NumberFormatter constants to PHP scripts
	formatter_register_constants( INIT_FUNC_ARGS_PASSTHRU );

	// Expose ICU error codes to PHP scripts.
	intl_expose_icu_error_codes( INIT_FUNC_ARGS_PASSTHRU );

	// Global error handling.
	intl_error_init( NULL );

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION( intl )
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION( intl )
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION( intl )
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION( intl )
{
	php_info_print_table_start();
	php_info_print_table_header( 2, "Internationalization support", "enabled" );
	php_info_print_table_row( 2, "version", INTL_MODULE_VERSION );
	php_info_print_table_end();
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
