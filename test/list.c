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


AG_TEST_INIT(new_01, "ag_list_new() can create a new sample list")
{
        AG_AUTO(ag_list) *l = sample_int();
        AG_TEST_ASSERT (l && ag_list_len(l) == 3);
}
AG_TEST_EXIT();


AG_TEST_INIT(new_02, "ag_list_new() can create a new empty list")
{
        AG_AUTO(ag_list) *l = ag_list_new();
        AG_TEST_ASSERT (l && ag_list_empty(l));
}
AG_TEST_EXIT();


AG_TEST_INIT(copy_01, "ag_list_copy() creates a shallow copy of a sample list")
{
        AG_AUTO(ag_list) *l = sample_int_2();
        AG_AUTO(ag_list) *l2 = ag_list_copy(l);
        AG_TEST_ASSERT (l2 == l);
}
AG_TEST_EXIT();


AG_TEST_INIT(copy_02, "ag_list_copy() increases the reference count by 1")
{
        AG_AUTO(ag_list) *l = sample_int();
        AG_AUTO(ag_list) *l2 = ag_list_copy(l);
        AG_TEST_ASSERT (ag_list_refc(l) == 2);
}
AG_TEST_EXIT();


AG_TEST_INIT(release_01, "ag_list_release() performs a no-op if passed NULL")
{
        ag_list_release(NULL);
        AG_TEST_ASSERT (true);
}
AG_TEST_EXIT();


AG_TEST_INIT(release_02, 
    "ag_list_release() performs a no-op if passed a handle to NULL")
{
        ag_list *l = NULL;
        ag_list_release(&l);
        AG_TEST_ASSERT (true);
}
AG_TEST_EXIT();


AG_TEST_INIT(release_03,
    "ag_list_reelease() disposes a single instance of a list")
{
        ag_list *l = sample_int();
        ag_list_release(&l);
        AG_TEST_ASSERT (!l);
}
AG_TEST_EXIT();


AG_TEST_INIT(release_04, "ag_list_dispose() reduces the reference count by 1")
{
        ag_list *l = sample_int();
        AG_AUTO(ag_list) *l2 = ag_list_copy(l);
        AG_AUTO(ag_list) *l3 = ag_list_copy(l2);
        ag_list_release(&l);

        AG_TEST_ASSERT (!l && ag_list_refc(l2) == 2);
}
AG_TEST_EXIT();



extern ag_test_suite *test_suite_list(void)
{
        ag_test *test[] = {
                new_01, new_02, copy_01,
                copy_02, release_01, release_02,
                release_03, release_04,
        };

        const char *desc[] = {
                new_01_desc, new_02_desc, copy_01_desc,
                copy_02_desc, release_01_desc, release_02_desc,
                release_03_desc, release_04_desc,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_list interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

