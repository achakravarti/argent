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

#define __AG_TEST_SUITE_ID__ 2


/*
 * Define the test cases for ag_string_new().
 */


AG_TEST_CASE("ag_string_new() can create an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new("");
        AG_TEST (s && !*s);
}


AG_TEST_CASE("ag_string_new() can create an ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_TEST (s && *s && ag_string_eq(s, "Hello, world!"));
}


AG_TEST_CASE("ag_string_new() can create a Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_TEST (s && *s && ag_string_eq(s, "नमस्ते दुनिया!"));
}


/*
 * Define the test case for ag_string_empty().
 */


AG_TEST_CASE("ag_string_new_empty() creates an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_TEST (s && !*s);
}


/*
 * Define the test cases for ag_string_new_fmt().
 */


AG_TEST_CASE("ag_string_new_fmt() can create an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_fmt("%s", "");
        AG_TEST (s && !*s);
}


AG_TEST_CASE("ag_string_new_fmt() can create a formatted ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new_fmt("%d. %s", 111,
            "Hello, world!");

        AG_TEST (s && *s && ag_string_eq(s, "111. Hello, world!"));
}


AG_TEST_CASE("ag_string_new_fmt() can create a formatted Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new_fmt("%d. %s", -12, "नमस्ते दुनिया!");
        AG_TEST (s && *s && ag_string_eq(s, "-12. नमस्ते दुनिया!"));
}


/*
 * Define the test cases for ag_string_copy().
 */


AG_TEST_CASE("ag_string_copy() can create a copy of an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (s2 && !*s2);
}


AG_TEST_CASE("ag_string_copy() can create a copy of an ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (s2 && *s2 && ag_string_eq(s, s2));
}


AG_TEST_CASE("ag_string_copy() can create a copy of a Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (s2 && *s2 && ag_string_eq(s, s2));
}


AG_TEST_CASE("ag_string_copy() increases the reference count by 1")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (ag_string_refc(s) == 2);
}


/*
 * Define the test cases for ag_astring_copy().
 */


AG_TEST_CASE("ag_string_clone() can create a copy of an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_clone(s);

        AG_TEST (s2 && !*s2);
}


AG_TEST_CASE("ag_string_clone() can create a copy of an ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_clone(s);

        AG_TEST (s2 && *s2 && ag_string_eq(s, s2));
}


AG_TEST_CASE(
    "ag_string_clone() can create a copy of a Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_clone(s);

        AG_TEST (s2 && *s2 && ag_string_eq(s, s2));
}


AG_TEST_CASE("ag_string_clone() has no effect on the reference count")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_clone(s);

        AG_TEST (ag_string_refc(s) == 1);
}


/*
 * Define the test cases for ag_string_release().
 */


AG_TEST_CASE("ag_string_release() performs a no-op if passed NULL")
{
        ag_string_release(NULL);
        AG_TEST (true);
}


AG_TEST_CASE("ag_string_release() performs a no-op if passed a handle to NULL")
{
        ag_string *s = NULL;
        ag_string_release(&s);

        AG_TEST (true);
}


AG_TEST_CASE("ag_string_release() disposes a single instance of a string")
{
        ag_string *s = ag_string_new("Hello, world!");
        ag_string_release(&s);

        AG_TEST (!s);
}


AG_TEST_CASE("ag_string_release() reduces the reference count by 1")
{
        ag_string *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);
        AG_AUTO(ag_string) *s3 = ag_string_copy(s2);
        ag_string_release(&s);

        AG_TEST (!s && ag_string_refc(s2) == 2);
}


/*
 * Define the test cases for ag_string_cmp().
 */


AG_TEST_CASE("ag_string_cmp() returns AG_CMP_EQ when comparing two empty"
    " strings")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new_empty();

        AG_TEST (ag_string_cmp(s, s2) == AG_CMP_EQ);
}


AG_TEST_CASE("ag_string_cmp() return AG_CMP_LT when comparing an empty string"
    " with another string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_cmp(s, s2) == AG_CMP_LT);
}


AG_TEST_CASE("ag_string_cmp() return AG_CMP_EQ when comparing two equal ASCII"
    " strings")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (ag_string_cmp(s, s2) == AG_CMP_EQ);
}


AG_TEST_CASE("ag_string_cmp() returns AG_CMP_LT when comparing an ASCII string"
    " that is lexicographically less than another")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_cmp(s, s2) == AG_CMP_LT);
}


AG_TEST_CASE("ag_string_cmp() returns AG_CMP_GT when comparing an ASCII string"
    " that is lexicographically greater than another")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_cmp(s2, s) == AG_CMP_GT);
}


AG_TEST_CASE("ag_string_cmp() returns AG_CMP_GT when comparing an ASCII string"
    " to an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_cmp(s2, s) == AG_CMP_GT);
}


AG_TEST_CASE("ag_string_cmp() returns AG_CMP_EQ when comparing two equal"
    " Unicode strings")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (ag_string_cmp(s, s2) == AG_CMP_EQ);
}


AG_TEST_CASE("ag_string_cmp() returns AG_CMP_LT when comparing a Unicode string"
    " that is lexicographically less than another")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (ag_string_cmp(s2, s) == AG_CMP_LT);
}


AG_TEST_CASE("ag_string_cmp() returns AG_CMP_GT when comparing a Unicode string"
    " that is lexicographically greater than another")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (ag_string_cmp(s, s2) == AG_CMP_GT);
}


/*
 * Define the test cases for ag_string_lt().
 */


AG_TEST_CASE("ag_string_lt() returns false when comparing two empty strings")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new_empty();

        AG_TEST (!ag_string_lt(s, s2));
}


AG_TEST_CASE("ag_string_lt() returns true when comparing an empty string with a"
    " non-empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_lt(s, s2));
}


AG_TEST_CASE("ag_string_lt() returns false when comparing a string with an"
    " empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_lt(s2, s));
}


AG_TEST_CASE("ag_string_lt() returns true when comparing an ASCII string that"
    " is lexicographically less than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_lt(s, s2));
}


AG_TEST_CASE("ag_string_lt() returns false when comparing an ASCII string that"
    " is lexicographically equal to another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (!ag_string_lt(s, s2));
}


AG_TEST_CASE("ag_string_lt() returns false when comparing an ASCII string that"
    " is lexicographically greater than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_lt(s2, s));
}


AG_TEST_CASE("ag_string_lt() returns true when comparing a Unicode string that"
    " is lexicographically less than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (ag_string_lt(s2, s));
}


AG_TEST_CASE("ag_string_lt() returns false when comparing a Unicode string that"
    " is lexicographically equal to another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (!ag_string_lt(s2, s));
}


AG_TEST_CASE("ag_string_lt() returns false when comparing a Unicode string that"
    " is lexicographically greater than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (!ag_string_lt(s, s2));
}


/*
 * Define the test cases for ag_string_eq().
 */


AG_TEST_CASE("ag_string_eq() returns true when comparing two empty strings")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new_empty();
        AG_TEST (ag_string_eq(s, s2));
}


AG_TEST_CASE("ag_string_eq() returns false when comparing an empty string with"
    " a non-empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_eq(s, s2));
}


AG_TEST_CASE("ag_string_eq() returns false when comparing a string with an"
    " empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_eq(s2, s));
}


AG_TEST_CASE("ag_string_eq() returns false when comparing an ASCII string that"
    " is lexicographically less than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_eq(s, s2));
}


AG_TEST_CASE("ag_string_eq() returns true when comparing an ASCII string that"
    " is lexicographically equal to another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (ag_string_eq(s, s2));
}


AG_TEST_CASE("ag_string_eq() returns false when comparing an ASCII string that"
    " is lexicographically greater than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_eq(s2, s));
}


AG_TEST_CASE("ag_string_eq() returns false when comparing a Unicode string that"
    " is lexicographically less than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (!ag_string_eq(s2, s));
}


AG_TEST_CASE("ag_string_eq() returns true when comparing a Unicode string that"
    " is lexicographically equal to another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (ag_string_eq(s2, s));
}


AG_TEST_CASE("ag_string_eq() returns false when comparing a Unicode string that"
    " is lexicographically greater than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (!ag_string_eq(s, s2));
}


/*
 * Define the test cases for ag_string_gt().
 */


AG_TEST_CASE("ag_string_gt() returns false when comparing two empty strings")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new_empty();

        AG_TEST (!ag_string_gt(s, s2));
}


AG_TEST_CASE("ag_string_gt() returns false when comparing an empty string with"
    " a non-empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_gt(s, s2));
}


AG_TEST_CASE("ag_string_gt() returns true when comparing a string with an empty"
    "  string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_gt(s2, s));
}


AG_TEST_CASE("ag_string_gt() returns false when comparing an ASCII string that"
    " is lexicographically less than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_gt(s, s2));
}


AG_TEST_CASE("ag_string_gt() returns false when comparing an ASCII string that"
    " is lexicographically equal to another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (!ag_string_gt(s, s2));
}


AG_TEST_CASE("ag_string_gt() returns true when comparing an ASCII string that"
    " is lexicographically greater than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_gt(s2, s));
}


AG_TEST_CASE("ag_string_gt() returns false when comparing a Unicode string that"
    " is lexicographically less than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (!ag_string_gt(s2, s));
}


AG_TEST_CASE("ag_string_gt() returns false when comparing a Unicode string that"
    " is lexicographically equal to another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (!ag_string_gt(s2, s));
}


AG_TEST_CASE("ag_string_gt() returns true when comparing a Unicode string that"
    " is lexicographically greater than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (ag_string_gt(s, s2));
}


/*
 * Define the test cases for ag_string_empty().
 */


AG_TEST_CASE("ag_string_empty() returns true for an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_TEST (ag_string_empty(s));
}


AG_TEST_CASE("ag_string_empty() returns false for an ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_TEST (!ag_string_empty(s));
}


AG_TEST_CASE("ag_string_empty() returns false for a Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_TEST (!ag_string_empty(s));
}


/*
 * Define the test cases for ag_string_len().  The len_03 test seems to indicate
 * that there may be a possible bug when considering combined characters as in
 * the Devenagari script.
 *
 * TODO: Research more on Devanagari string lengths.
 */


AG_TEST_CASE("ag_string_len() returns 0 for an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_TEST (!ag_string_len(s));
}


AG_TEST_CASE("ag_string_len() returns the length of an ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_TEST (ag_string_len(s) == 13);
}


AG_TEST_CASE("ag_string_len() returns the length of a Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_TEST (ag_string_len(s) == 14);
}


/*
 * Define the test cases for ag_string_sz().
 */


AG_TEST_CASE("ag_string_sz() returns 1 for an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_TEST (ag_string_sz(s) == 1);
}


AG_TEST_CASE("ag_string_sz() determines the size of an ASCII string") {
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_TEST (ag_string_sz(s) == 14);
}


AG_TEST_CASE("ag_string_sz() determines the size of Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_TEST (ag_string_sz(s) == 39);
}


/*
 * Define the test cases for ag_string_refc().
 */


AG_TEST_CASE("ag_string_refc() returns 1 for a single instance")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_TEST (ag_string_refc(s) == 1);
}


AG_TEST_CASE("ag_string_refc() detects incremented reference counts") {
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);
        AG_AUTO(ag_string) *s3 = ag_string_copy(s2);

        AG_TEST (ag_string_refc(s) == 3);
}


AG_TEST_CASE("ag_string_refc() detects decremented reference counts")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        ag_string *s2 = ag_string_copy(s);
        ag_string_release(&s2);
        AG_AUTO(ag_string) *s3 = ag_string_copy(s);

        AG_TEST (ag_string_refc(s) == 2);
}


/*
 * Define the test cases for ag_string_has().
 */


AG_TEST_CASE("ag_string_has() returns true when both needle and haystack are"
    " empty strings")
{
        AG_AUTO(ag_string) *h = ag_string_new_empty(),
            *n = ag_string_new_empty();

        AG_TEST (ag_string_has(h, n));
}


AG_TEST_CASE("ag_string_has() returns false when haystack is empty and needle"
    " is not")
{
        AG_AUTO(ag_string) *h = ag_string_new_empty(),
            *n = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_has(h, n));
}


AG_TEST_CASE("ag_string_has() returns false when haystack is not empty and"
    " needle is")
{
        AG_AUTO(ag_string) *h = ag_string_new("Hello, world!"),
            *n = ag_string_new_empty();

        AG_TEST (!ag_string_has(h, n));
}


AG_TEST_CASE("ag_string_has() returns true if it finds an ASCII needle in an"
    " ASCII haystack")
{
        AG_AUTO(ag_string) *h = ag_string_new("Hello, world!"),
            *n = ag_string_new("o, wo");

        AG_TEST (ag_string_has(h, n));
}


AG_TEST_CASE("ag_string_has() returns false if it doesn't find an ASCII needle"
    " in an ASCII haystack")
{
        AG_AUTO(ag_string) *h = ag_string_new("Hello, world!"),
            *n = ag_string_new("o, w!");

        AG_TEST (!ag_string_has(h, n));
}


AG_TEST_CASE("ag_string_has() returns true if it finds a Unicode needle in a"
    " Unicode haystack")
{
        AG_AUTO(ag_string) *h = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *n = ag_string_new("ते दुनि");

        AG_TEST (ag_string_has(h, n));
}


AG_TEST_CASE("ag_string_has() returns false if it doesn't find a Unicode needle"
    " in a Unicode haystack")
{
        AG_AUTO(ag_string) *h = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *n = ag_string_new("तेदुनि");

        AG_TEST (!ag_string_has(h, n));
}


/*
 * Define the test cases for ag_string_match(). Note that as of now the test
 * cases are only provided for empty and ASCII strings.
 *
 * TODO: Explore Unicode tests
 */


AG_TEST_CASE("ag_string_match() returns false if applied on an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_TEST (!ag_string_match(s, "^a[[:alnum:]]"));
}


AG_TEST_CASE("ag_string_match() returns false if the regex is an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_TEST (!ag_string_match(s, ""));
}


AG_TEST_CASE("ag_string_match() returns true if a regex match is found")
{
        AG_AUTO(ag_string) *s = ag_string_new("abc");
        AG_TEST (ag_string_match(s, "^a[[:alnum:]]"));
}


AG_TEST_CASE("ag_string_match() returns false if a regex match is not found")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_TEST (!ag_string_match(s, "^a[[:alnum:]]"));
}


/*
 * Define the test cases for ag_string_lower(). Note that these test cases are
 * only for empty and ASCII strings since ag_string_lower() doesn't support
 * Unicode strings as yet.
 *
 * TODO: Add Unicode tests for ag_string_lower().
 */


AG_TEST_CASE("ag_string_lower() has no effect on an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_lower(s);

        AG_TEST (ag_string_eq(s, s2));
}


AG_TEST_CASE("ag_string_lower() converts an ASCII string to lowercase")
{
        AG_AUTO(ag_string) *s = ag_string_new("HElLo, WOrlD!");
        AG_AUTO(ag_string) *s2 = ag_string_lower(s);

        AG_TEST (ag_string_eq(s2, "hello, world!"));
}


/*
 * Define the test cases for ag_string_upper(). Again, as in the case of the
 * test cases for ag_string_lower(), we don't currently have cases testing out
 * Unicode strings.
 *
 * TODO: Add Unicode tests for ag_string_upper().
 */


AG_TEST_CASE("ag_string_upper() has no effect on an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_upper(s);

        AG_TEST (ag_string_eq(s, s2));
}


AG_TEST_CASE("ag_string_upper() converts an ASCII string to uppercase")
{
        AG_AUTO(ag_string) *s = ag_string_new("heLlO, woRLd!");
        AG_AUTO(ag_string) *s2 = ag_string_upper(s);

        AG_TEST (ag_string_eq(s2, "HELLO, WORLD!"));
}


/*
 * Define the test cases for ag_string_proper(). Just as in the case of the unit
 * tests for ag_string_lower() and ag_string_upper(), we don't currently have
 * any tests for Unicode strings.
 *
 * TODO: Add Unicode tests for ag_string_proper().
 */


AG_TEST_CASE("ag_string_proper() has no effect on an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_proper(s);

        AG_TEST (ag_string_eq(s, s2));
}


AG_TEST_CASE("ag_string_proper() converts an ASCII string to proper case")
{
        AG_AUTO(ag_string) *s = ag_string_new("tHIS isN'T.iN pRopER cASe.");
        AG_AUTO(ag_string) *s2 = ag_string_proper(s);

        AG_TEST (ag_string_eq(s2, "This Isn't.In Proper Case."));
}


/*
 * Define the test cases for ag_string_split().
 */


AG_TEST_CASE("ag_string_split() returns an empty string if applied on an empty"
    " string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_split(s, " wo");

        AG_TEST (ag_string_empty(s2));
}


AG_TEST_CASE("ag_string_split() returns the original string if the pivot is an"
    " empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split(s, "");

        AG_TEST (ag_string_eq(s, s2));
}

AG_TEST_CASE("ag_string_split() returns an empty string if both the string and"
    " the pivot are empty")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_split(s, "");

        AG_TEST (ag_string_eq(s2, ""));
}


AG_TEST_CASE("ag_string_split() returns an empty string if the pivot is not"
    " found")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split(s, "xyz");

        AG_TEST (ag_string_empty(s2));
}


AG_TEST_CASE("ag_string_split() returns an empty string if both the string and"
    " pivot are the same")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split(s, s);

        AG_TEST (ag_string_empty(s2));
}


AG_TEST_CASE("ag_string_split() returns the left side of the pivot if it exists"
    " in an ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split(s, " wo");

        AG_TEST (ag_string_eq(s2, "Hello,"));
}


AG_TEST_CASE("ag_string_split() returns the left side of the pivot if it exists"
    " in a Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_split(s, "या");

        AG_TEST (ag_string_eq(s2, "नमस्ते दुनि"));
}


/*
 * Define the test cases for ag_string_split_right().
 */


AG_TEST_CASE("ag_string_split_right() returns an empty string if applied on an"
    " empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_split_right(s, " wo");

        AG_TEST (ag_string_empty(s2));
}


AG_TEST_CASE("ag_string_split_right() returns the original string if the pivot"
    " is an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split_right(s, "");

        AG_TEST (ag_string_eq(s, s2));
}


AG_TEST_CASE("ag_string_split_right() returns an empty string if both the"
    " string and the pivot are empty")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_split(s, "");

        AG_TEST (ag_string_eq(s2, ""));
}


AG_TEST_CASE("ag_string_split_right() returns an empty string if the pivot is"
    " not found")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split_right(s, "xyz");

        AG_TEST (ag_string_empty(s2));
}


AG_TEST_CASE("ag_string_split_right() returns an empty string if both the"
    " string and pivot are the same")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split_right(s, s);

        AG_TEST (ag_string_empty(s2));
}


AG_TEST_CASE("ag_string_split_right() returns the right side of the pivot if it"
    " exists in an ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split_right(s, " w");

        AG_TEST (ag_string_eq(s2, "orld!"));
}


AG_TEST_CASE("ag_string_split() returns the right side of the pivot if it"
    " exists in a Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_split_right(s, "स्ते");

        AG_TEST (ag_string_eq(s2, " दुनिया!"));
}


/* Define the test cases for ag_string_encode() */


AG_TEST_CASE("ag_string_url_encode() has no effect on an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_url_encode(s);

        AG_TEST (!*s2);
}


AG_TEST_CASE("ag_string_url_encode() URL encodes an ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_url_encode(s);
        
        AG_TEST (ag_string_eq(s2, "Hello%2C%20world%21"));
}


AG_TEST_CASE("ag_string_url_encode() URL encodes a Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Привет, мир!");
        AG_AUTO(ag_string) *s2 = ag_string_url_encode(s);
    
        AG_TEST (ag_string_eq(s2,
            "%D0%9F%D1%80%D0%B8%D0%B2%D0%B5%D1%82%2C%20%D0%BC%D0%B8%D1%80%21"));
}


/* Define the test cases for ag_string_decode() */


AG_TEST_CASE("ag_string_url_decode() has no effect on an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_url_decode(s);

        AG_TEST (!*s2);
}


AG_TEST_CASE("ag_string_url_decode() URL encodes an ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello%2C%20world%21");
        AG_AUTO(ag_string) *s2 = ag_string_url_decode(s);

        AG_TEST (ag_string_eq(s2, "Hello, world!"));
}


AG_TEST_CASE("ag_string_url_decode() URL decodes a Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new(
            "%D0%9F%D1%80%D0%B8%D0%B2%D0%B5%D1%82%2C%20%D0%BC%D0%B8%D1%80%21");
        AG_AUTO(ag_string) *s2 = ag_string_url_decode(s);

        AG_TEST (ag_string_eq(s2, "Привет, мир!"));
}


/*
 * Define the test_suite_list() testing interface function. This function is
 * responsible for creating a test suite from the test cases defined above.
 */


extern ag_test_suite *test_suite_string(void)
{
        return AG_TEST_SUITE_GENERATE("ag_string interface");
}

