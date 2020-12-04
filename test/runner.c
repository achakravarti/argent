#include "./test.h"
#include "../include/argent.h"
#include <stdio.h>


static enum ag_test_status sample_1(ag_test_case *tc)
{
        ag_test_case_desc_set(tc, "sample 1 test case");
        return ag_test_assert_debug (1);
}


static enum ag_test_status sample_2(ag_test_case *tc)
{
        ag_test_case_desc_set(tc, "sample 2 test case");
        return ag_test_assert (0);
}


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    /*ag_test_memblock();
    ag_test_string();
    ag_test_object();
    ag_test_log();
    ag_test_value();*/

        /*ag_test_case *tc = ag_test_case_new(&sample);
        printf("status = %d\n", ag_test_case_status(tc));
        printf("msg = %s\n", ag_test_case_msg(tc));

        ag_test_case_exec(tc);
        printf("status = %d\n", ag_test_case_status(tc));
        printf("msg = %s\n", ag_test_case_msg(tc));

        ag_test_case_dispose(&tc);*/

        ag_test_case *tc1 = ag_test_case_new(&sample_1);
        ag_test_case *tc2 = ag_test_case_new(&sample_2);

        ag_test_suite *ts1 = ag_test_suite_new("sample 1 test suite");

        ag_test_suite_push(ts1, tc1);
        ag_test_suite_push(ts1, tc2);
        ag_test_suite_exec_console(ts1);

        /*printf("len = %lu\n", ag_test_suite_len(ts1));
        printf("pass = %d\n", ag_test_suite_pass(ts1));
        printf("skip = %d\n", ag_test_suite_skip(ts1));
        printf("fail = %d\n", ag_test_suite_fail(ts1));
        printf("%s\n", ag_test_suite_str(ts1));*/

        ag_test_case_dispose(&tc1);
        ag_test_case_dispose(&tc2);

        ag_test_suite_dispose(&ts1);


    return 0;
}

