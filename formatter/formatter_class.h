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

#ifndef FORMATTER_CLASS_H
#define FORMATTER_CLASS_H

#include <php.h>

#include "intl_common.h"
#include "intl_error.h"
#include "formatter_data.h"

typedef struct {
	zend_object     zo;
	formatter_data  nf_data;
} NumberFormatter_object;

void formatter_register_class();
extern zend_class_entry *NumberFormatter_ce_ptr;

/* Auxiliary macros */

#define FORMATTER_METHOD_INIT_VARS             \
    zval*                    object  = NULL;   \
    NumberFormatter_object*  nfo     = NULL;   \
	intl_error_reset( NULL );                  \

#define FORMATTER_ERROR(nfo)                (nfo)->nf_data.error
#define FORMATTER_ERROR_P(nfo)              &(FORMATTER_ERROR(nfo))
#define FORMATTER_ERROR_CODE(nfo)           INTL_ERROR_CODE(FORMATTER_ERROR(nfo))

#define FORMATTER_METHOD_FETCH_OBJECT                                                   \
    nfo = (NumberFormatter_object *) zend_object_store_get_object( object TSRMLS_CC );	\
    intl_error_reset( FORMATTER_ERROR_P(nfo) );                                         \

#define FORMATTER_CHECK_STATUS(nfo, msg)                           \
    intl_error_set_code( NULL, FORMATTER_ERROR_CODE((nfo)) );      \
    if( U_FAILURE( FORMATTER_ERROR_CODE((nfo)) ) )                 \
    {                                                              \
        intl_errors_set_custom_msg(FORMATTER_ERROR_P(nfo), msg, 0);\
        RETURN_FALSE;                                              \
    }

#define FORMATTER_RETVAL_UTF8( nfo, ustring, ulen, free_it )                                    \
{                                                                                               \
    char *u8value;                                                                              \
    int u8len;                                                                                  \
    intl_convert_utf16_to_utf8(&u8value, &u8len, ustring, ulen, &FORMATTER_ERROR_CODE(nfo));    \
    if( (free_it) ) {                                                                           \
        efree( ustring );                                                                       \
    }                                                                                           \
    FORMATTER_CHECK_STATUS(nfo, "Error converting value to UTF-8");                             \
    RETVAL_STRINGL(u8value, u8len, 0);                                                          \
}

#endif // #ifndef FORMATTER_CLASS_H
