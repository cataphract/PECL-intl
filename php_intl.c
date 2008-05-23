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
   |          Kirti Velankar <kirtig@yahoo-inc.com>   			          |
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

#include "msgformat/msgformat.h"
#include "msgformat/msgformat_class.h"
#include "msgformat/msgformat_attr.h"
#include "msgformat/msgformat_format.h"
#include "msgformat/msgformat_parse.h"

#include "normalizer/normalizer.h"
#include "normalizer/normalizer_class.h"
#include "normalizer/normalizer_normalize.h"

#include "locale/locale.h"
#include "locale/locale_class.h"
#include "locale/locale_methods.h"
#include <unicode/uloc.h>
#include "php_ini.h"

#include "common/common_error.h"

#include <ext/standard/info.h>

#define INTL_MODULE_VERSION PHP_INTL_VERSION

/*
 * locale_get_default has a conflict since ICU also has 
 * a function with the same  name
 * in fact ICU appends the version no. to it also
 * Hence the following undef for ICU version
 * Same true for the locale_set_default function 
*/
#undef locale_get_default
#undef locale_set_default

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

static
ZEND_BEGIN_ARG_INFO_EX( locale_0_args, 0, 0, 0 )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( locale_1_arg, 0, 0, 1 )
        ZEND_ARG_INFO( 0, arg1 )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( locale_2_args, 0, 0, 2 )
        ZEND_ARG_INFO( 0, arg1 )
        ZEND_ARG_INFO( 0, arg2 )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( locale_get_args, 0, 0, 1 )
        ZEND_ARG_INFO( 0, locale )
        ZEND_ARG_INFO( 0, in_locale )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( locale_filter_args, 0, 0, 3 )
        ZEND_ARG_INFO( 0, langtag )
        ZEND_ARG_INFO( 0, range )
        ZEND_ARG_INFO( 0, canonical )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( locale_lookup_args, 0, 0, 3 )
        ZEND_ARG_INFO( 0, langtag )
        ZEND_ARG_INFO( 0, locale )
        ZEND_ARG_INFO( 0, canonical )
        ZEND_ARG_INFO( 0, fallback )
ZEND_END_ARG_INFO()


#define intl_0_args collator_static_0_args
#define intl_1_arg collator_static_1_arg

static
ZEND_BEGIN_ARG_INFO_EX( normalizer_args, 0, 0, 1 )
	ZEND_ARG_INFO( 0, input )
	ZEND_ARG_INFO( 0, form )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( grapheme_1_arg, 0, 0, 1 )
        ZEND_ARG_INFO( 0, string )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( grapheme_search_args, 0, 0, 2 )
        ZEND_ARG_INFO( 0, haystack )
        ZEND_ARG_INFO( 0, needle )
        ZEND_ARG_INFO( 0, offset )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( grapheme_substr_args, 0, 0, 2 )
        ZEND_ARG_INFO( 0, string )
        ZEND_ARG_INFO( 0, start )
        ZEND_ARG_INFO( 0, length )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( grapheme_strstr_args, 0, 0, 2 )
        ZEND_ARG_INFO( 0, haystack )
        ZEND_ARG_INFO( 0, needle )
        ZEND_ARG_INFO( 0, before_needle )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( grapheme_extract_args, 0, 0, 2 )
		ZEND_ARG_INFO( 0, arg1 )
		ZEND_ARG_INFO( 0, arg2 )
		ZEND_ARG_INFO( 0, arg3 )
		ZEND_ARG_INFO( 0, arg4 )
		ZEND_ARG_INFO( 1, arg5 )  /* 1 = pass by reference */
ZEND_END_ARG_INFO()

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

	// normalizer functions
	PHP_FE( normalizer_normalize, normalizer_args )
	PHP_FE( normalizer_is_normalized, normalizer_args )

	//Locale functions
        PHP_NAMED_FE( locale_get_default, zif_locale_get_default, locale_0_args)
        PHP_NAMED_FE( locale_set_default, zif_locale_set_default, locale_1_arg )
	PHP_FE( locale_get_primary_language, locale_1_arg )
	PHP_FE( locale_get_script, locale_1_arg )
	PHP_FE( locale_get_region, locale_1_arg )
	PHP_FE( locale_get_keywords, locale_1_arg )
	PHP_FE( locale_get_display_script, locale_get_args )
	PHP_FE( locale_get_display_region, locale_get_args )
	PHP_FE( locale_get_display_name, locale_get_args )
	PHP_FE( locale_get_display_language, locale_get_args )
	PHP_FE( locale_get_display_variant, locale_get_args )
	PHP_FE( locale_get_all_variants, locale_1_arg )
	PHP_FE( locale_canonicalize, locale_1_arg )
	PHP_FE( locale_compose, locale_1_arg )
	PHP_FE( locale_parse, locale_1_arg )
	PHP_FE( locale_filter_matches, locale_filter_args )
	PHP_FE( locale_lookup, locale_lookup_args )

	// MessageFormatter functions
	PHP_FE( msgfmt_create, NULL )
	PHP_FE( msgfmt_format, NULL )
	PHP_FE( msgfmt_format_message, NULL )
	PHP_FE( msgfmt_parse, NULL )
	PHP_FE( msgfmt_parse_message, NULL )
	PHP_FE( msgfmt_set_pattern, NULL )
	PHP_FE( msgfmt_get_pattern, NULL )
	PHP_FE( msgfmt_get_locale, NULL )
	PHP_FE( msgfmt_get_error_code, NULL )
	PHP_FE( msgfmt_get_error_message, NULL )

	// grapheme functions
	PHP_FE( grapheme_strlen, grapheme_1_arg )
	PHP_FE( grapheme_strpos, grapheme_search_args )
	PHP_FE( grapheme_stripos, grapheme_search_args )
	PHP_FE( grapheme_strrpos, grapheme_search_args )
	PHP_FE( grapheme_strripos, grapheme_search_args )
	PHP_FE( grapheme_substr, grapheme_substr_args )
	PHP_FE( grapheme_strstr, grapheme_strstr_args )
	PHP_FE( grapheme_stristr, grapheme_strstr_args )
	PHP_FE( grapheme_extract, grapheme_extract_args )

	// common functions
	PHP_FE( intl_get_error_code, intl_0_args )
	PHP_FE( intl_get_error_message, intl_0_args )
	PHP_FE( intl_is_failure, intl_1_arg )
	PHP_FE( intl_error_name, intl_1_arg )

	{ NULL, NULL, NULL }
};
/* }}} */

/* {{{ INI Settings */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY(LOCALE_INI_NAME, NULL, PHP_INI_ALL, OnUpdateStringUnempty, default_locale, zend_intl_globals, intl_globals)

PHP_INI_END()
/* }}} */


static PHP_GINIT_FUNCTION(intl);

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
	INTL_MODULE_VERSION,
	PHP_MODULE_GLOBALS(intl),   /* globals descriptor */
	PHP_GINIT(intl),            /* globals ctor */
	NULL,                       /* globals dtor */
	NULL,                       /* post deactivate */
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_INTL
ZEND_GET_MODULE( intl )
#endif

/* {{{ intl_init_globals */
static PHP_GINIT_FUNCTION(intl)
{
	memset( intl_globals, 0, sizeof(zend_intl_globals) );
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION( intl )
{
        //For the default locale php.ini setting
        REGISTER_INI_ENTRIES();

	REGISTER_LONG_CONSTANT("INTL_MAX_LOCALE_LEN", INTL_MAX_LOCALE_LEN, CONST_CS);

	// Register 'Collator' PHP class
	collator_register_Collator_class( TSRMLS_C );

	// Expose Collator constants to PHP scripts
	collator_register_constants( INIT_FUNC_ARGS_PASSTHRU );

	// Register 'NumberFormatter' PHP class
	formatter_register_class( TSRMLS_C );

	// Expose NumberFormatter constants to PHP scripts
	formatter_register_constants( INIT_FUNC_ARGS_PASSTHRU );

	// Register 'Normalizer' PHP class
	normalizer_register_Normalizer_class( TSRMLS_C );

	// Expose Normalizer constants to PHP scripts
	normalizer_register_constants( INIT_FUNC_ARGS_PASSTHRU );
	
	// Register 'Locale' PHP class
	locale_register_Locale_class( TSRMLS_C );

	// Expose Locale constants to PHP scripts
	locale_register_constants( INIT_FUNC_ARGS_PASSTHRU );

	msgformat_register_class(TSRMLS_C);

	grapheme_register_constants( INIT_FUNC_ARGS_PASSTHRU );

	// Expose ICU error codes to PHP scripts.
	intl_expose_icu_error_codes( INIT_FUNC_ARGS_PASSTHRU );

	// Global error handling.
	intl_error_init( NULL TSRMLS_CC );

        //Set the default_locale value
        if( INTL_G(default_locale) == NULL ) {
                INTL_G(default_locale) = pestrdup(uloc_getDefault(), 1) ;
        }

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION( intl )
{
    	//For the default locale php.ini setting
   	 UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION( intl )
{
        //Set the default_locale value
	if( INTL_G(default_locale) == NULL ) {
		INTL_G(default_locale) = pestrdup(uloc_getDefault(), 1) ;
	}

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION( intl )
{
	if(INTL_G(current_collator)) {
		INTL_G(current_collator) = NULL;
	}
	if (INTL_G(grapheme_iterator)) {
		grapheme_close_global_iterator( TSRMLS_C );
		INTL_G(grapheme_iterator) = NULL;
	}

	intl_error_reset( NULL TSRMLS_CC);
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

	//For the default locale php.ini setting
	DISPLAY_INI_ENTRIES() ;

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
