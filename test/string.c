#include <string.h>
#include "../src/api.h"
#include "./test.h"


/*******************************************************************************
 *                              EMPTY STRING TESTS
 */

                                   /* test case #1 for empty string [AgDM:??] */
static void empty_new(void)
{
    printf("ag_string_new_empty() can create an empty string");

    ag_string_smart_t *test = ag_string_new_empty();
    ag_require (test && !*test, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #2 for empty string [AgDM:??] */
static void empty_copy(void)
{
    printf("ag_string_copy() can copy an empty string");

    ag_string_smart_t *test = ag_string_new_empty();
    ag_string_smart_t *copy = ag_string_copy(test);
    ag_require (copy && !*copy, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #3 for empty string [AgDM:??] */
static void empty_copy_2(void)
{
    printf("ag_string_copy() creates a mutually exclusive copy of an empty"
           " string");

    ag_string_smart_t *s = ag_string_new_empty();
    ag_string_smart_t *cp = ag_string_copy(s);

    ag_string_smart_t *add = ag_string_new("Hello!");
    ag_string_add(&s, add);
    ag_require (!strcmp(s, "Hello!"), AG_ERNO_TEST, NULL);
    ag_require (!*cp, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #4 for empty string [AgDM:??] */
static void empty_len(void)
{
    printf("ag_string_len() reports 0 for an empty string");

    ag_string_smart_t *test = ag_string_new_empty();
    ag_require (ag_string_len(test) == 0, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #5 for empty string [AgDM:??] */
static void empty_sz(void)
{
    printf("ag_string_sz() reports 0 for an empty string");

    ag_string_smart_t *test = ag_string_new_empty();
    ag_require (!ag_string_sz(test), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #6 for empty string [AgDM:??] */
static void empty_cmp(void)
{
    printf("ag_string_cmp() detects two equal empty strings");

    ag_string_smart_t *lhs = ag_string_new_empty();
    ag_string_smart_t *rhs = ag_string_new_empty();
    ag_require (ag_string_cmp(lhs, rhs) == AG_TRISTATE_GND, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                   /* test case #7 for empty string [AgDM:??] */
static void empty_add(void)
{
    printf("ag_string_add() has no effect on adding two empty strings");

    ag_string_smart_t *s1 = ag_string_new_empty();
    ag_string_smart_t *s2 = ag_string_new_empty();
    ag_string_add(&s1, s2);
    ag_require (!*s1, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #8 for empty string [AgDM:??] */
static void empty_lt(void)
{
    printf("ag_string_lt() returns false for two empty strings");

    ag_string_smart_t *s1 = ag_string_new_empty();
    ag_string_smart_t *s2 = ag_string_new_empty();
    ag_require (!ag_string_lt(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #9 for empty string [AgDM:??] */
static void empty_eq(void)
{
    printf("ag_string_eq() returns true for two empty strings");

    ag_string_smart_t *s1 = ag_string_new_empty();
    ag_string_smart_t *s2 = ag_string_new_empty();
    ag_require (ag_string_eq(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                  /* test case #10 for empty string [AgDM:??] */
static void empty_gt(void)
{
    printf("ag_string_gt() returns false for two empty strings");

    ag_string_smart_t *s1 = ag_string_new_empty();
    ag_string_smart_t *s2 = ag_string_new_empty();
    ag_require (!ag_string_lt(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                  /* test case #11 for empty string [AgDM:??] */
static void empty_add_cstr(void)
{
    printf("ag_string_add_cstr() has no effect on adding null string to an"
            " empty string");

    ag_string_smart_t *s1 = ag_string_new_empty();
    ag_string_add_cstr(&s1, "");
    ag_require (!*s1, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                         /* runs empty string tests [AgDM:??] */
static void empty_test(void)
{
    empty_new();
    empty_copy();
    empty_copy_2();
    empty_len();
    empty_sz();
    empty_cmp();
    empty_add();
    empty_lt();
    empty_eq();
    empty_gt();
    empty_add_cstr();
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
static void ascii_copy_2(void)
{
    printf("ag_string_copy() creates a mutually exclusive copy of an ASCII"
           " string");

    ag_string_smart_t *s = ag_string_new("Hello");
    ag_string_smart_t *cp = ag_string_copy(s);

    ag_string_smart_t *add = ag_string_new(", world!");
    ag_string_add(&s, add);
    ag_require (!strcmp(s, "Hello, world!"), AG_ERNO_TEST, NULL);
    ag_require (!strcmp(cp, "Hello"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #4 for ASCII string [AgDM:??] */
static void ascii_len(void)
{
    printf("ag_string_len() reports the length of an ASCII string");

    ag_string_smart_t *test = ag_string_new("Hello, world!");
    ag_require (ag_string_len(test) == 13, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #5 for ASCII string [AgDM:??] */
static void ascii_sz(void)
{
    printf("ag_string_sz() reports the size of an ASCII string");

    const ag_string_t *sample = "Hello, world!";
    ag_string_smart_t *test = ag_string_new(sample);
    ag_require (ag_string_sz(test) == strlen(sample), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #6 for ASCII string [AgDM:??] */
static void ascii_cmp_1(void)
{
    printf("ag_string_cmp() detects two equal ASCII strings");

    ag_string_smart_t *lhs = ag_string_new("Hello, world!");
    ag_string_smart_t *rhs = ag_string_new("Hello, world!");
    ag_require (ag_string_cmp(lhs, rhs) == AG_TRISTATE_GND, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #7 for ASCII string [AgDM:??] */
static void ascii_cmp_2(void)
{
    printf("ag_string_cmp() detects two unequal ASCII strings");

    ag_string_smart_t *lhs = ag_string_new("Hello, world!");
    ag_string_smart_t *rhs = ag_string_new("Goodbye, moon?");
    ag_require (ag_string_cmp(lhs, rhs) != AG_TRISTATE_GND, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #8 for ASCII string [AgDM:??] */
static void ascii_cmp(void)
{
    printf("ag_string_cmp() detects a lexicographically smaller ASCII"
            " string");

    ag_string_smart_t *lhs = ag_string_new("Goodbye, moon?");
    ag_string_smart_t *rhs = ag_string_new("Hello, world!");
    ag_require (ag_string_cmp(lhs, rhs) == AG_TRISTATE_LO, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case #9 for ASCII string [AgDM:??] */
static void ascii_cmp_4(void)
{
    printf("ag_string_cmp() detects a lexicographically greater ASCII"
            "string");

    ag_string_smart_t *lhs = ag_string_new("Goodbye, moon?");
    ag_string_smart_t *rhs = ag_string_new("Hello, world!");
    ag_require (ag_string_cmp(rhs, lhs) == AG_TRISTATE_HI, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                  /* test case #10 for ASCII string [AgDM:??] */
static void ascii_add(void)
{
    printf("ag_string_add() adds two ASCII strings");

    ag_string_smart_t *s1 = ag_string_new("Hello");
    ag_string_smart_t *s2 = ag_string_new(", ");
    ag_string_smart_t *s3 = ag_string_new("world");
    ag_string_smart_t *s4 = ag_string_new("!");

    ag_string_add(&s1, s2);
    ag_string_add(&s1, s3);
    ag_string_add(&s1, s4);
    ag_require (!strcmp(s1, "Hello, world!"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                  /* test case #11 for ASCII string [AgDM:??] */
static void ascii_lt_1(void)
{
    printf("ag_string_lt() returns true if an ASCII string is lexicographically"
            " less than another");

    ag_string_smart_t *s1 = ag_string_new("Goodbye, moon?");
    ag_string_smart_t *s2 = ag_string_new("Hello, world!");
    ag_require (ag_string_lt(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                  /* test case #12 for ASCII string [AgDM:??] */
static void ascii_lt_2(void)
{
    printf("ag_string_lt() returns false if an ASCII string is not"
            " lexicographically less than another");

    ag_string_smart_t *s1 = ag_string_new("Hello, world!");
    ag_string_smart_t *s2 = ag_string_new("Goodbye, moon?");
    ag_require (!ag_string_lt(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                  /* test case #13 for ASCII string [AgDM:??] */
static void ascii_eq_1(void)
{
    printf("ag_string_eq() returns true if an ASCII string is lexicographically"
            " equal to another");

    ag_string_smart_t *s1 = ag_string_new("Hello, world!");
    ag_string_smart_t *s2 = ag_string_new("Hello, world!");
    ag_require (ag_string_eq(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                  /* test case #14 for ASCII string [AgDM:??] */
static void ascii_eq_2(void)
{
    printf("ag_string_eq() returns true if an ASCII string is lexicographically"
            " not equal to another");

    ag_string_smart_t *s1 = ag_string_new("Hello, world!");
    ag_string_smart_t *s2 = ag_string_new("Goodbye, moon?");
    ag_require (!ag_string_eq(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                  /* test case #15 for ASCII string [AgDM:??] */
static void ascii_gt_1(void)
{
    printf("ag_string_gt() returns true if an ASCII string is lexicographically"
            " greater than another");

    ag_string_smart_t *s1 = ag_string_new("Hello, world!");
    ag_string_smart_t *s2 = ag_string_new("Goodbye, moon?");
    ag_require (ag_string_gt(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                  /* test case #16 for ASCII string [AgDM:??] */
static void ascii_gt_2(void)
{
    printf("ag_string_gt() returns false if an ASCII string is not"
            " lexicographically greater than another");

    ag_string_smart_t *s1 = ag_string_new("Goodbye, moon?");
    ag_string_smart_t *s2 = ag_string_new("Hello, world!");
    ag_require (!ag_string_gt(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                  /* test case #17 for ASCII string [AgDM:??] */
static void ascii_add_cstr(void)
{
    printf("ag_string_add() adds a C-style ASCII string to a string instance");

    ag_string_smart_t *s1 = ag_string_new("Hello");

    ag_string_add_cstr(&s1, ", ");
    ag_string_add_cstr(&s1, "world");
    ag_string_add_cstr(&s1, "!");
    ag_require (!strcmp(s1, "Hello, world!"), AG_ERNO_TEST, NULL);

    printf("OK\n");
}

                                         /* runs ASCII string tests [AgDM:??] */
static void ascii_test(void)
{
    ascii_new();
    ascii_copy();
    ascii_copy_2();
    ascii_len();
    ascii_sz();
    ascii_cmp_1();
    ascii_cmp_2();
    ascii_cmp();
    ascii_cmp_4();
    ascii_add();
    ascii_lt_1();
    ascii_lt_2();
    ascii_eq_1();
    ascii_eq_2();
    ascii_gt_1();
    ascii_gt_2();
    ascii_add_cstr();
}


/*******************************************************************************
 *                             UNICODE STRING TESTS
 */

                                 /* test case #1 for Unicode string [AgDM:??] */
static void unicode_new(void)
{
    printf("ag_string_new() can create a Unicode string");

    const ag_string_t *expect = "Привет, мир!";
    ag_string_smart_t *test = ag_string_new(expect);
    ag_require (test && !strcmp(test, expect), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                 /* test case #2 for Unicode string [AgDM:??] */
static void unicode_copy(void)
{
    printf("ag_string_copy() can copy a Unicode string");

    const ag_string_t *expect = "Привет, мир!";
    ag_string_smart_t *test = ag_string_new(expect);
    ag_string_smart_t *copy = ag_string_copy(test);
    ag_require (test && !strcmp(copy, expect), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                 /* test case #3 for Unicode string [AgDM:??] */
static void unicode_copy_2(void)
{
    printf("ag_string_copy() creates a mutually exclusive copy of a Unicode"
           " string");

    ag_string_smart_t *s = ag_string_new("Привет");
    ag_string_smart_t *cp = ag_string_copy(s);

    ag_string_smart_t *add = ag_string_new(", мир!");
    ag_string_add(&s, add);
    ag_require (!strcmp(s, "Привет, мир!"), AG_ERNO_TEST, NULL);
    ag_require (!strcmp(cp, "Привет"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                 /* test case #4 for Unicode string [AgDM:??] */
static void unicode_len(void)
{
    printf("ag_string_len() reports the length of a Unicode string");

    ag_string_smart_t *test = ag_string_new("Привет, мир!");
    ag_require (ag_string_len(test) == 12, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                 /* test case #5 for Unicode string [AgDM:??] */
static void unicode_sz(void)
{
    printf("ag_string_sz() reports the size of a Unicode string");

    const ag_string_t *sample = "Привет, мир!";
    ag_string_smart_t *test = ag_string_new(sample);
    ag_require (ag_string_sz(test) == strlen(sample), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                 /* test case #6 for Unicode string [AgDM:??] */
static void unicode_cmp_1(void)
{
    printf("ag_string_cmp() detects two equal Unicode strings");

    ag_string_smart_t *lhs = ag_string_new("Привет, мир!");
    ag_string_smart_t *rhs = ag_string_new("Привет, мир!");
    ag_require (ag_string_cmp(lhs, rhs) == AG_TRISTATE_GND, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                 /* test case #7 for Unicode string [AgDM:??] */
static void unicode_cmp_2(void)
{
    printf("ag_string_cmp() detects two unequal Unicode strings");

    ag_string_smart_t *lhs = ag_string_new("Привет, мир!");
    ag_string_smart_t *rhs = ag_string_new("До свидания, луна?");
    ag_require (ag_string_cmp(lhs, rhs) != AG_TRISTATE_GND, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                 /* test case #8 for Unicode string [AgDM:??] */
static void unicode_cmp_3(void)
{
    printf("ag_string_cmp() detects a lexicographically smaller Unicode"
            " string");

    ag_string_smart_t *lhs = ag_string_new("До свидания, луна?");
    ag_string_smart_t *rhs = ag_string_new("Привет, мир!");
    ag_require (ag_string_cmp(lhs, rhs) == AG_TRISTATE_LO, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                 /* test case #9 for Unicode string [AgDM:??] */
static void unicode_cmp_4(void)
{
    printf("ag_string_cmp() detects a lexicographically greater Unicode"
            " string");

    ag_string_smart_t *lhs = ag_string_new("До свидания, луна?");
    ag_string_smart_t *rhs = ag_string_new("Привет, мир!");
    ag_require (ag_string_cmp(rhs, lhs) == AG_TRISTATE_HI, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                /* test case #10 for Unicode string [AgDM:??] */
static void unicode_add(void)
{
    printf("ag_string_add() adds two Unicode strings");

    ag_string_smart_t *expect = ag_string_new("До свидания, луна?");
    ag_string_smart_t *test = ag_string_new("До свидания");
    ag_string_smart_t *s1 = ag_string_new(", ");
    ag_string_smart_t *s2 = ag_string_new("луна");
    ag_string_smart_t *s3 = ag_string_new("?");

    ag_string_add(&test, s1);
    ag_string_add(&test, s2);
    ag_string_add(&test, s3);
    ag_require (!strcmp(test, expect), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                /* test case #11 for Unicode string [AgDM:??] */
static void unicode_lt_1(void)
{
    printf("ag_string_lt() returns true if a Unicode string is"
            " lexicographically less than another");

    ag_string_smart_t *s1 = ag_string_new("До свидания, луна?");
    ag_string_smart_t *s2 = ag_string_new("Привет, мир!");
    ag_require (ag_string_lt(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                /* test case #12 for Unicode string [AgDM:??] */
static void unicode_lt_2(void)
{
    printf("ag_string_lt() returns false if a Unicode string is not"
            " lexicographically less than another");

    ag_string_smart_t *s1 = ag_string_new("Привет, мир!");
    ag_string_smart_t *s2 = ag_string_new("До свидания, луна?");
    ag_require (!ag_string_lt(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                /* test case #13 for Unicode string [AgDM:??] */
static void unicode_eq_1(void)
{
    printf("ag_string_eq() returns true for two equal Unicode strings");

    ag_string_smart_t *s1 = ag_string_new("Привет, мир!");
    ag_string_smart_t *s2 = ag_string_new("Привет, мир!");
    ag_require (ag_string_eq(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                /* test case #14 for Unicode string [AgDM:??] */
static void unicode_eq_2(void)
{
    printf("ag_string_eq() returns false for two unequal Unicode strings");

    ag_string_smart_t *s1 = ag_string_new("До свидания, луна?");
    ag_string_smart_t *s2 = ag_string_new("Привет, мир!");
    ag_require (!ag_string_eq(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                /* test case #15 for Unicode string [AgDM:??] */
static void unicode_gt_1(void)
{
    printf("ag_string_gt() returns true if a Unicode string is"
            " lexicographically less than another");

    ag_string_smart_t *s1 = ag_string_new("Привет, мир!");
    ag_string_smart_t *s2 = ag_string_new("До свидания, луна?");
    ag_require (ag_string_gt(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                /* test case #16 for Unicode string [AgDM:??] */
static void unicode_gt_2(void)
{
    printf("ag_string_gt() returns false if a Unicode string is not"
            " lexicographically less than another");

    ag_string_smart_t *s1 = ag_string_new("До свидания, луна?");
    ag_string_smart_t *s2 = ag_string_new("Привет, мир!");
    ag_require (!ag_string_gt(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                /* test case #17 for Unicode string [AgDM:??] */
static void unicode_add_cstr(void)
{
    printf("ag_string_add() adds C-style Unicode string to a string instance");

    ag_string_smart_t *expect = ag_string_new("До свидания, луна?");
    ag_string_smart_t *s = ag_string_new("До свидания");
    ag_string_add_cstr(&s, ", ");
    ag_string_add_cstr(&s, "луна");
    ag_string_add_cstr(&s, "?");
    ag_require (!strcmp(s, expect), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                       /* runs Unicode string tests [AgDM:??] */
static void unicode_test(void)
{
    unicode_new();
    unicode_copy();
    unicode_copy_2();
    unicode_len();
    unicode_sz();
    unicode_cmp_1();
    unicode_cmp_2();
    unicode_cmp_3();
    unicode_cmp_4();
    unicode_add();
    unicode_lt_1();
    unicode_lt_2();
    unicode_eq_1();
    unicode_eq_2();
    unicode_gt_1();
    unicode_gt_2();
    unicode_add_cstr();
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

