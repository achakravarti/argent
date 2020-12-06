#include "./test.h"


static enum ag_test_status new_01(void)
{
        return ag_test_assert (1);
}


static enum ag_test_status new_02(void)
{
        return ag_test_assert (0);
}


extern _ag_test_suite *ag_test_suite_mblock(void)
{
        _ag_test *test[] = {
                &new_01,
                &new_02,
        };

        const char *desc[] = {
                "sample test",
                "another test",
        };

        _ag_test_suite *ctx = _ag_test_suite_new("ag_mblock interface");
        _ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

