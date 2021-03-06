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


#include "./alist.h"
#include "./field.h"
#include "./object.h"
#include "./test.h"
#include "./value.h"


/*
 * Define the __AG_TEST_SUITE_ID__ symbolic constant. This constant represents
 * the unique ID of the assocation list interface test suite, and is required by
 * the testing interface to metaprogrammatically generate the relevant testing
 * functions.
 */
#define __AG_TEST_SUITE_ID__ 7


/*
 * Define a few sample integer values. The AG_SAMPLE_VALUE_INT() macro generates
 * the following functions to return pointers to the sample integer values:
 *   - VALUE_1(): ag_int value 1
 *   - VALUE_2(): ag_int value 2
 *   - VALUE_3(): ag_int value 3
 *   - VALUE_4(): ag_int value 4
 */
AG_SAMPLE_VALUE_INT(VALUE_1, 1);
AG_SAMPLE_VALUE_INT(VALUE_2, 2);
AG_SAMPLE_VALUE_INT(VALUE_3, 3);
AG_SAMPLE_VALUE_INT(VALUE_4, 4);


/*
 * Define a few sample string values. The AG_SAMPLE_VALUE_STRING() amcro
 * generates the following functions to return pointers to the sample string
 * values:
 *   - VALUE_BAR()   : ag_string value "bar"
 *   - VALUE_FOO()   : ag_string value "foo"
 *   - VALUE_FOOBAR(): ag_string value "foobar"
 *   - VALUE_KEY()   : ag_string value "key"
 *   - VALUE_VAL()   : ag_string value "val"
 */
AG_SAMPLE_VALUE_STRING(VALUE_BAR, "bar");
AG_SAMPLE_VALUE_STRING(VALUE_FOO, "foo");
AG_SAMPLE_VALUE_STRING(VALUE_FOOBAR, "foobar");
AG_SAMPLE_VALUE_STRING(VALUE_KEY, "key");
AG_SAMPLE_VALUE_STRING(VALUE_VAL, "val");


AG_SAMPLE_FIELD(FIELD_KEY_VAL, VALUE_KEY(), VALUE_VAL());
AG_SAMPLE_FIELD(FIELD_1_FOO, VALUE_1(), VALUE_FOO());
AG_SAMPLE_FIELD(FIELD_2_BAR, VALUE_2(), VALUE_BAR());
AG_SAMPLE_FIELD(FIELD_2_FOO, VALUE_2(), VALUE_FOO());
AG_SAMPLE_FIELD(FIELD_3_FOOBAR, VALUE_3(), VALUE_FOOBAR());
AG_SAMPLE_FIELD(FIELD_3_BAR, VALUE_3(), VALUE_BAR());
AG_SAMPLE_FIELD(FIELD_4_FOOBAR, VALUE_4(), VALUE_FOOBAR());




static ag_alist *sample_empty(void);
static ag_alist *sample_single(void);
static ag_alist *sample_list(void);
static ag_alist *sample_list_2(void);


static bool     iterator(const ag_field *, void *, void *);
static bool     iterator_mutable(ag_field **, void *, void *);


AG_METATEST_OBJECT_COPY(ag_alist, sample_empty());
AG_METATEST_OBJECT_CLONE(ag_alist, sample_empty());
AG_METATEST_OBJECT_RELEASE(ag_alist, sample_empty());
AG_METATEST_OBJECT_EMPTY(ag_alist, sample_empty());
AG_METATEST_OBJECT_VALID_NOT(ag_alist, sample_empty());
AG_METATEST_OBJECT_TYPEID(ag_alist, sample_empty(), AG_TYPEID_ALIST);
AG_METATEST_OBJECT_UUID(ag_alist, sample_empty());
AG_METATEST_OBJECT_REFC(ag_alist, sample_empty());
AG_METATEST_OBJECT_LEN(ag_alist, sample_empty(), 0);
AG_METATEST_OBJECT_STR(ag_alist, sample_empty(), "()");
AG_METATEST_OBJECT_JSON_HAS(ag_alist, sample_empty(), "{\"object\":");

AG_METATEST_OBJECT_COPY(ag_alist, sample_single());
AG_METATEST_OBJECT_CLONE(ag_alist, sample_single());
AG_METATEST_OBJECT_RELEASE(ag_alist, sample_empty());
AG_METATEST_OBJECT_EMPTY_NOT(ag_alist, sample_single());
AG_METATEST_OBJECT_VALID(ag_alist, sample_single());
AG_METATEST_OBJECT_TYPEID(ag_alist, sample_single(), AG_TYPEID_ALIST);
AG_METATEST_OBJECT_UUID(ag_alist, sample_single());
AG_METATEST_OBJECT_REFC(ag_alist, sample_single());
AG_METATEST_OBJECT_LEN(ag_alist, sample_single(), 1); 
AG_METATEST_OBJECT_STR(ag_alist, sample_single(), "((key:val))");
AG_METATEST_OBJECT_JSON_HAS(ag_alist, sample_empty(), "{\"object\":");

AG_METATEST_OBJECT_COPY(ag_alist, sample_list());
AG_METATEST_OBJECT_CLONE(ag_alist, sample_list());
AG_METATEST_OBJECT_RELEASE(ag_alist, sample_empty());
AG_METATEST_OBJECT_EMPTY_NOT(ag_alist, sample_list());
AG_METATEST_OBJECT_VALID(ag_alist, sample_list());
AG_METATEST_OBJECT_TYPEID(ag_alist, sample_list(), AG_TYPEID_ALIST);
AG_METATEST_OBJECT_UUID(ag_alist, sample_list());
AG_METATEST_OBJECT_REFC(ag_alist, sample_list());
AG_METATEST_OBJECT_LEN(ag_alist, sample_list(), 3);
AG_METATEST_OBJECT_STR(ag_alist, sample_list(), "((1:foo) (2:bar) (3:foobar))");
AG_METATEST_OBJECT_JSON_HAS(ag_alist, sample_empty(), "{\"object\":");


AG_METATEST_OBJECT_CMP(ag_alist, sample_empty(), sample_single());
AG_METATEST_OBJECT_LT(ag_alist, sample_empty(), sample_single());
AG_METATEST_OBJECT_EQ(ag_alist, sample_empty(), sample_single());
AG_METATEST_OBJECT_GT(ag_alist, sample_empty(), sample_single());

AG_METATEST_OBJECT_CMP(ag_alist, sample_empty(), sample_list());
AG_METATEST_OBJECT_LT(ag_alist, sample_empty(), sample_list());
AG_METATEST_OBJECT_EQ(ag_alist, sample_empty(), sample_list());
AG_METATEST_OBJECT_GT(ag_alist, sample_empty(), sample_list());

AG_METATEST_OBJECT_CMP(ag_alist, sample_list(), sample_list_2());
AG_METATEST_OBJECT_LT(ag_alist, sample_list(), sample_list_2());
AG_METATEST_OBJECT_EQ(ag_alist, sample_list(), sample_list_2());
AG_METATEST_OBJECT_GT(ag_alist, sample_list(), sample_list_2());


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


AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_FOOBAR());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_BAR());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_FOO());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_KEY());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_VAL());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_1());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_2());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_3());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_empty(), VALUE_4());


AG_METATEST_ALIST_HAS_KEY(sample_single(), VALUE_KEY());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_single(), VALUE_VAL());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_single(), VALUE_FOO());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_single(), VALUE_BAR());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_single(), VALUE_FOOBAR());


AG_METATEST_ALIST_HAS_KEY(sample_list(), VALUE_1());
AG_METATEST_ALIST_HAS_KEY(sample_list(), VALUE_2());
AG_METATEST_ALIST_HAS_KEY(sample_list(), VALUE_3());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_list(), VALUE_4());


AG_METATEST_ALIST_HAS_KEY(sample_list_2(), VALUE_2());
AG_METATEST_ALIST_HAS_KEY(sample_list_2(), VALUE_3());
AG_METATEST_ALIST_HAS_KEY(sample_list_2(), VALUE_4());
AG_METATEST_ALIST_HAS_KEY_NOT(sample_list_2(), VALUE_1());


AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_FOOBAR());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_BAR());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_FOO());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_KEY());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_VAL());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_1());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_2());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_3());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_empty(), VALUE_4());


AG_METATEST_ALIST_HAS_VAL(sample_single(), VALUE_VAL());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_single(), VALUE_KEY());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_single(), VALUE_FOO());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_single(), VALUE_BAR());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_single(), VALUE_FOOBAR());


AG_METATEST_ALIST_HAS_VAL(sample_list(), VALUE_FOO());
AG_METATEST_ALIST_HAS_VAL(sample_list(), VALUE_FOOBAR());
AG_METATEST_ALIST_HAS_VAL(sample_list(), VALUE_BAR());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_list(), VALUE_KEY());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_list(), VALUE_VAL());


AG_METATEST_ALIST_HAS_VAL(sample_list_2(), VALUE_FOO());
AG_METATEST_ALIST_HAS_VAL(sample_list_2(), VALUE_FOOBAR());
AG_METATEST_ALIST_HAS_VAL(sample_list_2(), VALUE_BAR());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_list_2(), VALUE_KEY());
AG_METATEST_ALIST_HAS_VAL_NOT(sample_list_2(), VALUE_VAL());


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


AG_METATEST_ALIST_VAL(sample_single(), VALUE_KEY(), VALUE_VAL());


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


AG_METATEST_ALIST_VAL(sample_list(), VALUE_1(), VALUE_FOO());
AG_METATEST_ALIST_VAL(sample_list(), VALUE_2(), VALUE_BAR());
AG_METATEST_ALIST_VAL(sample_list(), VALUE_3(), VALUE_FOOBAR());


AG_METATEST_ALIST_VAL(sample_list_2(), VALUE_2(), VALUE_FOO());
AG_METATEST_ALIST_VAL(sample_list_2(), VALUE_3(), VALUE_BAR());
AG_METATEST_ALIST_VAL(sample_list_2(), VALUE_4(), VALUE_FOOBAR());


AG_METATEST_ALIST_VAL_SET(sample_single(), VALUE_KEY(), VALUE_FOO());
AG_METATEST_ALIST_VAL_SET(sample_single(), VALUE_KEY(), VALUE_BAR());
AG_METATEST_ALIST_VAL_SET(sample_single(), VALUE_KEY(), VALUE_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_single(), VALUE_KEY(), VALUE_KEY());
AG_METATEST_ALIST_VAL_SET(sample_single(), VALUE_KEY(), VALUE_VAL());


AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_1(), VALUE_KEY());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_1(), VALUE_VAL());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_1(), VALUE_FOO());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_1(), VALUE_BAR());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_1(), VALUE_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_1(), VALUE_1());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_1(), VALUE_2());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_1(), VALUE_3());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_1(), VALUE_4());


AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_2(), VALUE_KEY());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_2(), VALUE_VAL());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_2(), VALUE_FOO());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_2(), VALUE_BAR());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_2(), VALUE_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_2(), VALUE_1());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_2(), VALUE_2());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_2(), VALUE_3());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_2(), VALUE_4());


AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_3(), VALUE_KEY());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_3(), VALUE_VAL());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_3(), VALUE_FOO());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_3(), VALUE_BAR());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_3(), VALUE_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_3(), VALUE_1());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_3(), VALUE_2());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_3(), VALUE_3());
AG_METATEST_ALIST_VAL_SET(sample_list(), VALUE_3(), VALUE_4());


AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_2(), VALUE_KEY());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_2(), VALUE_VAL());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_2(), VALUE_FOO());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_2(), VALUE_BAR());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_2(), VALUE_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_2(), VALUE_1());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_2(), VALUE_2());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_2(), VALUE_3());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_2(), VALUE_4());


AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_3(), VALUE_KEY());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_3(), VALUE_VAL());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_3(), VALUE_FOO());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_3(), VALUE_BAR());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_3(), VALUE_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_3(), VALUE_1());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_3(), VALUE_2());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_3(), VALUE_3());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_3(), VALUE_4());


AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_4(), VALUE_KEY());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_4(), VALUE_VAL());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_4(), VALUE_FOO());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_4(), VALUE_BAR());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_4(), VALUE_FOOBAR());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_4(), VALUE_1());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_4(), VALUE_2());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_4(), VALUE_3());
AG_METATEST_ALIST_VAL_SET(sample_list_2(), VALUE_4(), VALUE_4());


AG_METATEST_ALIST_PARSE("", "=", "&", "()");
AG_METATEST_ALIST_PARSE("foo=bar", "=", "&", "((foo:bar))");
AG_METATEST_ALIST_PARSE("foo=bar&bar=foo", "=", "&", "((foo:bar) (bar:foo))");
AG_METATEST_ALIST_PARSE("foo=bar&bar=foo&key=val", "=", "&",
    "((foo:bar) (bar:foo) (key:val))");


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


/*
 * Define the test_suite_alist() function. We generate the test cases from the
 * above metatest definitions through a call to AG_TEST_SUITE_GENERATE().
 */
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

