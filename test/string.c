#include <string.h>
#include "../src/api.h"
#include "./test.h"


/*******************************************************************************
 *                              EMPTY STRING TESTS
 */

                                   /* test case #1 for empty string [AgDM:??] */
static void empty_test_new(void)
{
    printf("ag_string_new_empty() can create an empty string");

    ag_string_smart_t *test = ag_string_new_empty();
    ag_require (test && !*test, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #2 for empty string [AgDM:??] */
static void empty_test_copy(void)
{
    printf("ag_string_copy() can copy an empty string");

    ag_string_smart_t *test = ag_string_new_empty();
    ag_string_smart_t *copy = ag_string_copy(test);
    ag_require (copy && !*copy, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #3 for empty string [AgDM:??] */
static void empty_test_len(void)
{
    printf("ag_string_len() reports 0 for an empty string");

    ag_string_smart_t *test = ag_string_new_empty();
    ag_require (ag_string_len(test) == 0, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #4 for empty string [AgDM:??] */
static void empty_test_sz(void)
{
    printf("ag_string_sz() reports 0 for an empty string");

    ag_string_smart_t *test = ag_string_new_empty();
    ag_require (!ag_string_sz(test), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #5 for empty string [AgDM:??] */
static void empty_test_cmp(void)
{
    printf("ag_string_cmp() detects two equal empty strings...");

    ag_string_smart_t *lhs = ag_string_new_empty();
    ag_string_smart_t *rhs = ag_string_new_empty();
    ag_require (ag_string_cmp(lhs, rhs) == AG_TRISTATE_GND, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #6 for empty string [AgDM:??] */
static void empty_test_add(void)
{
    printf("ag_string_add() adds two empty strings...");

    ag_string_smart_t *test = ag_string_new_empty();
    ag_string_add(&test, "");
    ag_require (!*test, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                         /* runs empty string tests [AgDM:??] */
static void empty_test(void)
{
    empty_test_new();
    empty_test_copy();
    empty_test_len();
    empty_test_sz();
    empty_test_cmp();
    empty_test_add();
}


/*******************************************************************************
 *                              ASCII STRING TESTS
 */

                                   /* test case #1 for ASCII string [AgDM:??] */
static void ascii_new(void)
{
    printf("ag_string_new() can create an ASCII string...");

    const ag_string_t *expect = "Hello, world!";
    ag_string_smart_t *test = ag_string_new(expect);
    ag_require (test && !strcmp(test, expect), AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                   /* test case #2 for ASCII string [AgDM:??] */
static void ascii_copy(void)
{
    printf("ag_string_copy() can copy an ASCII string...");

    const ag_string_t *expect = "Hello, world!";
    ag_string_smart_t *test = ag_string_new(expect);
    ag_string_smart_t *copy = ag_string_copy(test);
    ag_require (test && !strcmp(copy, expect), AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                   /* test case #3 for ASCII string [AgDM:??] */
static void ascii_len(void)
{
    printf("ag_string_len() reports the length of an ASCII string...");

    ag_string_smart_t *test = ag_string_new("Hello, world!");
    ag_require (ag_string_len(test) == 13, AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                   /* test case #4 for ASCII string [AgDM:??] */
static void ascii_sz(void)
{
    printf("ag_string_sz() reports the size of an ASCII string...");

    const ag_string_t *sample = "Hello, world!";
    ag_string_smart_t *test = ag_string_new(sample);
    ag_require (ag_string_sz(test) == strlen(sample), AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                   /* test case #5 for ASCII string [AgDM:??] */
static void ascii_cmp_1(void)
{
    printf("ag_string_cmp() detects two equal ASCII strings...");

    ag_string_smart_t *lhs = ag_string_new("Hello, world!");
    ag_string_smart_t *rhs = ag_string_new("Hello, world!");
    ag_require (ag_string_cmp(lhs, rhs) == AG_TRISTATE_GND, AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                   /* test case #6 for ASCII string [AgDM:??] */
static void ascii_cmp_2(void)
{
    printf("ag_string_cmp() detects two unequal ASCII strings...");

    ag_string_smart_t *lhs = ag_string_new("Hello, world!");
    ag_string_smart_t *rhs = ag_string_new("Goodbye, moon?");
    ag_require (ag_string_cmp(lhs, rhs) != AG_TRISTATE_GND, AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                   /* test case #7 for ASCII string [AgDM:??] */
static void ascii_cmp(void)
{
    printf("ag_string_cmp() detects a lexicographically smaller ASCII"
            " string...");

    ag_string_smart_t *lhs = ag_string_new("Goodbye, moon?");
    ag_string_smart_t *rhs = ag_string_new("Hello, world!");
    ag_require (ag_string_cmp(lhs, rhs) == AG_TRISTATE_LO, AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                   /* test case #8 for ASCII string [AgDM:??] */
static void ascii_cmp_4(void)
{
    printf("ag_string_cmp() detects a lexicographically greater ASCII"
            "string...");

    ag_string_smart_t *lhs = ag_string_new("Goodbye, moon?");
    ag_string_smart_t *rhs = ag_string_new("Hello, world!");
    ag_require (ag_string_cmp(rhs, lhs) == AG_TRISTATE_HI, AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                   /* test case #9 for ASCII string [AgDM:??] */
static void ascii_add(void)
{
    printf("ag_string_add() adds two ASCII strings...");

    ag_string_smart_t *test = ag_string_new("Hello");
    ag_string_add(&test, ", ");
    ag_string_add(&test, "world");
    ag_string_add(&test, "!");
    ag_require (!strcmp(test, "Hello, world!"), AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                         /* runs ASCII string tests [AgDM:??] */
static void ascii_test(void)
{
    ascii_new();
    ascii_copy();
    ascii_len();
    ascii_sz();
    ascii_cmp_1();
    ascii_cmp_2();
    ascii_cmp();
    ascii_cmp_4();
    ascii_add();
}


/*******************************************************************************
 *                             UNICODE STRING TESTS
 */

                                 /* test case #1 for Unicode string [AgDM:??] */
static void unicode_new(void)
{
    printf("ag_string_new() can create a Unicode string...");

    const ag_string_t *expect = "Привет, мир!";
    ag_string_smart_t *test = ag_string_new(expect);
    ag_require (test && !strcmp(test, expect), AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                 /* test case #2 for Unicode string [AgDM:??] */
static void unicode_copy(void)
{
    printf("ag_string_copy() can copy a Unicode string...");

    const ag_string_t *expect = "Привет, мир!";
    ag_string_smart_t *test = ag_string_new(expect);
    ag_string_smart_t *copy = ag_string_copy(test);
    ag_require (test && !strcmp(copy, expect), AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                 /* test case #3 for Unicode string [AgDM:??] */
static void unicode_len(void)
{
    printf("ag_string_len() reports the length of a Unicode string...");

    ag_string_smart_t *test = ag_string_new("Привет, мир!");
    ag_require (ag_string_len(test) == 12, AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                 /* test case #4 for Unicode string [AgDM:??] */
static void unicode_sz(void)
{
    printf("ag_string_sz() reports the size of a Unicode string...");

    const ag_string_t *sample = "Привет, мир!";
    ag_string_smart_t *test = ag_string_new(sample);
    ag_require (ag_string_sz(test) == strlen(sample), AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                 /* test case #5 for Unicode string [AgDM:??] */
static void unicode_cmp_1(void)
{
    printf("ag_string_cmp() detects two equal Unicode strings...");

    ag_string_smart_t *lhs = ag_string_new("Привет, мир!");
    ag_string_smart_t *rhs = ag_string_new("Привет, мир!");
    ag_require (ag_string_cmp(lhs, rhs) == AG_TRISTATE_GND, AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                 /* test case #6 for Unicode string [AgDM:??] */
static void unicode_cmp_2(void)
{
    printf("ag_string_cmp() detects two unequal Unicode strings...");

    ag_string_smart_t *lhs = ag_string_new("Привет, мир!");
    ag_string_smart_t *rhs = ag_string_new("До свидания, луна?");
    ag_require (ag_string_cmp(lhs, rhs) != AG_TRISTATE_GND, AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                 /* test case #7 for Unicode string [AgDM:??] */
static void unicode_cmp_3(void)
{
    printf("ag_string_cmp() detects a lexicographically smaller Unicode"
            " string...");

    ag_string_smart_t *lhs = ag_string_new("До свидания, луна?");
    ag_string_smart_t *rhs = ag_string_new("Привет, мир!");
    ag_require (ag_string_cmp(lhs, rhs) == AG_TRISTATE_LO, AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                 /* test case #8 for Unicode string [AgDM:??] */
static void unicode_cmp_4(void)
{
    printf("ag_string_cmp() detects a lexicographically greater Unicode"
            " string...");

    ag_string_smart_t *lhs = ag_string_new("До свидания, луна?");
    ag_string_smart_t *rhs = ag_string_new("Привет, мир!");
    ag_require (ag_string_cmp(rhs, lhs) == AG_TRISTATE_HI, AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                /* test case #10 for Unicode string [AgDM:??] */
static void unicode_add(void)
{
    printf("ag_string_add() adds two Unicode strings...");

    const ag_string_t *expect = "До свидания, луна?";
    ag_string_smart_t *test = ag_string_new("До свидания");
    ag_string_add(&test, ", ");
    ag_string_add(&test, "луна");
    ag_string_add(&test, "?");
    ag_require (!strcmp(test, expect), AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                       /* runs Unicode string tests [AgDM:??] */
static void unicode_test(void)
{
    unicode_new();
    unicode_copy();
    unicode_len();
    unicode_sz();
    unicode_cmp_1();
    unicode_cmp_2();
    unicode_cmp_3();
    unicode_cmp_4();
    unicode_add();
}


/*******************************************************************************
 *                                  TEST SUITE
 */

                             /* implementation of string test suite [AgDM:??] */
extern void ag_test_string(void)
{
    printf("===============================================================\n");
    printf("Starting object model interface test suite...\n\n");

    empty_test();
    ascii_test();
    unicode_test();
    
    printf("\n");
}

