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


AG_TEST_INIT(release_01, "ag_str_release() performs a no-op if passed NULL") {
        ag_str_release(NULL);
        AG_TEST_ASSERT (true);
} AG_TEST_EXIT();


AG_TEST_INIT(release_02, "ag_str_release() perfroms a no-op if passed a handle"
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



extern ag_test_suite *test_suite_str(void)
{
        ag_test *test[] = {
                new_01, new_02, new_03, new_empty_01, new_fmt_01, new_fmt_02,
                new_fmt_03, copy_01, copy_02, copy_03, copy_04, release_01,
                release_02, release_03, release_04,
        };

        const char *desc[] = {
                new_01_desc, new_02_desc, new_03_desc, new_empty_01_desc,
                new_fmt_01_desc, new_fmt_02_desc, new_fmt_03_desc, copy_01_desc,
                copy_02_desc, copy_03_desc, copy_04_desc, release_01_desc,
                release_02_desc, release_03_desc, release_04_desc,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_str interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

