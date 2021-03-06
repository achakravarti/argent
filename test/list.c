/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Argent---infrastructure for building web services
 * Copyright (C) 2020 Abhishek Chakravarti
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact Abhishek Chakravarti at <abhishek@taranjali.org>.
 ******************************************************************************/


#include "./test.h"
#include "./object.h"


/*
 * Define the ID of the test suite for the list interface. We need this ID for
 * the testing macros to correctly generate the boilerplate testing code.
 */


#define __AG_TEST_SUITE_ID__ 6



/*
 * Declare the prototypes for generating sample integer lists. Both functions
 * generate sample integer lists, but with differing lengths.
 */


static ag_list *sample_int(void);
static ag_list *sample_int_2(void);


/*
 * Declare the protoptypes for the iterator functions that are used to test out
 * the map functions of lists. iterator() helps test out ag_list_map(), whereas
 * iterator_mutable helps test ag_list_map_mutable().
 */


static bool     iterator(const ag_value *, void *, void *);
static bool     iterator_mutable(ag_value **, void *, void *);


/*
 * Define the test cases for ag_list_new().
 */


AG_TEST_CASE("ag_list_new() can create a new sample list")
{
        AG_AUTO(ag_list) *l = sample_int();
        AG_TEST (l && ag_list_len(l) == 3);
}


AG_TEST_CASE("ag_list_new() can create a new empty list")
{
        AG_AUTO(ag_list) *l = ag_list_new();
        AG_TEST (l && ag_list_empty(l));
}


AG_METATEST_OBJECT_COPY(ag_list, ag_list_new());
AG_METATEST_OBJECT_CLONE(ag_list, ag_list_new());
AG_METATEST_OBJECT_RELEASE(ag_list, ag_list_new());
AG_METATEST_OBJECT_EMPTY(ag_list, ag_list_new());
AG_METATEST_OBJECT_VALID_NOT(ag_list, ag_list_new());
AG_METATEST_OBJECT_TYPEID(ag_list, ag_list_new(), AG_TYPEID_LIST);
AG_METATEST_OBJECT_UUID(ag_list, ag_list_new());
AG_METATEST_OBJECT_REFC(ag_list, ag_list_new());
AG_METATEST_OBJECT_LEN(ag_list, ag_list_new(), 0);
AG_METATEST_OBJECT_SZ(ag_list, ag_list_new(), 0);
AG_METATEST_OBJECT_HASH(ag_list, ag_list_new(), 0);
AG_METATEST_OBJECT_STR_HAS(ag_list, ag_list_new(), "list");

AG_METATEST_OBJECT_COPY(ag_list, sample_int());
AG_METATEST_OBJECT_CLONE(ag_list, sample_int());
AG_METATEST_OBJECT_RELEASE(ag_list, sample_int());
AG_METATEST_OBJECT_EMPTY_NOT(ag_list, sample_int())
AG_METATEST_OBJECT_VALID(ag_list, sample_int())
AG_METATEST_OBJECT_TYPEID(ag_list, sample_int(), AG_TYPEID_LIST);
AG_METATEST_OBJECT_UUID(ag_list, sample_int());
AG_METATEST_OBJECT_REFC(ag_list, sample_int());
AG_METATEST_OBJECT_LEN(ag_list, sample_int(), 3);
AG_METATEST_OBJECT_SZ(ag_list, sample_int(), 3 * sizeof(ag_int));
AG_METATEST_OBJECT_HASH(ag_list, sample_int(), ag_hash_new(-123) +
    ag_hash_new(0) + ag_hash_new(456));
AG_METATEST_OBJECT_STR_HAS(ag_list, sample_int(), "list");

AG_METATEST_OBJECT_COPY(ag_list, sample_int_2());
AG_METATEST_OBJECT_CLONE(ag_list, sample_int_2());
AG_METATEST_OBJECT_RELEASE(ag_list, sample_int_2());
AG_METATEST_OBJECT_EMPTY_NOT(ag_list, sample_int_2())
AG_METATEST_OBJECT_VALID(ag_list, sample_int_2())
AG_METATEST_OBJECT_TYPEID(ag_list, sample_int_2(), AG_TYPEID_LIST);
AG_METATEST_OBJECT_UUID(ag_list, sample_int_2());
AG_METATEST_OBJECT_REFC(ag_list, sample_int_2());
AG_METATEST_OBJECT_LEN(ag_list, sample_int_2(), 7);
AG_METATEST_OBJECT_SZ(ag_list, sample_int_2(), 7 * sizeof(ag_int));
AG_METATEST_OBJECT_HASH(ag_list, sample_int_2(), ag_hash_new(-123) + 
    ag_hash_new(0) + ag_hash_new(456) + ag_hash_new(-666) + ag_hash_new(0) +
    ag_hash_new(555) + ag_hash_new(734));
AG_METATEST_OBJECT_STR_HAS(ag_list, sample_int_2(), "list");


/*
 * Define the the test cases for ag_list_lt(). Note that since ag_list_lt() is a
 * specialised case of ag_list_cmp(), it helps to test test ag_list_cmp() too.
 */


AG_METATEST_OBJECT_CMP(ag_list, sample_int(), sample_int_2());
AG_METATEST_OBJECT_LT(ag_list, sample_int(), sample_int_2());
AG_METATEST_OBJECT_EQ(ag_list, sample_int(), sample_int_2());
AG_METATEST_OBJECT_GT(ag_list, sample_int(), sample_int_2());


/*
 * Define the test case for ag_list_get().
 */


AG_TEST_CASE("ag_list_get() gets the currently iterated value")
{
        AG_AUTO(ag_list) *l = sample_int_2();
        ag_list_start(&l);
        ag_list_next(&l);
        ag_list_next(&l);
        AG_AUTO(ag_value) *v = ag_list_get(l);

        AG_TEST (ag_value_int(v) == 456);
}


/*
 * Define the test case for ag_list_get_at().
 */


AG_TEST_CASE("ag_list_get_at() gets the value at a given index")
{
        AG_AUTO(ag_list) *l = sample_int_2();
        AG_AUTO(ag_value) *v = ag_list_get_at(l, 7);

        AG_TEST (ag_value_int(v) == 734);
}


/*
 * Define the test cases for ag_list_map().
 */


AG_TEST_CASE("ag_list_map() has no effect on an empty list")
{
        AG_AUTO(ag_list) *l = ag_list_new();
        ag_int sum = 0;
        ag_list_map(l, iterator, NULL, &sum);

        AG_TEST (!sum);
}


AG_TEST_CASE("ag_list_map() iterates through a non-empty list")
{
        AG_AUTO(ag_list) *l = sample_int_2();
        ag_int sum = 0;
        ag_list_map(l, iterator, NULL, &sum);

        AG_TEST (sum == 956);
}


/*
 * Define the test case for ag_list_set().
 */


AG_TEST_CASE("ag_list_set() sets the currently iterated value")
{
        AG_AUTO(ag_list) *l = sample_int_2();
        AG_AUTO(ag_value) *v = ag_value_new_int(1234);

        ag_list_start(&l);
        ag_list_next(&l);
        ag_list_next(&l);

        ag_list_set(&l, v);
        AG_AUTO(ag_value) *v2 = ag_list_get(l);

        AG_TEST (ag_value_int(v2) == 1234);
}


/*
 * Define the test case for ag_list_set_at().
 */


AG_TEST_CASE("ag_list_set_at() sets the value at a given index")
{
        AG_AUTO(ag_list) *l = sample_int_2();
        AG_AUTO(ag_value) *v = ag_value_new_int(1234);

        ag_list_set_at(&l, v, 3);
        AG_AUTO(ag_value) *v2 = ag_list_get_at(l, 3);

        AG_TEST (ag_value_int(v2) == 1234);
}


/*
 * Define the test cases for ag_list_map_mutable().
 */


AG_TEST_CASE("ag_list_map_mutable() has no effect on an empty list")
{
        AG_AUTO(ag_list) *l = ag_list_new();
        ag_int sum = 0;

        ag_list_map_mutable(&l, iterator_mutable, NULL, NULL);
        ag_list_map(l, iterator, NULL, &sum);

        AG_TEST (!sum);
}


AG_TEST_CASE("ag_list_map_mutable() iterates through a non-empty list")
{
        AG_AUTO(ag_list) *l = sample_int();
        ag_int sum = 0;

        ag_list_map_mutable(&l, iterator_mutable, NULL, NULL);
        ag_list_map(l, iterator, NULL, &sum);

        AG_TEST (sum == 15);
}


/*
 * Define the test_suite_list() testing interface function. This function is
 * responsible for creating a test suite from the test cases defined above.
 */


extern ag_test_suite *test_suite_list(void)
{
        return AG_TEST_SUITE_GENERATE("ag_list interface");
}


/*
 * Define the sample_int() helper function. This function is used by the test
 * cases defined above to generate a new sample integer list with 3 values.
 */


static ag_list *sample_int(void)
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(-123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(0);
        AG_AUTO(ag_value) *v3 = ag_value_new_int(456);

        AG_AUTO(ag_list) *l = ag_list_new();
        ag_list_push(&l, v1);
        ag_list_push(&l, v2);
        ag_list_push(&l, v3);

        return ag_list_copy(l);
}


/*
 * Define the sample_int_2() helper function. This function is similar to the
 * sample_int() helper function, generating a sample integer list with 7 values.
 */


static ag_list *sample_int_2(void)
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(-123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(0);
        AG_AUTO(ag_value) *v3 = ag_value_new_int(456);
        AG_AUTO(ag_value) *v4 = ag_value_new_int(-666);
        AG_AUTO(ag_value) *v5 = ag_value_new_int(0);
        AG_AUTO(ag_value) *v6 = ag_value_new_int(555);
        AG_AUTO(ag_value) *v7 = ag_value_new_int(734);

        AG_AUTO(ag_list) *l = ag_list_new();
        ag_list_push(&l, v1);
        ag_list_push(&l, v2);
        ag_list_push(&l, v3);
        ag_list_push(&l, v4);
        ag_list_push(&l, v5);
        ag_list_push(&l, v6);
        ag_list_push(&l, v7);

        return ag_list_copy(l);
}


/*
 * Define the iterator() helper function. This function is the callback function
 * used to test out ag_list_map(). This function leads to summation of the
 * integer values contained within a list.
 */


static bool iterator(const ag_value *val, void *in, void *out)
{
        (void)in;

        ag_int *s = out;
        ag_int i = ag_value_int(val);
        *s += i;

        return true;
}


/*
 * Define the iterator_mutable() helper function. This function is the iterator
 * callback function used to test out ag_list_map_mutable(). This function
 * modifies the value of the node it is applied on it, setting it to 5.
 */


static bool iterator_mutable(ag_value **val, void *in, void *out)
{
        (void)in;
        (void)out;

        AG_AUTO(ag_value) *v = ag_value_new_int(5);
        ag_value_release(val);
        *val = ag_value_copy(v);

        return true;
}

