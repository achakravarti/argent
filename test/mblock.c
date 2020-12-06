#include "./test.h"


struct test {
        int *i;
        int *j;
};


ag_test_init(new_01, "ag_mblock_new() allocates memory on the heap for an int")
{
        ag_mblock_auto *m = ag_mblock_new(sizeof(int));
        ag_test_assert (m);
}
ag_test_exit()


ag_test_init(new_02, "ag_mblock_new() allocates memory on the heap for a"
                " tests truct")
{
        struct test *t = ag_mblock_new(sizeof *t);
        t->i = ag_mblock_new(sizeof *t->i);
        t->j = ag_mblock_new(sizeof *t->j);

        ag_test_assert (t && t->i && t->j);
        
        ag_mblock_free((ag_mblock **)&t->i);
        ag_mblock_free((ag_mblock **)&t->j);
        ag_mblock_free((ag_mblock **)&t);

}
ag_test_exit()


ag_test_init(new_03, "ag_mblock_new() returns a block with a reference count"
                " of 1")
{
        ag_mblock_auto *m = ag_mblock_new(sizeof(int));
        ag_test_assert (ag_mblock_refc(m) == 1);
}
ag_test_exit()


ag_test_init(new_04, "ag_mblock_new() returns a block with the requested data"
                " size")
{
        ag_mblock_auto *m = ag_mblock_new(sizeof(int));
        ag_test_assert (ag_mblock_sz(m) == sizeof(int));
}
ag_test_exit()


ag_test_init(new_05, "ag_mblock_new() returns a block with a total size >="
                " requested data size")
{
        ag_mblock_auto *m = ag_mblock_new(sizeof(int));
        ag_test_assert (ag_mblock_sz_total(m) >= sizeof(int));
}
ag_test_exit()


ag_test_init(new_align_01, "ag_mblock_new_align() allocates memory on the heap"
                " for an int")
{
        ag_mblock_auto *m = ag_mblock_new_align(sizeof(int), 8);
        ag_test_assert (m);
}
ag_test_exit()


ag_test_init(new_align_02, "ag_mblock_new_align() allocates memory on the heap"
                " for a test struct")
{
        struct test *t = ag_mblock_new_align(sizeof *t, 8);
        t->i = ag_mblock_new_align(sizeof *t->i, 32);
        t->j = ag_mblock_new_align(sizeof *t->j, 32);

        ag_test_assert (t && t->i && t->j);
        
        ag_mblock_free((ag_mblock **)&t->i);
        ag_mblock_free((ag_mblock **)&t->j);
        ag_mblock_free((ag_mblock **)&t);
}
ag_test_exit()


ag_test_init(new_align_03, "ag_mblock_new_align() returns a block with a"
                "  reference count of 1")
{
        ag_mblock_auto *m = ag_mblock_new_align(sizeof(int), 8);
        ag_test_assert (ag_mblock_refc(m) == 1);
}
ag_test_exit()


ag_test_init(new_align_04, "ag_mblock_new_align() returns a block with the"
                " requested data size")
{
        ag_mblock_auto *m = ag_mblock_new_align(sizeof(int), 8);
        ag_test_assert (ag_mblock_sz(m) == sizeof(int));
}
ag_test_exit()


ag_test_init(new_align_05, "ag_mblock_new_align() returns a block with a total"
                " size >= requested data size")
{
        ag_mblock_auto *m = ag_mblock_new_align(sizeof(int), 8);
        ag_test_assert (ag_mblock_sz_total(m) >= sizeof(int));
}
ag_test_exit()


ag_test_init(new_align_06, "ag_mblock_new_align() returns a block with the"
                " required alignment")
{
        ag_mblock_auto *m = ag_mblock_new_align(sizeof(int), 32);
        ag_test_assert (ag_mblock_aligned(m, 32));
}
ag_test_exit()


ag_test_init(copy_01, "ag_mblock_copy() makes a copy of an int in the heap")
{
        int *i = ag_mblock_new(sizeof *i);
        *i = 555;

        int *j = ag_mblock_copy(i);
        ag_test_assert ((*j == 555));

        ag_mblock_free((ag_mblock **)&i);
        ag_mblock_free((ag_mblock **)&j);
}
ag_test_exit()


extern ag_test_suite *ag_test_suite_mblock(void)
{
        ag_test *test[] = {
                &new_01,
                &new_02,
                &new_03,
                &new_04,
                &new_05,
                &new_align_01,
                &new_align_02,
                &new_align_03,
                &new_align_04,
                &new_align_05,
                &new_align_06,
                &copy_01,
        };

        const char *desc[] = {
                new_01_desc,
                new_02_desc,
                new_03_desc,
                new_04_desc,
                new_05_desc,
                new_align_01_desc,
                new_align_02_desc,
                new_align_03_desc,
                new_align_04_desc,
                new_align_05_desc,
                new_align_06_desc,
                copy_01_desc,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_mblock interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

