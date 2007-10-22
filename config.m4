dnl config.m4 for extension intl

dnl ##########################################################################
dnl Initialize the extension
PHP_ARG_ENABLE(intl, whether to enable internationalization support,
[  --enable-intl           Enable internationalization support])

if test "$PHP_INTL" != "no"; then

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
		    normalizer/normalizer.c \
		    normalizer/normalizer_class.c \
		    normalizer/normalizer_create.c \
		    normalizer/normalizer_normalize.c \
		    locale/locale.c \
		    locale/locale_class.c \
		    locale/locale_methods.c \
		    ,$ext_shared,,$ICU_INCS)
fi
