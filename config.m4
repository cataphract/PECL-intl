dnl config.m4 for extension intl

dnl ##########################################################################
dnl
dnl PHP_SETUP_ICU([shared-add])
dnl
dnl Common setup macro for ICU
dnl
AC_DEFUN([PHP_SETUP_ICU],[
  unset PHP_ICU_DIR

  AC_MSG_CHECKING([for location of ICU headers and libraries])

  AC_ARG_WITH(icu-dir,
  [  --with-icu-dir=DIR      Specify where ICU libraries and headers can be found], 
  [
    if test "x$withval" != "xyes"; then
      PHP_ICU_DIR=$withval
    else
      PHP_ICU_DIR=DEFAULT
    fi
  ], [
    PHP_ICU_DIR=DEFAULT
  ])

  if test "$PHP_ICU_DIR" = "DEFAULT"; then
    ICU_CONFIG=icu-config
    for i in /usr/local/bin /usr/bin; do
      if test -x "$i/icu-config"; then
        ICU_CONFIG=$i/icu-config
        break;
      fi
    done
  else
    ICU_CONFIG="$PHP_ICU_DIR/bin/icu-config"
  fi

  dnl Trust icu-config to know better what the install prefix is..
  icu_install_prefix=`$ICU_CONFIG --prefix 2> /dev/null`
  if test "$?" != "0" || test -z "$icu_install_prefix"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Unable to detect ICU prefix or $ICU_CONFIG failed. Please verify ICU install prefix and make sure icu-config works.])
  else
    AC_MSG_RESULT([found in $icu_install_prefix])

    dnl Check ICU version
    AC_MSG_CHECKING([for ICU 3.4 or greater])
    icu_version_full=`$ICU_CONFIG --version`
    ac_IFS=$IFS
    IFS="."
    set $icu_version_full
    IFS=$ac_IFS
    icu_version=`expr [$]1 \* 1000 + [$]2`
    AC_MSG_RESULT([found $icu_version_full])
	
    if test "$icu_version" -lt "3004"; then
      AC_MSG_ERROR([ICU version 3.4 or later is required])
    fi

    ICU_INCS=`$ICU_CONFIG --cppflags-searchpath`
    ICU_LIBS=`$ICU_CONFIG --ldflags --ldflags`
    INTL_SHARED_LIBADD="$INTL_SHARED_LIBADD $ICU_LIBS"
    AC_DEFINE(HAVE_ICU,1,[ ])
    
    PHP_SUBST(INTL_SHARED_LIBADD)
  fi
])

dnl ##########################################################################
dnl Initialize the extension
PHP_ARG_ENABLE(intl, whether to enable internationalization support,
[  --enable-intl           Enable internationalization support])

if test "$PHP_INTL" != "no"; then

  dnl ## Check if ptrdiff_t type is defined.
  AC_CHECK_TYPES(ptrdiff_t)
  
  dnl ## check for ICU library location and version 
  PHP_SETUP_ICU

  PHP_NEW_EXTENSION(intl,
   			php_intl.c \
		    intl_error.c \
		    intl_convert.c \
		    collator/collator.c \
		    collator/collator_class.c \
		    collator/collator_sort.c \
		    collator/collator_convert.c \
		    collator/collator_locale.c \
		    collator/collator_compare.c \
		    collator/collator_attr.c \
		    collator/collator_create.c \
		    collator/collator_is_numeric.c \
		    collator/collator_error.c \
		    common/common_error.c \
		    formatter/formatter.c \
		    formatter/formatter_main.c \
		    formatter/formatter_class.c \
		    formatter/formatter_attr.c \
		    formatter/formatter_data.c \
		    formatter/formatter_format.c \
		    formatter/formatter_parse.c \
			locale/locale.c \
			locale/locale_class.c \
			locale/locale_methods.c \
		    ,$ext_shared,,$ICU_INCS)
fi
