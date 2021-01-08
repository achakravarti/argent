#include "./test.h"

#define __AG_TEST_SUITE_TAG__ 4


struct object_payload {
        int     x;
        int     y;

};


#define OBJECT_TYPEID ((ag_typeid)1)


static enum ag_cmp
object_cmp(const ag_object *lhs, const ag_object *rhs)
{
    const struct object_payload *p1 = ag_object_payload(lhs);
    const struct object_payload *p2 = ag_object_payload(rhs);

    if (p1->x == p2->x && p1->y == p2->y)
        return AG_CMP_EQ;

    return AG_CMP_LT;
}


static inline void
object_register(void)
{
    struct ag_object_vtable vt = {
            .clone = NULL, .release = NULL, .cmp = object_cmp,
            .valid = NULL, .sz = NULL,      .len = NULL,
            .hash = NULL,  .str = NULL,
    };

    ag_object_registry_set(OBJECT_TYPEID, &vt);
}


static ag_object *
object_sample(void)
{
    struct object_payload *p = ag_memblock_new(sizeof *p);
    p->x = 555;
    p->y = 666;

    return ag_object_new(OBJECT_TYPEID, p);
}


static inline ag_value *
object_sample_value(void)
{
    AG_AUTO(ag_object) *o = object_sample();

    return ag_value_new_object(o);
}


__AG_TEST_CASE("ag_value_new_int() creates a new int value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-123456);

        AG_TEST (v && ag_value_int(v) == -123456);
}


__AG_TEST_CASE("ag_value_copy() copies an int value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-1029394);
        AG_AUTO(ag_value) *cp = ag_value_copy(v);

        AG_TEST (ag_value_int(v) == ag_value_int(cp));
}


__AG_TEST_CASE("ag_value_type_int() is true for an int value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(11111);

        AG_TEST (v && ag_value_type_int(v));
}


__AG_TEST_CASE("ag_value_type_uint() is false for a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-1111);

        AG_TEST (v && !ag_value_type_uint(v));
}


__AG_TEST_CASE(
    "ag_value_type_float() is false for a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(1111);

        AG_TEST (v && !ag_value_type_float(v));
}


__AG_TEST_CASE(
    "ag_value_type_string() is false for a string value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-987654321);

        AG_TEST (v && !ag_value_type_string(v));
}


__AG_TEST_CASE(
    "ag_value_type_object() is false for an object value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(987654321);

        AG_TEST (v && !ag_value_type_object(v));
}


__AG_TEST_CASE("ag_value_lt() returns true for -123 < 123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(-123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(123);

        AG_TEST (ag_value_lt(v1, v2));
}


__AG_TEST_CASE("ag_value_lt() returns false for -123 < -123")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-123);

        AG_TEST (!ag_value_lt(v, v));
}


__AG_TEST_CASE("ag_value_lt() returns false for 123 < -123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(-123);

        AG_TEST (!ag_value_lt(v1, v2));
}


__AG_TEST_CASE("ag_value_eq() returns true for -123 == -123")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-123);

        AG_TEST (ag_value_eq(v, v));
}


__AG_TEST_CASE("ag_value_eq() returns false for -123 == 123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(-123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(123);

        AG_TEST (!ag_value_eq(v1, v2));
}


__AG_TEST_CASE("ag_value_gt() returns true for 123 > -123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(-123);

        AG_TEST (ag_value_gt(v1, v2));
}


__AG_TEST_CASE("ag_value_gt() returns false for -123 > -123")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-123);

        AG_TEST (!ag_value_gt(v, v));
}


__AG_TEST_CASE("ag_value_gt() returns false for -123 > 123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(-123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(123);

        AG_TEST (!ag_value_gt(v1, v2));
}




__AG_TEST_CASE("ag_value_new_uint() creates a new uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(123456);

        AG_TEST (v && ag_value_uint(v) == 123456);
}


__AG_TEST_CASE("ag_value_copy() copies a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(1029394);
        AG_AUTO(ag_value) *cp = ag_value_copy(v);

        AG_TEST (ag_value_uint(v) == ag_value_uint(cp));
}


__AG_TEST_CASE("ag_value_type_int() is false for a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(11111);

        AG_TEST (v && !ag_value_type_int(v));
}


__AG_TEST_CASE("ag_value_type_uint() is true for a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(701115);

        AG_TEST (v && ag_value_type_uint(v));
}


__AG_TEST_CASE(
    "ag_value_type_float() is false for a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(97324);

        AG_TEST (v && !ag_value_type_float(v));
}


__AG_TEST_CASE(
    "ag_value_type_string() is false for a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(987654321);

        AG_TEST (v && !ag_value_type_string(v));
}


__AG_TEST_CASE(
    "ag_value_type_object() is false for a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(108);

        AG_TEST (v && !ag_value_type_object(v));
}


__AG_TEST_CASE("ag_value_lt() returns true for 123 < 124")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_uint(123);
        AG_AUTO(ag_value) *v2 = ag_value_new_uint(124);

        AG_TEST (ag_value_lt(v1, v2));
}


__AG_TEST_CASE("ag_value_lt() returns false for 123 < 123")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(123);

        AG_TEST (!ag_value_lt(v, v));
}


__AG_TEST_CASE("ag_value_lt() returns false for 124 < 123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_uint(124);
        AG_AUTO(ag_value) *v2 = ag_value_new_uint(123);

        AG_TEST (!ag_value_lt(v1, v2));
}


__AG_TEST_CASE("ag_value_eq() returns true for 123 == 123")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(123);

        AG_TEST (ag_value_eq(v, v));
}


__AG_TEST_CASE("ag_value_eq() returns false for 123 == 124") {
        AG_AUTO(ag_value) *v1 = ag_value_new_uint(123);
        AG_AUTO(ag_value) *v2 = ag_value_new_uint(124);

        AG_TEST (!ag_value_eq(v1, v2));
}


__AG_TEST_CASE("ag_value_gt() returns true for 124 > 123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_uint(124);
        AG_AUTO(ag_value) *v2 = ag_value_new_uint(123);

        AG_TEST (ag_value_gt(v1, v2));
}


__AG_TEST_CASE("ag_value_gt() returns false for 123 > 123")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(123);

        AG_TEST (!ag_value_gt(v, v));
}


__AG_TEST_CASE("ag_value_gt() returns false for 123 > 124")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_uint(123);
        AG_AUTO(ag_value) *v2 = ag_value_new_uint(124);

        AG_TEST (!ag_value_gt(v1, v2));
}




__AG_TEST_CASE("ag_value_new_float() creates a new float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(-123.456);

        AG_TEST (v && ag_value_float(v) == -123.456);
}


__AG_TEST_CASE("ag_value_copy() copies a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(123.456);
        AG_AUTO(ag_value) *cp = ag_value_copy(v);

        AG_TEST (ag_value_float(v) == ag_value_float(cp));
}


__AG_TEST_CASE("ag_value_type_int() is false for a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(-123456.789);

        AG_TEST (v && !ag_value_type_int(v));
}


__AG_TEST_CASE(
    "ag_value_type_uint() is false for a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(123456.789);

        AG_TEST (v && !ag_value_type_uint(v));
}


__AG_TEST_CASE(
    "ag_value_type_float() is true for a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(-789.123456);

        AG_TEST (v && ag_value_type_float(v));
}


__AG_TEST_CASE(
    "ag_value_type_string() is false for a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(789.123456);

        AG_TEST (v && !ag_value_type_string(v));
}


__AG_TEST_CASE(
    "ag_value_type_object is false for a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(-9876.5433);

        AG_TEST (v && !ag_value_type_object(v));
}


__AG_TEST_CASE("ag_value_lt() returns true for 123.456 < 123.4567")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_float(123.456);
        AG_AUTO(ag_value) *v2 = ag_value_new_float(124.4567);

        AG_TEST (ag_value_lt(v1, v2));
}


__AG_TEST_CASE("ag_value_lt() returns false for 123.456 < 123.456")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(123.456);

        AG_TEST (!ag_value_lt(v, v));
}


__AG_TEST_CASE("ag_value_lt() returns false for 123.4567 < 123.456")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_float(123.4567);
        AG_AUTO(ag_value) *v2 = ag_value_new_float(123.456);

        AG_TEST (!ag_value_lt(v1, v2));
}


__AG_TEST_CASE(
    "ag_value_eq() returns true for -123.4567 == -123.4567")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(-123.4567);

        AG_TEST (ag_value_eq(v, v));
}


__AG_TEST_CASE(
    "ag_value_eq() returns false for -123.456 == -123.4567")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_float(-123);
        AG_AUTO(ag_value) *v2 = ag_value_new_float(123.4567);

        AG_TEST (!ag_value_eq(v1, v2));
}


__AG_TEST_CASE("ag_value_gt() returns true for 123.4567 > 123.456")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_float(123.4567);
        AG_AUTO(ag_value) *v2 = ag_value_new_float(123.456);

        AG_TEST (ag_value_gt(v1, v2));
}


__AG_TEST_CASE(
    "ag_value_gt() returns false for 123.4567 > 123.4567")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(123.4567);

        AG_TEST (!ag_value_gt(v, v));
}


__AG_TEST_CASE("ag_value_gt() returns false for 123.456 > 123.4567")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_float(123.456);
        AG_AUTO(ag_value) *v2 = ag_value_new_float(123.4567);

        AG_TEST (!ag_value_gt(v1, v2));
}




static inline ag_value *
string_sample_ascii(void)
{
    AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");

    return ag_value_new_string(s);
}

static inline ag_value *
string_sample_unicode(void)
{
    AG_AUTO(ag_string) *s = ag_string_new("Привет, мир!");

    return ag_value_new_string(s);
}


__AG_TEST_CASE("ag_value_new_string() creates a new string value")
{
        AG_AUTO(ag_value) *v = string_sample_ascii();
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");

        AG_TEST (v && ag_string_eq(ag_value_string(v), s));
}


__AG_TEST_CASE("ag_value_copy() copies a string value")
{
        AG_AUTO(ag_value) *v = string_sample_unicode();
        AG_AUTO(ag_value) *cp = ag_value_copy(v);

        AG_TEST (ag_string_eq(ag_value_string(v), ag_value_string(cp)));
}


__AG_TEST_CASE(
    "ag_value_type_int() is false for a string value")
{
        AG_AUTO(ag_value) *v = string_sample_ascii();

        AG_TEST (!ag_value_type_int(v));
}


__AG_TEST_CASE(
    "ag_value_type_uint() is false for a string value")
{
        AG_AUTO(ag_value) *v = string_sample_unicode();

        AG_TEST (!ag_value_type_int(v));
}


__AG_TEST_CASE(
    "ag_value_type_float() is false for a string value")
{
        AG_AUTO(ag_value) *v = string_sample_ascii();

        AG_TEST (!ag_value_type_float(v));
}


__AG_TEST_CASE(
    "ag_value_type_string() is true for a string value")
{
        AG_AUTO(ag_value) *v = string_sample_unicode();

        AG_TEST (ag_value_type_string(v));
}


__AG_TEST_CASE(
    "ag_value_type_object() is false for a string value")
{
        AG_AUTO(ag_value) *v = string_sample_ascii();

        AG_TEST (!ag_value_type_object(v));
}





__AG_TEST_CASE("ag_value_new_object() creates a new object value")
{
        AG_AUTO(ag_object) *o = object_sample();
        AG_AUTO(ag_value) *v = object_sample_value();

        AG_TEST (v && ag_object_eq(ag_value_object(o), o));
}


__AG_TEST_CASE("ag_value_copy() copies an object value")
{
        AG_AUTO(ag_value) *v = object_sample_value();
        AG_AUTO(ag_value) *cp = ag_value_copy(v);

        AG_TEST (ag_object_eq(ag_value_object(v), ag_value_object(cp)));
}


__AG_TEST_CASE(
    "ag_value_type_int() is false for an object value")
{
        AG_AUTO(ag_value) *v = object_sample_value();

        AG_TEST (!ag_value_type_int(v));
}


__AG_TEST_CASE(
    "ag_value_type_uint() is false for an object value")
{
        AG_AUTO(ag_value) *v = object_sample_value();

        AG_TEST (!ag_value_type_uint(v));
}


__AG_TEST_CASE(
    "ag_value_type_float() is false for an object value")
{
        AG_AUTO(ag_value) *v = object_sample_value();

        AG_TEST (!ag_value_type_float(v));
}


__AG_TEST_CASE(
    "ag_value_type_string() is false for an object value")
{
        AG_AUTO(ag_value) *v = object_sample_value();

        AG_TEST (!ag_value_type_string(v));
}


__AG_TEST_CASE(
    "ag_value_type_object() is true for an object value")
{
        AG_AUTO(ag_value) *v = object_sample_value();

        AG_TEST (ag_value_type_object(v));
}




extern ag_test_suite *
test_suite_value(void)
{
        object_register();

        /*ag_test *test[] = {
                int_new,         int_copy,       int_type_int,
                int_type_uint,   int_type_float, int_type_string,
                int_type_object, int_lt_1,       int_lt_2,
                int_lt_3,        int_eq_1,       int_eq_2,
                int_gt_1,        int_gt_2,       int_gt_3,

                uint_new,         uint_copy,       uint_type_int,
                uint_type_uint,   uint_type_float, uint_type_string,
                uint_type_object, uint_lt_1,       uint_lt_2,
                uint_lt_3,        uint_eq_1,       uint_eq_2,
                uint_gt_1,        uint_gt_2,       uint_gt_3,

                float_new,         float_copy,      float_type_int,
                float_type_uint,   float_type_float,float_type_string,
                float_type_object, float_lt_1,      float_lt_2,
                float_lt_3,        float_eq_1,      float_eq_2,
                float_gt_1,        float_gt_2,      float_gt_3,

                string_new,         string_copy,       string_type_int,
                string_type_uint,   string_type_float, string_type_string,
                string_type_object,

                object_new,         object_copy,       object_type_int,
                object_type_uint,   object_type_float, object_type_string,
                object_type_object,
        };

        const char *desc[] = {
                int_new_desc,         int_copy_desc,       int_type_int_desc,
                int_type_uint_desc,   int_type_float_desc, int_type_string_desc,
                int_type_object_desc, int_lt_1_desc,       int_lt_2_desc,
                int_lt_3_desc,        int_eq_1_desc,       int_eq_2_desc,
                int_gt_1_desc,        int_gt_2_desc,       int_gt_3_desc,

                uint_new_desc,         uint_copy_desc,
                uint_type_int_desc,    uint_type_uint_desc,
                uint_type_float_desc,  uint_type_string_desc,
                uint_type_object_desc, uint_lt_1_desc,
                uint_lt_2_desc,        uint_lt_3_desc,
                uint_eq_1_desc,        uint_eq_2_desc,
                uint_gt_1_desc,        uint_gt_2_desc,
                uint_gt_3_desc,

                float_new_desc,         float_copy_desc,
                float_type_int_desc,    float_type_uint_desc,
                float_type_float_desc,  float_type_string_desc,
                float_type_object_desc, float_lt_1_desc,
                float_lt_2_desc,        float_lt_3_desc,
                float_eq_1_desc,        float_eq_2_desc,
                float_gt_1_desc,        float_gt_2_desc,
                float_gt_3_desc,

                string_new_desc,         string_copy_desc,
                string_type_int_desc,    string_type_uint_desc,
                string_type_float_desc,  string_type_string_desc,
                string_type_object_desc,

                object_new_desc,         object_copy_desc,
                object_type_int_desc,    object_type_uint_desc,
                object_type_float_desc,  object_type_string_desc,
                object_type_object_desc,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_value interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;*/

        return AG_TEST_SUITE_GENERATE("ag_value interface");
}

