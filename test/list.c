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
        AG_AUTO(ag_value) *v1 = ag_value_new_int(-123);
        AG_AUTO(ag_value) *v2 = ag_value_new_int(0);
        AG_AUTO(ag_value) *v3 = ag_value_new_int(456);
        AG_AUTO(ag_value) *v4 = ag_value_new_int(-666);
        AG_AUTO(ag_value) *v5 = ag_value_new_int(0);
        AG_AUTO(ag_value) *v6 = ag_value_new_int(555);
        AG_AUTO(ag_value) *v7 = ag_value_new_int(734);

        AG_AUTO(ag_list) *l = ag_list_new();
        ag_list_push(&l, v1);
        ag_list_push(&l, v2);
        ag_list_push(&l, v3);
        ag_list_push(&l, v4);
        ag_list_push(&l, v5);
        ag_list_push(&l, v6);
        ag_list_push(&l, v7);

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


AG_TEST_INIT(clone_01, "ag_list_clone() creates a clone of an empty list")
{
        AG_AUTO(ag_list) *l = ag_list_new();
        AG_AUTO(ag_list) *l2 = ag_list_clone(l);

        AG_TEST_ASSERT (l2 && l != l2);
}
AG_TEST_EXIT();


AG_TEST_INIT(clone_02, "ag_list_clone() creates a clone of a sample list")
{
        AG_AUTO(ag_list) *l = sample_int_2();
        AG_AUTO(ag_list) *l2 = ag_list_clone(l);

        AG_TEST_ASSERT (l2 && l != l2);
}
AG_TEST_EXIT();


AG_TEST_INIT(clone_03, "ag_list_clone() has the same items as its parent")
{
        AG_AUTO(ag_list) *l = sample_int_2();
        AG_AUTO(ag_list) *l2 = ag_list_clone(l);

        AG_TEST_ASSERT (ag_list_eq(l, l2));
}
AG_TEST_EXIT();


AG_TEST_INIT(clone_04, "ag_list_clone() does not change the reference count")
{
        AG_AUTO(ag_list) *l = sample_int_2();
        AG_AUTO(ag_list) *l2 = ag_list_clone(l);

        AG_TEST_ASSERT (ag_list_refc(l) == 1);
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


AG_TEST_INIT(lt_01, 
    "ag_list_lt() returns true if a list is lexicographically smaller than"
    " another")
{
        AG_AUTO(ag_list) *l = sample_int();
        AG_AUTO(ag_list) *l2 = sample_int_2();

        AG_TEST_ASSERT (ag_list_lt(l, l2));
}
AG_TEST_EXIT();


AG_TEST_INIT(lt_02, 
    "ag_list_lt() returns false if a list is lexicographically equal to"
     " another")
{
        AG_AUTO(ag_list) *l = sample_int();
        AG_AUTO(ag_list) *l2 = sample_int();

        AG_TEST_ASSERT (!ag_list_lt(l, l2));
}
AG_TEST_EXIT();


AG_TEST_INIT(lt_03,
    "ag_list_lt() returns false if a list is lexicographically greater than"
    " another")
{
        AG_AUTO(ag_list) *l = sample_int_2();
        AG_AUTO(ag_list) *l2 = sample_int();

        AG_TEST_ASSERT (!ag_list_lt(l, l2));
}
AG_TEST_EXIT();


AG_TEST_INIT(eq_01, 
    "ag_list_eq() returns true if two lists are lexicographically equal")
{
        AG_AUTO(ag_list) *l = sample_int();
        AG_AUTO(ag_list) *l2 = sample_int();

        AG_TEST_ASSERT (ag_list_eq(l, l2));
}
AG_TEST_EXIT();


AG_TEST_INIT(eq_02, 
    "ag_list_eq() returns false if a list is lexicographically less than"
     " another")
{
        AG_AUTO(ag_list) *l = sample_int();
        AG_AUTO(ag_list) *l2 = sample_int_2();

        AG_TEST_ASSERT (!ag_list_eq(l, l2));
}
AG_TEST_EXIT();


AG_TEST_INIT(eq_03,
    "ag_list_eq() returns false if a list is lexicographically greater than"
    " another")
{
        AG_AUTO(ag_list) *l = sample_int_2();
        AG_AUTO(ag_list) *l2 = sample_int();

        AG_TEST_ASSERT (!ag_list_eq(l2, l));
}
AG_TEST_EXIT();


AG_TEST_INIT(gt_01, 
    "ag_list_lt() returns true if a list is lexicographically greater than"
    " another")
{
        AG_AUTO(ag_list) *l = sample_int();
        AG_AUTO(ag_list) *l2 = sample_int_2();

        AG_TEST_ASSERT (ag_list_gt(l2, l));
}
AG_TEST_EXIT();


AG_TEST_INIT(gt_02, 
    "ag_list_gt() returns false if a list is lexicographically equal to"
     " another")
{
        AG_AUTO(ag_list) *l = sample_int();
        AG_AUTO(ag_list) *l2 = sample_int();

        AG_TEST_ASSERT (!ag_list_gt(l, l2));
}
AG_TEST_EXIT();


AG_TEST_INIT(gt_03,
    "ag_list_gt() returns false if a list is lexicographically less than"
    " another")
{
        AG_AUTO(ag_list) *l = sample_int_2();
        AG_AUTO(ag_list) *l2 = sample_int();

        AG_TEST_ASSERT (!ag_list_gt(l2, l));
}
AG_TEST_EXIT();


AG_TEST_INIT(empty_01, "ag_list_empty() returns true for an empty list")
{
        AG_AUTO(ag_list) *l = ag_list_new();

        AG_TEST_ASSERT (ag_list_empty(l));
}
AG_TEST_EXIT();


AG_TEST_INIT(empty_02, "ag_list_empty() returns false for a non-empty list")
{
        AG_AUTO(ag_list) *l = sample_int();

        AG_TEST_ASSERT (!ag_list_empty(l));
}
AG_TEST_EXIT();


AG_TEST_INIT(typeid_01, "ag_list_typeid() returns AG_TYPEID_LIST")
{
        AG_AUTO(ag_list) *l = sample_int();

        AG_TEST_ASSERT (ag_list_typeid(l) == AG_TYPEID_LIST);
}
AG_TEST_EXIT();


AG_TEST_INIT(uuid_01, "ag_list_uuid() returns the UUID of a list")
{
        AG_AUTO(ag_list) *l = sample_int();
        AG_AUTO(ag_uuid) *u = ag_list_uuid(l);
        AG_AUTO(ag_string) *s = ag_uuid_str(u);

        AG_TEST_ASSERT (!ag_string_empty(s));
}
AG_TEST_EXIT();


AG_TEST_INIT(valid_01, "ag_list_valid() returns false for an empty list")
{
        AG_AUTO(ag_list) *l = ag_list_new();

        AG_TEST_ASSERT (!ag_list_valid(l));
}
AG_TEST_EXIT();


AG_TEST_INIT(valid_02, "ag_list_valid() returns true for an int list")
{
        AG_AUTO(ag_list) *l = sample_int();

        AG_TEST_ASSERT (ag_list_valid(l));
}
AG_TEST_EXIT();


AG_TEST_INIT(sz_01, "ag_list_sz() returns 0 for an empty list")
{
        AG_AUTO(ag_list) *l = ag_list_new();

        AG_TEST_ASSERT (!ag_list_sz(l));
}
AG_TEST_EXIT();


AG_TEST_INIT(sz_02, "ag_list_sz() returns the cumulative size of an int list")
{
        AG_AUTO(ag_list) *l = sample_int_2();

        AG_TEST_ASSERT (ag_list_sz(l) == sizeof(ag_int) * 7);
}
AG_TEST_EXIT();


AG_TEST_INIT(refc_01, "ag_list_refc() returns 1 for a single instance")
{
        AG_AUTO(ag_list) *l = ag_list_new();

        AG_TEST_ASSERT (ag_list_refc(l) == 1);
}
AG_TEST_EXIT();


AG_TEST_INIT(refc_02,
    "ag_list_refc() returns the reference count of a shallow copy")
{
        AG_AUTO(ag_list) *l = sample_int();
        AG_AUTO(ag_list) *l2 = ag_list_copy(l);
        AG_AUTO(ag_list) *l3 = ag_list_copy(l2);

        AG_TEST_ASSERT (ag_list_refc(l) == 3);
}
AG_TEST_EXIT();


AG_TEST_INIT(len_01, "ag_list_len() returns 0 for an empty list")
{
        AG_AUTO(ag_list) *l = ag_list_new();
        
        AG_TEST_ASSERT (!ag_list_len(l));
}
AG_TEST_EXIT();


AG_TEST_INIT(len_02,
    "ag_list_len() returns the number of values in a non empty list")
{
        AG_AUTO(ag_list) *l = sample_int_2();

        AG_TEST_ASSERT (ag_list_len(l) == 7);
}
AG_TEST_EXIT();


AG_TEST_INIT(hash_01, "ag_list_hash() returns 0 for an empty list")
{
        AG_AUTO(ag_list) *l = ag_list_new();

        AG_TEST_ASSERT (!ag_list_hash(l));
}
AG_TEST_EXIT();


AG_TEST_INIT(hash_02,
    "ag_list_hash() returns the cumulative hash for an int list")
{
        AG_AUTO(ag_list) *l = sample_int();
        ag_hash h = ag_hash_new(-123) + ag_hash_new(0) + ag_hash_new(456);

        AG_TEST_ASSERT (ag_list_hash(l) == h);
}
AG_TEST_EXIT();


extern ag_test_suite *test_suite_list(void)
{
        ag_test *test[] = {
                new_01, new_02, 
                
                copy_01, copy_02, 

                clone_01, clone_02, clone_03,
                clone_04,
                
                release_01, release_02, release_03, 
                release_04,

                lt_01, lt_02, lt_03,
                
                eq_01, eq_02, eq_03,
                
                gt_01, gt_02, gt_03,

                empty_01, empty_02,

                typeid_01, uuid_01,

                valid_01, valid_02,

                sz_01, sz_02,

                refc_01, refc_02,

                len_01, len_02,

                hash_01, hash_02,
        };

        const char *desc[] = {
                new_01_desc, new_02_desc, 
                
                copy_01_desc, copy_02_desc, 
                
                clone_01_desc, clone_02_desc, 
                clone_03_desc, clone_04_desc,
                
                release_01_desc, release_02_desc,
                release_03_desc, release_04_desc,

                lt_01_desc, lt_02_desc,
                lt_03_desc,
                
                eq_01_desc, eq_02_desc,
                eq_03_desc,
                
                gt_01_desc, gt_02_desc,
                gt_03_desc,

                empty_01_desc, empty_02_desc,

                typeid_01_desc, uuid_01_desc,

                valid_01_desc, valid_02_desc,

                sz_01_desc, sz_02_desc,

                refc_01_desc, refc_02_desc,

                len_01_desc, len_02_desc,

                hash_01_desc, hash_02_desc,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_list interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

