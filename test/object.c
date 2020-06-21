#include "../src/api.h"
#include "./test.h"
#include <string.h>




/*******************************************************************************
 *                             DERIVED OBJECT TESTS 
 */


                                      /* payload for derived object [AgDM:??] */
struct derived_payload {
    int *x;
    int *y;
};


                                       /* type ID of derived object [AgDM:??] */
#define DERIVED_OBJECT 101


                      /* method to deep copy derived object payload [AgDM:??] */
static ag_memblock_t *derived_method_copy(const ag_memblock_t *payload)
{
    const struct derived_payload *p = payload;
    struct derived_payload *cp = ag_memblock_new(sizeof *p);

    cp->x = ag_memblock_new(sizeof *cp->x);
    cp->y = ag_memblock_new(sizeof *cp->y);

    *cp->x = *p->x;
    *cp->y = *p->y;

    return cp;
}


                                  /* method to dispose derived object payload */
static inline void derived_method_dispose(ag_memblock_t *payload)
{
    struct derived_payload *p = (struct derived_payload *) payload;
    ag_memblock_free((void **) &p->x);
    ag_memblock_free((void **) &p->y);
}


                              /* method to get ID of derived object [AgDM:??] */
static inline size_t derived_method_id(const ag_object_t *obj)
{
    const struct derived_payload *p = ag_object_payload(obj);
    return *p->x + *p->y;
}


                            /* method to get size of derived object [AgDM:??] */
static inline size_t derived_method_sz(const ag_object_t *obj)
{
    const struct derived_payload *p = ag_object_payload(obj);
    return sizeof (p->x) + sizeof (p->y);
}


                          /* method to get length of derived object [AgDM:??] */
static inline size_t derived_method_len(const ag_object_t *obj)
{
    const struct derived_payload *p = ag_object_payload(obj);
    return (*p->x + *p->y) / 2;
}


                            /* method to get hash of derived object [AgDM:??] */
static inline size_t derived_method_hash(const ag_object_t *obj)
{
    const struct derived_payload *p = ag_object_payload(obj);
    return (*p->x + *p->y) % 2;
}


                            /* method to compare two derived object [AgDM:??] */
static inline enum ag_object_cmp derived_method_cmp(const ag_object_t *lhs, 
        const ag_object_t *rhs)
{
    size_t lid = ag_object_id(lhs);
    size_t rid = ag_object_id(rhs);

    if (lid == rid)
        return AG_OBJECT_CMP_EQ;

    return lid < rid ? AG_OBJECT_CMP_LT : AG_OBJECT_CMP_GT;
}


           /* method to get string representation of derived object [AgDM:??] */
static inline const char *derived_method_str(const ag_object_t *obj)
{
    (void) obj;
    return "derived";
}


                                   /* creates sample derived object [AgDM:??] */
static ag_object_t *derived_sample(void)
{
    struct derived_payload *p = ag_memblock_new(sizeof *p);

    p->x = ag_memblock_new(sizeof *p->x);
    p->y = ag_memblock_new(sizeof *p->y);

    *p->x = 555;
    *p->y = 666; 

   return ag_object_new(DERIVED_OBJECT, p);
}


                                 /* test case #1 for derived object [AgDM:??] */
static void derived_test_new(void)
{    
    printf("ag_object_new() creates a derived object with a payload");

    ag_object_smart_t *obj = derived_sample();
    ag_require (obj, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                 /* test case #2 for derived object [AgDM:??] */
static void derived_test_dispose(void)
{
    printf("ag_object_dispose() releases a derived object");
    
    ag_object_t *obj = derived_sample();
    ag_require (obj, AG_ERNO_TEST, NULL);

    ag_object_dispose(&obj);
    ag_require (!obj, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                 /* test case #3 for derived object [AgDM:??] */
static void derived_test_type(void)
{
    printf("ag_object_type() gets the type of a derived object");
    
    ag_object_smart_t *obj = derived_sample();
    size_t t = ag_object_type(obj);

    ag_require (t == DERIVED_OBJECT, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


                                 /* test case #4 for derived object [AgDM:??] */
static void derived_test_id(void)
{
    printf("ag_object_id() gets the ID of a derived object");
    
    ag_object_smart_t *obj = derived_sample();
    ag_require (ag_object_id(obj) == derived_method_id(obj), 
            AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                 /* test case #5 for derived object [AgDM:??] */
static void derived_test_hash(void)
{
    printf("ag_object_hash() gets the hash of a derived object");
    
    ag_object_smart_t *obj = derived_sample();
    ag_require (ag_object_hash(obj) == derived_method_hash(obj), 
            AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                 /* test case #6 for derived object [AgDM:??] */
static void derived_test_sz(void)
{
    printf("ag_object_sz() gets the size of a derived object");
    
    ag_object_smart_t *obj = derived_sample();
    ag_require (ag_object_sz(obj) == derived_method_sz(obj), 
            AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                 /* test case #7 for derived object [AgDM:??] */
static void derived_test_len(void)
{
    printf("ag_object_len() gets the length of a derived object");
    
    ag_object_smart_t *obj = derived_sample();
    ag_require (ag_object_len(obj) == derived_method_len(obj), 
            AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                 /* test case #8 for derived object [AgDM:??] */
static void derived_test_empty(void)
{
    printf("ag_object_empty() returns false for a non-empty derived object");
    
    ag_object_smart_t *obj = derived_sample();
    ag_require (!ag_object_empty(obj), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                 /* test case #9 for derived object [AgDM:??] */
static void derived_test_cmp(void)
{
    printf("ag_object_cmp() returns AG_OBJECT_CMP_EQ for the same derived"
            " object");

    ag_object_smart_t *o1 = derived_sample();
    ag_object_smart_t *o2 = derived_sample();

    ag_require (ag_object_cmp(o1, o2) == AG_OBJECT_CMP_EQ, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


                                /* test case #10 for derived object [AgDM:??] */
static void derived_test_lt(void)
{
    printf("ag_object_lt() returns false for the same derived object");

    ag_object_smart_t *o1 = derived_sample();
    ag_object_smart_t *o2 = derived_sample();

    ag_require (!ag_object_lt(o1, o2), AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


                                /* test case #11 for derived object [AgDM:??] */
static void derived_test_eq(void)
{
    printf("ag_object_eq() returns true for the same derived object");

    ag_object_smart_t *o1 = derived_sample();
    ag_object_smart_t *o2 = derived_sample();

    ag_require (ag_object_eq(o1, o2), AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


                                /* test case #12 for derived object [AgDM:??] */
static void derived_test_gt(void)
{
    printf("ag_object_gt() returns false for the same derived object");

    ag_object_smart_t *o1 = derived_sample();
    ag_object_smart_t *o2 = derived_sample();

    ag_require (!ag_object_gt(o1, o2), AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


                                /* test case #13 for derived object [AgDM:??] */
static void derived_test_payload(void)
{
    printf("ag_object_payload() gets the payload of a derived object");

    ag_object_smart_t *obj = derived_sample();
    ag_require (obj, AG_ERNO_TEST, NULL);

    const struct derived_payload *p = ag_object_payload(obj);
    ag_require (*p->x == 555, AG_ERNO_TEST, NULL);
    ag_require (*p->y == 666, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                /* test case #14 for derived object [AgDM:??] */
static void derived_test_payload_mutable(void)
{
    printf("ag_object_payload_mutable() gets a mutable handle to the payload"
            " of a derived object");

    ag_object_smart_t *obj = derived_sample();
    ag_require (obj, AG_ERNO_TEST, NULL);

    struct derived_payload *p = ag_object_payload_mutable(&obj);
    *p->x = 666;
    *p->y = 555;
    
    ag_require (*p->x == 666, AG_ERNO_TEST, NULL);
    ag_require (*p->y == 555, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                /* test case #15 for derived object [AgDM:??] */
static void derived_test_str(void)
{
    printf("ag_object_str() returns the string representation of a derived"
            " object");

    ag_object_smart_t *o = derived_sample();
    ag_require (!strcmp("derived", ag_object_str(o)), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}







struct payload {
    int x;
    int y;
};


#define TEST_OBJECT 101
#define TEST_OBJECT_NOMETHODS 102


static void test_setup(void)
{
    struct ag_object_vtable vt = {
        .copy = NULL,
        .dispose = NULL,
        .id = NULL,
        .sz = NULL,
        .len = NULL,
        .hash = NULL,
        .cmp = NULL,
        .str = NULL
    };

    struct ag_object_vtable dvt = {
        .copy = &derived_method_copy,
        .dispose = &derived_method_dispose,
        .id = &derived_method_id,
        .sz = &derived_method_sz,
        .len = &derived_method_len,
        .hash = &derived_method_hash,
        .cmp = &derived_method_cmp,
        .str = &derived_method_str
    };

    ag_object_init(32);
    ag_object_register(TEST_OBJECT_NOMETHODS, &vt);
    ag_object_register(DERIVED_OBJECT, &dvt);
}


static inline void test_teardown(void)
{
    ag_object_exit();
}


static ag_object_t *sample_object_nomethod(void)
{
    struct payload *p = ag_memblock_new(sizeof *p);
    p->x = 555;
    p->y = 666;

    return ag_object_new(TEST_OBJECT_NOMETHODS, p);
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
            " object");

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

    derived_test_new();
    derived_test_dispose();
    derived_test_type();
    derived_test_id();
    derived_test_hash();
    derived_test_sz();
    derived_test_len();
    derived_test_empty();
    derived_test_cmp();
    derived_test_lt();
    derived_test_eq();
    derived_test_gt();
    derived_test_payload();
    derived_test_payload_mutable();
    derived_test_str();

    printf("\n");
    test_teardown();
}
