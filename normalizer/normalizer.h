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

#ifndef NORMALIZER_NORMALIZER_H
#define NORMALIZER_NORMALIZER_H

#include <php.h>
#include <unicode/utypes.h>
#include <unicode/unorm.h>

#define NORMALIZER_FORM_C "UNORM_FORM_C"
#define NORMALIZER_FORM_D "UNORM_FORM_D"
#define NORMALIZER_FORM_KC "UNORM_FORM_KC"
#define NORMALIZER_FORM_KD "UNORM_FORM_KD"
#define NORMALIZER_NONE "UNORM_NONE"

#define NORMALIZER_OPTION_DEFAULT 0

void normalizer_register_constants( INIT_FUNC_ARGS );

#endif // NORMALIZER_NORMALIZER_H
