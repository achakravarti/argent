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

static void int_test(void)
{
    int_new();
    int_copy();
    int_is_int();
    int_is_uint();
    int_is_float();
    int_is_string();
    int_is_object();
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

static void uint_test(void)
{
    uint_new();
    uint_copy();
    uint_is_int();
    uint_is_uint();
    uint_is_float();
    uint_is_string();
    uint_is_object();
}


/*******************************************************************************
 *                              FLOAT VALUE TESTS
 */

static void float_new(void)
{
    printf("ag_value_new_float() creates a new float value");
    printf("...OK\n");
}

static void float_copy(void)
{
    printf("ag_value_copy() copies a float value");
    printf("...OK\n");
}

static void float_is_int(void)
{
    printf("ag_value_is_int() is false for a float value");
    printf("...OK\n");
}

static void float_is_uint(void)
{
    printf("ag_value_is_uint() is false for a float value");
    printf("...OK\n");
}

static void float_is_float(void)
{
    printf("ag_value_is_float() is true for a float value");
    printf("...OK\n");
}

static void float_is_string(void)
{
    printf("ag_value_is_string() is false for a float value");
    printf("...OK\n");
}

static void float_is_object(void)
{
    printf("ag_value_is_object is false for a float value ");
    printf("...OK\n");
}

static void float_test(void)
{
    /*float_new();
    float_copy();
    float_is_int();
    float_is_uint();
    float_is_float();
    float_is_string();
    float_is_object();*/
}

/*******************************************************************************
 *                              STRING VALUE TESTS
 */

static void string_new(void)
{
    printf("ag_value_new_string() creates a new string value");
    printf("...OK\n");
}

static void string_copy(void)
{
    printf("ag_value_copy() copies a string value");
    printf("...OK\n");
}

static void string_is_int(void)
{
    printf("ag_value_is_int() is false for a string value");
    printf("...OK\n");
}

static void string_is_uint(void)
{
    printf("ag_value_is_uint() is false for a string value");
    printf("...OK\n");
}

static void string_is_float(void)
{
    printf("ag_value_is_float() is false for a string value");
    printf("...OK\n");
}

static void string_is_string(void)
{
    printf("ag_value_is_string() is true for a string value");
    printf("...OK\n");
}

static void string_is_object(void)
{
    printf("ag_value_is_object() is false for a string value");
    printf("...OK\n");
}

static void string_test(void)
{
    /*string_new();
    string_copy();
    string_is_int();
    string_is_uint();
    string_is_float();
    string_is_string();
    string_is_object();*/
}


/*******************************************************************************
 *                              OBJECT VALUE TESTS
 */

static void object_new(void)
{
    printf("ag_value_new_object() creates a new object value");
    printf("...OK\n");
}

static void object_copy(void)
{
    printf("ag_value_copy() copies an object value");
    printf("...OK\n");
}

static void object_is_int(void)
{
    printf("ag_value_is_int() is false for an object value");
    printf("...OK\n");
}

static void object_is_uint(void)
{
    printf("ag_value_is_uint() is false for an object value");
    printf("...OK\n");
}

static void object_is_float(void)
{
    printf("ag_value_is_float() is false for an object value");
    printf("...OK\n");
}

static void object_is_string(void)
{
    printf("ag_value_is_string() is false for an object value");
    printf("...OK\n");
}

static void object_is_object(void)
{
    printf("ag_value_is_object() is true for an object value");
    printf("...OK\n");
}

static void object_test(void)
{
    /*object_new();
    object_copy();
    object_is_int();
    object_is_uint();
    object_is_float();
    object_is_string();
    object_is_object();*/
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
