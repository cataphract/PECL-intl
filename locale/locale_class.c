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
   | Authors: Kirti Velankar <kirtig@yahoo-inc.com>                  	  |
   +----------------------------------------------------------------------+
 */

#include <unicode/uloc.h>

#include "php_intl.h"

#include "intl_error.h"
#include "locale_class.h"
#include "locale_methods.h"
#include "locale.h"

zend_class_entry *Locale_ce_ptr = NULL;

/////////////////////////////////////////////////////////////////////////////
// Auxiliary functions needed by objects of 'Locale' class
/////////////////////////////////////////////////////////////////////////////


/* {{{ Locale_objects_free */
void Locale_objects_free( zend_object *object TSRMLS_DC )
{
	Locale_object* lo = (Locale_object*)object;

	zend_object_std_dtor( &lo->zo TSRMLS_CC );

	locale_object_destroy( lo TSRMLS_CC );

	efree( lo );
}
/* }}} */

/* {{{ Locale_object_create */
zend_object_value Locale_object_create(
	zend_class_entry *ce TSRMLS_DC )
{
	zend_object_value    retval;
	Locale_object*     intern;

	intern = ecalloc( 1, sizeof(Locale_object) );
	//intl_error_init( LOCALE_ERROR_P( intern ) TSRMLS_CC );
	intl_error_init( NULL TSRMLS_CC );
	zend_object_std_init( &intern->zo, ce TSRMLS_CC );

	retval.handle = zend_objects_store_put(
		intern,
		NULL,
		(zend_objects_free_object_storage_t)Locale_objects_free,
		NULL TSRMLS_CC );

	retval.handlers = zend_get_std_object_handlers();

	return retval;
}
/* }}} */

/////////////////////////////////////////////////////////////////////////////
// 'Locale' class registration structures & functions
/////////////////////////////////////////////////////////////////////////////

/* {{{ Locale methods arguments info */
// NOTE: modifying 'locale_XX_args' do not forget to
//       modify approptiate 'locale_XX_args' for
//       the procedural API.

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

/* }}} */

/* {{{ Locale_class_functions
 * Every 'Locale' class method has an entry in this table
 */

function_entry Locale_class_functions[] = {
	ZEND_FENTRY( getDefault, zif_locale_get_default , locale_0_args , ZEND_ACC_PUBLIC|ZEND_ACC_STATIC  )
	ZEND_FENTRY( setDefault, zif_locale_set_default , locale_1_arg , ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	ZEND_FENTRY( getPrimaryLanguage, ZEND_FN( locale_get_primary_language ), locale_1_arg , ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	ZEND_FENTRY( getScript, ZEND_FN( locale_get_script ), locale_1_arg , ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	ZEND_FENTRY( getRegion, ZEND_FN( locale_get_region ), locale_1_arg , ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	ZEND_FENTRY( getVariant, ZEND_FN( locale_get_variant ), locale_1_arg , ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	ZEND_FENTRY( getKeywords, ZEND_FN( locale_get_keywords ), locale_1_arg , ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	ZEND_FENTRY( getDisplayScript, ZEND_FN( locale_get_display_script ), locale_2_args , ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	ZEND_FENTRY( getDisplayRegion, ZEND_FN( locale_get_display_region ), locale_2_args , ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	ZEND_FENTRY( getDisplayName, ZEND_FN( locale_get_display_name ), locale_2_args , ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	ZEND_FENTRY( getDisplayLanguage, ZEND_FN( locale_get_display_language ), locale_2_args , ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	ZEND_FENTRY( getDisplayVariant, ZEND_FN( locale_get_display_variant ), locale_2_args , ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	{ NULL, NULL, NULL }
};
/* }}} */

/* {{{ locale_register_Locale_class
 * Initialize 'Locale' class
 */
void locale_register_Locale_class( TSRMLS_D )
{
	zend_class_entry ce;

	// Create and register 'Locale' class.
	INIT_CLASS_ENTRY( ce, "Locale", Locale_class_functions );
	ce.create_object = Locale_object_create;
	Locale_ce_ptr = zend_register_internal_class( &ce TSRMLS_CC );

	// Declare 'Locale' class properties.
	if( !Locale_ce_ptr )
	{
		zend_error( E_ERROR,
			"Locale: Failed to register Locale class.");
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
