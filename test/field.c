#include "./test.h"
#include "./object.h"


#define __AG_TEST_SUITE_ID__ 6


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



AG_TEST_OBJECT_COPY(ag_field, sample_int_str_small());
AG_TEST_OBJECT_COPY(ag_field, sample_int_str_large());


/*
 * Define the test cases for ag_field_clone().
 */


AG_TEST_OBJECT_CLONE(ag_field, sample_int_str_small());
AG_TEST_OBJECT_CLONE(ag_field, sample_int_str_large());


/*
 * Define the test cases for ag_field_release().
 */


AG_TEST_OBJECT_RELEASE(ag_field, sample_int_str_small());
AG_TEST_OBJECT_RELEASE(ag_field, sample_int_str_large());


/*
 * Define the the test cases for ag_field_lt(). Note that since ag_field_lt() is
 * a specialised case of ag_field_cmp(), it helps to test test ag_field_cmp()
 * too.
 */


AG_TEST_OBJECT_CMP(ag_field, sample_int_str_small(), sample_int_str_large());
AG_TEST_OBJECT_LT(ag_field, sample_int_str_small(), sample_int_str_large());
AG_TEST_OBJECT_EQ(ag_field, sample_int_str_small(), sample_int_str_large());
AG_TEST_OBJECT_GT(ag_field, sample_int_str_small(), sample_int_str_large());


/*
 * Define the test case for ag_field_empty().
 */


AG_TEST_OBJECT_EMPTY_NOT(ag_field, sample_int_str_small())
AG_TEST_OBJECT_EMPTY_NOT(ag_field, sample_int_str_large())

/*
 * Define the test case for ag_field_typeid().
 */


AG_TEST_OBJECT_TYPEID(ag_field, sample_int_str_small(), AG_TYPEID_FIELD);
AG_TEST_OBJECT_TYPEID(ag_field, sample_int_str_large(), AG_TYPEID_FIELD);


/*
 * Define the test case for ag_field_uuid()
 */


AG_TEST_OBJECT_UUID(ag_field, sample_int_str_small());
AG_TEST_OBJECT_UUID(ag_field, sample_int_str_large());



/*
 * Define the test case for ag_field_valid().
 */


AG_TEST_OBJECT_VALID(ag_field, sample_int_str_small());
AG_TEST_OBJECT_VALID(ag_field, sample_int_str_large());


/*
 * Define the test case for ag_field_sz(),
 */


AG_TEST_CASE("ag_field_sz() returns the cumulative size of a field")
{
        AG_AUTO(ag_field) *f = sample_int_str_small();
        AG_AUTO(ag_value) *v = ag_field_val(f);
        const ag_string *s = ag_value_string(v);

        AG_TEST (ag_field_sz(f) == sizeof(int64_t) + ag_string_sz(s));
}


/*
 * Define the test cases for ag_field_refc().
 */


AG_TEST_CASE("ag_field_refc() returns 1 for a single instance")
{
        AG_AUTO(ag_field) *f = sample_int_str_small();
        AG_TEST (ag_field_refc(f) == 1);
}


AG_TEST_CASE("ag_field_refc() returns the reference count of a shallow copy")
{
        AG_AUTO(ag_field) *f = sample_int_str_small();
        AG_AUTO(ag_field) *cp = ag_field_copy(f);
        AG_AUTO(ag_field) *cp2 = ag_field_copy(cp);

        AG_TEST (ag_field_refc(f) == 3);
}


/*
 * Define the test case for ag_field_len()
 */


AG_TEST_OBJECT_LEN(ag_field, sample_int_str_small(), 5);
AG_TEST_OBJECT_LEN(ag_field, sample_int_str_large(), 5);


/*
 * Define the test case for ag_field_hash()
 */


AG_TEST_CASE("ag_field_hash() gets the hash of the key")
{
        AG_AUTO(ag_field) *f = sample_int_str_small();
        AG_AUTO(ag_value) *v = ag_field_key(f);
        ag_int i = ag_value_int(v);

        AG_TEST (ag_field_hash(f) == ag_hash_new(i));
}


/*
 * Define the test case for ag_field_str().
 */

AG_TEST_CASE("ag_field_str() generates a string representation of a field")
{
        AG_AUTO(ag_field) *f = sample_int_str_small();
        AG_AUTO(ag_string) *s = ag_field_str(f);

        AG_TEST (!ag_string_empty(s));
}


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


