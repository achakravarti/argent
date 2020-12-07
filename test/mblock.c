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
ag_test_exit();


ag_test_init(new_02, "ag_mblock_new() allocates memory on the heap for a test"
                " struct")
{
        struct test *t = ag_mblock_new(sizeof *t);
        t->i = ag_mblock_new(sizeof *t->i);
        t->j = ag_mblock_new(sizeof *t->j);

        ag_test_assert (t && t->i && t->j);
        
        ag_mblock_free((ag_mblock **)&t->i);
        ag_mblock_free((ag_mblock **)&t->j);
        ag_mblock_free((ag_mblock **)&t);

}
ag_test_exit();


ag_test_init(new_03, "ag_mblock_new() returns a block with a reference count"
                " of 1")
{
        ag_mblock_auto *m = ag_mblock_new(sizeof(int));
        ag_test_assert (ag_mblock_refc(m) == 1);
}
ag_test_exit();


ag_test_init(new_04, "ag_mblock_new() returns a block with the requested data"
                " size")
{
        ag_mblock_auto *m = ag_mblock_new(sizeof(int));
        ag_test_assert (ag_mblock_sz(m) == sizeof(int));
}
ag_test_exit();


ag_test_init(new_05, "ag_mblock_new() returns a block with a total size >="
                " requested data size")
{
        ag_mblock_auto *m = ag_mblock_new(sizeof(int));
        ag_test_assert (ag_mblock_sz_total(m) >= sizeof(int));
}
ag_test_exit();


ag_test_init(new_align_01, "ag_mblock_new_align() allocates memory on the heap"
                " for an int")
{
        ag_mblock_auto *m = ag_mblock_new_align(sizeof(int), 8);
        ag_test_assert (m);
}
ag_test_exit();


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
ag_test_exit();


ag_test_init(new_align_03, "ag_mblock_new_align() returns a block with a"
                " reference count of 1")
{
        ag_mblock_auto *m = ag_mblock_new_align(sizeof(int), 8);
        ag_test_assert (ag_mblock_refc(m) == 1);
}
ag_test_exit();


ag_test_init(new_align_04, "ag_mblock_new_align() returns a block with the"
                " requested data size")
{
        ag_mblock_auto *m = ag_mblock_new_align(sizeof(int), 8);
        ag_test_assert (ag_mblock_sz(m) == sizeof(int));
}
ag_test_exit();


ag_test_init(new_align_05, "ag_mblock_new_align() returns a block with a total"
                " size >= requested data size")
{
        ag_mblock_auto *m = ag_mblock_new_align(sizeof(int), 8);
        ag_test_assert (ag_mblock_sz_total(m) >= sizeof(int));
}
ag_test_exit();


ag_test_init(new_align_06, "ag_mblock_new_align() returns a block with the"
                " required alignment")
{
        ag_mblock_auto *m = ag_mblock_new_align(sizeof(int), 32);
        ag_test_assert (ag_mblock_aligned(m, 32));
}
ag_test_exit();


ag_test_init(copy_01, "ag_mblock_copy() makes a copy of an int in the heap")
{
        int *i = ag_mblock_new(sizeof *i);
        *i = 555;

        int *j = ag_mblock_copy(i);
        ag_test_assert (*j == 555);

        ag_mblock_free((ag_mblock **)&i);
        ag_mblock_free((ag_mblock **)&j);
}
ag_test_exit();


ag_test_init(copy_02, "ag_mblock_copy() makes a copy of a test structure")
{
        struct test *t = ag_mblock_new(sizeof *t);
        t->i = ag_mblock_new(sizeof *t->i);
        t->j = ag_mblock_new(sizeof *t->j);

        *t->i = 555;
        *t->j = -666;

        struct test *cp = ag_mblock_copy(t);
        ag_test_assert (*cp->i == *t->i && *cp->j == *t->j);

        ag_mblock_free((ag_mblock **)&t->i);
        ag_mblock_free((ag_mblock **)&t->j);
        ag_mblock_free((ag_mblock **)&t);
        ag_mblock_free((ag_mblock **)&cp);
}
ag_test_exit();


ag_test_init(copy_03, "ag_mblock_copy() returns a copy with the same address")
{
        ag_mblock_auto *src = ag_mblock_new(sizeof(int));
        ag_mblock_auto *cp = ag_mblock_copy(src);
        ag_test_assert (src == cp);
}
ag_test_exit();


ag_test_init(copy_04, "ag_mblock_copy() increases the reference count by 1")
{
        ag_mblock_auto *src = ag_mblock_new(sizeof(int));
        ag_mblock_auto *cp = ag_mblock_copy(src);
        ag_mblock_auto *cp2 = ag_mblock_copy(cp);
        
        ag_test_assert (ag_mblock_refc(src) == 3);
}
ag_test_exit();


ag_test_init(copy_05, "ag_mblock_copy() syncs reference count of both source"
                " and copy")
{
        ag_mblock_auto *src = ag_mblock_new(sizeof(int));
        ag_mblock_auto *cp = ag_mblock_copy(src);
        ag_mblock_auto *cp2 = ag_mblock_copy(cp);

        ag_test_assert (ag_mblock_refc(src) == ag_mblock_refc(cp)
                        && ag_mblock_refc(src) == ag_mblock_refc(cp2));
}
ag_test_exit();

ag_test_init(copy_06, "ag_mblock_copy() preserves the data size of the source")
{
        ag_mblock_auto *src = ag_mblock_new(sizeof(int));
        ag_mblock_auto *cp = ag_mblock_copy(src);

        ag_test_assert (ag_mblock_sz(src) == ag_mblock_sz(cp));
}
ag_test_exit();


ag_test_init(copy_07, "ag_mblock_copy() preserves the total size of the source")
{
        ag_mblock_auto *src = ag_mblock_new(sizeof(int));
        ag_mblock_auto *cp = ag_mblock_copy(src);

        ag_test_assert (ag_mblock_sz(src) == ag_mblock_sz(cp));
}
ag_test_exit();

ag_test_init(copy_deep_01, "ag_mblock_copy_deep() makes a copy of an int in the"
                " heap")
{
        int *i = ag_mblock_new(sizeof *i);
        *i = 555;

        int *j = ag_mblock_copy_deep(i);
        ag_test_assert (*j == 555);

        ag_mblock_free((ag_mblock **)&i);
        ag_mblock_free((ag_mblock **)&j);
}
ag_test_exit();

ag_test_init(copy_deep_02, "ag_mblock_copy_deep() makes a copy of a test"
                " structure")
{
        struct test *t = ag_mblock_new(sizeof *t);
        t->i = ag_mblock_new(sizeof *t->i);
        t->j = ag_mblock_new(sizeof *t->j);

        *t->i = 555;
        *t->j = -666;

        struct test *cp = ag_mblock_copy_deep(t);
        ag_test_assert (*cp->i == *t->i && *cp->j == *t->j);

        ag_mblock_free((ag_mblock **)&t->i);
        ag_mblock_free((ag_mblock **)&t->j);
        ag_mblock_free((ag_mblock **)&t);
        ag_mblock_free((ag_mblock **)&cp);
}
ag_test_exit();


ag_test_init(copy_deep_03, "ag_mblock_copy_deep() returns a copy with another"
                " address")
{
        ag_mblock_auto *src = ag_mblock_new(sizeof(int));
        ag_mblock_auto *cp = ag_mblock_copy_deep(src);
        ag_test_assert (src != cp);
}
ag_test_exit();


ag_test_init(copy_deep_04, "ag_mblock_copy_deep() sets reference to 1 of the"
                " copy")
{
        ag_mblock_auto *src = ag_mblock_new(sizeof(int));
        ag_mblock_auto *cp = ag_mblock_copy(src);
        ag_mblock_auto *cp2 = ag_mblock_copy_deep(cp);
        
        ag_test_assert (ag_mblock_refc(cp2) == 1);
}
ag_test_exit();

ag_test_init(copy_deep_05, "ag_mblock_copy_deep() does not change the reference"
                " count of the source")
{
        ag_mblock_auto *src = ag_mblock_new(sizeof(int));
        ag_mblock_auto *cp = ag_mblock_copy(src);
        ag_mblock_auto *cp2 = ag_mblock_copy_deep(cp);
        
        ag_test_assert (ag_mblock_refc(src) == 2 && ag_mblock_refc(cp) == 2);
}
ag_test_exit();

ag_test_init(copy_deep_06, "ag_mblock_copy_deep() preserves the data size of"
                " the source")
{
        ag_mblock_auto *src = ag_mblock_new(sizeof(int));
        ag_mblock_auto *cp = ag_mblock_copy_deep(src);

        ag_test_assert (ag_mblock_sz(src) == ag_mblock_sz(cp));
}
ag_test_exit();


ag_test_init(copy_deep_07, "ag_mblock_copy_deep() maintains the total size of"
                " the source")
{
        ag_mblock_auto *src = ag_mblock_new(sizeof(int));
        ag_mblock_auto *cp = ag_mblock_copy_deep(src);

        ag_test_assert (ag_mblock_sz(src) >= ag_mblock_sz(cp));
}
ag_test_exit();


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
                &copy_02,
                &copy_03,
                &copy_04,
                &copy_05,
                &copy_06,
                &copy_07,
                &copy_deep_01,
                &copy_deep_02,
                &copy_deep_03,
                &copy_deep_04,
                &copy_deep_05,
                &copy_deep_06,
                &copy_deep_07,
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
                copy_02_desc,
                copy_03_desc,
                copy_04_desc,
                copy_05_desc,
                copy_06_desc,
                copy_07_desc,
                copy_deep_01_desc,
                copy_deep_02_desc,
                copy_deep_03_desc,
                copy_deep_04_desc,
                copy_deep_05_desc,
                copy_deep_06_desc,
                copy_deep_07_desc,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_mblock interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

