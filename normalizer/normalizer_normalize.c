/*
   +----------------------------------------------------------------------+
   | PHP Version 5														  |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,	  |
   | that is bundled with this package in the file LICENSE, and is		  |
   | available through the world-wide-web at the following url:			  |
   | http://www.php.net/license/3_01.txt								  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to		  |
   | license@php.net so we can mail you a copy immediately.				  |
   +----------------------------------------------------------------------+
   | Authors: Ed Batutis <ed@batutis.com>								  |
   +----------------------------------------------------------------------+
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_intl.h"
#include "unicode/unorm.h"
#include "normalizer_class.h"
#include "normalizer_normalize.h"
#include "intl_convert.h"

/* {{{ proto string Normalizer::normalize( string $input [, string $form = FORM_C] [, array $options] )
 * Normalize a string. }}} */
/* {{{ proto string normalizer_normalize( string $input [, string $form = FORM_C] [, array $options]  )
 * Normalize a string.
 */
PHP_FUNCTION( normalizer_normalize )
{
	char*			input = NULL;
	char*			form = NULL;
	int			input_len = 0;
	int			form_len = 0;
	zval*			options_array;
		
	UChar*			uinput = NULL;
	int			uinput_len = 0;
	int			expansion_factor = 1;
	int			mode = UNORM_NFC;
	int			options = 0; 
	UErrorCode		status = U_ZERO_ERROR;
		
	UChar*			uret_buf = NULL;
	int			uret_len = 0;
		
	char*			ret_buf = NULL;
	int32_t			ret_len = 0;

	int32_t			size_needed;
		
	NORMALIZER_METHOD_INIT_VARS

	intl_error_reset( NULL TSRMLS_CC );

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "s|sa",
				&input, &input_len, &form, &form_len, &options_array) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
						 "normalizer_normalize: unable to parse input params", 1 TSRMLS_CC );

		RETURN_NULL();
	}

	// form is optional, defaults to FORM_C
	if ( form_len > 0 ) {
		// estimate how much the buffer will need to expand during normalization
		expansion_factor = 1;

		if ( strcmp(form, "UNORM_FORM_C") == 0 ) {
			mode = UNORM_NFC;
		}
		else if ( strcmp(form, "UNORM_FORM_D") == 0 ) {
			mode = UNORM_NFD;
			expansion_factor = 3;
		}
		else if ( strcmp(form, "UNORM_FORM_KC") == 0 ) {
			mode = UNORM_NFKC;
		}
		else if ( strcmp(form, "UNORM_FORM_KD") == 0 ) {
			mode = UNORM_NFKD;
			expansion_factor = 3;
		}
		else if ( strcmp(form, "UNORM_NONE") == 0 ) {
			mode = UNORM_NONE;
		}
		else {
			intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
						 "normalizer_normalize: illegal normalization form", 1 TSRMLS_CC );

			RETURN_NULL();
		}
	}

	/*
	 * Normalize string (converting it to UTF-16 first).
	 */

	// First convert the string to UTF-16.
	intl_convert_utf8_to_utf16(&uinput, &uinput_len, input, input_len, &status );

	if( U_FAILURE( status ) )
	{
		// Set global error code.
		intl_error_set_code( NULL, status TSRMLS_CC );

		// Set error messages.
		intl_error_set_custom_msg( NULL, "Error converting input string to UTF-16", 1 TSRMLS_CC );
		efree( uinput );
		RETURN_NULL();
	}


	// Allocate memory for the destination buffer for normalization
	uret_len = uinput_len * expansion_factor;
	uret_buf = eumalloc( uret_len + 1 );

	// normalize
	size_needed = unorm_normalize( uinput, uinput_len, mode, options, uret_buf, uret_len, &status);
	
	// Bail out if an unexpected error occured.
	// (U_BUFFER_OVERFLOW_ERROR means that *target buffer is not large enough).
	// (U_STRING_NOT_TERMINATED_WARNING usually means that the input string is empty).
	if( U_FAILURE(status) && status != U_BUFFER_OVERFLOW_ERROR && status != U_STRING_NOT_TERMINATED_WARNING ) {
		efree( uret_buf );
		efree( uinput );
		RETURN_NULL();
	}

	if ( size_needed > uret_len ) {
		// realloc does not seem to work properly - memory is corrupted
		// uret_buf =  eurealloc(uret_buf, size_needed + 1);
		efree( uret_buf );
		uret_buf = eumalloc( size_needed + 1 );
		uret_len = size_needed;

		status = U_ZERO_ERROR;

		// try normalize again
		size_needed = unorm_normalize( uinput, uinput_len, mode, options, uret_buf, uret_len, &status);

		// Bail out if an unexpected error occured.
		if( U_FAILURE(status)  ) {
			// Set error messages.
			intl_error_set_custom_msg( NULL,"Error normalizing string", 1 TSRMLS_CC );
			efree( uret_buf );
			efree( uinput );
			RETURN_NULL();
		}
	}

	efree( uinput );

	// the buffer we actually used
	uret_len = size_needed;

	// Convert normalized string from UTF-16 to UTF-8.
	intl_convert_utf16_to_utf8( &ret_buf, &ret_len, uret_buf, uret_len, &status );
	efree( uret_buf );
	if( U_FAILURE( status ) )
	{
		intl_error_set( NULL, status,
				"normalizer_normalize: error converting normalized text UTF-8", 1 TSRMLS_CC );
		RETURN_NULL();
	}

	// Return it.
	RETVAL_STRINGL( ret_buf, ret_len, FALSE );
}
/* }}} */

/* {{{ proto bool Normalizer::isNormalized( string $input [, string $form = FORM_C] [, array $options] )
 * Test if a string is in a given normalization form. }}} */
/* {{{ proto bool normalizer_is_normalize( string $input [, string $form = FORM_C] [, array $options]  )
 * Test if a string is in a given normalization form.
 */
PHP_FUNCTION( normalizer_is_normalized )
{
	char*	 	input = NULL;
	char*	 	form = NULL;
	int		input_len = 0;
	int		form_len = 0;
	zval*	 	options_array = NULL;

	UChar*	 	uinput = NULL;
	int		uinput_len = 0;
	int		mode = UNORM_NFC;
	int		options = 0; 
	UErrorCode	status = U_ZERO_ERROR;
		
	UChar*		uret_buf = NULL;
	int		uret_len = 0;

	UBool		uret = FALSE;
		
	NORMALIZER_METHOD_INIT_VARS

	intl_error_reset( NULL TSRMLS_CC );

	// Parse parameters.
	if( zend_parse_method_parameters( ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "s|sa",
				&input, &input_len, &form, &form_len, &options_array) == FAILURE )
	{
		intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
				"normalizer_is_normalized: unable to parse input params", 1 TSRMLS_CC );

		RETURN_FALSE;
	}

	if ( form_len > 0 ) {
		if ( strcmp(form, "UNORM_FORM_C") == 0 ) {
			mode = UNORM_NFC;
		}
		else if ( strcmp(form, "UNORM_FORM_D") == 0 ) {
			mode = UNORM_NFD;
		}
		else if ( strcmp(form, "UNORM_FORM_KC") == 0 ) {
			mode = UNORM_NFKC;
		}
		else if ( strcmp(form, "UNORM_FORM_KD") == 0 ) {
			mode = UNORM_NFKD;
		}
		// 'none' doesn't make any sense and is not allowed
		//else if ( strcmp(form, "UNORM_NONE") == 0 ) {
		//	 mode = UNORM_NONE;
		//}
		else {
			intl_error_set( NULL, U_ILLEGAL_ARGUMENT_ERROR,
					 "normalizer_normalize: illegal normalization form", 1 TSRMLS_CC );

			RETURN_FALSE;
		}
	}

	/*
	 * Test normalization of string (converting it to UTF-16 first).
	 */

	// First convert the string to UTF-16.
	intl_convert_utf8_to_utf16(&uinput, &uinput_len, input, input_len, &status );

	if( U_FAILURE( status ) )
	{
		// Set global error code.
		intl_error_set_code( NULL, status TSRMLS_CC );

		// Set error messages.
		intl_error_set_custom_msg( NULL, "Error converting string to UTF-16.", 1 TSRMLS_CC );
		efree( uinput );
		RETURN_FALSE;
	}


	// test string
	uret = unorm_isNormalizedWithOptions( uinput, uinput_len, mode, options, &status);
	
	efree( uinput );

	// Bail out if an unexpected error occured.
	if( U_FAILURE(status)  ) {
		// Set error messages.
		intl_error_set_custom_msg( NULL,"Error testing if string is the given normalization form.", 1 TSRMLS_CC );
		RETURN_FALSE;
	}

	if ( uret )
		RETURN_TRUE;
				
	RETURN_FALSE;
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
