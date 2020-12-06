#include "./test.h"


static enum ag_test_status new_01(ag_test_case *tc)
{
        ag_test_case_desc_set(tc, "sample test");
        return ag_test_assert (1);
}


extern ag_test_suite *ag_test_suite_mblock(void)
{
        ag_test_suite *ctx = ag_test_suite_new("ag_mblock interface");

        ag_test_case *new1 = ag_test_case_new(&new_01);
        ag_test_suite_push(ctx, new1);
        ag_test_case_free(&new1);

        return ctx;
}

