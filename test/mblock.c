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


extern ag_test_suite *ag_test_suite_mblock(void)
{
        ag_test *test[] = {
                &new_01,
                &new_02,
                &new_03,
                &new_04,
                &new_05,
        };

        const char *desc[] = {
                NEW_01,
                NEW_02,
                NEW_03,
                NEW_04,
                NEW_05,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_mblock interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

