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
   | Authors: Ed Batutis <ed@batutis.com>                                 |
   +----------------------------------------------------------------------+
 */

#include "normalizer_class.h"
#include "php_intl.h"
#include "normalizer_create.h"
#include "normalizer_normalize.h"
#include "intl_error.h"

#include <unicode/unorm.h>

zend_class_entry *Normalizer_ce_ptr = NULL;

/////////////////////////////////////////////////////////////////////////////
// Auxiliary functions needed by objects of 'Normalizer' class
/////////////////////////////////////////////////////////////////////////////

/* {{{ Normalizer_objects_dtor */
static void Normalizer_objects_dtor(
	void *object,
	zend_object_handle handle TSRMLS_DC )
{
	zend_objects_destroy_object( object, handle TSRMLS_CC );
}
/* }}} */

/* {{{ Normalizer_objects_free */
void Normalizer_objects_free( zend_object *object TSRMLS_DC )
{
	Normalizer_object* co = (Normalizer_object*)object;

	zend_object_std_dtor( &co->zo TSRMLS_CC );

	normalizer_object_destroy( co TSRMLS_CC );

	efree( co );
}
/* }}} */

/* {{{ Normalizer_object_create */
zend_object_value Normalizer_object_create(
	zend_class_entry *ce TSRMLS_DC )
{
	zend_object_value    retval;
	Normalizer_object*     intern;

	intern = ecalloc( 1, sizeof(Normalizer_object) );
	intl_error_init( NORMALIZER_ERROR_P( intern ) TSRMLS_CC );
	zend_object_std_init( &intern->zo, ce TSRMLS_CC );

	retval.handle = zend_objects_store_put(
		intern,
		Normalizer_objects_dtor,
		(zend_objects_free_object_storage_t)Normalizer_objects_free,
		NULL TSRMLS_CC );

	retval.handlers = zend_get_std_object_handlers();

	return retval;
}
/* }}} */

/////////////////////////////////////////////////////////////////////////////
// 'Normalizer' class registration structures & functions
/////////////////////////////////////////////////////////////////////////////

/* {{{ Normalizer methods arguments info */

static
ZEND_BEGIN_ARG_INFO_EX( normalizer_1_arg, 0, 0, 1 )
	ZEND_ARG_INFO( 0, arg1 )
ZEND_END_ARG_INFO()

static
ZEND_BEGIN_ARG_INFO_EX( normalizer_3_args, 0, 0, 3 )
	ZEND_ARG_INFO( 0, arg1 )
	ZEND_ARG_INFO( 0, arg2 )
	ZEND_ARG_INFO( 0, arg3 )
ZEND_END_ARG_INFO()

/* }}} */

/* {{{ Normalizer_class_functions
 * Every 'Normalizer' class method has an entry in this table
 */

function_entry Normalizer_class_functions[] = {
	PHP_ME( Normalizer, __construct, normalizer_1_arg, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR )
	ZEND_FENTRY( normalize, ZEND_FN( normalizer_normalize ), normalizer_3_args, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	ZEND_FENTRY( isNormalized, ZEND_FN( normalizer_is_normalized ), normalizer_3_args, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC )
	{ NULL, NULL, NULL }
};
/* }}} */

/* {{{ normalizer_register_Normalizer_class
 * Initialize 'Normalizer' class
 */
void normalizer_register_Normalizer_class( TSRMLS_D )
{
	zend_class_entry ce;

	// Create and register 'Normalizer' class.
	INIT_CLASS_ENTRY( ce, "Normalizer", Normalizer_class_functions );
	ce.create_object = Normalizer_object_create;
	Normalizer_ce_ptr = zend_register_internal_class( &ce TSRMLS_CC );

	// Declare 'Normalizer' class properties.
	if( !Normalizer_ce_ptr )
	{
		zend_error( E_ERROR,
			"Normalizer: attempt to create properties "
			"on a non-registered class." );
		return;
	}
}
/* }}} */

/* {{{ void normalizer_object_init( Normalize_object* co )
 * Initialize internals of Normalizer_object.
 * Must be called before any other call to 'normalizer_object_...' functions.
 */
void normalizer_object_init( Normalizer_object* co TSRMLS_DC )
{
	if( !co )
		return;

	intl_error_init( NORMALIZER_ERROR_P( co ) TSRMLS_CC );
}
/* }}} */

/* {{{ void normalizer_object_destroy( Normalizer_object* co )
 * Clean up mem allocted by internals of Normalizer_object
 */
void normalizer_object_destroy( Normalizer_object* co TSRMLS_DC )
{
	if( !co )
		return;

	intl_errors_reset( NORMALIZER_ERROR_P( co ) TSRMLS_CC );
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
