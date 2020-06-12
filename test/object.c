#include "../src/api.h"
#include "./test.h"


struct payload {
    int x;
    int y;
};


#define TEST_OBJECT 101
#define TEST_OBJECT_NOMETHODS 102

static void test_setup(void)
{
    struct ag_object_method m = {
        .copy = NULL,
        .dispose = NULL,
        .sz = NULL,
        .len = NULL,
        .hash = NULL,
        .cmp = NULL,
        .str = NULL
    };

    ag_object_vtable_init(32);
    ag_object_register(TEST_OBJECT_NOMETHODS, &m);
}


static inline void test_teardown(void)
{
    ag_object_vtable_exit();
}


static ag_object_t *sample_object_nomethod(void)
{
    struct payload *p = ag_memblock_new(sizeof *p);
    p->x = 555;
    p->y = 666;

    return ag_object_new_noid(TEST_OBJECT_NOMETHODS, p);
}


static void test_new(void)
{
    printf("ag_object_new() creates an object with a payload");

    ag_object_smart_t *obj = sample_object_nomethod();
    ag_require (obj, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


static void test_dispose(void)
{
    printf("ag_object_dispose() releases an object");
    
    ag_object_smart_t *obj = sample_object_nomethod();
    ag_require (obj, AG_ERNO_TEST, NULL);

    ag_object_dispose(&obj);
    ag_require (!obj, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


extern void ag_test_object(void)
{
    printf("===============================================================\n");
    printf("Starting object model interface test suite...\n\n");

    test_setup();
    test_new();
    test_dispose();

    printf("\n");
    test_teardown();
}
