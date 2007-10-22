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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_intl.h"
#include "normalizer_class.h"
#include "normalizer_create.h"

/* {{{ proto Normalizer Normalizer::__construct( )
 * Normalizer object constructor.
 */
PHP_METHOD( Normalizer, __construct )
{
	// NORMALIZER_METHOD_INIT_VARS

	/* Fetch the object. */
	// not needed for anything currently
	// object = getThis();
	// no  = (Normalizer_object*) zend_object_store_get_object( object TSRMLS_CC );

	intl_error_reset( NULL TSRMLS_CC );

	php_set_error_handling( EH_NORMAL, NULL TSRMLS_CC );
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
