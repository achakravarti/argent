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


extern void ag_test_object(void)
{
    printf("===============================================================\n");
    printf("Starting object model interface test suite...\n\n");


    test_setup();
    printf("\n");
    test_teardown();
}
