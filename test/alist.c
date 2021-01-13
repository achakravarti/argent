#include "./test.h"
#include "./object.h"


#define __AG_TEST_SUITE_ID__ 7


static ag_alist *sample_empty(void);
static ag_alist *sample_single(void);
static ag_alist *sample_list(void);


AG_TEST_OBJECT_COPY(ag_alist, sample_empty());
AG_TEST_OBJECT_COPY(ag_alist, sample_single());
AG_TEST_OBJECT_COPY(ag_alist, sample_list());


extern ag_test_suite *test_suite_alist(void)
{
        return AG_TEST_SUITE_GENERATE("ag_alist interface");
}


static ag_alist *
sample_empty(void)
{
        return ag_alist_new_empty();
}


static ag_alist *
sample_single(void)
{
        AG_AUTO(ag_string) *k = ag_string_new("key");
        AG_AUTO(ag_string) *v = ag_string_new("val");
        
        AG_AUTO(ag_value) *kv = ag_value_new_string(k);
        AG_AUTO(ag_value) *vv = ag_value_new_string(v);

        AG_AUTO(ag_field) *f = ag_field_new(kv, vv);
        return ag_alist_new(f);
}


static ag_alist *
sample_list(void)
{
        AG_AUTO(ag_string) *v1 = ag_string_new("foo");
        AG_AUTO(ag_string) *v2 = ag_string_new("bar");
        AG_AUTO(ag_string) *v3 = ag_string_new("foobar");

        AG_AUTO(ag_value) *vv1 = ag_value_new_string(v1);
        AG_AUTO(ag_value) *vv2 = ag_value_new_string(v2);
        AG_AUTO(ag_value) *vv3 = ag_value_new_string(v3);

        AG_AUTO(ag_value) *kv1 = ag_value_new_int(1);
        AG_AUTO(ag_value) *kv2 = ag_value_new_int(2);
        AG_AUTO(ag_value) *kv3 = ag_value_new_int(3);

        AG_AUTO(ag_field) *f1 = ag_field_new(kv1, vv1);
        AG_AUTO(ag_field) *f2 = ag_field_new(kv2, vv2);
        AG_AUTO(ag_field) *f3 = ag_field_new(kv3, vv3);

        const ag_field *f[] = {f1, f2, f3};
        return ag_alist_new_array(f, 3);
}

