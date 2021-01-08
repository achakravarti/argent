#include "./test.h"

#define __AG_TEST_SUITE_ID__ 4


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


AG_TEST_CASE("ag_value_new_int() creates a new int value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-123456);

        AG_TEST (v && ag_value_int(v) == -123456);
}


AG_TEST_CASE("ag_value_copy() copies an int value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-1029394);
        AG_AUTO(ag_value) *cp = ag_value_copy(v);

        AG_TEST (ag_value_int(v) == ag_value_int(cp));
}


AG_TEST_CASE("ag_value_type_int() is true for an int value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(11111);

        AG_TEST (v && ag_value_type_int(v));
}


AG_TEST_CASE("ag_value_type_uint() is false for a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-1111);

        AG_TEST (v && !ag_value_type_uint(v));
}


AG_TEST_CASE(
    "ag_value_type_float() is false for a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(1111);

        AG_TEST (v && !ag_value_type_float(v));
}


AG_TEST_CASE(
    "ag_value_type_string() is false for a string value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-987654321);

        AG_TEST (v && !ag_value_type_string(v));
}


AG_TEST_CASE(
    "ag_value_type_object() is false for an object value")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(987654321);

        AG_TEST (v && !ag_value_type_object(v));
}


AG_TEST_CASE("ag_value_lt() returns true for -123 < 123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(-123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(123);

        AG_TEST (ag_value_lt(v1, v2));
}


AG_TEST_CASE("ag_value_lt() returns false for -123 < -123")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-123);

        AG_TEST (!ag_value_lt(v, v));
}


AG_TEST_CASE("ag_value_lt() returns false for 123 < -123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(-123);

        AG_TEST (!ag_value_lt(v1, v2));
}


AG_TEST_CASE("ag_value_eq() returns true for -123 == -123")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-123);

        AG_TEST (ag_value_eq(v, v));
}


AG_TEST_CASE("ag_value_eq() returns false for -123 == 123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(-123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(123);

        AG_TEST (!ag_value_eq(v1, v2));
}


AG_TEST_CASE("ag_value_gt() returns true for 123 > -123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(-123);

        AG_TEST (ag_value_gt(v1, v2));
}


AG_TEST_CASE("ag_value_gt() returns false for -123 > -123")
{
        AG_AUTO(ag_value) *v = ag_value_new_int(-123);

        AG_TEST (!ag_value_gt(v, v));
}


AG_TEST_CASE("ag_value_gt() returns false for -123 > 123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(-123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(123);

        AG_TEST (!ag_value_gt(v1, v2));
}


AG_TEST_CASE("ag_value_new_uint() creates a new uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(123456);

        AG_TEST (v && ag_value_uint(v) == 123456);
}


AG_TEST_CASE("ag_value_copy() copies a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(1029394);
        AG_AUTO(ag_value) *cp = ag_value_copy(v);

        AG_TEST (ag_value_uint(v) == ag_value_uint(cp));
}


AG_TEST_CASE("ag_value_type_int() is false for a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(11111);

        AG_TEST (v && !ag_value_type_int(v));
}


AG_TEST_CASE("ag_value_type_uint() is true for a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(701115);

        AG_TEST (v && ag_value_type_uint(v));
}


AG_TEST_CASE(
    "ag_value_type_float() is false for a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(97324);

        AG_TEST (v && !ag_value_type_float(v));
}


AG_TEST_CASE(
    "ag_value_type_string() is false for a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(987654321);

        AG_TEST (v && !ag_value_type_string(v));
}


AG_TEST_CASE(
    "ag_value_type_object() is false for a uint value")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(108);

        AG_TEST (v && !ag_value_type_object(v));
}


AG_TEST_CASE("ag_value_lt() returns true for 123 < 124")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_uint(123);
        AG_AUTO(ag_value) *v2 = ag_value_new_uint(124);

        AG_TEST (ag_value_lt(v1, v2));
}


AG_TEST_CASE("ag_value_lt() returns false for 123 < 123")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(123);

        AG_TEST (!ag_value_lt(v, v));
}


AG_TEST_CASE("ag_value_lt() returns false for 124 < 123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_uint(124);
        AG_AUTO(ag_value) *v2 = ag_value_new_uint(123);

        AG_TEST (!ag_value_lt(v1, v2));
}


AG_TEST_CASE("ag_value_eq() returns true for 123 == 123")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(123);

        AG_TEST (ag_value_eq(v, v));
}


AG_TEST_CASE("ag_value_eq() returns false for 123 == 124") {
        AG_AUTO(ag_value) *v1 = ag_value_new_uint(123);
        AG_AUTO(ag_value) *v2 = ag_value_new_uint(124);

        AG_TEST (!ag_value_eq(v1, v2));
}


AG_TEST_CASE("ag_value_gt() returns true for 124 > 123")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_uint(124);
        AG_AUTO(ag_value) *v2 = ag_value_new_uint(123);

        AG_TEST (ag_value_gt(v1, v2));
}


AG_TEST_CASE("ag_value_gt() returns false for 123 > 123")
{
        AG_AUTO(ag_value) *v = ag_value_new_uint(123);

        AG_TEST (!ag_value_gt(v, v));
}


AG_TEST_CASE("ag_value_gt() returns false for 123 > 124")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_uint(123);
        AG_AUTO(ag_value) *v2 = ag_value_new_uint(124);

        AG_TEST (!ag_value_gt(v1, v2));
}


AG_TEST_CASE("ag_value_new_float() creates a new float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(-123.456);

        AG_TEST (v && ag_value_float(v) == -123.456);
}


AG_TEST_CASE("ag_value_copy() copies a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(123.456);
        AG_AUTO(ag_value) *cp = ag_value_copy(v);

        AG_TEST (ag_value_float(v) == ag_value_float(cp));
}


AG_TEST_CASE("ag_value_type_int() is false for a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(-123456.789);

        AG_TEST (v && !ag_value_type_int(v));
}


AG_TEST_CASE(
    "ag_value_type_uint() is false for a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(123456.789);

        AG_TEST (v && !ag_value_type_uint(v));
}


AG_TEST_CASE(
    "ag_value_type_float() is true for a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(-789.123456);

        AG_TEST (v && ag_value_type_float(v));
}


AG_TEST_CASE(
    "ag_value_type_string() is false for a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(789.123456);

        AG_TEST (v && !ag_value_type_string(v));
}


AG_TEST_CASE(
    "ag_value_type_object is false for a float value")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(-9876.5433);

        AG_TEST (v && !ag_value_type_object(v));
}


AG_TEST_CASE("ag_value_lt() returns true for 123.456 < 123.4567")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_float(123.456);
        AG_AUTO(ag_value) *v2 = ag_value_new_float(124.4567);

        AG_TEST (ag_value_lt(v1, v2));
}


AG_TEST_CASE("ag_value_lt() returns false for 123.456 < 123.456")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(123.456);

        AG_TEST (!ag_value_lt(v, v));
}


AG_TEST_CASE("ag_value_lt() returns false for 123.4567 < 123.456")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_float(123.4567);
        AG_AUTO(ag_value) *v2 = ag_value_new_float(123.456);

        AG_TEST (!ag_value_lt(v1, v2));
}


AG_TEST_CASE(
    "ag_value_eq() returns true for -123.4567 == -123.4567")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(-123.4567);

        AG_TEST (ag_value_eq(v, v));
}


AG_TEST_CASE(
    "ag_value_eq() returns false for -123.456 == -123.4567")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_float(-123);
        AG_AUTO(ag_value) *v2 = ag_value_new_float(123.4567);

        AG_TEST (!ag_value_eq(v1, v2));
}


AG_TEST_CASE("ag_value_gt() returns true for 123.4567 > 123.456")
{
        AG_AUTO(ag_value) *v1 = ag_value_new_float(123.4567);
        AG_AUTO(ag_value) *v2 = ag_value_new_float(123.456);

        AG_TEST (ag_value_gt(v1, v2));
}


AG_TEST_CASE(
    "ag_value_gt() returns false for 123.4567 > 123.4567")
{
        AG_AUTO(ag_value) *v = ag_value_new_float(123.4567);

        AG_TEST (!ag_value_gt(v, v));
}


AG_TEST_CASE("ag_value_gt() returns false for 123.456 > 123.4567")
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


AG_TEST_CASE("ag_value_new_string() creates a new string value")
{
        AG_AUTO(ag_value) *v = string_sample_ascii();
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");

        AG_TEST (v && ag_string_eq(ag_value_string(v), s));
}


AG_TEST_CASE("ag_value_copy() copies a string value")
{
        AG_AUTO(ag_value) *v = string_sample_unicode();
        AG_AUTO(ag_value) *cp = ag_value_copy(v);

        AG_TEST (ag_string_eq(ag_value_string(v), ag_value_string(cp)));
}


AG_TEST_CASE(
    "ag_value_type_int() is false for a string value")
{
        AG_AUTO(ag_value) *v = string_sample_ascii();

        AG_TEST (!ag_value_type_int(v));
}


AG_TEST_CASE(
    "ag_value_type_uint() is false for a string value")
{
        AG_AUTO(ag_value) *v = string_sample_unicode();

        AG_TEST (!ag_value_type_int(v));
}


AG_TEST_CASE(
    "ag_value_type_float() is false for a string value")
{
        AG_AUTO(ag_value) *v = string_sample_ascii();

        AG_TEST (!ag_value_type_float(v));
}


AG_TEST_CASE(
    "ag_value_type_string() is true for a string value")
{
        AG_AUTO(ag_value) *v = string_sample_unicode();

        AG_TEST (ag_value_type_string(v));
}


AG_TEST_CASE(
    "ag_value_type_object() is false for a string value")
{
        AG_AUTO(ag_value) *v = string_sample_ascii();

        AG_TEST (!ag_value_type_object(v));
}


AG_TEST_CASE("ag_value_new_object() creates a new object value")
{
        AG_AUTO(ag_object) *o = object_sample();
        AG_AUTO(ag_value) *v = object_sample_value();

        AG_TEST (v && ag_object_eq(ag_value_object(o), o));
}


AG_TEST_CASE("ag_value_copy() copies an object value")
{
        AG_AUTO(ag_value) *v = object_sample_value();
        AG_AUTO(ag_value) *cp = ag_value_copy(v);

        AG_TEST (ag_object_eq(ag_value_object(v), ag_value_object(cp)));
}


AG_TEST_CASE(
    "ag_value_type_int() is false for an object value")
{
        AG_AUTO(ag_value) *v = object_sample_value();

        AG_TEST (!ag_value_type_int(v));
}


AG_TEST_CASE(
    "ag_value_type_uint() is false for an object value")
{
        AG_AUTO(ag_value) *v = object_sample_value();

        AG_TEST (!ag_value_type_uint(v));
}


AG_TEST_CASE(
    "ag_value_type_float() is false for an object value")
{
        AG_AUTO(ag_value) *v = object_sample_value();

        AG_TEST (!ag_value_type_float(v));
}


AG_TEST_CASE(
    "ag_value_type_string() is false for an object value")
{
        AG_AUTO(ag_value) *v = object_sample_value();

        AG_TEST (!ag_value_type_string(v));
}


AG_TEST_CASE(
    "ag_value_type_object() is true for an object value")
{
        AG_AUTO(ag_value) *v = object_sample_value();

        AG_TEST (ag_value_type_object(v));
}


extern ag_test_suite *
test_suite_value(void)
{
        object_register();

        return AG_TEST_SUITE_GENERATE("ag_value interface");
}

