#include "./test.h"


/* 
 * The following unit tests test out ag_str_new() with statically allocated
 * empty, ASCII and Unicode strings.
 */


AG_TEST_INIT(new_01, "ag_str_new() can create an empty string") {
        ag_str_auto *s = ag_str_new("");
        AG_TEST_ASSERT (s && !*s);
} AG_TEST_EXIT();


AG_TEST_INIT(new_02, "ag_str_new() can create an ASCII string") {
        ag_str_auto *s = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (s && *s && ag_str_eq(s, "Hello, world!"));
} AG_TEST_EXIT();


AG_TEST_INIT(new_03, "ag_str_new() can create a Unicode string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        AG_TEST_ASSERT (s && *s && ag_str_eq(s, "नमस्ते दुनिया!"));
} AG_TEST_EXIT();


/* This unit test checks whether ag_str_new_empty() behaves as expected. */
AG_TEST_INIT(new_empty_01, "ag_str_new_empty() creates an empty string") {
        ag_str_auto *s = ag_str_new_empty();

        AG_TEST_ASSERT (s && !*s);
} AG_TEST_EXIT();


/* 
 * The following unit tests check whether ag_str_new_fmt() performs as expected
 * with in the case of statically allocated empty and formatted ASCII and
 * Unicode strings.
 */


AG_TEST_INIT(new_fmt_01, "ag_str_new_fmt() can create an empty string") {
        ag_str_auto *s = ag_str_new_fmt("%s", "");
        AG_TEST_ASSERT (s && !*s);
} AG_TEST_EXIT();


AG_TEST_INIT(new_fmt_02, "ag_str_new_fmt() can create a formatted ASCII"
                " string") {
        ag_str_auto *s = ag_str_new_fmt("%d. %s", 111, "Hello, world!");
        AG_TEST_ASSERT (s && *s && ag_str_eq(s, "111. Hello, world!"));
} AG_TEST_EXIT();


AG_TEST_INIT(new_fmt_03, "ag_str_new_fmt() can create a formatted Unicode"
                " string") {
        ag_str_auto *s = ag_str_new_fmt("%d. %s", -12, "नमस्ते दुनिया!");
        AG_TEST_ASSERT (s && *s && ag_str_eq(s, "-12. नमस्ते दुनिया!"));
} AG_TEST_EXIT();


/*
 * The following unit tests check whether ag_str_copy() can create shallow
 * copies of empty, ASCII and Unicode string instances.
 */


AG_TEST_INIT(copy_01, "ag_str_copy() can create a copy of an empty string") {
        ag_str_auto *s = ag_str_new_empty();
        ag_str_auto *s2 = ag_str_copy(s);
        AG_TEST_ASSERT (s2 && !*s2);
} AG_TEST_EXIT();


AG_TEST_INIT(copy_02, "ag_str_copy() can create a copy of an ASCII string") {
        ag_str_auto *s = ag_str_new("Hello, world!");
        ag_str_auto *s2 = ag_str_copy(s);
        AG_TEST_ASSERT (s2 && *s2 && ag_str_eq(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(copy_03, "ag_str_copy() can create a copy of a Unicode string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        ag_str_auto *s2 = ag_str_copy(s);
        AG_TEST_ASSERT (s2 && *s2 && ag_str_eq(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(copy_04, "ag_str_copy() increases the reference count by 1") {
        ag_str_auto *s = ag_str_new("Hello, world!");
        ag_str_auto *s2 = ag_str_copy(s);
        AG_TEST_ASSERT (ag_str_refc(s) == 2);
} AG_TEST_EXIT();


/*
 * The following unit tests check whether ag_str_release() releases a string
 * correctly, or perform a safe no-op if provided invalid strings.
 */


AG_TEST_INIT(release_01, "ag_str_release() performs a no-op if passed NULL") {
        ag_str_release(NULL);
        AG_TEST_ASSERT (true);
} AG_TEST_EXIT();


AG_TEST_INIT(release_02, "ag_str_release() performs a no-op if passed a handle"
                " to NULL") {
        ag_str *s = NULL;
        ag_str_release(&s);
        AG_TEST_ASSERT (true);
} AG_TEST_EXIT();


AG_TEST_INIT(release_03, "ag_str_release() disposes a single instance of a"
                " string") {
        ag_str *s = ag_str_new("Hello, world!");
        ag_str_release(&s);
        AG_TEST_ASSERT (!s);
} AG_TEST_EXIT();


AG_TEST_INIT(release_04, "ag_str_release() reduces the reference count by 1") {
        ag_str *s = ag_str_new("Hello, world!");
        ag_str_auto *s2 = ag_str_copy(s);
        ag_str_auto *s3 = ag_str_copy(s2);
        ag_str_release(&s);
        AG_TEST_ASSERT (!s && ag_str_refc(s2) == 2);
} AG_TEST_EXIT();


/*
 * The following test cases check whenter ag_str_cmp() behaves as expected with
 * different combinations of empty, ASCII and Unicode strings.
 */


AG_TEST_INIT(cmp_01, "ag_str_cmp() returns AG_CMP_EQ when comparing two empty"
                " strings") {
        ag_str_auto *s = ag_str_new_empty();
        ag_str_auto *s2 = ag_str_new_empty();
        AG_TEST_ASSERT (ag_str_cmp(s, s2) == AG_CMP_EQ);
} AG_TEST_EXIT();


AG_TEST_INIT(cmp_02, "ag_str_cmp() return AG_CMP_LT when comparing an empty"
                " string with another string") {
        ag_str_auto *s = ag_str_new_empty();
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (ag_str_cmp(s, s2) == AG_CMP_LT);
} AG_TEST_EXIT();


AG_TEST_INIT(cmp_03, "ag_str_cmp() return AG_CMP_EQ when comparing two equal"
                " ASCII strings") {
        ag_str_auto *s = ag_str_new("Hello, world!");
        ag_str_auto *s2 = ag_str_copy(s);
        AG_TEST_ASSERT (ag_str_cmp(s, s2) == AG_CMP_EQ);
} AG_TEST_EXIT();


AG_TEST_INIT(cmp_04, "ag_str_cmp() returns AG_CMP_LT when comparing an ASCII"
                " string that is lexicographically less than another") {
        ag_str_auto *s = ag_str_new("Goodbye, world!");
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (ag_str_cmp(s, s2) == AG_CMP_LT);
} AG_TEST_EXIT();


AG_TEST_INIT(cmp_05, "ag_str_cmp() returns AG_CMP_GT when comparing an ASCII"
                " string that is lexicographically greater than another") {
        ag_str_auto *s = ag_str_new("Goodbye, world!");
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (ag_str_cmp(s2, s) == AG_CMP_GT);
} AG_TEST_EXIT();


AG_TEST_INIT(cmp_06, "ag_str_cmp() returns AG_CMP_GT when comparing an ASCII"
                " string to an empty string") {
        ag_str_auto *s = ag_str_new_empty();
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (ag_str_cmp(s2, s) == AG_CMP_GT);
} AG_TEST_EXIT();


AG_TEST_INIT(cmp_07, "ag_str_cmp() returns AG_CMP_EQ when comparing two equal"
                " Unicode strings") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        ag_str_auto *s2 = ag_str_copy(s);
        AG_TEST_ASSERT (ag_str_cmp(s, s2) == AG_CMP_EQ);
} AG_TEST_EXIT();


AG_TEST_INIT(cmp_08, "ag_str_cmp() returns AG_CMP_LT when comparing a Unicode"
                " string that is lexicographically less than another") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        ag_str_auto *s2 = ag_str_new("अभिषेक बहुत बुरा गाता है।");
        AG_TEST_ASSERT (ag_str_cmp(s2, s) == AG_CMP_LT);
} AG_TEST_EXIT();


AG_TEST_INIT(cmp_09, "ag_str_cmp() returns AG_CMP_GT when comparing a Unicode"
                " string that is lexicographically greater than another") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        ag_str_auto *s2 = ag_str_new("अभिषेक बहुत बुरा गाता है।");
        AG_TEST_ASSERT (ag_str_cmp(s, s2) == AG_CMP_GT);
} AG_TEST_EXIT();


/*
 * The following unit tests check whether ag_str_lt() works as expected with
 * different combinations of empty, ASCII and Unicode strings.
 */


AG_TEST_INIT(lt_01, "ag_str_lt() returns false when comparing two empty"
                " strings") {
        ag_str_auto *s = ag_str_new_empty();
        ag_str_auto *s2 = ag_str_new_empty();
        AG_TEST_ASSERT (!ag_str_lt(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(lt_02, "ag_str_lt() returns true when comparing an empty string"
                " with a non-empty string") {
        ag_str_auto *s = ag_str_new_empty();
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (ag_str_lt(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(lt_03, "ag_str_lt() returns false when comparing a string with an"
                " empty string") {
        ag_str_auto *s = ag_str_new_empty();
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (!ag_str_lt(s2, s));
} AG_TEST_EXIT();


AG_TEST_INIT(lt_04, "ag_str_lt() returns true when comparing an ASCII string"
                " that is lexicographically less than another string") {
        ag_str_auto *s = ag_str_new("Goodbye, world!");
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (ag_str_lt(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(lt_05, "ag_str_lt() returns false when comparing an ASCII string"
                " that is lexicographically equal to another string") {
        ag_str_auto *s = ag_str_new("Goodbye, world!");
        ag_str_auto *s2 = ag_str_copy(s);
        AG_TEST_ASSERT (!ag_str_lt(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(lt_06, "ag_str_lt() returns false when comparing an ASCII string"
                " that is lexicographically greater than another string") {
        ag_str_auto *s = ag_str_new("Goodbye, world!");
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (!ag_str_lt(s2, s));
} AG_TEST_EXIT();


AG_TEST_INIT(lt_07, "ag_str_lt() returns true when comparing a Unicode string"
                " that is lexicographically less than another string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        ag_str_auto *s2 = ag_str_new("अभिषेक बहुत बुरा गाता है।");
        AG_TEST_ASSERT (ag_str_lt(s2, s));
} AG_TEST_EXIT();


AG_TEST_INIT(lt_08, "ag_str_lt() returns false when comparing a Unicode string"
                " that is lexicographically equal to another string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        ag_str_auto *s2 = ag_str_copy(s);
        AG_TEST_ASSERT (!ag_str_lt(s2, s));
} AG_TEST_EXIT();


AG_TEST_INIT(lt_09, "ag_str_lt() returns false when comparing a Unicode string"
                " that is lexicographically greater than another string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        ag_str_auto *s2 = ag_str_new("अभिषेक बहुत बुरा गाता है।");
        AG_TEST_ASSERT (!ag_str_lt(s, s2));
} AG_TEST_EXIT();


/*
 * The following unit tests check whether ag_str_eq() works as expected with
 * different combinations of empty, ASCII and Unicode strings.
 */


AG_TEST_INIT(eq_01, "ag_str_eq() returns true when comparing two empty"
                " strings") {
        ag_str_auto *s = ag_str_new_empty();
        ag_str_auto *s2 = ag_str_new_empty();
        AG_TEST_ASSERT (ag_str_eq(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(eq_02, "ag_str_eq() returns false when comparing an empty string"
                " with a non-empty string") {
        ag_str_auto *s = ag_str_new_empty();
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (!ag_str_eq(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(eq_03, "ag_str_eq() returns false when comparing a string with an"
                " empty string") {
        ag_str_auto *s = ag_str_new_empty();
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (!ag_str_eq(s2, s));
} AG_TEST_EXIT();


AG_TEST_INIT(eq_04, "ag_str_eq() returns false when comparing an ASCII string"
                " that is lexicographically less than another string") {
        ag_str_auto *s = ag_str_new("Goodbye, world!");
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (!ag_str_eq(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(eq_05, "ag_str_eq() returns true when comparing an ASCII string"
                " that is lexicographically equal to another string") {
        ag_str_auto *s = ag_str_new("Goodbye, world!");
        ag_str_auto *s2 = ag_str_copy(s);
        AG_TEST_ASSERT (ag_str_eq(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(eq_06, "ag_str_eq() returns false when comparing an ASCII string"
                " that is lexicographically greater than another string") {
        ag_str_auto *s = ag_str_new("Goodbye, world!");
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (!ag_str_eq(s2, s));
} AG_TEST_EXIT();


AG_TEST_INIT(eq_07, "ag_str_eq() returns false when comparing a Unicode string"
                " that is lexicographically less than another string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        ag_str_auto *s2 = ag_str_new("अभिषेक बहुत बुरा गाता है।");
        AG_TEST_ASSERT (!ag_str_eq(s2, s));
} AG_TEST_EXIT();


AG_TEST_INIT(eq_08, "ag_str_eq() returns true when comparing a Unicode string"
                " that is lexicographically equal to another string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        ag_str_auto *s2 = ag_str_copy(s);
        AG_TEST_ASSERT (ag_str_eq(s2, s));
} AG_TEST_EXIT();


AG_TEST_INIT(eq_09, "ag_str_eq() returns false when comparing a Unicode string"
                " that is lexicographically greater than another string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        ag_str_auto *s2 = ag_str_new("अभिषेक बहुत बुरा गाता है।");
        AG_TEST_ASSERT (!ag_str_eq(s, s2));
} AG_TEST_EXIT();


/*
 * The following unit tests check whether ag_str_gt() works as expected with
 * different combinations of empty, ASCII and Unicode strings.
 */


AG_TEST_INIT(gt_01, "ag_str_gt() returns false when comparing two empty"
                " strings") {
        ag_str_auto *s = ag_str_new_empty();
        ag_str_auto *s2 = ag_str_new_empty();
        AG_TEST_ASSERT (!ag_str_gt(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(gt_02, "ag_str_gt() returns false when comparing an empty string"
                " with a non-empty string") {
        ag_str_auto *s = ag_str_new_empty();
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (!ag_str_gt(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(gt_03, "ag_str_gt() returns true when comparing a string with an"
                " empty string") {
        ag_str_auto *s = ag_str_new_empty();
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (ag_str_gt(s2, s));
} AG_TEST_EXIT();


AG_TEST_INIT(gt_04, "ag_str_gt() returns false when comparing an ASCII string"
                " that is lexicographically less than another string") {
        ag_str_auto *s = ag_str_new("Goodbye, world!");
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (!ag_str_gt(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(gt_05, "ag_str_gt() returns false when comparing an ASCII string"
                " that is lexicographically equal to another string") {
        ag_str_auto *s = ag_str_new("Goodbye, world!");
        ag_str_auto *s2 = ag_str_copy(s);
        AG_TEST_ASSERT (!ag_str_gt(s, s2));
} AG_TEST_EXIT();


AG_TEST_INIT(gt_06, "ag_str_gt() returns true when comparing an ASCII string"
                " that is lexicographically greater than another string") {
        ag_str_auto *s = ag_str_new("Goodbye, world!");
        ag_str_auto *s2 = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (ag_str_gt(s2, s));
} AG_TEST_EXIT();


AG_TEST_INIT(gt_07, "ag_str_gt() returns false when comparing a Unicode string"
                " that is lexicographically less than another string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        ag_str_auto *s2 = ag_str_new("अभिषेक बहुत बुरा गाता है।");
        AG_TEST_ASSERT (!ag_str_gt(s2, s));
} AG_TEST_EXIT();


AG_TEST_INIT(gt_08, "ag_str_gt() returns false when comparing a Unicode string"
                " that is lexicographically equal to another string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        ag_str_auto *s2 = ag_str_copy(s);
        AG_TEST_ASSERT (!ag_str_gt(s2, s));
} AG_TEST_EXIT();


AG_TEST_INIT(gt_09, "ag_str_gt() returns true when comparing a Unicode string"
                " that is lexicographically greater than another string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        ag_str_auto *s2 = ag_str_new("अभिषेक बहुत बुरा गाता है।");
        AG_TEST_ASSERT (ag_str_gt(s, s2));
} AG_TEST_EXIT();


/*
 * The followint tests check whether ag_str_empty() behaves as expected with
 * empty, ASCII and Unicode strings.
 */


AG_TEST_INIT(empty_01, "ag_str_empty() returns true for an empty string") {
        ag_str_auto *s = ag_str_new_empty();
        AG_TEST_ASSERT (ag_str_empty(s));
} AG_TEST_EXIT();


AG_TEST_INIT(empty_02, "ag_str_empty() returns false for an ASCII string") {
        ag_str_auto *s = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (!ag_str_empty(s));
} AG_TEST_EXIT();


AG_TEST_INIT(empty_03, "ag_str_empty() returns false for a Unicode string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        AG_TEST_ASSERT (!ag_str_empty(s));
} AG_TEST_EXIT();


/*
 * The following unit tests check whether ag_str_len() returns the correct
 * lexicographical length for empty, ASCII and Unicode strings. The len_03 tests
 * seems to indicate that there may be a possible bug when considering combined
 * characters as in the Devenagari script. TODO: need to research more on this.
 */


AG_TEST_INIT(len_01, "ag_str_len() returns 0 for an empty string") {
        ag_str_auto *s = ag_str_new_empty();
        AG_TEST_ASSERT (!ag_str_len(s));
} AG_TEST_EXIT();


AG_TEST_INIT(len_02, "ag_str_len() returns the length of an ASCII string") {
        ag_str_auto *s = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (ag_str_len(s) == 13);
} AG_TEST_EXIT();


AG_TEST_INIT(len_03, "ag_str_len() returns the length of a Unicode string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        AG_TEST_ASSERT (ag_str_len(s) == 14);
} AG_TEST_EXIT();


/*
 * The following tests check whether ag_str_sz() works as expected with empty,
 * ASCII and Unicode strings.
 */


AG_TEST_INIT(sz_01, "ag_str_sz() returns 1 for an empty string") {
        ag_str_auto *s = ag_str_new_empty();
        AG_TEST_ASSERT (ag_str_sz(s) == 1);
} AG_TEST_EXIT();


AG_TEST_INIT(sz_02, "ag_str_sz() determines the size of an ASCII string") {
        ag_str_auto *s = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (ag_str_sz(s) == 14);
} AG_TEST_EXIT();


AG_TEST_INIT(sz_03, "ag_str_sz() determines the size of Unicode string") {
        ag_str_auto *s = ag_str_new("नमस्ते दुनिया!");
        AG_TEST_ASSERT (ag_str_sz(s) == 39);
} AG_TEST_EXIT();


/*
 * The following unit tests check whether ag_str_refc() returns the correct
 * reference count depending on the current number of soft copies.
 */


AG_TEST_INIT(refc_01, "ag_str_refc() returns 1 for a single instance") {
        ag_str_auto *s = ag_str_new("Hello, world!");
        AG_TEST_ASSERT (ag_str_refc(s) == 1);
} AG_TEST_EXIT();


AG_TEST_INIT(refc_02, "ag_str_refc() detects incremented reference counts") {
        ag_str_auto *s = ag_str_new("Hello, world!");
        ag_str_auto *s2 = ag_str_copy(s);
        ag_str_auto *s3 = ag_str_copy(s2);
        AG_TEST_ASSERT (ag_str_refc(s) == 3);
} AG_TEST_EXIT();


AG_TEST_INIT(refc_03, "ag_str_refc() detects decremented reference counts") {
        ag_str_auto *s = ag_str_new("Hello, world!");
        ag_str *s2 = ag_str_copy(s);
        ag_str_release(&s2);
        ag_str_auto *s3 = ag_str_copy(s);
        AG_TEST_ASSERT (ag_str_refc(s) == 2);
} AG_TEST_EXIT();


extern ag_test_suite *test_suite_str(void)
{
        ag_test *test[] = {
                new_01, new_02, new_03, new_empty_01, new_fmt_01, new_fmt_02,
                new_fmt_03, copy_01, copy_02, copy_03, copy_04, release_01,
                release_02, release_03, release_04, cmp_01, cmp_02, cmp_03,
                cmp_04, cmp_05, cmp_06, cmp_07, cmp_08, cmp_09, lt_01, lt_02,
                lt_03, lt_04, lt_05, lt_06, lt_07, lt_08, lt_09, eq_01, eq_02,
                eq_03, eq_04, eq_05, eq_06, eq_07, eq_08, eq_09, gt_01, gt_02,
                gt_03, gt_04, gt_05, gt_06, gt_07, gt_08, gt_09, empty_01,
                empty_02, empty_03, len_01, len_02, len_03, sz_01, sz_02, sz_03,
                refc_01, refc_02, refc_03,
        };

        const char *desc[] = {
                new_01_desc, new_02_desc, new_03_desc, new_empty_01_desc,
                new_fmt_01_desc, new_fmt_02_desc, new_fmt_03_desc, copy_01_desc,
                copy_02_desc, copy_03_desc, copy_04_desc, release_01_desc,
                release_02_desc, release_03_desc, release_04_desc, cmp_01_desc,
                cmp_02_desc, cmp_03_desc, cmp_04_desc, cmp_05_desc, cmp_06_desc,
                cmp_07_desc, cmp_08_desc, cmp_09_desc, lt_01_desc, lt_02_desc,
                lt_03_desc, lt_04_desc, lt_05_desc, lt_06_desc, lt_07_desc,
                lt_08_desc, lt_09_desc, eq_01_desc, eq_02_desc, eq_03_desc,
                eq_04_desc, eq_05_desc, eq_06_desc, eq_07_desc, eq_08_desc,
                eq_09_desc, gt_01_desc, gt_02_desc, gt_03_desc, gt_04_desc,
                gt_05_desc, gt_06_desc, gt_07_desc, gt_08_desc, gt_09_desc,
                empty_01_desc, empty_02_desc, empty_03_desc, len_01_desc,
                len_02_desc, len_03_desc, sz_01_desc, sz_02_desc, sz_03_desc,
                refc_01_desc, refc_02_desc, refc_03_desc,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_str interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

