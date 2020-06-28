#include "../src/api.h"
#include "./test.h"
#include <string.h>




/*******************************************************************************
 *                              BASE OBJECT TESTS
 */


                                         /* payload for base object [AgDM:??] */
struct base_payload {
    int x;
    int y;
};


                                          /* type ID of base object [AgDM:??] */
#define BASE_OBJECT 102


                                      /* creates sample base object [AgDM:??] */
static ag_object_t *base_sample(void)
{
    struct base_payload *p = ag_memblock_new(sizeof *p);
    p->x = 555;
    p->y = 666;

    return ag_object_new(BASE_OBJECT, p);
}


                                    /* test case #1 for base object [AgDM:??] */
static void base_test_new(void)
{
    printf("ag_object_new() creates a base object");

    ag_object_smart_t *obj = base_sample();
    ag_require (obj, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                    /* test case #2 for base object [AgDM:??] */
static void base_test_dispose(void)
{
    printf("ag_object_dispose() releases a base object");
    
    ag_object_t *obj = base_sample();
    ag_require (obj, AG_ERNO_TEST, NULL);

    ag_object_dispose(&obj);
    ag_require (!obj, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                    /* test case #3 for base object [AgDM:??] */
static void base_test_type(void)
{
    printf("ag_object_type() gets the type of a base object");
    
    ag_object_smart_t *obj = base_sample();
    size_t t = ag_object_type(obj);

    ag_require (t == BASE_OBJECT, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


                                    /* test case #4 for base object [AgDM:??] */
static void base_test_id(void)
{
    printf("ag_object_id() defaults to 0 for a base object");
    
    ag_object_smart_t *obj = base_sample();
    size_t id = ag_object_id(obj);

    ag_require (!id, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


                                    /* test case #5 for base object [AgDM:??] */
static void base_test_hash(void)
{
    printf("ag_object_hash() defaults to 0 for a base object");
    
    ag_object_smart_t *obj = base_sample();
    size_t h = ag_object_hash(obj);

    ag_require (!h, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


                                    /* test case #6 for base object [AgDM:??] */
static void base_test_sz(void)
{
    printf("ag_object_sz() defaults to 0 for a base object");
    
    ag_object_smart_t *o = base_sample();
    ag_require (ag_object_sz(o) == ag_memblock_sz(ag_object_payload(o)), 
            AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                    /* test case #7 for base object [AgDM:??] */
static void base_test_empty(void)
{
    printf("ag_object_empty() defaults to false for a base object");
    
    ag_object_smart_t *o = base_sample();
    ag_require (!ag_object_empty(o), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                    /* test case #8 for base object [AgDM:??] */
static void base_test_len(void)
{
    printf("ag_object_len() defaults to 1 for a base object");
    
    ag_object_smart_t *o = base_sample();
    ag_require (ag_object_len(o) == 1, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                    /* test case #9 for base object [AgDM:??] */
static void base_test_cmp(void)
{
    printf("ag_object_cmp() returns AG_TRISTATE_GND for the same base object");

    ag_object_smart_t *o1 = base_sample();
    ag_object_smart_t *o2 = base_sample();

    ag_require (ag_object_cmp(o1, o2) == AG_TRISTATE_GND, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


                                   /* test case #10 for base object [AgDM:??] */
static void base_test_lt(void)
{
    printf("ag_object_lt() returns false for the same base object");

    ag_object_smart_t *o1 = base_sample();
    ag_object_smart_t *o2 = base_sample();

    ag_require (!ag_object_lt(o1, o2), AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


                                   /* test case #11 for base object [AgDM:??] */
static void base_test_eq(void)
{
    printf("ag_object_eq() returns true for the same base object");

    ag_object_smart_t *o1 = base_sample();
    ag_object_smart_t *o2 = base_sample();

    ag_require (ag_object_eq(o1, o2), AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


                                   /* test case #12 for base object [AgDM:??] */
static void base_test_gt(void)
{
    printf("ag_object_gt() returns false for the same base object");

    ag_object_smart_t *o1 = base_sample();
    ag_object_smart_t *o2 = base_sample();

    ag_require (!ag_object_gt(o1, o2), AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


                                   /* test case #13 for base object [AgDM:??] */
static void base_test_payload(void)
{
    printf("ag_object_payload() gets the payload of a base object");

    ag_object_smart_t *obj = base_sample();
    ag_require (obj, AG_ERNO_TEST, NULL);

    const struct base_payload *p = ag_object_payload(obj);
    ag_require (p->x == 555, AG_ERNO_TEST, NULL);
    ag_require (p->y == 666, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                   /* test case #14 for base object [AgDM:??] */
static void base_test_payload_mutable(void)
{
    printf("ag_object_payload_mutable() gets a mutable handle to the payload"
            " of a base object");

    ag_object_smart_t *obj = base_sample();
    ag_require (obj, AG_ERNO_TEST, NULL);

    struct base_payload *p = ag_object_payload_mutable(&obj);
    p->x = 666;
    p->y = 555;
    
    ag_require (p->x == 666, AG_ERNO_TEST, NULL);
    ag_require (p->y == 555, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                   /* test case #15 for base object [AgDM:??] */
static void base_test_str(void)
{
    printf("ag_object_str() returns a string for a base object");

    ag_object_smart_t *o = base_sample();
    const char *s = ag_object_str(o);

    ag_require (s && *s, AG_ERNO_TEST, NULL);
    printf("...OK\n");
}


                                   /* test case #16 for base object [AgDM:??] */
static void base_test_refc(void)
{
    printf("ag_object_refc() returns 1 when a base object is created");
    
    ag_object_smart_t *o = base_sample();
    ag_require (ag_object_refc(o) == 1, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                   /* test case #17 for base object [AgDM:??] */
static void base_test_refc_2(void)
{
    printf("ag_object_refc() accounts for a base object being copied");
    
    ag_object_smart_t *o = base_sample();
    ag_object_smart_t *cp1 = ag_object_copy(o);
    ag_object_smart_t *cp2 = ag_object_copy(cp1);
    ag_require (ag_object_refc(o) == 3, AG_ERNO_TEST, NULL);
    ag_require (ag_object_refc(cp1) == 3, AG_ERNO_TEST, NULL);
    ag_require (ag_object_refc(cp2) == 3, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                   /* test case #18 for base object [AgDM:??] */
static void base_test_refc_3(void)
{
    printf("ag_object_refc() accounts for a base object being disposed");
    
    ag_object_t *o = base_sample();
    ag_object_smart_t *cp1 = ag_object_copy(o);
    ag_object_t *cp2 = ag_object_copy(cp1);

    ag_object_dispose(&o);
    ag_object_dispose(&cp2);
    ag_require (ag_object_refc(cp1) == 1, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


static void base_test_refc_4(void)
{
    printf("ag_object_refc() accounts for mutable handles to base object"
            " payloads");
    
    ag_object_t *o = base_sample();
    ag_object_t *cp1 = ag_object_copy(o);
    ag_object_t *cp2 = ag_object_copy(cp1);

    struct base_payload *p = ag_object_payload_mutable(&cp1);
    (void) p;

    ag_require (ag_object_refc(o) == 2, AG_ERNO_TEST, NULL);
    ag_require (ag_object_refc(cp1) == 1, AG_ERNO_TEST, NULL);
    ag_require (ag_object_refc(cp2) == 2, AG_ERNO_TEST, NULL);

    ag_object_dispose(&cp1);
    ag_object_dispose(&cp2);
    ag_require (ag_object_refc(o) == 1, AG_ERNO_TEST, NULL);
    ag_object_dispose(&o);

    printf("...OK\n");
}




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
static inline enum ag_tristate derived_method_cmp(const ag_object_t *lhs, 
        const ag_object_t *rhs)
{
    size_t lid = ag_object_id(lhs);
    size_t rid = ag_object_id(rhs);

    if (lid == rid)
        return AG_TRISTATE_GND;

    return lid < rid ? AG_TRISTATE_LO : AG_TRISTATE_HI;
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
    printf("ag_object_cmp() returns AG_TRISTATE_GND for the same derived"
            " object");

    ag_object_smart_t *o1 = derived_sample();
    ag_object_smart_t *o2 = derived_sample();

    ag_require (ag_object_cmp(o1, o2) == AG_TRISTATE_GND, AG_ERNO_TEST, NULL);
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


                                /* test case #16 for derived object [AgDM:??] */
static void derived_test_refc(void)
{
    printf("ag_object_refc() returns 1 when a derived object is created");
    
    ag_object_smart_t *o = derived_sample();
    ag_require (ag_object_refc(o) == 1, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                /* test case #17 for derived object [AgDM:??] */
static void derived_test_refc_2(void)
{
    printf("ag_object_refc() accounts for a derived object being copied");
    
    ag_object_smart_t *o = derived_sample();
    ag_object_smart_t *cp1 = ag_object_copy(o);
    ag_object_smart_t *cp2 = ag_object_copy(cp1);
    ag_require (ag_object_refc(o) == 3, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


                                /* test case #18 for derived object [AgDM:??] */
static void derived_test_refc_3(void)
{
    printf("ag_object_refc() accounts for a derived object being disposed");
    
    ag_object_t *o = derived_sample();
    ag_object_smart_t *cp1 = ag_object_copy(o);
    ag_object_t *cp2 = ag_object_copy(cp1);

    ag_object_dispose(&o);
    ag_object_dispose(&cp2);
    ag_require (ag_object_refc(cp1) == 1, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


static void derived_test_refc_4(void)
{
    printf("ag_object_refc() accounts for mutable handles to derived object"
            " payloads");
    
    ag_object_t *o = derived_sample();
    ag_object_t *cp1 = ag_object_copy(o);
    ag_object_t *cp2 = ag_object_copy(cp1);

    struct base_payload *p = ag_object_payload_mutable(&cp1);
    (void) p;

    ag_require (ag_object_refc(o) == 2, AG_ERNO_TEST, NULL);
    ag_require (ag_object_refc(cp1) == 1, AG_ERNO_TEST, NULL);
    ag_require (ag_object_refc(cp2) == 2, AG_ERNO_TEST, NULL);

    ag_object_dispose(&cp1);
    ag_object_dispose(&cp2);
    ag_require (ag_object_refc(o) == 1, AG_ERNO_TEST, NULL);
    ag_object_dispose(&o);

    printf("...OK\n");
}




extern void ag_test_object(void)
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
    
    ag_object_init(32);
    ag_object_register(BASE_OBJECT, &vt);

    vt.copy = &derived_method_copy;
    vt.dispose = &derived_method_dispose;
    vt.id = &derived_method_id;
    vt.sz = &derived_method_sz;
    vt.len = &derived_method_len;
    vt.hash = &derived_method_hash;
    vt.cmp = &derived_method_cmp;
    vt.str = &derived_method_str;
    ag_object_register(DERIVED_OBJECT, &vt);


    printf("===============================================================\n");
    printf("Starting object model interface test suite...\n\n");

    base_test_new();
    base_test_dispose();
    base_test_type();
    base_test_id();
    base_test_hash();
    base_test_sz();
    base_test_len();
    base_test_empty();
    base_test_cmp();
    base_test_lt();
    base_test_eq();
    base_test_gt();
    base_test_payload();
    base_test_payload_mutable();
    base_test_str();
    base_test_refc();
    base_test_refc_2();
    base_test_refc_3();
    base_test_refc_4();

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
    derived_test_refc();
    derived_test_refc_2();
    derived_test_refc_3();
    derived_test_refc_4();

    printf("\n");
    ag_object_exit();
}
