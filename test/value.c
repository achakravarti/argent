#include "../src/api.h"
#include "./test.h"



/*******************************************************************************
 *                               INT VALUE TESTS
 */

static void int_new(void)
{
    printf("ag_value_new_int() creates a new int value");

    ag_value_smart_t *v = ag_value_new_int(-123456);
    ag_require (v && ag_value_int(v) == -123456, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void int_copy(void)
{
    printf("ag_value_copy() copies an int value");

    ag_value_smart_t *v = ag_value_new_int(-1029394);
    ag_value_smart_t *cp = ag_value_copy(v);
    ag_require (ag_value_int(v) == ag_value_int(cp), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void int_is_int(void)
{
    printf("ag_value_is_int() is true for an int value");

    ag_value_smart_t *v = ag_value_new_int(11111);
    ag_require (v && ag_value_is_int(v), AG_ERNO_TEST, NULL);
    
    printf("...OK\n");
}

static void int_is_uint(void)
{
    printf("ag_value_is_uint() is false for an int value");
    
    ag_value_smart_t *v = ag_value_new_int(-1111);
    ag_require (v && !ag_value_is_uint(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void int_is_float(void)
{
    printf("ag_value_is_float() is false for an int value");

    ag_value_smart_t *v = ag_value_new_int(1111);
    ag_require (v && !ag_value_is_float(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void int_is_string(void)
{
    printf("ag_value_is_string() is false of an int value");

    ag_value_smart_t *v = ag_value_new_int(-987654321);
    ag_require (v && !ag_value_is_string(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void int_is_object(void)
{
    printf("ag_value_is_object() is false for an int value");

    ag_value_smart_t *v = ag_value_new_int(987654321);
    ag_require (v && !ag_value_is_object(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void int_lt_1(void)
{
    printf("ag_value_lt() returns true for -123 < 123");

    ag_value_smart_t *v1 = ag_value_new_int(-123);
    ag_value_smart_t *v2 = ag_value_new_int(123);
    ag_require (ag_value_lt(v1, v2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void int_lt_2(void)
{
    printf("ag_value_lt() returns false for -123 < -123");

    ag_value_smart_t *v = ag_value_new_int(-123);
    ag_require (!ag_value_lt(v, v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void int_lt_3(void)
{
    printf("ag_value_lt() returns false for 123 < -123");

    ag_value_smart_t *v1 = ag_value_new_int(123);
    ag_value_smart_t *v2 = ag_value_new_int(-123);
    ag_require (!ag_value_lt(v1, v2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void int_eq_1(void)
{
    printf("ag_value_eq() returns true for -123 == -123");

    ag_value_smart_t *v = ag_value_new_int(-123);
    ag_require (ag_value_eq(v, v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void int_eq_2(void)
{
    printf("ag_value_eq() returns false for -123 == 123");

    ag_value_smart_t *v1 = ag_value_new_int(-123);
    ag_value_smart_t *v2 = ag_value_new_int(123);
    ag_require (!ag_value_eq(v1, v2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void int_gt_1(void)
{
    printf("ag_value_gt() returns true for 123 > -123");

    ag_value_smart_t *v1 = ag_value_new_int(123);
    ag_value_smart_t *v2 = ag_value_new_int(-123);
    ag_require (ag_value_gt(v1, v2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void int_gt_2(void)
{
    printf("ag_value_gt() returns false for -123 > -123");

    ag_value_smart_t *v = ag_value_new_int(-123);
    ag_require (!ag_value_gt(v, v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void int_gt_3(void)
{
    printf("ag_value_gt() returns false for -123 > 123");

    ag_value_smart_t *v1 = ag_value_new_int(-123);
    ag_value_smart_t *v2 = ag_value_new_int(123);
    ag_require (!ag_value_gt(v1, v2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


static void int_test(void)
{
    int_new();
    int_copy();
    int_is_int();
    int_is_uint();
    int_is_float();
    int_is_string();
    int_is_object();
    int_lt_1();
    int_lt_2();
    int_lt_3();
    int_eq_1();
    int_eq_2();
    int_gt_1();
    int_gt_2();
    int_gt_3();
}


/*******************************************************************************
 *                               UINT VALUE TESTS
 */

static void uint_new(void)
{
    printf("ag_value_new_uint() creates a new uint value");

    ag_value_smart_t *v = ag_value_new_uint(123456);
    ag_require (v && ag_value_uint(v) == 123456, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_copy(void)
{
    printf("ag_value_copy() copies a uint value");

    ag_value_smart_t *v = ag_value_new_uint(1029394);
    ag_value_smart_t *cp = ag_value_copy(v);
    ag_require (ag_value_uint(v) == ag_value_uint(cp), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_is_int(void)
{
    printf("ag_value_is_int() is false for a uint value");

    ag_value_smart_t *v = ag_value_new_uint(11111);
    ag_require (v && !ag_value_is_int(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_is_uint(void)
{
    printf("ag_value_is_uint() is true for a uint value");
    
    ag_value_smart_t *v = ag_value_new_uint(701115);
    ag_require (v && ag_value_is_uint(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_is_float(void)
{
    printf("ag_value_is_float() is false for a uint value");
    
    ag_value_smart_t *v = ag_value_new_uint(97324);
    ag_require (v && !ag_value_is_float(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_is_string(void)
{
    printf("ag_value_is_string() is false for a uint value");
    
    ag_value_smart_t *v = ag_value_new_uint(987654321);
    ag_require (v && !ag_value_is_string(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_is_object(void)
{
    printf("ag_value_is_object() is false for a uint value");
    
    ag_value_smart_t *v = ag_value_new_uint(108);
    ag_require (v && !ag_value_is_object(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_lt_1(void)
{
    printf("ag_value_lt() returns true for 123 < 124");

    ag_value_smart_t *v1 = ag_value_new_uint(123);
    ag_value_smart_t *v2 = ag_value_new_uint(124);
    ag_require (ag_value_lt(v1, v2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_lt_2(void)
{
    printf("ag_value_lt() returns false for 123 < 123");

    ag_value_smart_t *v = ag_value_new_uint(123);
    ag_require (!ag_value_lt(v, v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_lt_3(void)
{
    printf("ag_value_lt() returns false for 124 < 123");

    ag_value_smart_t *v1 = ag_value_new_uint(124);
    ag_value_smart_t *v2 = ag_value_new_uint(123);
    ag_require (!ag_value_lt(v1, v2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_eq_1(void)
{
    printf("ag_value_eq() returns true for 123 == 123");

    ag_value_smart_t *v = ag_value_new_uint(123);
    ag_require (ag_value_eq(v, v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_eq_2(void)
{
    printf("ag_value_eq() returns false for 123 == 124");

    ag_value_smart_t *v1 = ag_value_new_uint(123);
    ag_value_smart_t *v2 = ag_value_new_uint(124);
    ag_require (!ag_value_eq(v1, v2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_gt_1(void)
{
    printf("ag_value_gt() returns true for 124 > 123");

    ag_value_smart_t *v1 = ag_value_new_uint(124);
    ag_value_smart_t *v2 = ag_value_new_uint(123);
    ag_require (ag_value_gt(v1, v2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_gt_2(void)
{
    printf("ag_value_gt() returns false for 123 > 123");

    ag_value_smart_t *v = ag_value_new_uint(123);
    ag_require (!ag_value_gt(v, v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_gt_3(void)
{
    printf("ag_value_gt() returns false for 123 > 124");

    ag_value_smart_t *v1 = ag_value_new_uint(123);
    ag_value_smart_t *v2 = ag_value_new_uint(124);
    ag_require (!ag_value_gt(v1, v2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void uint_test(void)
{
    uint_new();
    uint_copy();
    uint_is_int();
    uint_is_uint();
    uint_is_float();
    uint_is_string();
    uint_is_object();
    uint_lt_1();
    uint_lt_2();
    uint_lt_3();
    uint_eq_1();
    uint_eq_2();
    uint_gt_1();
    uint_gt_2();
    uint_gt_3();
}


/*******************************************************************************
 *                              FLOAT VALUE TESTS
 */

static void float_new(void)
{
    printf("ag_value_new_float() creates a new float value");
    
    ag_value_smart_t *v = ag_value_new_float(-123.456);
    ag_require (v && ag_value_float(v) == -123.456, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void float_copy(void)
{
    printf("ag_value_copy() copies a float value");

    ag_value_smart_t *v = ag_value_new_float(123.456);
    ag_value_smart_t *cp = ag_value_copy(v);
    ag_require (ag_value_float(v) == ag_value_float(cp), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void float_is_int(void)
{
    printf("ag_value_is_int() is false for a float value");
    
    ag_value_smart_t *v = ag_value_new_float(-123456.789);
    ag_require (v && !ag_value_is_int(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void float_is_uint(void)
{
    printf("ag_value_is_uint() is false for a float value");
    
    ag_value_smart_t *v = ag_value_new_float(123456.789);
    ag_require (v && !ag_value_is_uint(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void float_is_float(void)
{
    printf("ag_value_is_float() is true for a float value");
    
    ag_value_smart_t *v = ag_value_new_float(-789.123456);
    ag_require (v && ag_value_is_float(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void float_is_string(void)
{
    printf("ag_value_is_string() is false for a float value");
    
    ag_value_smart_t *v = ag_value_new_float(789.123456);
    ag_require (v && !ag_value_is_string(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void float_is_object(void)
{
    printf("ag_value_is_object is false for a float value ");
    
    ag_value_smart_t *v = ag_value_new_float(-9876.5433);
    ag_require (v && !ag_value_is_object(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void float_test(void)
{
    float_new();
    float_copy();
    float_is_int();
    float_is_uint();
    float_is_float();
    float_is_string();
    float_is_object();
}

/*******************************************************************************
 *                              STRING VALUE TESTS
 */

static inline ag_value_t *string_sample_ascii(void)
{
    ag_string_smart_t *s = ag_string_new("Hello, world!");
    return ag_value_new_string(s);
}

static inline ag_value_t *string_sample_unicode(void)
{
    ag_string_smart_t *s = ag_string_new("Привет, мир!");
    return ag_value_new_string(s);
}

static void string_new(void)
{
    printf("ag_value_new_string() creates a new string value");

    ag_value_smart_t *v = string_sample_ascii();
    ag_value_smart_t *s = ag_string_new("Hello, world!");
    ag_require (v && ag_string_eq(ag_value_string(v), s), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void string_copy(void)
{
    printf("ag_value_copy() copies a string value");
    
    ag_value_smart_t *v = string_sample_unicode();
    ag_value_smart_t *cp = ag_value_copy(v);
    ag_require (ag_string_eq(ag_value_string(v), ag_value_string(cp)),
        AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void string_is_int(void)
{
    printf("ag_value_is_int() is false for a string value");

    ag_value_smart_t *v = string_sample_ascii();
    ag_require (!ag_value_is_int(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void string_is_uint(void)
{
    printf("ag_value_is_uint() is false for a string value");
    
    ag_value_smart_t *v = string_sample_unicode();
    ag_require (!ag_value_is_int(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void string_is_float(void)
{
    printf("ag_value_is_float() is false for a string value");
    
    ag_value_smart_t *v = string_sample_ascii();
    ag_require (!ag_value_is_float(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void string_is_string(void)
{
    printf("ag_value_is_string() is true for a string value");
    
    ag_value_smart_t *v = string_sample_unicode();
    ag_require (ag_value_is_string(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void string_is_object(void)
{
    printf("ag_value_is_object() is false for a string value");
    
    ag_value_smart_t *v = string_sample_ascii();
    ag_require (!ag_value_is_object(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void string_test(void)
{
    string_new();
    string_copy();
    string_is_int();
    string_is_uint();
    string_is_float();
    string_is_string();
    string_is_object();
}


/*******************************************************************************
 *                              OBJECT VALUE TESTS
 */

struct object_payload {
    int x;
    int y;
};

#define OBJECT_CLASS 103

static inline enum ag_tristate object_cmp(const ag_object_t *lhs, 
    const ag_object_t *rhs)
{
    const struct object_payload *p1 = ag_object_payload(lhs);
    const struct object_payload *p2 = ag_object_payload(rhs);

    if (p1->x == p2->x && p1->y == p2->y)
        return AG_TRISTATE_GND;

    return AG_TRISTATE_LO;
}

static inline void object_register(void)
{
    struct ag_object_vtable vt = {
        .copy = NULL,
        .dispose = NULL,
        .id = NULL,
        .sz = NULL,
        .len = NULL,
        .hash = NULL,
        .cmp = &object_cmp,
        .str = NULL
    };

    ag_object_init(32);
    ag_object_register(OBJECT_CLASS, &vt);
}


static ag_object_t *object_sample(void)
{
    struct object_payload *p = ag_memblock_new(sizeof *p);
    p->x = 555;
    p->y = 666;
    return ag_object_new(OBJECT_CLASS, p);
}

static inline ag_value_t *object_sample_value(void)
{
    ag_object_smart_t *o = object_sample();
    return ag_value_new_object(o);
}


static void object_new(void)
{
    printf("ag_value_new_object() creates a new object value");

    ag_object_smart_t *o = object_sample();
    ag_value_smart_t *v = object_sample_value();
    ag_require (v && ag_object_eq(ag_value_object(o), o), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void object_copy(void)
{
    printf("ag_value_copy() copies an object value");
    
    ag_value_smart_t *v = object_sample_value();
    ag_value_smart_t *cp = ag_value_copy(v);
    ag_require (ag_object_eq(ag_value_object(v), ag_value_object(cp)), 
        AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void object_is_int(void)
{
    printf("ag_value_is_int() is false for an object value");

    ag_value_smart_t *v = object_sample_value();
    ag_require (!ag_value_is_int(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void object_is_uint(void)
{
    printf("ag_value_is_uint() is false for an object value");

    ag_value_smart_t *v = object_sample_value();
    ag_require (!ag_value_is_uint(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void object_is_float(void)
{
    printf("ag_value_is_float() is false for an object value");

    ag_value_smart_t *v = object_sample_value();
    ag_require (!ag_value_is_float(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void object_is_string(void)
{
    printf("ag_value_is_string() is false for an object value");

    ag_value_smart_t *v = object_sample_value();
    ag_require (!ag_value_is_string(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void object_is_object(void)
{
    printf("ag_value_is_object() is true for an object value");

    ag_value_smart_t *v = object_sample_value();
    ag_require (ag_value_is_object(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void object_test(void)
{

    object_register();

    object_new();
    object_copy();
    object_is_int();
    object_is_uint();
    object_is_float();
    object_is_string();
    object_is_object();
}


/*******************************************************************************
 *                                  TEST SUITE
 */


extern void ag_test_value(void)
{
    printf("===============================================================\n");
    printf("Starting dynamic type interface test suite...\n\n");

    int_test();
    uint_test();
    float_test();
    string_test();
    object_test();

    printf("\n");
}
