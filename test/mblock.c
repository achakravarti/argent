#include "./test.h"

#include <string.h>

struct test {
        int *i;
        int *j;
};

struct test2 {
        int i;
        int j;
};


AG_TEST_INIT(new_01, "ag_mblock_new() allocates memory on the heap for"
                " an int")
{
        AG_AUTO(ag_mblock) *m = ag_mblock_new(sizeof(int));
        AG_TEST_ASSERT (m);
}
AG_TEST_EXIT();


AG_TEST_INIT(new_02, "ag_mblock_new() allocates memory on the heap for"
                " a test struct")
{
        struct test *t = ag_mblock_new(sizeof *t);
        t->i = ag_mblock_new(sizeof *t->i);
        t->j = ag_mblock_new(sizeof *t->j);

        AG_TEST_ASSERT (t && t->i && t->j);
        
        ag_mblock_release((ag_mblock **)&t->i);
        ag_mblock_release((ag_mblock **)&t->j);
        ag_mblock_release((ag_mblock **)&t);

}
AG_TEST_EXIT();


AG_TEST_INIT(new_03, "ag_mblock_new() returns a block with a reference count"
                " of 1")
{
        AG_AUTO(ag_mblock) *m = ag_mblock_new(sizeof(int));
        AG_TEST_ASSERT (ag_mblock_refc(m) == 1);
}
AG_TEST_EXIT();


AG_TEST_INIT(new_04, "ag_mblock_new() returns a block with the requested data"
                " size")
{
        AG_AUTO(ag_mblock) *m = ag_mblock_new(sizeof(int));
        AG_TEST_ASSERT (ag_mblock_sz(m) == sizeof(int));
}
AG_TEST_EXIT();


AG_TEST_INIT(new_05, "ag_mblock_new() returns a block with a total size >="
                " requested data size")
{
        AG_AUTO(ag_mblock) *m = ag_mblock_new(sizeof(int));
        AG_TEST_ASSERT (ag_mblock_sz_total(m) >= sizeof(int));
}
AG_TEST_EXIT();


AG_TEST_INIT(new_align_01, "ag_mblock_new_align() allocates memory on the heap"
                " for an int")
{
        AG_AUTO(ag_mblock) *m = ag_mblock_new_align(sizeof(int), 8);
        AG_TEST_ASSERT (m);
}
AG_TEST_EXIT();


AG_TEST_INIT(new_align_02, "ag_mblock_new_align() allocates memory on the heap"
                " for a test struct")
{
        struct test *t = ag_mblock_new_align(sizeof *t, 8);
        t->i = ag_mblock_new_align(sizeof *t->i, 32);
        t->j = ag_mblock_new_align(sizeof *t->j, 32);

        AG_TEST_ASSERT (t && t->i && t->j);
        
        ag_mblock_release((ag_mblock **)&t->i);
        ag_mblock_release((ag_mblock **)&t->j);
        ag_mblock_release((ag_mblock **)&t);
}
AG_TEST_EXIT();


AG_TEST_INIT(new_align_03, "ag_mblock_new_align() returns a block with a"
                " reference count of 1")
{
        AG_AUTO(ag_mblock) *m = ag_mblock_new_align(sizeof(int), 8);
        AG_TEST_ASSERT (ag_mblock_refc(m) == 1);
}
AG_TEST_EXIT();


AG_TEST_INIT(new_align_04, "ag_mblock_new_align() returns a block with the"
                " requested data size")
{
        AG_AUTO(ag_mblock) *m = ag_mblock_new_align(sizeof(int), 8);
        AG_TEST_ASSERT (ag_mblock_sz(m) == sizeof(int));
}
AG_TEST_EXIT();


AG_TEST_INIT(new_align_05, "ag_mblock_new_align() returns a block with a total"
                " size >= requested data size")
{
        AG_AUTO(ag_mblock) *m = ag_mblock_new_align(sizeof(int), 8);
        AG_TEST_ASSERT (ag_mblock_sz_total(m) >= sizeof(int));
}
AG_TEST_EXIT();


AG_TEST_INIT(new_align_06, "ag_mblock_new_align() returns a block with the"
                " required alignment")
{
        AG_AUTO(ag_mblock) *m = ag_mblock_new_align(sizeof(int), 32);
        AG_TEST_ASSERT (ag_mblock_aligned(m, 32));
}
AG_TEST_EXIT();


AG_TEST_INIT(copy_01, "ag_mblock_copy() makes a deep copy of an int in the"
                " heap")
{
        int *i = ag_mblock_new(sizeof *i);
        *i = 555;

        int *j = ag_mblock_copy(i);
        AG_TEST_ASSERT (*j == 555);

        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();

AG_TEST_INIT(copy_02, "ag_mblock_copy() makes a deep copy of a test structure"
                " in the heap")
{
        struct test *t = ag_mblock_new(sizeof *t);
        t->i = ag_mblock_new(sizeof *t->i);
        t->j = ag_mblock_new(sizeof *t->j);

        *t->i = 555;
        *t->j = -666;

        struct test *cp = ag_mblock_copy(t);
        AG_TEST_ASSERT (*cp->i == *t->i && *cp->j == *t->j);

        ag_mblock_release((ag_mblock **)&t->i);
        ag_mblock_release((ag_mblock **)&t->j);
        ag_mblock_release((ag_mblock **)&t);
        ag_mblock_release((ag_mblock **)&cp);
}
AG_TEST_EXIT();


AG_TEST_INIT(copy_03, "ag_mblock_copy() returns a copy with another address")
{
        AG_AUTO(ag_mblock) *src = ag_mblock_new(sizeof(int));
        AG_AUTO(ag_mblock) *cp = ag_mblock_copy(src);
        AG_TEST_ASSERT (src != cp);
}
AG_TEST_EXIT();


AG_TEST_INIT(copy_04, "ag_mblock_copy() sets the reference count of the copy"
                " to 1")
{
        AG_AUTO(ag_mblock) *src = ag_mblock_new(sizeof(int));
        AG_AUTO(ag_mblock) *cp = ag_mblock_copy(src);
        
        AG_TEST_ASSERT (ag_mblock_refc(cp) == 1);
}
AG_TEST_EXIT();

AG_TEST_INIT(copy_05, "ag_mblock_copy() does not change the reference count of"
                " the source")
{
        AG_AUTO(ag_mblock) *src = ag_mblock_new(sizeof(int));
        AG_AUTO(ag_mblock) *cp = ag_mblock_copy(src);
        
        AG_TEST_ASSERT (ag_mblock_refc(src) == 1);
}
AG_TEST_EXIT();


AG_TEST_INIT(copy_06, "ag_mblock_copy() preserves the data size of the source")
{
        AG_AUTO(ag_mblock) *src = ag_mblock_new(sizeof(int));
        AG_AUTO(ag_mblock) *cp = ag_mblock_copy(src);

        AG_TEST_ASSERT (ag_mblock_sz(src) == ag_mblock_sz(cp));
}
AG_TEST_EXIT();


AG_TEST_INIT(copy_07, "ag_mblock_copy() maintains the total size of the source")
{
        AG_AUTO(ag_mblock) *src = ag_mblock_new(sizeof(int));
        AG_AUTO(ag_mblock) *cp = ag_mblock_copy(src);

        AG_TEST_ASSERT (ag_mblock_sz(src) >= ag_mblock_sz(cp));
}
AG_TEST_EXIT();

AG_TEST_INIT(copy_align_01, "ag_mblock_copy_align() makes a deep copy of an int"
                " in the heap")
{
        int *i = ag_mblock_new(sizeof *i);
        *i = 555;

        int *j = ag_mblock_copy_align(i, 8);
        AG_TEST_ASSERT (*j == 555);

        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(copy_align_02, "ag_mblock_copy_align() makes a deep copy of a test"
                " structure in the heap")
{
        struct test *t = ag_mblock_new(sizeof *t);
        t->i = ag_mblock_new(sizeof *t->i);
        t->j = ag_mblock_new(sizeof *t->j);

        *t->i = 555;
        *t->j = -666;

        struct test *cp = ag_mblock_copy_align(t, 8);
        AG_TEST_ASSERT (*cp->i == *t->i && *cp->j == *t->j);

        ag_mblock_release((ag_mblock **)&t->i);
        ag_mblock_release((ag_mblock **)&t->j);
        ag_mblock_release((ag_mblock **)&t);
        ag_mblock_release((ag_mblock **)&cp);
}
AG_TEST_EXIT();


AG_TEST_INIT(copy_align_03, "ag_mblock_copy_align() returns a copy with another"
                " address")
{
        AG_AUTO(ag_mblock) *src = ag_mblock_new(sizeof(int));
        AG_AUTO(ag_mblock) *cp = ag_mblock_copy_align(src, 8);
        AG_TEST_ASSERT (src != cp);
}
AG_TEST_EXIT();


AG_TEST_INIT(copy_align_04, "ag_mblock_copy_align() sets the reference count of"
                " the copy to 1")
{
        AG_AUTO(ag_mblock) *src = ag_mblock_new(sizeof(int));
        ag_mblock_retain(src);
        AG_AUTO(ag_mblock) *cp = src;
        AG_AUTO(ag_mblock) *cp2 = ag_mblock_copy_align(cp, 8);
        
        AG_TEST_ASSERT (ag_mblock_refc(cp2) == 1);
}
AG_TEST_EXIT();

AG_TEST_INIT(copy_align_05, "ag_mblock_copy_align() does not change the"
                " reference count of the source")
{
        AG_AUTO(ag_mblock) *src = ag_mblock_new(sizeof(int));
        ag_mblock_retain(src);
        AG_AUTO(ag_mblock) *cp = src;
        AG_AUTO(ag_mblock) *cp2 = ag_mblock_copy_align(cp, 8);
        
        AG_TEST_ASSERT (ag_mblock_refc(src) == 2 && ag_mblock_refc(cp) == 2);
}
AG_TEST_EXIT();


AG_TEST_INIT(copy_align_06, "ag_mblock_copy_align() preserves the data size of"
                " the source")
{
        AG_AUTO(ag_mblock) *src = ag_mblock_new(sizeof(int));
        AG_AUTO(ag_mblock) *cp = ag_mblock_copy_align(src, 8);

        AG_TEST_ASSERT (ag_mblock_sz(src) == ag_mblock_sz(cp));
}
AG_TEST_EXIT();


AG_TEST_INIT(copy_align_07, "ag_mblock_copy_align() maintains the total size of"
                " the source")
{
        AG_AUTO(ag_mblock) *src = ag_mblock_new(sizeof(int));
        AG_AUTO(ag_mblock) *cp = ag_mblock_copy_align(src, 8);

        AG_TEST_ASSERT (ag_mblock_sz(src) >= ag_mblock_sz(cp));
}
AG_TEST_EXIT();


AG_TEST_INIT(copy_align_08, "ag_mblock_copy_align() honours its alignment"
                " request")
{
        AG_AUTO(ag_mblock) *src = ag_mblock_new(sizeof(int));
        AG_AUTO(ag_mblock) *cp = ag_mblock_copy_align(src, 32);

        AG_TEST_ASSERT (ag_mblock_aligned(cp, 32));
}
AG_TEST_EXIT();


AG_TEST_INIT(release_01, "ag_mblock_release() performs a no-op if passed NULL")
{
        ag_mblock_release(NULL);
        AG_TEST_ASSERT (true);
}
AG_TEST_EXIT();


AG_TEST_INIT(release_02, "ag_mblock_release() performs a no-op if passed a"
                " handle to a null pointer")
{
        ag_mblock *m = NULL;
        ag_mblock_release((ag_mblock **) &m);
        AG_TEST_ASSERT (true);
}
AG_TEST_EXIT();


AG_TEST_INIT(release_03, "ag_mblock_release() release an int on the heap")
{
        int *i = ag_mblock_new(sizeof *i);
        ag_mblock_release((ag_mblock **)&i);
        AG_TEST_ASSERT (!i);
}
AG_TEST_EXIT();


AG_TEST_INIT(release_04, "ag_mblock_release() releases a test struct on the"
                " heap")
{
        struct test *t = ag_mblock_new(sizeof *t);
        ag_mblock_release((ag_mblock **)&t);
        AG_TEST_ASSERT (!t);
}
AG_TEST_EXIT();


AG_TEST_INIT(release_05, "ag_mblock_release() reduces the reference count by 1"
                "for lazy copies")
{
        int *i = ag_mblock_new(sizeof *i);
        ag_mblock_retain(i);
        AG_AUTO(ag_mblock) *j = i;
        ag_mblock_release((ag_mblock **)&i);

        AG_TEST_ASSERT (ag_mblock_refc(j) == 1);
}
AG_TEST_EXIT();


AG_TEST_INIT(release_06, "ag_mblock_release() on a deep copy does not alter the"
                " reference count of the source")
{
        AG_AUTO(ag_mblock) *i = ag_mblock_new(sizeof(int));
        ag_mblock_retain(i);
        AG_AUTO(ag_mblock) *j = i;
        ag_mblock *k = ag_mblock_copy(j);
        ag_mblock_release(&k);
        
        AG_TEST_ASSERT (ag_mblock_refc(i) == 2);
}
AG_TEST_EXIT();



AG_TEST_INIT(cmp_01, "ag_mblock_cmp() returns AG_CMP_EQ for two int memory"
                " blocks with the same value")
{
        int *i = ag_mblock_new(sizeof *i);
        *i = 555;
        int *j = ag_mblock_new(sizeof *j);
        *j = 555;

        AG_TEST_ASSERT(ag_mblock_cmp(i, j) == AG_CMP_EQ);

        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(cmp_02, "ag_mblock_cmp() returns AG_CMP_EQ when comparing a"
                " shallow copy of an int with its source")
{
        int *i = ag_mblock_new(sizeof *i);
        *i = 555;
        ag_mblock_retain(i);
        int *j = i;

        AG_TEST_ASSERT(ag_mblock_cmp(i, j) == AG_CMP_EQ);

        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(cmp_03, "ag_mblock_cmp() returns AG_CMP_EQ when comparing a deep"
                " copy of an int with its source")
{
        int *i = ag_mblock_new(sizeof *i);
        *i = 555;
        int *j = ag_mblock_copy(i);

        AG_TEST_ASSERT(ag_mblock_cmp(i, j) == AG_CMP_EQ);

        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(cmp_04, "ag_mblock_cmp() returns AG_CMP_EQ for two memory blocks"
                " with the same struct with scalar fields")
{
        struct test2 *a = ag_mblock_new(sizeof *a);
        struct test2 *b = ag_mblock_new(sizeof *b);
        a->i = 555;
        a->j = -666;
        b->i = 555;
        b->j = -666;

        AG_TEST_ASSERT(ag_mblock_cmp(a, b) == AG_CMP_EQ);

        ag_mblock_release((ag_mblock **)&a);
        ag_mblock_release((ag_mblock **)&b);
}
AG_TEST_EXIT();


AG_TEST_INIT(cmp_05, "ag_mblock_cmp() returns AG_CMP_EQ when comparing a"
                " shallow copy of a struct with scalar fields")
{
        struct test2 *a = ag_mblock_new(sizeof *a);
        a->i = 555;
        a->j = -666;

        ag_mblock_retain(a);
        struct test2 *b = a;

        AG_TEST_ASSERT(ag_mblock_cmp(a, b) == AG_CMP_EQ);

        ag_mblock_release((ag_mblock **)&a);
        ag_mblock_release((ag_mblock **)&b);
}
AG_TEST_EXIT();


AG_TEST_INIT(cmp_06, "ag_mblock_cmp() returns AG_CMP_EQ when comparing a deep"
                " copy of a struct with scalar fields")
{
        struct test2 *a = ag_mblock_new(sizeof *a);
        a->i = 555;
        a->j = -666;
        struct test2 *b = ag_mblock_copy(a);

        AG_TEST_ASSERT(ag_mblock_cmp(a, b) == AG_CMP_EQ);

        ag_mblock_release((ag_mblock **)&a);
        ag_mblock_release((ag_mblock **)&b);
}
AG_TEST_EXIT();


AG_TEST_INIT(cmp_07, "ag_mblock_cmp() returns AG_CMP_LT when comparing an int"
                " memory block to another whose first differing byte is"
                " greater")
{
        int *i = ag_mblock_new(sizeof *i);
        *i = 1;
        int *j = ag_mblock_new(sizeof *j);
        *j = 2;

        AG_TEST_ASSERT(ag_mblock_cmp(i, j) == AG_CMP_LT);

        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(cmp_08, "ag_mblock_cmp() returns AG_CMP_GT when comparing an int"
                " memory block to another whose first differing byte is lower")
{
        int *i = ag_mblock_new(sizeof *i);
        *i = 2;
        int *j = ag_mblock_new(sizeof *j);
        *j = 1;

        AG_TEST_ASSERT(ag_mblock_cmp(i, j) == AG_CMP_GT);

        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(cmp_09, "ag_mblock_cmp() returns AG_CMP_LT when comparing a memory"
                " block holding a struct with scalar fields to another whose"
                " first differing byte is greater")
{
        struct test2 *a = ag_mblock_new(sizeof *a);
        a->i = 1;
        a->j = 2;
        //struct test2 *b = ag_mblock_copy_deep(a);
        struct test2 *b = ag_mblock_copy(a);
        b->i = 2;
        b->j = 2;

        AG_TEST_ASSERT(ag_mblock_cmp(a, b) == AG_CMP_LT);

        ag_mblock_release((ag_mblock **)&a);
        ag_mblock_release((ag_mblock **)&b);
}
AG_TEST_EXIT();


AG_TEST_INIT(cmp_10, "ag_mblock_cmp() returns AG_CMP_GT when comparing a memory"
                " block holding a struct with scalar fields to another whose"
                " first differing byte is lower")
{
        struct test2 *a = ag_mblock_new(sizeof *a);
        a->i = 2;
        a->j = 1;
        //struct test2 *b = ag_mblock_copy_deep(a);
        struct test2 *b = ag_mblock_copy(a);
        b->i = 1;
        b->j = 1;

        AG_TEST_ASSERT(ag_mblock_cmp(a, b) == AG_CMP_GT);

        ag_mblock_release((ag_mblock **)&a);
        ag_mblock_release((ag_mblock **)&b);
}
AG_TEST_EXIT();


AG_TEST_INIT(lt_01, "ag_mblock_lt() returns true for an int memory block with"
                " a value lower than another")
{
        int *i = ag_mblock_new(sizeof *i);
        int *j = ag_mblock_new(sizeof *j);
        *i = 5;
        *j = 6;

        AG_TEST_ASSERT (ag_mblock_lt(i, j));

        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(lt_02, "ag_mblock_lt() returns false for an int memory block with"
                " the same value as another")
{
        int *i = ag_mblock_new(sizeof *i);
        int *j = ag_mblock_new(sizeof *j);
        *i = 5;
        *j = 5;

        AG_TEST_ASSERT (!ag_mblock_lt(i, j));

        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(lt_03, "ag_mblock_lt() returns false for an int memory block with"
                " a greater value than another")
{
        int *i = ag_mblock_new(sizeof *i);
        int *j = ag_mblock_new(sizeof *j);
        *i = 6;
        *j = 5;

        AG_TEST_ASSERT (!ag_mblock_lt(i, j));
        
        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(gt_01, "ag_mblock_gt() returns true for an int memory block with"
                " a value greater than another")
{
        int *i = ag_mblock_new(sizeof *i);
        int *j = ag_mblock_new(sizeof *j);
        *i = 6;
        *j = 5;

        AG_TEST_ASSERT (ag_mblock_gt(i, j));

        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(gt_02, "ag_mblock_gt() returns false for an int memory block with"
                " the same value as another")
{
        int *i = ag_mblock_new(sizeof *i);
        int *j = ag_mblock_new(sizeof *j);
        *i = 5;
        *j = 5;

        AG_TEST_ASSERT (!ag_mblock_gt(i, j));

        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(gt_03, "ag_mblock_gt() returns false for an int memory block with"
                " a lower value than another")
{
        int *i = ag_mblock_new(sizeof *i);
        int *j = ag_mblock_new(sizeof *j);
        *i = 5;
        *j = 6;

        AG_TEST_ASSERT (!ag_mblock_gt(i, j));
        
        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(eq_01, "ag_mblock_eq() returns true for two int memory blocks with"
                " the same value")
{
        int *i = ag_mblock_new(sizeof *i);
        int *j = ag_mblock_new(sizeof *j);
        *i = 5;
        *j = 5;

        AG_TEST_ASSERT (ag_mblock_eq(i, j));

        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(eq_02, "ag_mblock_eq() returns false for an int memory block with"
                " a lower value than another")
{
        int *i = ag_mblock_new(sizeof *i);
        int *j = ag_mblock_new(sizeof *j);
        *i = 5;
        *j = 6;

        AG_TEST_ASSERT (!ag_mblock_eq(i, j));

        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();


AG_TEST_INIT(eq_03, "ag_mblock_eq() returns false for an int memory block with"
                " a greater value than another")
{
        int *i = ag_mblock_new(sizeof *i);
        int *j = ag_mblock_new(sizeof *j);
        *i = 6;
        *j = 5;

        AG_TEST_ASSERT (!ag_mblock_eq(i, j));
        
        ag_mblock_release((ag_mblock **)&i);
        ag_mblock_release((ag_mblock **)&j);
}
AG_TEST_EXIT();

AG_TEST_INIT(resize_01, "ag_mblock_resize() resizes an existing memory block")
{
        char *bfr = ag_mblock_new(10);
        ag_mblock_resize((ag_mblock **)&bfr, 15);

        AG_TEST_ASSERT (ag_mblock_sz(bfr) == 15);

        ag_mblock_release((ag_mblock **)&bfr);
}
AG_TEST_EXIT();


AG_TEST_INIT(resize_02, "ag_mblock_resize() preserves data when resizing to a"
                " larger size")
{
        char *bfr = ag_mblock_new(6);
        strncpy(bfr, "Hello", 6);
        bfr[5] = '\0';
        ag_mblock_resize((ag_mblock **)&bfr, 10);

        AG_TEST_ASSERT (!strcmp(bfr, "Hello"));

        ag_mblock_release((ag_mblock **)&bfr);
}
AG_TEST_EXIT();


AG_TEST_INIT(resize_align_01, "ag_mblock_resize_align() resizes an existing"
                " memory block")
{
        char *bfr = ag_mblock_new(10);
        ag_mblock_resize_align((ag_mblock **)&bfr, 15, 8);

        AG_TEST_ASSERT (ag_mblock_sz(bfr) == 15);

        ag_mblock_release((ag_mblock **)&bfr);
}
AG_TEST_EXIT();


AG_TEST_INIT(resize_align_02, "ag_mblock_resize_align() preserves data when"
                " resizing to a larger size")
{
        char *bfr = ag_mblock_new(6);
        strncpy(bfr, "Hello", 6);
        bfr[5] = '\0';
        ag_mblock_resize_align((ag_mblock **)&bfr, 10, 8);

        AG_TEST_ASSERT (!strcmp(bfr, "Hello"));

        ag_mblock_release((ag_mblock **)&bfr);
}
AG_TEST_EXIT();


AG_TEST_INIT(resize_align_03, "ag_mblock_resize_align() aligns to the requested"
                " boundary")
{
        char *bfr = ag_mblock_new(6);
        strncpy(bfr, "Hello", 6);
        bfr[5] = '\0';
        ag_mblock_resize_align((ag_mblock **)&bfr, 10, 32);

        AG_TEST_ASSERT (ag_mblock_aligned(bfr, 32));

        ag_mblock_release((ag_mblock **)&bfr);
}
AG_TEST_EXIT();


AG_TEST_INIT(str_01, "ag_mblock_str() generates the string representation of a"
                " memory block")
{
        int *i = ag_mblock_new(sizeof *i);
        AG_AUTO(ag_str) *s = ag_mblock_str(i);

        AG_TEST_ASSERT (s && *s);

        ag_mblock_release((ag_mblock **)&i);
}
AG_TEST_EXIT();




extern ag_test_suite *ag_test_suite_mblock(void)
{
        ag_test *test[] = {
                &new_01, &new_02, &new_03, &new_04, &new_05, &new_align_01,
                &new_align_02, &new_align_03, &new_align_04, &new_align_05,
                &new_align_06, &copy_01, &copy_02, &copy_03, &copy_04, &copy_05,
                &copy_06, &copy_07, &copy_align_01, &copy_align_02,
                &copy_align_03, &copy_align_04, &copy_align_05, &copy_align_06,
                &copy_align_07, &copy_align_08, &release_01, &release_02,
                &release_03, &release_04, release_05, &release_06, &cmp_01,
                &cmp_02, &cmp_03, &cmp_04, cmp_05, &cmp_06, &cmp_07, &cmp_08,
                &cmp_09, &cmp_10, &lt_01, &lt_02, &lt_03, &gt_01, &gt_02,
                &gt_03, &eq_01, &eq_02, &eq_03, &resize_01, &resize_02,
                &resize_align_01, &resize_align_02, &resize_align_03, &str_01,
        };

        const char *desc[] = {
                new_01_desc, new_02_desc, new_03_desc, new_04_desc, new_05_desc,
                new_align_01_desc, new_align_02_desc, new_align_03_desc,
                new_align_04_desc, new_align_05_desc, new_align_06_desc,
                copy_01_desc, copy_02_desc, copy_03_desc, copy_04_desc,
                copy_05_desc, copy_06_desc, copy_07_desc, copy_align_01_desc,
                copy_align_02_desc, copy_align_03_desc, copy_align_04_desc,
                copy_align_05_desc, copy_align_06_desc, copy_align_07_desc,
                copy_align_08_desc, release_01_desc, release_02_desc,
                release_03_desc, release_04_desc, release_05_desc,
                release_06_desc, cmp_01_desc, cmp_02_desc, cmp_03_desc,
                cmp_04_desc, cmp_05_desc, cmp_06_desc, cmp_07_desc, cmp_08_desc,
                cmp_09_desc, cmp_10_desc, lt_01_desc, lt_02_desc, lt_03_desc,
                gt_01_desc, gt_02_desc, gt_03_desc, eq_01_desc, eq_02_desc,
                eq_03_desc, resize_01_desc, resize_02_desc,
                resize_align_01_desc, resize_align_02_desc,
                resize_align_03_desc, str_01_desc,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_mblock interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

