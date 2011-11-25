#ifndef PHP_FE_END
# define PHP_FE_END { NULL, NULL, NULL, 0, 0 }
#endif

#if PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION < 3

#define zend_parse_parameters_none() \
	zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "")

#define Z_DELREF_P ZVAL_DELREF
#define Z_ADDREF_P ZVAL_ADDREF

# undef ZEND_BEGIN_ARG_INFO_EX
# define ZEND_BEGIN_ARG_INFO_EX(name, pass_rest_by_reference, return_reference, required_num_args) \
    static const zend_arg_info name[] = { \
        { NULL, 0, NULL, 0, 0, 0, pass_rest_by_reference, return_reference, required_num_args },

static inline zend_class_entry *php_date_get_date_ce(void)
{
	zend_class_entry **ret;
	TSRMLS_FETCH();
	zend_lookup_class_ex("DateTime", sizeof("DateTime")-1, 0, &ret TSRMLS_CC);
	assert(*ret);
	return *ret;
}

#endif

