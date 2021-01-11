#include "./test.h"

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


/*
 * Define the test cases for ag_field_copy().
 */


AG_TEST_CASE("ag_field_copy() creates a shallow copy a field")
{
        AG_AUTO(ag_field) *f = sample_int_str_small();
        AG_AUTO(ag_field) *cp = ag_field_copy(f);

        AG_TEST (cp == f);
}


AG_TEST_CASE("ag_field_copy() increases the reference count by 1")
{
        AG_AUTO(ag_field) *f = sample_int_str_small();
        AG_AUTO(ag_field) *cp = ag_field_copy(f);

        AG_TEST (ag_field_refc(f) == 2);
}


/*
 * Define the test cases for ag_field_clone().
 */


AG_TEST_CASE("ag_field_clone() creates a clone of a field")
{
        AG_AUTO(ag_field) *f = sample_int_str_small();
        AG_AUTO(ag_field) *cp = ag_field_clone(f);

        AG_TEST (cp && (f != cp));
}


AG_TEST_CASE("ag_field_clone() is identical to its parent")
{
        AG_AUTO(ag_field) *f = sample_int_str_small();
        AG_AUTO(ag_field) *cp = ag_field_clone(f);

        AG_TEST (ag_field_eq(f, cp));
}


AG_TEST_CASE("ag_field_clone() does not change the reference count")
{
        AG_AUTO(ag_field) *f = sample_int_str_small();
        AG_AUTO(ag_field) *cp = ag_field_clone(f);

        AG_TEST (ag_field_eq(f, cp));
}


/*
 * Define the test cases for ag_field_release().
 */


AG_TEST_CASE("ag_field_release() reduces the reference count by 1")
{
        ag_field *f = sample_int_str_small();
        AG_AUTO(ag_field) *cp = ag_field_copy(f);
        AG_AUTO(ag_list) *cp2 = ag_field_copy(cp);
        ag_field_release(&f);

        AG_TEST (!f && ag_field_refc(cp2) == 2);
}


AG_TEST_CASE("ag_field_release() performs a no-op if passed NULL")
{
        ag_field_release(NULL);
        AG_TEST (true);
}


AG_TEST_CASE("ag_field_release() performs a no-op if passed a handle to NULL")
{
        ag_list *f = NULL;
        ag_field_release(&f);

        AG_TEST (true);
}


AG_TEST_CASE("ag_field_release() disposes a single instance of a field")
{
        ag_list *f = sample_int_str_small();
        ag_field_release(&f);

        AG_TEST (!f);
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


