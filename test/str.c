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


extern ag_test_suite *test_suite_str(void)
{
        ag_test *test[] = {
                new_01, new_02, new_03, new_empty_01, new_fmt_01, new_fmt_02,
                new_fmt_03, copy_01, copy_02, copy_03, copy_04, release_01,
                release_02, release_03, release_04, cmp_01, cmp_02, cmp_03,
                cmp_04, cmp_05, cmp_06, cmp_07, cmp_08, cmp_09, lt_01, lt_02,
                lt_03, lt_04, lt_05, lt_06, lt_07, lt_08, lt_09,
        };

        const char *desc[] = {
                new_01_desc, new_02_desc, new_03_desc, new_empty_01_desc,
                new_fmt_01_desc, new_fmt_02_desc, new_fmt_03_desc, copy_01_desc,
                copy_02_desc, copy_03_desc, copy_04_desc, release_01_desc,
                release_02_desc, release_03_desc, release_04_desc, cmp_01_desc,
                cmp_02_desc, cmp_03_desc, cmp_04_desc, cmp_05_desc, cmp_06_desc,
                cmp_07_desc, cmp_08_desc, cmp_09_desc, lt_01_desc, lt_02_desc,
                lt_03_desc, lt_04_desc, lt_05_desc, lt_06_desc, lt_07_desc,
                lt_08_desc, lt_09_desc,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_str interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

