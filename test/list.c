#include "./test.h"


static ag_list *sample_int(void)
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(-123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(0);
        AG_AUTO(ag_value) *v3 = ag_value_new_int(456);

        AG_AUTO(ag_list) *l = ag_list_new();
        ag_list_push(&l, v1);
        ag_list_push(&l, v2);
        ag_list_push(&l, v3);

        return ag_list_copy(l);
}


static ag_list *sample_int_2(void)
{
        AG_AUTO(ag_value) *v1 = ag_value_new_int(-666);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(0);
        AG_AUTO(ag_value) *v3 = ag_value_new_int(555);
        AG_AUTO(ag_value) *v4 = ag_value_new_int(734);

        AG_AUTO(ag_list) *l = ag_list_new();
        ag_list_push(&l, v1);
        ag_list_push(&l, v2);
        ag_list_push(&l, v3);
        ag_list_push(&l, v4);

        return ag_list_copy(l);
}


AG_TEST_INIT(new_01, "ag_list_new() can create a new sample list") {
        AG_AUTO(ag_list) *l = sample_int();
        AG_TEST_ASSERT (l && ag_list_len(l) == 3);
} AG_TEST_EXIT();


extern ag_test_suite *test_suite_list(void)
{
        ag_test *test[] = {
                new_01,
        };

        const char *desc[] = {
                new_01_desc,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_list interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

