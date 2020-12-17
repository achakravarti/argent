#include "./test.h"
#include "../include/argent.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


/*static enum ag_test_status sample_1(ag_test_case *tc)
{
        ag_test_case_desc_set(tc, "sample 1 test case");
        return ag_test_assert_debug (1);
}


static enum ag_test_status sample_2(ag_test_case *tc)
{
        ag_test_case_desc_set(tc, "sample 2 test case");
        return ag_test_assert (0);
}*/


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    /*ag_test_memblock();
    ag_test_string();
    ag_test_object();
    ag_test_log();
    ag_test_value();*/

        /*ag_test_case *tc1 = ag_test_case_new(&sample_1);
        ag_test_case *tc2 = ag_test_case_new(&sample_2);

        ag_test_suite *ts1 = ag_test_suite_new("sample 1 test suite");
        ag_test_harness *th = ag_test_harness_new();

        ag_test_suite_push(ts1, tc1);
        ag_test_suite_push(ts1, tc2);
        ag_test_harness_push(th, ts1);
        
        ag_test_harness_exec(th);
        ag_test_harness_log(th, stdout);


        ag_test_case_dispose(&tc1);
        ag_test_case_dispose(&tc2);
        ag_test_suite_dispose(&ts1);
        ag_test_harness_dispose(&th);*/

        ag_init();

        ag_test_harness *th = ag_test_harness_new();

        ag_test_suite *mblock = ag_test_suite_mblock();
        ag_test_suite *log = test_log();

        ag_test_harness_push(th, mblock);
        ag_test_harness_push(th, log);

        ag_test_suite_release(&mblock);
        ag_test_suite_release(&log);

        ag_test_harness_exec(th);
        ag_test_harness_log(th, stdout);

        ag_test_harness_free(&th);

        ag_exit(EXIT_SUCCESS);

        return 0;
}

