#include "./test.h"
#include "../include/argent.h"
#include <stdio.h>


static enum ag_test_status sample(void)
{
        return ag_test_assert_debug (1);
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

        ag_test_case *tc = ag_test_case_new("sample", &sample);
        printf("status = %d\n", ag_test_case_status(tc));
        printf("msg = %s\n", ag_test_case_msg(tc));
        ag_test_case_exec(tc);
        printf("status = %d\n", ag_test_case_status(tc));
        printf("msg = %s\n", ag_test_case_msg(tc));
        ag_test_case_dispose(&tc);

    return 0;
}

