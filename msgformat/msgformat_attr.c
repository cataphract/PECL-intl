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
#include "msgformat_class.h"
#include "msgformat_attr.h"

#include <unicode/ustring.h>


/* {{{ proto unicode MessageFormatter::getPattern( )
 * Get formatter pattern. }}} */
/* {{{ proto string msgfmt_get_pattern( MessageFormatter $mf )
 * Get formatter pattern.
 */
PHP_FUNCTION( msgfmt_get_pattern )
{
	UChar  value_buf[64];
	int    length = USIZE( value_buf );
	UChar* value  = value_buf;
	MSG_FORMAT_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &object, MessageFormatter_ce_ptr ) == FAILURE )
	{
		intl_error_set( INTL_DATA_ERROR_P(mfo), U_ILLEGAL_ARGUMENT_ERROR,	
			"msgfmt_get_pattern: unable to parse input params", 0 TSRMLS_CC );
		RETURN_FALSE;
	}

	// Fetch the object.
	MSG_FORMAT_METHOD_FETCH_OBJECT;

	length = umsg_toPattern(MSG_FORMAT_OBJECT(mfo), value, length, &INTL_DATA_ERROR_CODE(mfo));
	if(INTL_DATA_ERROR_CODE(mfo) == U_BUFFER_OVERFLOW_ERROR && length >= USIZE( value_buf )) {
		++length; // to avoid U_STRING_NOT_TERMINATED_WARNING
		INTL_DATA_ERROR_CODE(mfo) = U_ZERO_ERROR;
		value = eumalloc(length);
		length = umsg_toPattern(MSG_FORMAT_OBJECT(mfo), value, length, &INTL_DATA_ERROR_CODE(mfo) );
		if(U_FAILURE(INTL_DATA_ERROR_CODE(mfo))) {
			efree(value);
			value = value_buf;
		}
	}
	INTL_METHOD_CHECK_STATUS(mfo, "Error getting formatter pattern" );

	RETURN_UNICODEL(value, length, ( value == value_buf ) );
}
/* }}} */

/* {{{ proto bool MessageFormatter::setPattern( string $pattern )
 * Set formatter pattern. }}} */
/* {{{ proto bool msgfmt_set_pattern( MessageFormatter $mf, string $pattern )
 * Set formatter pattern.
 */
PHP_FUNCTION( msgfmt_set_pattern )
{
	int         slength = 0;
	UChar*	    svalue  = NULL;
	MSG_FORMAT_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ou",
		&object, MessageFormatter_ce_ptr, &svalue, &slength ) == FAILURE )
	{
		intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR,	
			"msgfmt_set_pattern: unable to parse input params", 0 TSRMLS_CC);
		RETURN_FALSE;
	}

	MSG_FORMAT_METHOD_FETCH_OBJECT;

	// TODO: add parse error information
	umsg_applyPattern(MSG_FORMAT_OBJECT(mfo), svalue, slength, NULL, &INTL_DATA_ERROR_CODE(mfo));
	INTL_METHOD_CHECK_STATUS(mfo, "Error setting symbol value");

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string MessageFormatter::getLocale()
 * Get formatter locale. }}} */
/* {{{ proto string msgfmt_get_locale(MessageFormatter $mf)
 * Get formatter locale.
 */
PHP_FUNCTION( msgfmt_get_locale )
{
	char *loc;
	MSG_FORMAT_METHOD_INIT_VARS;

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O",
		&object, MessageFormatter_ce_ptr ) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"msgfmt_get_locale: unable to parse input params", 0 TSRMLS_CC );

		RETURN_FALSE;
	}

	// Fetch the object.
	MSG_FORMAT_METHOD_FETCH_OBJECT;

	loc = (char *)umsg_getLocale(MSG_FORMAT_OBJECT(mfo));
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
