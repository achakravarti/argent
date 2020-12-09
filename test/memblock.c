#if 0
#include "../src/api.h"
#include "./test.h"


static void test_new(void)
{
    printf("ag_memblock_new() dynamically allocates a heap memory block");

    int *bfr = ag_memblock_new(sizeof *bfr);
    *bfr = 555;

    ag_require (*bfr == 555, AG_ERNO_TEST, NULL);
    ag_memblock_free((ag_memblock_t **) &bfr);

    printf("...OK\n");
}


static void test_copy(void)
{
    printf("ag_memblock_copy() copies an existing heap memory block");
    
    int *bfr = ag_memblock_new(sizeof *bfr);
    *bfr = 555;

    int *cp = ag_memblock_copy(bfr);
    ag_require (*cp == 555, AG_ERNO_TEST, NULL);

    ag_memblock_free((ag_memblock_t **) &bfr);
    ag_memblock_free((ag_memblock_t **) &cp);

    printf("...OK\n");
}


static void test_resize(void)
{
    printf("ag_memblock_resize() resizes an existing heap memory block");

    int *bfr = ag_memblock_new(sizeof *bfr);
    *bfr = 555;
    ag_require (*bfr == 555, AG_ERNO_TEST, NULL);

    ag_memblock_resize((ag_memblock_t **) &bfr, sizeof *bfr * 2);
    ag_require (*bfr == 555, AG_ERNO_TEST, NULL);

    bfr[1] = 666;
    ag_require (bfr[1] == 666, AG_ERNO_TEST, NULL);
    ag_memblock_free((ag_memblock_t **) &bfr);
    
    printf("...OK\n");
}


static void test_sz(void)
{
    printf("ag_memblock_sz() gets the size of an existing heap memory block");

    int *bfr = ag_memblock_new(sizeof *bfr);
    ag_require (ag_memblock_sz(bfr) >= sizeof *bfr, AG_ERNO_TEST, NULL);

    ag_memblock_free((ag_memblock_t **) &bfr);
    printf("...OK\n");
}


static void test_free(void)
{
    printf("ag_memblock_free() releases an existing heap memory block");

    int *bfr = ag_memblock_new(sizeof *bfr);
    ag_require (bfr, AG_ERNO_TEST, NULL);

    ag_memblock_free((ag_memblock_t **) &bfr);
    ag_require (!bfr, AG_ERNO_TEST, NULL);
    
    printf("...OK\n");
}


extern void ag_test_memblock(void)
{
    printf("===============================================================\n");
    printf("Starting memory block interface test suite...\n\n");

    test_new();
    test_copy();
    test_resize();
    test_sz();
    test_free();

    printf("\n");
}
#endif
