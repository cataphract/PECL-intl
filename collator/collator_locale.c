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
#include "collator_locale.h"
#include "intl_convert.h"

#include <zend_API.h>

/* {{{ proto string Collator::getLocale( int $type )
 * Gets the locale name of the collator. }}} */
/* {{{ proto string collator_get_locale( Collator $coll, int $type )
 * Gets the locale name of the collator.
 */
PHP_FUNCTION( collator_get_locale )
{
	int    type        = 0;
	char*  locale_name = NULL;

	COLLATOR_METHOD_INIT_VARS

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol",
		&object, Collator_ce_ptr, &type ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"collator_get_locale: unable to parse input params", 0 );

		RETURN_FALSE;
	}

	// Fetch the object.
	COLLATOR_METHOD_FETCH_OBJECT;

	// Get locale by specified type.
	locale_name = (char*) ucol_getLocaleByType(
		co->ucoll, type, COLLATOR_ERROR_CODE_P( co ) );
	COLLATOR_CHECK_STATUS( co, "Error getting locale by type" );

	// Return it.
	RETURN_ASCII_STRINGL( locale_name, strlen(locale_name), TRUE );
}
/* }}} */

/* {{{ proto static string Collator::getDisplayName( string $obj_loc, string $disp_loc )
 * Get the display name for a Collator. }}} */
/* {{{ proto string collator_get_display_name( Collator $coll, string $obj_loc, string $disp_loc )
 * Get the display name for a Collator.
 */
PHP_FUNCTION( collator_get_display_name )
{
	char*       obj_loc      = NULL;
	int         obj_loc_len  = 0;

	char*       disp_loc     = NULL;
	int         disp_loc_len = 0;

	UChar*      uname        = NULL;
	int32_t     uname_len    = 0;

	int32_t     buflen       = 512;
	UErrorCode  status       = U_ZERO_ERROR;

	intl_error_reset( NULL );

	// Parse parameters.
	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ss",
		&obj_loc, &obj_loc_len,
		&disp_loc, &disp_loc_len ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
 			"collator_get_display_name: unable to parse input params", 0 );
		RETURN_FALSE;
	}


	// Get display name of the given locale.
	do
	{
		uname = eurealloc( uname, buflen );
		uname_len = buflen;

		buflen = ucol_getDisplayName( obj_loc, disp_loc,
			uname, uname_len, &status );
		if( U_FAILURE( status ) )
		{
			if( status == U_BUFFER_OVERFLOW_ERROR )
			{
				status = U_ZERO_ERROR;
				continue;
			}

			intl_error_set( NULL, status,
 				"collator_get_display_name: unable to get locale display name", 0 );
			efree( uname );
			RETURN_FALSE;
		}

	} while( buflen > uname_len );

	// Return it.
	RETURN_UNICODEL( uname, buflen, FALSE );
}
/* }}} */

/* {{{ proto static array(string) Collator::getAvailableLocales()
 * Get a list of all locales for which a valid collator may be
 * opened. }}} */
/* {{{ proto array(string) collator_get_available_locales()
 * Get a list of all locales for which a valid collator may be
 * opened.
 */
PHP_FUNCTION( collator_get_available_locales )
{
	UEnumeration*     e        = NULL;
	UErrorCode        status   = U_ZERO_ERROR;
	const UChar*      l        = NULL;
	int32_t           l_len    = 0;

	intl_error_reset( NULL );

	// Get an enumeration representing the list of available locales.
	e = ucol_openAvailableLocales( &status );
	if( e == NULL )
	{
		intl_error_set( NULL, status,
			"collator_get_available_locales: unable to get the list of available locales", 0 );

		RETURN_FALSE;
	}

	// Traverse it, filling the return array.
	array_init( return_value );
	while( ( l = uenum_unext( e, &l_len, &status ) ) != NULL )
		add_next_index_unicodel( return_value, (UChar*)l, l_len, TRUE );

	uenum_close( e );
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
