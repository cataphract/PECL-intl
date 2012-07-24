#ifndef PHP_FE_END
# define PHP_FE_END { NULL, NULL, NULL, 0, 0 }
#endif

#if PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION < 4

static inline void object_properties_init(zend_object *object, zend_class_entry *class_type)
{
	zend_hash_copy(object->properties, &class_type->default_properties,
		(copy_ctor_func_t)zval_add_ref, NULL, sizeof(zval*));
}

#endif

#if PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION < 3

#define zend_parse_parameters_none() \
	zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "")

#define Z_DELREF_P ZVAL_DELREF
#define Z_ADDREF_P ZVAL_ADDREF

#define zval_addref_p(pz) ZVAL_ADDREF(pz)

# undef ZEND_BEGIN_ARG_INFO_EX
# define ZEND_BEGIN_ARG_INFO_EX(name, pass_rest_by_reference, return_reference, required_num_args) \
    static const zend_arg_info name[] = { \
        { NULL, 0, NULL, 0, 0, 0, pass_rest_by_reference, return_reference, required_num_args },

#undef ZEND_FENTRY
#define ZEND_FENTRY(zend_name, name, arg_info, flags)	\
	{ #zend_name, name, (struct _zend_arg_info *)arg_info, (zend_uint) (sizeof(arg_info)/sizeof(struct _zend_arg_info)-1), flags },

#undef INIT_CLASS_ENTRY
#define INIT_CLASS_ENTRY(class_container, class_name, functions)	\
	 INIT_OVERLOADED_CLASS_ENTRY(class_container, class_name, (zend_function_entry*)functions, NULL, NULL, NULL)

#define array_init_size(arg, size) _array_init((arg) ZEND_FILE_LINE_CC)

#define zend_parse_parameters_none()	\
	zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "")

static inline zend_class_entry *php_date_get_date_ce(void)
{
	zend_class_entry **ret;
	TSRMLS_FETCH();
	zend_lookup_class_ex("DateTime", sizeof("DateTime")-1, 0, &ret TSRMLS_CC);
	assert(*ret);
	return *ret;
}

static inline zend_class_entry *php_date_get_timezone_ce(void)
{
    zend_class_entry **ret;
    TSRMLS_FETCH();
    zend_lookup_class_ex("DateTimeZone", sizeof("DateTimeZone")-1, 0, &ret TSRMLS_CC);
    assert(*ret);
    return *ret;
}


#ifdef ZVAL_STRING
#undef ZVAL_STRING
#define ZVAL_STRING(z, s, duplicate) {		\
		const char *__s=(s);				\
		(z)->value.str.len = strlen(__s);	\
		(z)->value.str.val = (duplicate?estrndup(__s, (z)->value.str.len):(char*)__s); \
		(z)->type = IS_STRING;				\
	}
#undef ZVAL_STRINGL
#define ZVAL_STRINGL(z, s, l, duplicate) {	\
		const char *__s=(s); int __l=l;		\
		(z)->value.str.len = __l;			\
		(z)->value.str.val = (duplicate?estrndup(__s, __l):(char*)__s);	\
		(z)->type = IS_STRING;				\
	}
#endif

#define E_DEPRECATED E_STRICT

#include <ext/date/lib/timelib_structs.h>
typedef struct _php_date_obj {
    zend_object   std;
    timelib_time *time;
} php_date_obj;

typedef struct _php_timezone_obj {
    zend_object     std;
    int             initialized;
    int             type;
    union {
        timelib_tzinfo *tz; /* TIMELIB_ZONETYPE_ID; */
        timelib_sll     utc_offset; /* TIMELIB_ZONETYPE_OFFSET */
        struct                      /* TIMELIB_ZONETYPE_ABBR */
        {
            timelib_sll  utc_offset;
            char        *abbr;
            int          dst;
        } z;
    } tzi;
} php_timezone_obj;

#endif

