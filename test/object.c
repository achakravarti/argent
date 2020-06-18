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
    
    ag_object_t *obj = sample_object_nomethod();
    ag_require (obj, AG_ERNO_TEST, NULL);

    ag_object_dispose(&obj);
    ag_require (!obj, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


static void test_type(void)
{
    printf("ag_object_type() gets the type of an object");
    
    ag_object_smart_t *obj = sample_object_nomethod();
    size_t t = ag_object_type(obj);

    ag_require (t == TEST_OBJECT_NOMETHODS, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


static void test_id(void)
{
    printf("ag_object_id() defaults to 0 for an object with no ID");
    
    ag_object_smart_t *obj = sample_object_nomethod();
    size_t id = ag_object_id(obj);

    ag_require (!id, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


static void test_hash(void)
{
    printf("ag_object_hash() defaults to 0 for an object with no ID");
    
    ag_object_smart_t *obj = sample_object_nomethod();
    size_t h = ag_object_hash(obj);

    ag_require (!h, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


static void test_sz(void)
{
    printf("ag_object_sz() defaults to 0");
    
    ag_object_smart_t *obj = sample_object_nomethod();
    size_t sz = ag_object_sz(obj);

    ag_require (!sz, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


static void test_empty(void)
{
    printf("ag_object_empty() defaults to true");
    
    ag_object_smart_t *obj = sample_object_nomethod();
    bool e = ag_object_empty(obj);

    ag_require (e, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


static void test_len(void)
{
    printf("ag_object_len() defaults to 0");
    
    ag_object_smart_t *obj = sample_object_nomethod();
    size_t l = ag_object_sz(obj);

    ag_require (l == 0, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


static void test_cmp(void)
{
    printf("ag_object_cmp() returns AG_OBJECT_CMP_EQ for the same default"
            "  object");

    ag_object_smart_t *o1 = sample_object_nomethod();
    ag_object_smart_t *o2 = sample_object_nomethod();

    ag_require (ag_object_cmp(o1, o2) == AG_OBJECT_CMP_EQ, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


static void test_lt(void)
{
    printf("ag_object_lt() returns false for the same default object");

    ag_object_smart_t *o1 = sample_object_nomethod();
    ag_object_smart_t *o2 = sample_object_nomethod();

    ag_require (!ag_object_lt(o1, o2), AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


static void test_eq(void)
{
    printf("ag_object_eq() returns true for the same default object");

    ag_object_smart_t *o1 = sample_object_nomethod();
    ag_object_smart_t *o2 = sample_object_nomethod();

    ag_require (ag_object_eq(o1, o2), AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


static void test_gt(void)
{
    printf("ag_object_gt() returns false for the same default object");

    ag_object_smart_t *o1 = sample_object_nomethod();
    ag_object_smart_t *o2 = sample_object_nomethod();

    ag_require (!ag_object_gt(o1, o2), AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


static void test_payload(void)
{
    printf("ag_object_payload() gets a handle to the payload");

    ag_object_smart_t *obj = sample_object_nomethod();
    ag_require (obj, AG_ERNO_TEST, NULL);

    const struct payload *p = ag_object_payload(obj);
    ag_require (p->x == 555, AG_ERNO_TEST, NULL);
    ag_require (p->y == 666, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


static void test_payload_mutable(void)
{
    printf("ag_object_payload_mutable() gets a mutable handle to the payload");

    ag_object_smart_t *obj = sample_object_nomethod();
    ag_require (obj, AG_ERNO_TEST, NULL);

    struct payload *p = ag_object_payload_mutable(&obj);
    p->x = 666;
    p->y = 555;
    
    ag_require (p->x == 666, AG_ERNO_TEST, NULL);
    ag_require (p->y == 555, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


static void test_str(void)
{
    printf("ag_object_str() returns a string for a default object");

    ag_object_smart_t *o = sample_object_nomethod();
    const char *s = ag_object_str(o);

    ag_require (s && *s, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


extern void ag_test_object(void)
{
    printf("===============================================================\n");
    printf("Starting object model interface test suite...\n\n");

    test_setup();
    test_new();
    test_dispose();
    test_type();
    test_id();
    test_hash();
    test_sz();
    test_len();
    test_empty();
    test_cmp();
    test_lt();
    test_eq();
    test_gt();
    test_payload();
    test_payload_mutable();
    test_str();

    printf("\n");
    test_teardown();
}
