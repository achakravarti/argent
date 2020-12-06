#include "./test.h"


struct test {
        int *i;
        int *j;
};


#define NEW_01 "ag_mblock_new() allocates memory on the heap for an int"
static enum ag_test_status new_01(void)
{
        int *i = ag_mblock_new(sizeof *i);
        bool ok = i;
        
        ag_mblock_free((ag_mblock **)&i);
        return ag_test_assert (ok);
}


#define NEW_02 "ag_mblock_new() allocates memory on the heap for a test struct"
static enum ag_test_status new_02(void)
{
        struct test *t = ag_mblock_new(sizeof *t);
        t->i = ag_mblock_new(sizeof *t->i);
        t->j = ag_mblock_new(sizeof *t->j);

        bool ok = t && t->i && t->j;
        
        ag_mblock_free((ag_mblock **)&t->i);
        ag_mblock_free((ag_mblock **)&t->j);
        ag_mblock_free((ag_mblock **)&t);

        return ag_test_assert (ok);
}


#define NEW_03 "ag_mblock_new() returns a block with a reference count of 1"
static enum ag_test_status new_03(void)
{
        int *i = ag_mblock_new(sizeof *i);
        bool ok = ag_mblock_refc((ag_mblock *)i) == 1;
        
        ag_mblock_free((ag_mblock **)&i);
        return ag_test_assert (ok);
}


#define NEW_04 "ag_mblock_new() returns a block with the requested data size"
static enum ag_test_status new_04(void)
{
        int *i = ag_mblock_new(sizeof *i);
        bool ok = ag_mblock_sz((ag_mblock *)i) == sizeof(int);
        
        ag_mblock_free((ag_mblock **)&i);
        return ag_test_assert (ok);
}


#define NEW_05 "ag_mblock_new() returns a block with a total size >=" \
                " requested data size"
static enum ag_test_status new_05(void)
{
        int *i = ag_mblock_new(sizeof *i);
        bool ok = ag_mblock_sz_total((ag_mblock *)i) >= sizeof(int);
        
        ag_mblock_free((ag_mblock **)&i);
        return ag_test_assert (ok);
}


#define NEW_ALIGN_01 "ag_mblock_new_align() allocates memory on the heap for" \
                " an int"
static enum ag_test_status new_align_01(void)
{
        int *i = ag_mblock_new_align(sizeof *i, 8);
        bool ok = i;
        
        ag_mblock_free((ag_mblock **)&i);
        return ag_test_assert (ok);
}


#define NEW_ALIGN_02 "ag_mblock_new_align() allocates memory on the heap" \
                " for a test struct"
static enum ag_test_status new_align_02(void)
{
        struct test *t = ag_mblock_new_align(sizeof *t, 8);
        t->i = ag_mblock_new_align(sizeof *t->i, 32);
        t->j = ag_mblock_new_align(sizeof *t->j, 32);

        bool ok = t && t->i && t->j;
        
        ag_mblock_free((ag_mblock **)&t->i);
        ag_mblock_free((ag_mblock **)&t->j);
        ag_mblock_free((ag_mblock **)&t);

        return ag_test_assert (ok);
}


#define NEW_ALIGN_03 "ag_mblock_new_align() returns a block with a reference" \
                " count of 1"
static enum ag_test_status new_align_03(void)
{
        int *i = ag_mblock_new_align(sizeof *i, 8);
        bool ok = ag_mblock_refc((ag_mblock *)i) == 1;
        
        ag_mblock_free((ag_mblock **)&i);
        return ag_test_assert (ok);
}


#define NEW_ALIGN_04 "ag_mblock_new_align() returns a block with the" \
                " requested data size"
static enum ag_test_status new_align_04(void)
{
        int *i = ag_mblock_new_align(sizeof *i, 8);
        bool ok = ag_mblock_sz((ag_mblock *)i) == sizeof(int);
        
        ag_mblock_free((ag_mblock **)&i);
        return ag_test_assert (ok);
}


#define NEW_ALIGN_05 "ag_mblock_new_align() returns a block with a total" \
                " size >= requested data size"
static enum ag_test_status new_align_05(void)
{
        int *i = ag_mblock_new_align(sizeof *i, 8);
        bool ok = ag_mblock_sz_total((ag_mblock *)i) >= sizeof(int);
        
        ag_mblock_free((ag_mblock **)&i);
        return ag_test_assert (ok);
}


#define NEW_ALIGN_06 "ag_mblock_new_align() returns a block with the required" \
                " alignment"
static enum ag_test_status new_align_06(void)
{
        int *i = ag_mblock_new_align(sizeof *i, 32);
        bool ok = ag_mblock_aligned((ag_mblock *)i, 32);
        
        ag_mblock_free((ag_mblock **)&i);
        return ag_test_assert (ok);
}





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
        };

        const char *desc[] = {
                NEW_01,
                NEW_02,
                NEW_03,
                NEW_04,
                NEW_05,
                NEW_ALIGN_01,
                NEW_ALIGN_02,
                NEW_ALIGN_03,
                NEW_ALIGN_04,
                NEW_ALIGN_05,
                NEW_ALIGN_06,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_mblock interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

