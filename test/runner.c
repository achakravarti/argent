#include "./test.h"
#include "../include/argent.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

        ag_init();

        ag_test_harness *th = ag_test_harness_new();

        ag_test_suite *log = test_log();
        ag_test_suite *mblock = ag_test_suite_mblock();
        ag_test_suite *str = test_suite_str();
        ag_test_suite *obj = test_suite_obj();

        ag_test_harness_push(th, log);
        ag_test_harness_push(th, mblock);
        ag_test_harness_push(th, str);
        ag_test_harness_push(th, obj);

        ag_test_suite_release(&log);
        ag_test_suite_release(&mblock);
        ag_test_suite_release(&str);
        ag_test_suite_release(&obj);

        ag_test_harness_exec(th);
        ag_test_harness_log(th, stdout);

        ag_test_harness_release(&th);

        ag_exit(EXIT_SUCCESS);

        return 0;
}

