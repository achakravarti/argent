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


#include "./field.h"
#include "./test.h"
#include "./object.h"
#include "./value.h"


#define __AG_TEST_SUITE_ID__ 5


AG_SAMPLE_VALUE_INT(NEGATIVE, -1);
AG_SAMPLE_VALUE_INT(POSITIVE, 2);
AG_SAMPLE_VALUE_STRING(SMALL, "small");
AG_SAMPLE_VALUE_STRING(LARGE, "large");


AG_SAMPLE_FIELD(FIELD_SMALL, NEGATIVE(), SMALL());
AG_SAMPLE_FIELD(FIELD_LARGE, POSITIVE(), LARGE());


/*
 * Define the test cases for ag_field_new().
 */


AG_TEST_CASE("ag_field_new() can create a new sample field")
{
        AG_AUTO(ag_field) *f = FIELD_SMALL();
        AG_AUTO(ag_value) *k = ag_field_key(f);
        AG_AUTO(ag_value) *v = ag_field_val(f);
        const ag_string *s = ag_value_string(v);

        AG_TEST (f && ag_value_int(k) == -1 && ag_string_eq(s, "small"));
}


AG_TEST_CASE("ag_field_parse() can parse a string representing a field")
{
        AG_AUTO(ag_field) *f = ag_field_parse("foo=bar", "=");
        AG_AUTO(ag_value) *k = ag_field_key(f);
        AG_AUTO(ag_value) *v = ag_field_val(f);

        const ag_string *ks = ag_value_string(k);
        const ag_string *vs = ag_value_string(v);

        AG_TEST (f && ag_string_eq(ks, "foo") && ag_string_eq(vs, "bar"));
}


AG_METATEST_OBJECT_MANAGER(ag_field, FIELD_SMALL());
AG_METATEST_OBJECT_MANAGER(ag_field, FIELD_LARGE());


/*
 * Define the the test cases for ag_field_lt(). Note that since ag_field_lt() is
 * a specialised case of ag_field_cmp(), it helps to test test ag_field_cmp()
 * too.
 */


AG_METATEST_OBJECT_CMP(ag_field, FIELD_SMALL(), FIELD_LARGE());
AG_METATEST_OBJECT_LT(ag_field, FIELD_SMALL(), FIELD_LARGE());
AG_METATEST_OBJECT_EQ(ag_field, FIELD_SMALL(), FIELD_LARGE());
AG_METATEST_OBJECT_GT(ag_field, FIELD_SMALL(), FIELD_LARGE());


/*
 * Define the test case for ag_field_empty().
 */


AG_METATEST_OBJECT_EMPTY_NOT(ag_field, FIELD_SMALL());
AG_METATEST_OBJECT_EMPTY_NOT(ag_field, FIELD_LARGE());

/*
 * Define the test case for ag_field_typeid().
 */


AG_METATEST_OBJECT_TYPEID(ag_field, FIELD_SMALL(), AG_TYPEID_FIELD);
AG_METATEST_OBJECT_TYPEID(ag_field, FIELD_LARGE(), AG_TYPEID_FIELD);


/*
 * Define the test case for ag_field_uuid()
 */


AG_METATEST_OBJECT_UUID(ag_field, FIELD_SMALL());
AG_METATEST_OBJECT_UUID(ag_field, FIELD_LARGE());



/*
 * Define the test case for ag_field_valid().
 */


AG_METATEST_OBJECT_VALID(ag_field, FIELD_SMALL());
AG_METATEST_OBJECT_VALID(ag_field, FIELD_LARGE());


/*
 * Define the test case for ag_field_sz(),
 */


AG_METATEST_OBJECT_SZ(ag_field, FIELD_SMALL(), 6 + sizeof(int64_t));
AG_METATEST_OBJECT_SZ(ag_field, FIELD_LARGE(), 6 + sizeof(int64_t));


/*
 * Define the test cases for ag_field_refc().
 */


AG_METATEST_OBJECT_REFC(ag_field, FIELD_SMALL());
AG_METATEST_OBJECT_REFC(ag_field, FIELD_LARGE());


/*
 * Define the test case for ag_field_len()
 */


AG_METATEST_OBJECT_LEN(ag_field, FIELD_SMALL(), 5);
AG_METATEST_OBJECT_LEN(ag_field, FIELD_LARGE(), 5);


/*
 * Define the test case for ag_field_hash()
 */


AG_METATEST_OBJECT_HASH(ag_field, FIELD_SMALL(), ag_hash_new(-1));
AG_METATEST_OBJECT_HASH(ag_field, FIELD_LARGE(), ag_hash_new(2));


/*
 * Define the test case for ag_field_str().
 */


AG_METATEST_OBJECT_STR(ag_field, FIELD_SMALL(), "-1:small");
AG_METATEST_OBJECT_STR(ag_field, FIELD_LARGE(), "2:large");


extern ag_test_suite *
test_suite_field(void)
{
        return AG_TEST_SUITE_GENERATE("ag_field interface");
}

