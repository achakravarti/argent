#include "./alist.h"
#include "./object.h"
#include "./test.h"


#define __AG_TEST_SUITE_ID__ 7


/* Generates function to return a sample string value */
#define VALUE_STR(tag, str)                                     \
        static inline ag_value *VALUE_STR_ ## tag(void)         \
        {                                                       \
                AG_AUTO(ag_string) *s = ag_string_new(str);     \
                return ag_value_new_string(s);                  \
        }


/* Generates function to return a sample int value */
#define VALUE_INT(tag, i)                               \
        static inline ag_value *VALUE_INT_ ## tag(void) \
        {                                               \
                return ag_value_new_int(i);             \
        }

/* Generate function to return a sample field */
#define FIELD(tag, key, val)                            \
        static inline ag_field *FIELD_ ## tag(void)     \
        {                                               \
                AG_AUTO(ag_value) *k = key;             \
                AG_AUTO(ag_value) *v = val;             \
                return ag_field_new(k, v);              \
        }


VALUE_STR(BAR, "bar");
VALUE_STR(FOO, "foo");
VALUE_STR(FOOBAR, "foobar");
VALUE_STR(KEY, "key");
VALUE_STR(VAL, "val");


VALUE_INT(1, 1);
VALUE_INT(2, 2);
VALUE_INT(3, 3);
VALUE_INT(4, 4);


FIELD(KEY_VAL, VALUE_STR_KEY(), VALUE_STR_VAL());
FIELD(1_FOO, VALUE_INT_1(), VALUE_STR_FOO());
FIELD(2_BAR, VALUE_INT_2(), VALUE_STR_BAR());
FIELD(2_FOO, VALUE_INT_2(), VALUE_STR_FOO());
FIELD(3_FOOBAR, VALUE_INT_3(), VALUE_STR_FOOBAR());
FIELD(3_BAR, VALUE_INT_3(), VALUE_STR_BAR());
FIELD(4_FOOBAR, VALUE_INT_4(), VALUE_STR_FOOBAR());


static ag_alist *sample_empty(void);
static ag_alist *sample_single(void);
static ag_alist *sample_list(void);
static ag_alist *sample_list_2(void);


AG_TEST_OBJECT_COPY(ag_alist, sample_empty());
AG_TEST_OBJECT_COPY(ag_alist, sample_single());
AG_TEST_OBJECT_COPY(ag_alist, sample_list());

AG_TEST_OBJECT_CLONE(ag_alist, sample_empty());
AG_TEST_OBJECT_CLONE(ag_alist, sample_single());
AG_TEST_OBJECT_CLONE(ag_alist, sample_list());

AG_TEST_OBJECT_RELEASE(ag_alist, sample_empty());
AG_TEST_OBJECT_RELEASE(ag_alist, sample_single());
AG_TEST_OBJECT_RELEASE(ag_alist, sample_list());

AG_TEST_OBJECT_CMP(ag_alist, sample_empty(), sample_single());
AG_TEST_OBJECT_CMP(ag_alist, sample_empty(), sample_list());
AG_TEST_OBJECT_CMP(ag_alist, sample_list(), sample_list_2());
AG_TEST_OBJECT_LT(ag_alist, sample_empty(), sample_single());
AG_TEST_OBJECT_LT(ag_alist, sample_empty(), sample_list());
AG_TEST_OBJECT_LT(ag_alist, sample_list(), sample_list_2());
AG_TEST_OBJECT_EQ(ag_alist, sample_empty(), sample_single());
AG_TEST_OBJECT_EQ(ag_alist, sample_empty(), sample_list());
AG_TEST_OBJECT_EQ(ag_alist, sample_list(), sample_list_2());
AG_TEST_OBJECT_GT(ag_alist, sample_empty(), sample_single());
AG_TEST_OBJECT_GT(ag_alist, sample_empty(), sample_list());
AG_TEST_OBJECT_GT(ag_alist, sample_list(), sample_list_2());


AG_TEST_OBJECT_EMPTY(ag_alist, sample_empty());
AG_TEST_OBJECT_EMPTY_NOT(ag_alist, sample_single())
AG_TEST_OBJECT_EMPTY_NOT(ag_alist, sample_list())


AG_TEST_OBJECT_TYPEID(ag_alist, sample_empty(), AG_TYPEID_ALIST);
AG_TEST_OBJECT_TYPEID(ag_alist, sample_single(), AG_TYPEID_ALIST);
AG_TEST_OBJECT_TYPEID(ag_alist, sample_list(), AG_TYPEID_ALIST);


AG_TEST_OBJECT_UUID(ag_alist, sample_empty());
AG_TEST_OBJECT_UUID(ag_alist, sample_single());
AG_TEST_OBJECT_UUID(ag_alist, sample_list());


AG_TEST_OBJECT_REFC(ag_alist, sample_empty());
AG_TEST_OBJECT_REFC(ag_alist, sample_single());
AG_TEST_OBJECT_REFC(ag_alist, sample_list());


AG_TEST_OBJECT_LEN(ag_alist, sample_empty(), 0);
AG_TEST_OBJECT_LEN(ag_alist, sample_single(), 1); AG_TEST_OBJECT_LEN(ag_alist, sample_list(), 3);


AG_TEST_OBJECT_STR_HAS(ag_alist, sample_empty(), "list");


AG_METATEST_ALIST_HAS_NOT(sample_empty(), FIELD_KEY_VAL());
AG_METATEST_ALIST_HAS_NOT(sample_empty(), FIELD_1_FOO());
AG_METATEST_ALIST_HAS_NOT(sample_empty(), FIELD_2_BAR());
AG_METATEST_ALIST_HAS_NOT(sample_empty(), FIELD_2_FOO());
AG_METATEST_ALIST_HAS_NOT(sample_empty(), FIELD_3_FOOBAR());
AG_METATEST_ALIST_HAS_NOT(sample_empty(), FIELD_3_BAR());
AG_METATEST_ALIST_HAS_NOT(sample_empty(), FIELD_4_FOOBAR());


AG_METATEST_ALIST_HAS(sample_single(), FIELD_KEY_VAL());


AG_METATEST_ALIST_HAS(sample_list(), FIELD_1_FOO());
AG_METATEST_ALIST_HAS(sample_list(), FIELD_2_BAR());
AG_METATEST_ALIST_HAS(sample_list(), FIELD_3_FOOBAR());
AG_METATEST_ALIST_HAS_NOT(sample_list(), FIELD_2_FOO());
AG_METATEST_ALIST_HAS_NOT(sample_list(), FIELD_3_BAR());
AG_METATEST_ALIST_HAS_NOT(sample_list(), FIELD_4_FOOBAR());


AG_METATEST_ALIST_HAS(sample_list_2(), FIELD_2_FOO());
AG_METATEST_ALIST_HAS(sample_list_2(), FIELD_3_BAR());
AG_METATEST_ALIST_HAS(sample_list_2(), FIELD_4_FOOBAR());
AG_METATEST_ALIST_HAS_NOT(sample_list_2(), FIELD_1_FOO());
AG_METATEST_ALIST_HAS_NOT(sample_list_2(), FIELD_2_BAR());
AG_METATEST_ALIST_HAS_NOT(sample_list_2(), FIELD_3_FOOBAR());


AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_STR_FOOBAR());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_STR_BAR());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_STR_FOO());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_STR_KEY());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_STR_VAL());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_INT_1());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_INT_2());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_INT_3());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_INT_4());


AG_METATEST_ALIST_HAS_KEY(sample_single(), VALUE_STR_KEY());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_single(), VALUE_STR_VAL());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_single(), VALUE_STR_FOO());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_single(), VALUE_STR_BAR());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_single(), VALUE_STR_FOOBAR());


AG_METATEST_ALIST_HAS_KEY(sample_list(), VALUE_INT_1());
AG_METATEST_ALIST_HAS_KEY(sample_list(), VALUE_INT_2());
AG_METATEST_ALIST_HAS_KEY(sample_list(), VALUE_INT_3());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_list(), VALUE_INT_4());


AG_METATEST_ALIST_HAS_KEY(sample_list_2(), VALUE_INT_2());
AG_METATEST_ALIST_HAS_KEY(sample_list_2(), VALUE_INT_3());
AG_METATEST_ALIST_HAS_KEY(sample_list_2(), VALUE_INT_4());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_list_2(), VALUE_INT_1());


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
        AG_AUTO(ag_field) *f = FIELD_KEY_VAL();
        return ag_alist_new(f);
}




static ag_alist *
sample_list(void)
{
        AG_AUTO(ag_field) *f1 = FIELD_1_FOO();
        AG_AUTO(ag_field) *f2 = FIELD_2_BAR();
        AG_AUTO(ag_field) *f3 = FIELD_3_FOOBAR();

        const ag_field *f[] = {f1, f2, f3};
        return ag_alist_new_array(f, 3);
}

        
static ag_alist *
sample_list_2(void)
{
        AG_AUTO(ag_value) *vv1 = VALUE_STR_FOO();
        AG_AUTO(ag_value) *vv2 = VALUE_STR_BAR();
        AG_AUTO(ag_value) *vv3 = VALUE_STR_FOOBAR();

        AG_AUTO(ag_value) *kv1 = VALUE_INT_2();
        AG_AUTO(ag_value) *kv2 = VALUE_INT_3();
        AG_AUTO(ag_value) *kv3 = VALUE_INT_4();

        AG_AUTO(ag_field) *f1 = FIELD_2_FOO();
        AG_AUTO(ag_field) *f2 = FIELD_3_BAR();
        AG_AUTO(ag_field) *f3 = FIELD_4_FOOBAR();

        const ag_field *f[] = {f1, f2, f3};
        return ag_alist_new_array(f, 3);
}

