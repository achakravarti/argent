#include "./test.h"


static enum ag_test_status new_01(void)
{
        return ag_test_assert (1);
}


static enum ag_test_status new_02(void)
{
        return ag_test_assert (0);
}


extern ag_test_suite *ag_test_suite_mblock(void)
{
        ag_test *test[] = {
                &new_01,
                &new_02,
        };

        const char *desc[] = {
                "sample test",
                "another test",
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_mblock interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

