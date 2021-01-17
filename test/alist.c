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


static bool     iterator(const ag_field *, void *, void *);
static bool     iterator_mutable(ag_field **, void *, void *);


AG_METATEST_OBJECT_COPY(ag_alist, sample_empty());
AG_METATEST_OBJECT_COPY(ag_alist, sample_single());
AG_METATEST_OBJECT_COPY(ag_alist, sample_list());

AG_METATEST_OBJECT_CLONE(ag_alist, sample_empty());
AG_METATEST_OBJECT_CLONE(ag_alist, sample_single());
AG_METATEST_OBJECT_CLONE(ag_alist, sample_list());

AG_METATEST_OBJECT_RELEASE(ag_alist, sample_empty());
AG_METATEST_OBJECT_RELEASE(ag_alist, sample_single());
AG_METATEST_OBJECT_RELEASE(ag_alist, sample_list());

AG_METATEST_OBJECT_CMP(ag_alist, sample_empty(), sample_single());
AG_METATEST_OBJECT_CMP(ag_alist, sample_empty(), sample_list());
AG_METATEST_OBJECT_CMP(ag_alist, sample_list(), sample_list_2());
AG_METATEST_OBJECT_LT(ag_alist, sample_empty(), sample_single());
AG_METATEST_OBJECT_LT(ag_alist, sample_empty(), sample_list());
AG_METATEST_OBJECT_LT(ag_alist, sample_list(), sample_list_2());
AG_METATEST_OBJECT_EQ(ag_alist, sample_empty(), sample_single());
AG_METATEST_OBJECT_EQ(ag_alist, sample_empty(), sample_list());
AG_METATEST_OBJECT_EQ(ag_alist, sample_list(), sample_list_2());
AG_METATEST_OBJECT_GT(ag_alist, sample_empty(), sample_single());
AG_METATEST_OBJECT_GT(ag_alist, sample_empty(), sample_list());
AG_METATEST_OBJECT_GT(ag_alist, sample_list(), sample_list_2());


AG_METATEST_OBJECT_EMPTY(ag_alist, sample_empty());
AG_METATEST_OBJECT_EMPTY_NOT(ag_alist, sample_single());
AG_METATEST_OBJECT_EMPTY_NOT(ag_alist, sample_list());


AG_METATEST_OBJECT_TYPEID(ag_alist, sample_empty(), AG_TYPEID_ALIST);
AG_METATEST_OBJECT_TYPEID(ag_alist, sample_single(), AG_TYPEID_ALIST);
AG_METATEST_OBJECT_TYPEID(ag_alist, sample_list(), AG_TYPEID_ALIST);


AG_METATEST_OBJECT_UUID(ag_alist, sample_empty());
AG_METATEST_OBJECT_UUID(ag_alist, sample_single());
AG_METATEST_OBJECT_UUID(ag_alist, sample_list());


AG_METATEST_OBJECT_REFC(ag_alist, sample_empty());
AG_METATEST_OBJECT_REFC(ag_alist, sample_single());
AG_METATEST_OBJECT_REFC(ag_alist, sample_list());


AG_METATEST_OBJECT_LEN(ag_alist, sample_empty(), 0);
AG_METATEST_OBJECT_LEN(ag_alist, sample_single(), 1); AG_METATEST_OBJECT_LEN(ag_alist, sample_list(), 3);


AG_METATEST_OBJECT_STR_HAS(ag_alist, sample_empty(), "list");


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


AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_STR_FOOBAR());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_STR_BAR());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_STR_FOO());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_STR_KEY());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_STR_VAL());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_INT_1());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_INT_2());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_INT_3());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_INT_4());


AG_METATEST_ALIST_HAS_VAL(sample_single(), VALUE_STR_VAL());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_single(), VALUE_STR_KEY());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_single(), VALUE_STR_FOO());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_single(), VALUE_STR_BAR());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_single(), VALUE_STR_FOOBAR());


AG_METATEST_ALIST_HAS_VAL(sample_list(), VALUE_STR_FOO());
AG_METATEST_ALIST_HAS_VAL(sample_list(), VALUE_STR_FOOBAR());
AG_METATEST_ALIST_HAS_VAL(sample_list(), VALUE_STR_BAR());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_list(), VALUE_STR_KEY());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_list(), VALUE_STR_VAL());


AG_METATEST_ALIST_HAS_VAL(sample_list_2(), VALUE_STR_FOO());
AG_METATEST_ALIST_HAS_VAL(sample_list_2(), VALUE_STR_FOOBAR());
AG_METATEST_ALIST_HAS_VAL(sample_list_2(), VALUE_STR_BAR());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_list_2(), VALUE_STR_KEY());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_list_2(), VALUE_STR_VAL());


AG_METATEST_ALIST_GET(sample_single(), 0, FIELD_KEY_VAL());


AG_METATEST_ALIST_GET(sample_list(), 0, FIELD_1_FOO());
AG_METATEST_ALIST_GET(sample_list(), 1, FIELD_2_BAR());
AG_METATEST_ALIST_GET(sample_list(), 2, FIELD_3_FOOBAR());


AG_METATEST_ALIST_GET(sample_list_2(), 0, FIELD_2_FOO());
AG_METATEST_ALIST_GET(sample_list_2(), 1, FIELD_3_BAR());
AG_METATEST_ALIST_GET(sample_list_2(), 2, FIELD_4_FOOBAR());


AG_METATEST_ALIST_GET_AT(sample_single(), 1, FIELD_KEY_VAL());


AG_METATEST_ALIST_GET_AT(sample_list(), 1, FIELD_1_FOO());
AG_METATEST_ALIST_GET_AT(sample_list(), 2, FIELD_2_BAR());
AG_METATEST_ALIST_GET_AT(sample_list(), 3, FIELD_3_FOOBAR());


AG_METATEST_ALIST_GET_AT(sample_list_2(), 1, FIELD_2_FOO());
AG_METATEST_ALIST_GET_AT(sample_list_2(), 2, FIELD_3_BAR());
AG_METATEST_ALIST_GET_AT(sample_list_2(), 3, FIELD_4_FOOBAR());


AG_METATEST_ALIST_VAL(sample_single(), VALUE_STR_KEY(), VALUE_STR_VAL());


AG_METATEST_ALIST_SET(sample_single(), 0, FIELD_1_FOO());
AG_METATEST_ALIST_SET(sample_single(), 0, FIELD_2_FOO());
AG_METATEST_ALIST_SET(sample_single(), 0, FIELD_3_BAR());
AG_METATEST_ALIST_SET(sample_single(), 0, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET(sample_single(), 0, FIELD_KEY_VAL());


AG_METATEST_ALIST_SET(sample_list(), 0, FIELD_1_FOO());
AG_METATEST_ALIST_SET(sample_list(), 0, FIELD_2_FOO());
AG_METATEST_ALIST_SET(sample_list(), 0, FIELD_3_BAR());
AG_METATEST_ALIST_SET(sample_list(), 0, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET(sample_list(), 0, FIELD_KEY_VAL());
AG_METATEST_ALIST_SET(sample_list(), 1, FIELD_1_FOO());
AG_METATEST_ALIST_SET(sample_list(), 1, FIELD_2_FOO());
AG_METATEST_ALIST_SET(sample_list(), 1, FIELD_3_BAR());
AG_METATEST_ALIST_SET(sample_list(), 1, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET(sample_list(), 1, FIELD_KEY_VAL());
AG_METATEST_ALIST_SET(sample_list(), 2, FIELD_1_FOO());
AG_METATEST_ALIST_SET(sample_list(), 2, FIELD_2_FOO());
AG_METATEST_ALIST_SET(sample_list(), 2, FIELD_3_BAR());
AG_METATEST_ALIST_SET(sample_list(), 2, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET(sample_list(), 2, FIELD_KEY_VAL());


AG_METATEST_ALIST_SET(sample_list_2(), 0, FIELD_1_FOO());
AG_METATEST_ALIST_SET(sample_list_2(), 0, FIELD_2_FOO());
AG_METATEST_ALIST_SET(sample_list_2(), 0, FIELD_3_BAR());
AG_METATEST_ALIST_SET(sample_list_2(), 0, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET(sample_list_2(), 0, FIELD_KEY_VAL());
AG_METATEST_ALIST_SET(sample_list_2(), 1, FIELD_1_FOO());
AG_METATEST_ALIST_SET(sample_list_2(), 1, FIELD_2_FOO());
AG_METATEST_ALIST_SET(sample_list_2(), 1, FIELD_3_BAR());
AG_METATEST_ALIST_SET(sample_list_2(), 1, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET(sample_list_2(), 1, FIELD_KEY_VAL());
AG_METATEST_ALIST_SET(sample_list_2(), 2, FIELD_1_FOO());
AG_METATEST_ALIST_SET(sample_list_2(), 2, FIELD_2_FOO());
AG_METATEST_ALIST_SET(sample_list_2(), 2, FIELD_3_BAR());
AG_METATEST_ALIST_SET(sample_list_2(), 2, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET(sample_list_2(), 2, FIELD_KEY_VAL());


AG_METATEST_ALIST_SET_AT(sample_single(), 1, FIELD_1_FOO());
AG_METATEST_ALIST_SET_AT(sample_single(), 1, FIELD_2_FOO());
AG_METATEST_ALIST_SET_AT(sample_single(), 1, FIELD_3_BAR());
AG_METATEST_ALIST_SET_AT(sample_single(), 1, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET_AT(sample_single(), 1, FIELD_KEY_VAL());


AG_METATEST_ALIST_SET_AT(sample_list(), 1, FIELD_1_FOO());
AG_METATEST_ALIST_SET_AT(sample_list(), 1, FIELD_2_FOO());
AG_METATEST_ALIST_SET_AT(sample_list(), 1, FIELD_3_BAR());
AG_METATEST_ALIST_SET_AT(sample_list(), 1, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET_AT(sample_list(), 1, FIELD_KEY_VAL());
AG_METATEST_ALIST_SET_AT(sample_list(), 2, FIELD_1_FOO());
AG_METATEST_ALIST_SET_AT(sample_list(), 2, FIELD_2_FOO());
AG_METATEST_ALIST_SET_AT(sample_list(), 2, FIELD_3_BAR());
AG_METATEST_ALIST_SET_AT(sample_list(), 2, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET_AT(sample_list(), 2, FIELD_KEY_VAL());
AG_METATEST_ALIST_SET_AT(sample_list(), 3, FIELD_1_FOO());
AG_METATEST_ALIST_SET_AT(sample_list(), 3, FIELD_2_FOO());
AG_METATEST_ALIST_SET_AT(sample_list(), 3, FIELD_3_BAR());
AG_METATEST_ALIST_SET_AT(sample_list(), 3, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET_AT(sample_list(), 3, FIELD_KEY_VAL());


AG_METATEST_ALIST_SET_AT(sample_list_2(), 1, FIELD_1_FOO());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 1, FIELD_2_FOO());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 1, FIELD_3_BAR());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 1, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 1, FIELD_KEY_VAL());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 2, FIELD_1_FOO());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 2, FIELD_2_FOO());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 2, FIELD_3_BAR());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 2, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 2, FIELD_KEY_VAL());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 3, FIELD_1_FOO());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 3, FIELD_2_FOO());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 3, FIELD_3_BAR());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 3, FIELD_4_FOOBAR());
AG_METATEST_ALIST_SET_AT(sample_list_2(), 3, FIELD_KEY_VAL());


AG_METATEST_ALIST_VAL(sample_list(), VALUE_INT_1(), VALUE_STR_FOO());
AG_METATEST_ALIST_VAL(sample_list(), VALUE_INT_2(), VALUE_STR_BAR());
AG_METATEST_ALIST_VAL(sample_list(), VALUE_INT_3(), VALUE_STR_FOOBAR());


AG_METATEST_ALIST_VAL(sample_list_2(), VALUE_INT_2(), VALUE_STR_FOO());
AG_METATEST_ALIST_VAL(sample_list_2(), VALUE_INT_3(), VALUE_STR_BAR());
AG_METATEST_ALIST_VAL(sample_list_2(), VALUE_INT_4(), VALUE_STR_FOOBAR());


AG_METATEST_ALIST_VAL_SET(sample_single(), VALUE_STR_KEY(), VALUE_STR_FOO());
AG_METATEST_ALIST_VAL_SET(sample_single(), VALUE_STR_KEY(), VALUE_STR_BAR());
AG_METATEST_ALIST_VAL_SET(sample_single(), VALUE_STR_KEY(), VALUE_STR_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_single(), VALUE_STR_KEY(), VALUE_STR_KEY());
AG_METATEST_ALIST_VAL_SET(sample_single(), VALUE_STR_KEY(), VALUE_STR_VAL());


AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_1(), VALUE_STR_KEY());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_1(), VALUE_STR_VAL());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_1(), VALUE_STR_FOO());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_1(), VALUE_STR_BAR());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_1(), VALUE_STR_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_1(), VALUE_INT_1());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_1(), VALUE_INT_2());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_1(), VALUE_INT_3());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_1(), VALUE_INT_4());


AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_2(), VALUE_STR_KEY());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_2(), VALUE_STR_VAL());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_2(), VALUE_STR_FOO());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_2(), VALUE_STR_BAR());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_2(), VALUE_STR_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_2(), VALUE_INT_1());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_2(), VALUE_INT_2());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_2(), VALUE_INT_3());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_2(), VALUE_INT_4());


AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_3(), VALUE_STR_KEY());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_3(), VALUE_STR_VAL());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_3(), VALUE_STR_FOO());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_3(), VALUE_STR_BAR());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_3(), VALUE_STR_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_3(), VALUE_INT_1());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_3(), VALUE_INT_2());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_3(), VALUE_INT_3());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_INT_3(), VALUE_INT_4());


AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_2(), VALUE_STR_KEY());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_2(), VALUE_STR_VAL());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_2(), VALUE_STR_FOO());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_2(), VALUE_STR_BAR());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_2(), VALUE_STR_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_2(), VALUE_INT_1());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_2(), VALUE_INT_2());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_2(), VALUE_INT_3());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_2(), VALUE_INT_4());


AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_3(), VALUE_STR_KEY());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_3(), VALUE_STR_VAL());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_3(), VALUE_STR_FOO());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_3(), VALUE_STR_BAR());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_3(), VALUE_STR_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_3(), VALUE_INT_1());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_3(), VALUE_INT_2());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_3(), VALUE_INT_3());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_3(), VALUE_INT_4());


AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_4(), VALUE_STR_KEY());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_4(), VALUE_STR_VAL());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_4(), VALUE_STR_FOO());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_4(), VALUE_STR_BAR());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_4(), VALUE_STR_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_4(), VALUE_INT_1());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_4(), VALUE_INT_2());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_4(), VALUE_INT_3());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_INT_4(), VALUE_INT_4());


AG_TEST_CASE("ag_alist_map(): sample_empty() => no effect")
{
        AG_AUTO(ag_alist) *a = sample_empty();
        ag_int sum = 0;
        ag_alist_map(a, iterator, NULL, &sum);

        AG_TEST (!sum);
}


AG_TEST_CASE("ag_alist_map(): sample_list() => sum")
{
        AG_AUTO(ag_alist) *a = sample_list();
        ag_int sum = 0;
        ag_alist_map(a, iterator, NULL, &sum);

        AG_TEST (sum == 6);
}


AG_TEST_CASE("ag_alist_map_mutable(): sample_empty() => no effect")
{
        AG_AUTO(ag_alist) *a = sample_empty();
        ag_int sum = 0;

        ag_alist_map_mutable(&a, iterator_mutable, NULL, NULL);
        ag_alist_map(a, iterator, NULL, &sum);

        AG_TEST (!sum);
}


AG_TEST_CASE("ag_alist_map_mutable(): sample_list() => sum")
{
        AG_AUTO(ag_alist) *a = sample_list();
        ag_int sum = 0;

        ag_alist_map_mutable(&a, iterator_mutable, NULL, NULL);
        ag_alist_map(a, iterator, NULL, &sum);

        AG_TEST (sum = 99);
}


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


static bool
iterator(const ag_field *attr, void *in, void *out)
{
        (void)in;
        ag_int *s = out;

        AG_AUTO(ag_value) *k = ag_field_key(attr);
        ag_int i = ag_value_int(k);
        *s += i;

        return true;
}


static bool
iterator_mutable(ag_field **attr, void *in, void *out)
{
        (void)in;
        (void)out;

        AG_AUTO(ag_value) *v = ag_value_new_int(5);
        ag_field_val_set(attr, v);

        return true;
}

