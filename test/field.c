#include "./test.h"
#include "./object.h"


#define __AG_TEST_SUITE_ID__ 5


static ag_field *sample_int_str_small(void);
static ag_field *sample_int_str_large(void);


/*
 * Define the test cases for ag_field_new().
 */


AG_TEST_CASE("ag_field_new() can create a new sample field")
{
        AG_AUTO(ag_field) *f = sample_int_str_small();
        AG_AUTO(ag_value) *k = ag_field_key(f);
        AG_AUTO(ag_value) *v = ag_field_val(f);
        const ag_string *s = ag_value_string(v);

        AG_TEST (f && ag_value_int(k) == -1 && ag_string_eq(s, "small"));
}



AG_METATEST_OBJECT_COPY(ag_field, sample_int_str_small());
AG_METATEST_OBJECT_COPY(ag_field, sample_int_str_large());


/*
 * Define the test cases for ag_field_clone().
 */


AG_METATEST_OBJECT_CLONE(ag_field, sample_int_str_small());
AG_METATEST_OBJECT_CLONE(ag_field, sample_int_str_large());


/*
 * Define the test cases for ag_field_release().
 */


AG_METATEST_OBJECT_RELEASE(ag_field, sample_int_str_small());
AG_METATEST_OBJECT_RELEASE(ag_field, sample_int_str_large());


/*
 * Define the the test cases for ag_field_lt(). Note that since ag_field_lt() is
 * a specialised case of ag_field_cmp(), it helps to test test ag_field_cmp()
 * too.
 */


AG_METATEST_OBJECT_CMP(ag_field, sample_int_str_small(), sample_int_str_large());
AG_METATEST_OBJECT_LT(ag_field, sample_int_str_small(), sample_int_str_large());
AG_METATEST_OBJECT_EQ(ag_field, sample_int_str_small(), sample_int_str_large());
AG_METATEST_OBJECT_GT(ag_field, sample_int_str_small(), sample_int_str_large());


/*
 * Define the test case for ag_field_empty().
 */


AG_METATEST_OBJECT_EMPTY_NOT(ag_field, sample_int_str_small())
AG_METATEST_OBJECT_EMPTY_NOT(ag_field, sample_int_str_large())

/*
 * Define the test case for ag_field_typeid().
 */


AG_METATEST_OBJECT_TYPEID(ag_field, sample_int_str_small(), AG_TYPEID_FIELD);
AG_METATEST_OBJECT_TYPEID(ag_field, sample_int_str_large(), AG_TYPEID_FIELD);


/*
 * Define the test case for ag_field_uuid()
 */


AG_METATEST_OBJECT_UUID(ag_field, sample_int_str_small());
AG_METATEST_OBJECT_UUID(ag_field, sample_int_str_large());



/*
 * Define the test case for ag_field_valid().
 */


AG_METATEST_OBJECT_VALID(ag_field, sample_int_str_small());
AG_METATEST_OBJECT_VALID(ag_field, sample_int_str_large());


/*
 * Define the test case for ag_field_sz(),
 */


AG_METATEST_OBJECT_SZ(ag_field, sample_int_str_small(), 6 + sizeof(int64_t));
AG_METATEST_OBJECT_SZ(ag_field, sample_int_str_large(), 6 + sizeof(int64_t));


/*
 * Define the test cases for ag_field_refc().
 */


AG_METATEST_OBJECT_REFC(ag_field, sample_int_str_small());
AG_METATEST_OBJECT_REFC(ag_field, sample_int_str_large());


/*
 * Define the test case for ag_field_len()
 */


AG_METATEST_OBJECT_LEN(ag_field, sample_int_str_small(), 5);
AG_METATEST_OBJECT_LEN(ag_field, sample_int_str_large(), 5);


/*
 * Define the test case for ag_field_hash()
 */


AG_METATEST_OBJECT_HASH(ag_field, sample_int_str_small(), ag_hash_new(-1));
AG_METATEST_OBJECT_HASH(ag_field, sample_int_str_large(), ag_hash_new(2));


/*
 * Define the test case for ag_field_str().
 */


AG_METATEST_OBJECT_STR(ag_field, sample_int_str_small(), "-1:small");
AG_METATEST_OBJECT_STR(ag_field, sample_int_str_large(), "2:large");


extern ag_test_suite *
test_suite_field(void)
{
        return AG_TEST_SUITE_GENERATE("ag_field interface");
}


static ag_field *
sample_int_str_small(void)
{
        AG_AUTO(ag_string) *s = ag_string_new("small");
        AG_AUTO(ag_value) *k = ag_value_new_int(-1);
        AG_AUTO(ag_value) *v = ag_value_new_string(s);

        return ag_field_new(k, v);
}


static ag_field *
sample_int_str_large(void)
{
        AG_AUTO(ag_string) *s = ag_string_new("large");
        AG_AUTO(ag_value) *k = ag_value_new_int(2);
        AG_AUTO(ag_value) *v = ag_value_new_string(s);
        
        return ag_field_new(k, v);
}


