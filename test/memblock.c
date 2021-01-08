#include "./test.h"

#include <string.h>

#define __AG_TEST_SUITE_ID__ 1

struct test {
        int     *i;
        int     *j;
};


struct test2 {
        int     i;
        int     j;
};




__AG_TEST_CASE("ag_memblock_new() allocates memory on the heap for an int")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new(sizeof(int));
        AG_TEST (m);
}


__AG_TEST_CASE("ag_memblock_new() allocates memory on the heap for a test struct")
{
        struct test *t = ag_memblock_new(sizeof *t);
        t->i = ag_memblock_new(sizeof *t->i);
        t->j = ag_memblock_new(sizeof *t->j);

        bool chk = (t && t->i && t->j);

        ag_memblock_release((ag_memblock **)&t->i);
        ag_memblock_release((ag_memblock **)&t->j);
        ag_memblock_release((ag_memblock **)&t);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_new() returns a block with a reference count of 1")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new(sizeof(int));
        AG_TEST (ag_memblock_refc(m) == 1);
}


__AG_TEST_CASE("ag_memblock_new() returns a block with the requested data size")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new(sizeof(int));
        AG_TEST (ag_memblock_sz(m) == sizeof(int));
}


__AG_TEST_CASE("ag_memblock_new() returns a block with a total size >="
    " requested data size")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new(sizeof(int));
        AG_TEST (ag_memblock_sz_total(m) >= sizeof(int));
}


__AG_TEST_CASE("ag_memblock_new_align() allocates memory on the heap for an int")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new_align(sizeof(int), 8);
        AG_TEST (m);
}


__AG_TEST_CASE("ag_memblock_new_align() allocates memory on the heap for a test"
    " struct")
{
        struct test *t = ag_memblock_new_align(sizeof *t, 8);
        t->i = ag_memblock_new_align(sizeof *t->i, 32);
        t->j = ag_memblock_new_align(sizeof *t->j, 32);

        bool chk = (t && t->i && t->j);

        ag_memblock_release((ag_memblock **)&t->i);
        ag_memblock_release((ag_memblock **)&t->j);
        ag_memblock_release((ag_memblock **)&t);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_new_align() returns a block with a reference count"
    " of 1")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new_align(sizeof(int), 8);
        AG_TEST (ag_memblock_refc(m) == 1);
}


__AG_TEST_CASE("ag_memblock_new_align() returns a block with the requested data"
    " size")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new_align(sizeof(int), 8);
        AG_TEST (ag_memblock_sz(m) == sizeof(int));
}


__AG_TEST_CASE("ag_memblock_new_align() returns a block with a total size >="
    " requested data size")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new_align(sizeof(int), 8);
        AG_TEST (ag_memblock_sz_total(m) >= sizeof(int));
}


__AG_TEST_CASE("ag_memblock_new_align() returns a block with the required"
    " alignment")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new_align(sizeof(int), 32);
        AG_TEST (ag_memblock_aligned(m, 32));
}


__AG_TEST_CASE("ag_memblock_clone() makes a deep copy of an int in the heap")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 555;

        int *j = ag_memblock_clone(i);
        bool chk = (*j == 555);

        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_clone() makes a deep copy of a test structure in"
    " the heap")
{
        struct test *t = ag_memblock_new(sizeof *t);
        t->i = ag_memblock_new(sizeof *t->i);
        t->j = ag_memblock_new(sizeof *t->j);

        *t->i = 555;
        *t->j = -666;

        struct test *cp = ag_memblock_clone(t);
        bool chk = (*cp->i == *t->i && *cp->j == *t->j);

        ag_memblock_release((ag_memblock **)&t->i);
        ag_memblock_release((ag_memblock **)&t->j);
        ag_memblock_release((ag_memblock **)&t);
        ag_memblock_release((ag_memblock **)&cp);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_clone() returns a copy with another address")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone(src);

        AG_TEST (src != cp);
}


__AG_TEST_CASE("ag_memblock_clone() sets the reference count of the copy to 1")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone(src);

        AG_TEST (ag_memblock_refc(cp) == 1);
}


__AG_TEST_CASE("ag_memblock_clone() does not change the reference count of the"
    " source")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone(src);

        AG_TEST (ag_memblock_refc(src) == 1);
}


__AG_TEST_CASE("ag_memblock_clone() preserves the data size of the source")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone(src);

        AG_TEST (ag_memblock_sz(src) == ag_memblock_sz(cp));
}


__AG_TEST_CASE("ag_memblock_clone() maintains the total size of the source")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone(src);

        AG_TEST (ag_memblock_sz(src) >= ag_memblock_sz(cp));
}


__AG_TEST_CASE("ag_memblock_clone_align() makes a deep copy of an int")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 555;

        int *j = ag_memblock_clone_align(i, 8);
        bool chk = (*j == 555);

        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_clone_align() makes a deep copy of a test structure")
{
        struct test *t = ag_memblock_new(sizeof *t);
        t->i = ag_memblock_new(sizeof *t->i);
        t->j = ag_memblock_new(sizeof *t->j);

        *t->i = 555;
        *t->j = -666;

        struct test *cp = ag_memblock_clone_align(t, 8);
        bool chk = (*cp->i == *t->i && *cp->j == *t->j);

        ag_memblock_release((ag_memblock **)&t->i);
        ag_memblock_release((ag_memblock **)&t->j);
        ag_memblock_release((ag_memblock **)&t);
        ag_memblock_release((ag_memblock **)&cp);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_clone_align() returns a copy with another address")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone_align(src, 8);

        AG_TEST (src != cp);
}


__AG_TEST_CASE("ag_memblock_clone_align() sets the reference count of the copy"
    " to 1")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_copy(src);
        AG_AUTO(ag_memblock) *cp2 = ag_memblock_clone_align(cp, 8);

        AG_TEST (ag_memblock_refc(cp2) == 1);
}


__AG_TEST_CASE("ag_memblock_clone_align() does not change the reference count"
    "of the source")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_copy(src);
        AG_AUTO(ag_memblock) *cp2 = ag_memblock_clone_align(cp, 8);

        AG_TEST (ag_memblock_refc(src) == 2 && ag_memblock_refc(cp) == 2);
}


__AG_TEST_CASE("ag_memblock_clone_align() preserves the data size of the source")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone_align(src, 8);

        AG_TEST (ag_memblock_sz(src) == ag_memblock_sz(cp));
}


__AG_TEST_CASE("ag_memblock_clone_align() maintains the total size of the source")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone_align(src, 8);

        AG_TEST (ag_memblock_sz(src) >= ag_memblock_sz(cp));
}


__AG_TEST_CASE("ag_memblock_clone_align() honours its alignment request")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone_align(src, 32);

        AG_TEST (ag_memblock_aligned(cp, 32));
}


__AG_TEST_CASE("ag_memblock_release() performs a no-op if passed NULL")
{
        ag_memblock_release(NULL);
        AG_TEST (true);
}


__AG_TEST_CASE("ag_memblock_release() performs a no-op if passed a handle to a"
    " null pointer")
{
        ag_memblock *m = NULL;
        ag_memblock_release((ag_memblock **) &m);

        AG_TEST (true);
}


__AG_TEST_CASE("ag_memblock_release() release an int on the heap")
{
        int *i = ag_memblock_new(sizeof *i);
        ag_memblock_release((ag_memblock **)&i);

        AG_TEST (!i);
}


__AG_TEST_CASE("ag_memblock_release() releases a test struct on the heap")
{
        struct test *t = ag_memblock_new(sizeof *t);
        ag_memblock_release((ag_memblock **)&t);

        AG_TEST (!t);
}


__AG_TEST_CASE("ag_memblock_release() reduces the reference count by 1 for lazy"
    " copies")
{
        int *i = ag_memblock_new(sizeof *i);
        AG_AUTO(ag_memblock) *j = ag_memblock_copy(i);
        ag_memblock_release((ag_memblock **)&i);

        AG_TEST (ag_memblock_refc(j) == 1);
}


__AG_TEST_CASE("ag_memblock_release() on a deep copy does not alter the"
    "reference count of the source")
{
        AG_AUTO(ag_memblock) *i = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *j = ag_memblock_copy(i);
        ag_memblock *k = ag_memblock_clone(j);
        ag_memblock_release(&k);

        AG_TEST (ag_memblock_refc(i) == 2);
}



__AG_TEST_CASE("ag_memblock_cmp() returns AG_CMP_EQ for two int memory blocks"
    "with the same value")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 555;
        int *j = ag_memblock_new(sizeof *j);
        *j = 555;

        bool chk = (ag_memblock_cmp(i, j) == AG_CMP_EQ);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_cmp() returns AG_CMP_EQ when comparing a shallow"
    " copy of an int with its source")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 555;
        int *j = ag_memblock_copy(i);

        bool chk = (ag_memblock_cmp(i, j) == AG_CMP_EQ);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_cmp() returns AG_CMP_EQ when comparing a deep copy"
    " of an int with its source")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 555;
        int *j = ag_memblock_clone(i);

        bool chk = (ag_memblock_cmp(i, j) == AG_CMP_EQ);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_cmp() returns AG_CMP_EQ for two memory blocks with"
    " the same struct with scalar fields")
{
        struct test2 *a = ag_memblock_new(sizeof *a);
        struct test2 *b = ag_memblock_new(sizeof *b);
        a->i = 555;
        a->j = -666;
        b->i = 555;
        b->j = -666;

        bool chk = (ag_memblock_cmp(a, b) == AG_CMP_EQ);
        ag_memblock_release((ag_memblock **)&a);
        ag_memblock_release((ag_memblock **)&b);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_cmp() returns AG_CMP_EQ when comparing a shallow"
    " copy of a struct with scalar fields")
{
        struct test2 *a = ag_memblock_new(sizeof *a);
        a->i = 555;
        a->j = -666;

        struct test2 *b = ag_memblock_copy(a);
        bool chk = (ag_memblock_cmp(a, b) == AG_CMP_EQ);

        ag_memblock_release((ag_memblock **)&a);
        ag_memblock_release((ag_memblock **)&b);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_cmp() returns AG_CMP_EQ when comparing a deep copy"
    " of a struct with scalar fields")
{
        struct test2 *a = ag_memblock_new(sizeof *a);
        a->i = 555;
        a->j = -666;
        struct test2 *b = ag_memblock_clone(a);

        bool chk = (ag_memblock_cmp(a, b) == AG_CMP_EQ);
        ag_memblock_release((ag_memblock **)&a);
        ag_memblock_release((ag_memblock **)&b);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_cmp() returns AG_CMP_LT when comparing an int"
    " memory block to another whose first differing byte is greater")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 1;
        int *j = ag_memblock_new(sizeof *j);
        *j = 2;

        bool chk = (ag_memblock_cmp(i, j) == AG_CMP_LT);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_cmp() returns AG_CMP_GT when comparing an int"
    " memory block to another whose first differing byte is lower")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 2;
        int *j = ag_memblock_new(sizeof *j);
        *j = 1;

        bool chk = (ag_memblock_cmp(i, j) == AG_CMP_GT);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_cmp() returns AG_CMP_LT when comparing a memory"
    " block holding a struct with scalar fields to another whose first"
    " differing byte is greater")
{
        struct test2 *a = ag_memblock_new(sizeof *a);
        a->i = 1;
        a->j = 2;

        struct test2 *b = ag_memblock_clone(a);
        b->i = 2;
        b->j = 2;

        bool chk = (ag_memblock_cmp(a, b) == AG_CMP_LT);
        ag_memblock_release((ag_memblock **)&a);
        ag_memblock_release((ag_memblock **)&b);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_cmp() returns AG_CMP_GT when comparing a memory"
    " block holding a struct with scalar fields to another whose first"
    " differing byte is lower")
{
        struct test2 *a = ag_memblock_new(sizeof *a);
        a->i = 2;
        a->j = 1;

        struct test2 *b = ag_memblock_clone(a);
        b->i = 1;
        b->j = 1;

        bool chk = ag_memblock_cmp(a, b) == AG_CMP_GT;
        ag_memblock_release((ag_memblock **)&a);
        ag_memblock_release((ag_memblock **)&b);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_lt() returns true for an int memory block with a"
    " value lower than another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 5;
        *j = 6;

        bool chk = ag_memblock_lt(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_lt() returns false for an int memory block with the"
    " same value as another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 5;
        *j = 5;

        bool chk = !ag_memblock_lt(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_lt() returns false for an int memory block with a"
    " greater value than another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 6;
        *j = 5;

        bool chk = !ag_memblock_lt(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_gt() returns true for an int memory block with a"
    " value greater than another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 6;
        *j = 5;

        bool chk = ag_memblock_gt(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE(
    "ag_memblock_gt() returns false for an int memory block with the same value"
    " as another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 5;
        *j = 5;

        bool chk = !ag_memblock_gt(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE(
    "ag_memblock_gt() returns false for an int memory block with a lower value"
    " than another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 5;
        *j = 6;

        bool chk = !ag_memblock_gt(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE(
    "ag_memblock_eq() returns true for two int memory blocks with the same"
    " value")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 5;
        *j = 5;

        bool chk = ag_memblock_eq(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE(
    "ag_memblock_eq() returns false for an int memory block with a lower value"
    " than another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 5;
        *j = 6;

        bool chk = !ag_memblock_eq(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE(
    "ag_memblock_eq() returns false for an int memory block with a greater"
    " value than another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 6;
        *j = 5;

        bool chk = !ag_memblock_eq(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


__AG_TEST_CASE("ag_memblock_resize() resizes an existing memory block")
{
        char *bfr = ag_memblock_new(10);
        ag_memblock_resize((ag_memblock **)&bfr, 15);

        bool chk = (ag_memblock_sz(bfr) == 15);
        ag_memblock_release((ag_memblock **)&bfr);

        AG_TEST (chk);
}


__AG_TEST_CASE(
    "ag_memblock_resize() preserves data when resizing to a larger size")
{
        char *bfr = ag_memblock_new(6);
        strncpy(bfr, "Hello", 6);
        bfr[5] = '\0';
        ag_memblock_resize((ag_memblock **)&bfr, 10);

        bool chk = !strcmp(bfr, "Hello");
        ag_memblock_release((ag_memblock **)&bfr);

        AG_TEST (chk);
}


__AG_TEST_CASE(
    "ag_memblock_resize_align() resizes an existing memory block")
{
        char *bfr = ag_memblock_new(10);
        ag_memblock_resize_align((ag_memblock **)&bfr, 15, 8);

        bool chk = (ag_memblock_sz(bfr) == 15);
        ag_memblock_release((ag_memblock **)&bfr);

        AG_TEST (chk);
}


__AG_TEST_CASE(
    "ag_memblock_resize_align() preserves data when resizing to a larger size")
{
        char *bfr = ag_memblock_new(6);
        strncpy(bfr, "Hello", 6);
        bfr[5] = '\0';
        ag_memblock_resize_align((ag_memblock **)&bfr, 10, 8);

        bool chk = !strcmp(bfr, "Hello");
        ag_memblock_release((ag_memblock **)&bfr);

        AG_TEST (chk);
}


__AG_TEST_CASE(
    "ag_memblock_resize_align() aligns to the requested boundary")
{
        char *bfr = ag_memblock_new(6);
        strncpy(bfr, "Hello", 6);
        bfr[5] = '\0';
        ag_memblock_resize_align((ag_memblock **)&bfr, 10, 32);

        bool chk = ag_memblock_aligned(bfr, 32);
        ag_memblock_release((ag_memblock **)&bfr);

        AG_TEST (chk);
}


__AG_TEST_CASE(
    "ag_memblock_str() generates the string representation of a memory block")
{
        int *i = ag_memblock_new(sizeof *i);
        AG_AUTO(ag_string) *s = ag_memblock_str(i);

        bool chk = s && *s;
        ag_memblock_release((ag_memblock **)&i);

        AG_TEST (chk);
}

#if 0
AG_TEST_CASE(new_01,
    "ag_memblock_new() allocates memory on the heap for an int")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new(sizeof(int));

        AG_TEST (m);
}


AG_TEST_CASE(new_02,
    "ag_memblock_new() allocates memory on the heap for a test struct")
{
        struct test *t = ag_memblock_new(sizeof *t);
        t->i = ag_memblock_new(sizeof *t->i);
        t->j = ag_memblock_new(sizeof *t->j);

        bool chk = (t && t->i && t->j);

        ag_memblock_release((ag_memblock **)&t->i);
        ag_memblock_release((ag_memblock **)&t->j);
        ag_memblock_release((ag_memblock **)&t);

        AG_TEST (chk);
}


AG_TEST_CASE(new_03,
    "ag_memblock_new() returns a block with a reference count of 1")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new(sizeof(int));

        AG_TEST (ag_memblock_refc(m) == 1);
}


AG_TEST_CASE(new_04,
    "ag_memblock_new() returns a block with the requested data size")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new(sizeof(int));

        AG_TEST (ag_memblock_sz(m) == sizeof(int));
}


AG_TEST_CASE(new_05,
    "ag_memblock_new() returns a block with a total size >= requested data"
    " size")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new(sizeof(int));

        AG_TEST (ag_memblock_sz_total(m) >= sizeof(int));
}


AG_TEST_CASE(new_align_01,
    "ag_memblock_new_align() allocates memory on the heap for an int")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new_align(sizeof(int), 8);

        AG_TEST (m);
}


AG_TEST_CASE(new_align_02,
    "ag_memblock_new_align() allocates memory on the heap for a test struct")
{
        struct test *t = ag_memblock_new_align(sizeof *t, 8);
        t->i = ag_memblock_new_align(sizeof *t->i, 32);
        t->j = ag_memblock_new_align(sizeof *t->j, 32);

        bool chk = (t && t->i && t->j);

        ag_memblock_release((ag_memblock **)&t->i);
        ag_memblock_release((ag_memblock **)&t->j);
        ag_memblock_release((ag_memblock **)&t);

        AG_TEST (chk);
}


AG_TEST_CASE(new_align_03,
    "ag_memblock_new_align() returns a block with a reference count of 1")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new_align(sizeof(int), 8);

        AG_TEST (ag_memblock_refc(m) == 1);
}


AG_TEST_CASE(new_align_04,
    "ag_memblock_new_align() returns a block with the requested data size")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new_align(sizeof(int), 8);

        AG_TEST (ag_memblock_sz(m) == sizeof(int));
}


AG_TEST_CASE(new_align_05,
    "ag_memblock_new_align() returns a block with a total size >= requested"
    " data size")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new_align(sizeof(int), 8);

        AG_TEST (ag_memblock_sz_total(m) >= sizeof(int));
}


AG_TEST_CASE(new_align_06,
    "ag_memblock_new_align() returns a block with the required alignment")
{
        AG_AUTO(ag_memblock) *m = ag_memblock_new_align(sizeof(int), 32);

        AG_TEST (ag_memblock_aligned(m, 32));
}


AG_TEST_CASE(clone_01,
    "ag_memblock_clone() makes a deep copy of an int in the heap")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 555;

        int *j = ag_memblock_clone(i);
        bool chk = (*j == 555);

        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}

AG_TEST_CASE(clone_02,
    "ag_memblock_clone() makes a deep copy of a test structure in the heap")
{
        struct test *t = ag_memblock_new(sizeof *t);
        t->i = ag_memblock_new(sizeof *t->i);
        t->j = ag_memblock_new(sizeof *t->j);

        *t->i = 555;
        *t->j = -666;

        struct test *cp = ag_memblock_clone(t);
        bool chk = (*cp->i == *t->i && *cp->j == *t->j);

        ag_memblock_release((ag_memblock **)&t->i);
        ag_memblock_release((ag_memblock **)&t->j);
        ag_memblock_release((ag_memblock **)&t);
        ag_memblock_release((ag_memblock **)&cp);

        AG_TEST (chk);
}


AG_TEST_CASE(clone_03,
    "ag_memblock_clone() returns a copy with another address")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone(src);

        AG_TEST (src != cp);
}


AG_TEST_CASE(clone_04,
    "ag_memblock_clone() sets the reference count of the copy to 1")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone(src);

        AG_TEST (ag_memblock_refc(cp) == 1);
}

AG_TEST_CASE(clone_05,
    "ag_memblock_clone() does not change the reference count of the source")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone(src);

        AG_TEST (ag_memblock_refc(src) == 1);
}


AG_TEST_CASE(clone_06,
    "ag_memblock_clone() preserves the data size of the source")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone(src);

        AG_TEST (ag_memblock_sz(src) == ag_memblock_sz(cp));
}


AG_TEST_CASE(clone_07,
    "ag_memblock_clone() maintains the total size of the source")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone(src);

        AG_TEST (ag_memblock_sz(src) >= ag_memblock_sz(cp));
}

AG_TEST_CASE(clone_align_01,
    "ag_memblock_clone_align() makes a deep copy of an int")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 555;

        int *j = ag_memblock_clone_align(i, 8);
        bool chk = (*j == 555);

        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(clone_align_02,
    "ag_memblock_clone_align() makes a deep copy of a test structure")
{
        struct test *t = ag_memblock_new(sizeof *t);
        t->i = ag_memblock_new(sizeof *t->i);
        t->j = ag_memblock_new(sizeof *t->j);

        *t->i = 555;
        *t->j = -666;

        struct test *cp = ag_memblock_clone_align(t, 8);
        bool chk = (*cp->i == *t->i && *cp->j == *t->j);

        ag_memblock_release((ag_memblock **)&t->i);
        ag_memblock_release((ag_memblock **)&t->j);
        ag_memblock_release((ag_memblock **)&t);
        ag_memblock_release((ag_memblock **)&cp);

        AG_TEST (chk);
}


AG_TEST_CASE(clone_align_03,
    "ag_memblock_clone_align() returns a copy with another address")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone_align(src, 8);

        AG_TEST (src != cp);
}


AG_TEST_CASE(clone_align_04,
    "ag_memblock_clone_align() sets the reference count of the copy to 1")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_copy(src);
        AG_AUTO(ag_memblock) *cp2 = ag_memblock_clone_align(cp, 8);

        AG_TEST (ag_memblock_refc(cp2) == 1);
}

AG_TEST_CASE(clone_align_05,
    "ag_memblock_clone_align() does not change the reference count of the"
    " source")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_copy(src);
        AG_AUTO(ag_memblock) *cp2 = ag_memblock_clone_align(cp, 8);

        AG_TEST (ag_memblock_refc(src) == 2 && ag_memblock_refc(cp) == 2);
}


AG_TEST_CASE(clone_align_06,
    "ag_memblock_clone_align() preserves the data size of the source")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone_align(src, 8);

        AG_TEST (ag_memblock_sz(src) == ag_memblock_sz(cp));
}


AG_TEST_CASE(clone_align_07,
    "ag_memblock_clone_align() maintains the total size of the source")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone_align(src, 8);

        AG_TEST (ag_memblock_sz(src) >= ag_memblock_sz(cp));
}


AG_TEST_CASE(clone_align_08,
    "ag_memblock_clone_align() honours its alignment request")
{
        AG_AUTO(ag_memblock) *src = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *cp = ag_memblock_clone_align(src, 32);

        AG_TEST (ag_memblock_aligned(cp, 32));
}


AG_TEST_CASE(release_01,
    "ag_memblock_release() performs a no-op if passed NULL")
{
        ag_memblock_release(NULL);

        AG_TEST (true);
}


AG_TEST_CASE(release_02, "ag_memblock_release() performs a no-op if passed a"
                " handle to a null pointer")
{
        ag_memblock *m = NULL;
        ag_memblock_release((ag_memblock **) &m);

        AG_TEST (true);
}


AG_TEST_CASE(release_03, "ag_memblock_release() release an int on the heap")
{
        int *i = ag_memblock_new(sizeof *i);
        ag_memblock_release((ag_memblock **)&i);

        AG_TEST (!i);
}


AG_TEST_CASE(release_04,
    "ag_memblock_release() releases a test struct on the heap")
{
        struct test *t = ag_memblock_new(sizeof *t);
        ag_memblock_release((ag_memblock **)&t);

        AG_TEST (!t);
}


AG_TEST_CASE(release_05,
    "ag_memblock_release() reduces the reference count by 1 for lazy copies")
{
        int *i = ag_memblock_new(sizeof *i);
        AG_AUTO(ag_memblock) *j = ag_memblock_copy(i);
        ag_memblock_release((ag_memblock **)&i);

        AG_TEST (ag_memblock_refc(j) == 1);
}


AG_TEST_CASE(release_06,
    "ag_memblock_release() on a deep copy does not alter the reference count of"
    " the source")
{
        AG_AUTO(ag_memblock) *i = ag_memblock_new(sizeof(int));
        AG_AUTO(ag_memblock) *j = ag_memblock_copy(i);
        ag_memblock *k = ag_memblock_clone(j);
        ag_memblock_release(&k);

        AG_TEST (ag_memblock_refc(i) == 2);
}



AG_TEST_CASE(cmp_01,
    "ag_memblock_cmp() returns AG_CMP_EQ for two int memory blocks with the"
    " same value")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 555;
        int *j = ag_memblock_new(sizeof *j);
        *j = 555;

        bool chk = (ag_memblock_cmp(i, j) == AG_CMP_EQ);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(cmp_02,
    "ag_memblock_cmp() returns AG_CMP_EQ when comparing a shallow copy of an"
    " int with its source")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 555;
        int *j = ag_memblock_copy(i);

        bool chk = (ag_memblock_cmp(i, j) == AG_CMP_EQ);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(cmp_03,
    "ag_memblock_cmp() returns AG_CMP_EQ when comparing a deep copy of an int"
    " with its source")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 555;
        int *j = ag_memblock_clone(i);

        bool chk = (ag_memblock_cmp(i, j) == AG_CMP_EQ);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(cmp_04,
    "ag_memblock_cmp() returns AG_CMP_EQ for two memory blocks with the same"
    " struct with scalar fields")
{
        struct test2 *a = ag_memblock_new(sizeof *a);
        struct test2 *b = ag_memblock_new(sizeof *b);
        a->i = 555;
        a->j = -666;
        b->i = 555;
        b->j = -666;

        bool chk = (ag_memblock_cmp(a, b) == AG_CMP_EQ);
        ag_memblock_release((ag_memblock **)&a);
        ag_memblock_release((ag_memblock **)&b);

        AG_TEST (chk);
}


AG_TEST_CASE(cmp_05,
    "ag_memblock_cmp() returns AG_CMP_EQ when comparing a shallow copy of a"
    " struct with scalar fields")
{
        struct test2 *a = ag_memblock_new(sizeof *a);
        a->i = 555;
        a->j = -666;

        struct test2 *b = ag_memblock_copy(a);
        bool chk = (ag_memblock_cmp(a, b) == AG_CMP_EQ);

        ag_memblock_release((ag_memblock **)&a);
        ag_memblock_release((ag_memblock **)&b);

        AG_TEST (chk);
}


AG_TEST_CASE(cmp_06,
    "ag_memblock_cmp() returns AG_CMP_EQ when comparing a deep copy of a struct"
    " with scalar fields")
{
        struct test2 *a = ag_memblock_new(sizeof *a);
        a->i = 555;
        a->j = -666;
        struct test2 *b = ag_memblock_clone(a);

        bool chk = (ag_memblock_cmp(a, b) == AG_CMP_EQ);
        ag_memblock_release((ag_memblock **)&a);
        ag_memblock_release((ag_memblock **)&b);

        AG_TEST (chk);
}


AG_TEST_CASE(cmp_07,
    "ag_memblock_cmp() returns AG_CMP_LT when comparing an int memory block to"
    " another whose first differing byte is greater")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 1;
        int *j = ag_memblock_new(sizeof *j);
        *j = 2;

        bool chk = (ag_memblock_cmp(i, j) == AG_CMP_LT);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(cmp_08,
    "ag_memblock_cmp() returns AG_CMP_GT when comparing an int memory block to"
    " another whose first differing byte is lower")
{
        int *i = ag_memblock_new(sizeof *i);
        *i = 2;
        int *j = ag_memblock_new(sizeof *j);
        *j = 1;

        bool chk = (ag_memblock_cmp(i, j) == AG_CMP_GT);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(cmp_09,
    "ag_memblock_cmp() returns AG_CMP_LT when comparing a memory block holding"
    " a struct with scalar fields to another whose first differing byte is"
    " greater")
{
        struct test2 *a = ag_memblock_new(sizeof *a);
        a->i = 1;
        a->j = 2;

        struct test2 *b = ag_memblock_clone(a);
        b->i = 2;
        b->j = 2;

        bool chk = (ag_memblock_cmp(a, b) == AG_CMP_LT);
        ag_memblock_release((ag_memblock **)&a);
        ag_memblock_release((ag_memblock **)&b);

        AG_TEST (chk);
}


AG_TEST_CASE(cmp_10,
    "ag_memblock_cmp() returns AG_CMP_GT when comparing a memory block holding"
    " a struct with scalar fields to another whose first differing byte is"
    " lower")
{
        struct test2 *a = ag_memblock_new(sizeof *a);
        a->i = 2;
        a->j = 1;

        struct test2 *b = ag_memblock_clone(a);
        b->i = 1;
        b->j = 1;

        bool chk = ag_memblock_cmp(a, b) == AG_CMP_GT;
        ag_memblock_release((ag_memblock **)&a);
        ag_memblock_release((ag_memblock **)&b);

        AG_TEST (chk);
}


AG_TEST_CASE(lt_01,
    "ag_memblock_lt() returns true for an int memory block with a value lower"
    " than another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 5;
        *j = 6;

        bool chk = ag_memblock_lt(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(lt_02,
    "ag_memblock_lt() returns false for an int memory block with the same value"
    " as another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 5;
        *j = 5;

        bool chk = !ag_memblock_lt(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(lt_03,
    "ag_memblock_lt() returns false for an int memory block with a greater"
    " value than another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 6;
        *j = 5;

        bool chk = !ag_memblock_lt(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(gt_01,
    "ag_memblock_gt() returns true for an int memory block with a value greater"
    " than another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 6;
        *j = 5;

        bool chk = ag_memblock_gt(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(gt_02,
    "ag_memblock_gt() returns false for an int memory block with the same value"
    " as another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 5;
        *j = 5;

        bool chk = !ag_memblock_gt(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(gt_03,
    "ag_memblock_gt() returns false for an int memory block with a lower value"
    " than another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 5;
        *j = 6;

        bool chk = !ag_memblock_gt(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(eq_01,
    "ag_memblock_eq() returns true for two int memory blocks with the same"
    " value")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 5;
        *j = 5;

        bool chk = ag_memblock_eq(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(eq_02,
    "ag_memblock_eq() returns false for an int memory block with a lower value"
    " than another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 5;
        *j = 6;

        bool chk = !ag_memblock_eq(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(eq_03,
    "ag_memblock_eq() returns false for an int memory block with a greater"
    " value than another")
{
        int *i = ag_memblock_new(sizeof *i);
        int *j = ag_memblock_new(sizeof *j);
        *i = 6;
        *j = 5;

        bool chk = !ag_memblock_eq(i, j);
        ag_memblock_release((ag_memblock **)&i);
        ag_memblock_release((ag_memblock **)&j);

        AG_TEST (chk);
}


AG_TEST_CASE(resize_01, "ag_memblock_resize() resizes an existing memory block")
{
        char *bfr = ag_memblock_new(10);
        ag_memblock_resize((ag_memblock **)&bfr, 15);

        bool chk = (ag_memblock_sz(bfr) == 15);
        ag_memblock_release((ag_memblock **)&bfr);

        AG_TEST (chk);
}


AG_TEST_CASE(resize_02,
    "ag_memblock_resize() preserves data when resizing to a larger size")
{
        char *bfr = ag_memblock_new(6);
        strncpy(bfr, "Hello", 6);
        bfr[5] = '\0';
        ag_memblock_resize((ag_memblock **)&bfr, 10);

        bool chk = !strcmp(bfr, "Hello");
        ag_memblock_release((ag_memblock **)&bfr);

        AG_TEST (chk);
}


AG_TEST_CASE(resize_align_01,
    "ag_memblock_resize_align() resizes an existing memory block")
{
        char *bfr = ag_memblock_new(10);
        ag_memblock_resize_align((ag_memblock **)&bfr, 15, 8);

        bool chk = (ag_memblock_sz(bfr) == 15);
        ag_memblock_release((ag_memblock **)&bfr);

        AG_TEST (chk);
}


AG_TEST_CASE(resize_align_02,
    "ag_memblock_resize_align() preserves data when resizing to a larger size")
{
        char *bfr = ag_memblock_new(6);
        strncpy(bfr, "Hello", 6);
        bfr[5] = '\0';
        ag_memblock_resize_align((ag_memblock **)&bfr, 10, 8);

        bool chk = !strcmp(bfr, "Hello");
        ag_memblock_release((ag_memblock **)&bfr);

        AG_TEST (chk);
}


AG_TEST_CASE(resize_align_03,
    "ag_memblock_resize_align() aligns to the requested boundary")
{
        char *bfr = ag_memblock_new(6);
        strncpy(bfr, "Hello", 6);
        bfr[5] = '\0';
        ag_memblock_resize_align((ag_memblock **)&bfr, 10, 32);

        bool chk = ag_memblock_aligned(bfr, 32);
        ag_memblock_release((ag_memblock **)&bfr);

        AG_TEST (chk);
}


AG_TEST_CASE(str_01,
    "ag_memblock_str() generates the string representation of a memory block")
{
        int *i = ag_memblock_new(sizeof *i);
        AG_AUTO(ag_string) *s = ag_memblock_str(i);

        bool chk = s && *s;
        ag_memblock_release((ag_memblock **)&i);

        AG_TEST (chk);
}
#endif



extern ag_test_suite *test_suite_memblock(void)
{
        /*ag_test *test[] = {
                new_01,                 new_02,                 new_03,
                new_04,                 new_05,
                new_align_01,           new_align_02,           new_align_03,
                new_align_04,           new_align_05,           new_align_06,
                clone_01,               clone_02,               clone_03,
                clone_04,               clone_05,               clone_06,
                clone_07,
                clone_align_01,         clone_align_02,         clone_align_03,
                clone_align_04,         clone_align_05,         clone_align_06,
                clone_align_07,         clone_align_08,
                release_01,             release_02,             release_03,
                release_04,             release_05,             release_06,
                cmp_01,                 cmp_02,                 cmp_03,
                cmp_04,                 cmp_05,                 cmp_06,
                cmp_07,                 cmp_08,                 cmp_09,
                cmp_10,
                lt_01,                  lt_02,                  lt_03,
                gt_01,                  gt_02,                  gt_03,
                eq_01,                  eq_02,                  eq_03,
                resize_01,              resize_02,
                resize_align_01,        resize_align_02,        resize_align_03,
                str_01, };

        const char *desc[] = { 
                new_01_desc,                    new_02_desc, 
                new_03_desc,                    new_04_desc, 
                new_05_desc, 
                new_align_01_desc,              new_align_02_desc,
                new_align_03_desc,              new_align_04_desc, 
                new_align_05_desc,              new_align_06_desc, 
                clone_01_desc,                  clone_02_desc, 
                clone_03_desc,                  clone_04_desc, 
                clone_05_desc,                  clone_06_desc, 
                clone_07_desc,                  clone_align_01_desc, 
                clone_align_02_desc,            clone_align_03_desc,
                clone_align_04_desc,            clone_align_05_desc, 
                clone_align_06_desc,            clone_align_07_desc, 
                clone_align_08_desc,            release_01_desc,
                release_02_desc,                release_03_desc, 
                release_04_desc,                release_05_desc, 
                release_06_desc, 
                cmp_01_desc,            cmp_02_desc,
                cmp_03_desc,            cmp_04_desc, 
                cmp_05_desc,            cmp_06_desc, 
                cmp_07_desc,            cmp_08_desc, 
                cmp_09_desc,            cmp_10_desc, 
                lt_01_desc,             lt_02_desc,
                lt_03_desc, 
                gt_01_desc,             gt_02_desc, 
                gt_03_desc, 
                eq_01_desc,             eq_02_desc, eq_03_desc, 
                resize_01_desc,         resize_02_desc,
                resize_align_01_desc,   resize_align_02_desc,
                resize_align_03_desc, 
                str_01_desc, 
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_memblock interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;*/

        return AG_TEST_SUITE_GENERATE("ag_memblock interface");        
}

