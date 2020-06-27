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
    ag_require (!ag_string_cmp(lhs, rhs), AG_ERNO_TEST, NULL);

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




/*******************************************************************************
 *                              ASCII STRING TESTS
 */




/*******************************************************************************
 *                             UNICODE STRING TESTS
 */




/*******************************************************************************
 *                                  TEST SUITE
 */


                                         /* runs empty string tests [AgDM:??] */
static void test_empty(void)
{
    empty_test_new();
    empty_test_copy();
    empty_test_len();
    empty_test_sz();
    empty_test_cmp();
    empty_test_add();
}


                             /* implementation of string test suite [AgDM:??] */
extern void ag_test_string(void)
{
    printf("===============================================================\n");
    printf("Starting object model interface test suite...\n\n");

    test_empty();
    
    printf("\n");
}

