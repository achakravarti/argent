#include "./test.h"
#include "../include/argent.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
        (void) argc;
        (void) argv;

        ag_init();

        ag_test_harness *th = ag_test_harness_new();

        ag_test_suite *log = test_suite_log();
        ag_test_suite *mblock = test_suite_memblock();
        ag_test_suite *str = test_suite_string();
        ag_test_suite *obj = test_suite_object();
        ag_test_suite *val = test_suite_value();
        ag_test_suite *fld = test_suite_field();
        ag_test_suite *lst = test_suite_list();
        ag_test_suite *alst = test_suite_alist();
        ag_test_suite *url = test_suite_http_url();
        ag_test_suite *client = test_suite_http_client();
        ag_test_suite *method= test_suite_http_method();

        ag_test_harness_push(th, log);
        ag_test_harness_push(th, mblock);
        ag_test_harness_push(th, str);
        ag_test_harness_push(th, obj);
        ag_test_harness_push(th, val);
        ag_test_harness_push(th, fld);
        ag_test_harness_push(th, lst);
        ag_test_harness_push(th, alst);
        ag_test_harness_push(th, url);
        ag_test_harness_push(th, client);
        ag_test_harness_push(th, method);

        ag_test_suite_release(&log);
        ag_test_suite_release(&mblock);
        ag_test_suite_release(&str);
        ag_test_suite_release(&obj);
        ag_test_suite_release(&val);
        ag_test_suite_release(&fld);
        ag_test_suite_release(&lst);
        ag_test_suite_release(&alst);
        ag_test_suite_release(&url);
        ag_test_suite_release(&client);
        ag_test_suite_release(&method);

        ag_test_harness_exec(th);
        ag_test_harness_log(th, stdout);

        ag_test_harness_release(&th);

        ag_exit(EXIT_SUCCESS);

        return 0;
}

