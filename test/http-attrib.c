#include "../include/argent.h"
#include "./test.h"




/*******************************************************************************
 *                                MANAGER TESTS
 */


/*
 * new_01(): ag_http_attrib_new() test #1.
 */
static void new_01(void)
{
    printf("ag_http_attrib_new(): @key && @val ~empty => new attrib");

    ag_http_attrib_smart_t *a = ag_http_attrib_new("foo", "bar");
    ag_require (a, AG_ERNO_TEST, NULL);

    ag_string_smart_t *k = ag_http_attrib_key(a);
    ag_require (ag_string_eq(k, "foo"), AG_ERNO_TEST, NULL);

    ag_string_smart_t *v = ag_http_attrib_val(a);
    ag_require (ag_string_eq(v, "bar"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * new_empty_01(): ag_http_attrib_new_empty() test #2.
 */
static void new_empty_01(void)
{
    printf("ag_http_attrib_new_empty(): @key ~empty => new empty attrib");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new_empty("foo");
    ag_require (a, AG_ERNO_TEST, NULL);

    ag_string_smart_t *k = ag_http_attrib_key(a);
    ag_require (ag_string_eq(k, "foo"), AG_ERNO_TEST, NULL);
    
    ag_string_smart_t *v = ag_http_attrib_val(a);
    ag_require (ag_string_eq(v, ""), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * copy_01(): ag_http_attrib_copy() test #1.
 */
static void copy_01(void)
{
    printf("ag_http_attrib_copy(): @ctx ~null => @ctx");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("foo", "bar");
    ag_http_attrib_smart_t *cp = ag_http_attrib_copy(a);
    ag_require (cp, AG_ERNO_TEST, NULL);

    ag_string_smart_t *k = ag_http_attrib_key(cp);
    ag_require (ag_string_eq(k, "foo"), AG_ERNO_TEST, NULL);
    
    ag_string_smart_t *v = ag_http_attrib_val(cp);
    ag_require (ag_string_eq(v, "bar"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * dispose_01(): ag_http_attrib_dispose() test #1.
 */
static void dispose_01(void)
{
    printf("ag_http_attrib_dispose(): @ctx valid => @ctx null");
    
    ag_http_attrib_t *a = ag_http_attrib_new("foo", "bar");
    ag_http_attrib_dispose(&a);
    ag_require (!a, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * dispose_02(): ag_http_attrib_dispose() test #2.
 */
static void dispose_02(void)
{
    printf("ag_http_attrib_dispose(): @ctx null => no-op");
    
    ag_http_attrib_dispose(NULL);

    printf("...OK\n");
}


/*
 * dispose_03(): ag_http_attrib_dispose() test #3.
 */
static void dispose_03(void)
{
    printf("ag_http_attrib_dispose(): @ctx ~null && ptrval null => @ctx null");
    
    ag_http_attrib_t *a = NULL;
    ag_http_attrib_dispose(&a);
    ag_require (!a, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}




/*******************************************************************************
 *                               COMPARATOR TESTS
 */


/*
 * cmp_01(): ag_http_attrib_cmp() test #1.
 */
static void cmp_01(void)
{
    printf("ag_http_attrib_cmp(): @ctx < @cmp => AG_TRISTATE_LO");

    ag_http_attrib_smart_t *a = ag_http_attrib_new("alpha", "beta");
    ag_http_attrib_smart_t *b = ag_http_attrib_new("charlie", "delta");
    ag_require (ag_http_attrib_cmp(a, b) == AG_TRISTATE_LO, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * cmp_02(): ag_http_attrib_cmp() test #2.
 */
static void cmp_02(void)
{
    printf("ag_http_attrib_cmp(): @ctx == @cmp => AG_TRISTATE_GND");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("alpha", "beta");
    ag_http_attrib_smart_t *b = ag_http_attrib_copy(a);
    ag_require (ag_http_attrib_cmp(a, b) == AG_TRISTATE_GND, AG_ERNO_TEST,
            NULL);

    printf("...OK\n");
}


/*
 * cmp_03(): ag_http_attrib_cmp() test #3.
 */
static void cmp_03(void)
{
    printf("ag_http_attrib_cmp(): @ctx > @cmp => AG_TRISTATE_HI");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("alpha", "beta");
    ag_http_attrib_smart_t *b = ag_http_attrib_new("charlie", "delta");
    ag_require (ag_http_attrib_cmp(b, a) == AG_TRISTATE_HI, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * lt_01(): ag_http_attrib_lt() test #1.
 */
static void lt_01(void)
{
    printf("ag_http_attrib_lt(): @ctx < @cmp => true");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("alpha", "beta");
    ag_http_attrib_smart_t *b = ag_http_attrib_new("charlie", "delta");
    ag_require (ag_http_attrib_lt(a, b), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * lt_02(): ag_http_attrib_lt() test #2.
 */
static void lt_02(void)
{
    printf("ag_http_attrib_lt(): @ctx > @cmp => false");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("alpha", "beta");
    ag_http_attrib_smart_t *b = ag_http_attrib_new("charlie", "delta");
    ag_require (!ag_http_attrib_lt(b, a), AG_ERNO_TEST, NULL);
   
    printf("...OK\n");
}


/*
 * lt_03(): ag_http_attrib_lt() test #3.
 */
static void lt_03(void)
{
    printf("ag_http_attrib_lt(): @ctx == @cmp => false");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("alpha", "beta");
    ag_http_attrib_smart_t *b = ag_http_attrib_copy(a);
    ag_require (!ag_http_attrib_lt(b, a), AG_ERNO_TEST, NULL);
   
    printf("...OK\n");
}


/*
 * eq_01(): ag_http_attrib_eq() test #1.
 */
static void eq_01(void)
{
    printf("ag_http_attrib_eq(): @ctx == @cmp => true");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("alpha", "beta");
    ag_http_attrib_smart_t *b = ag_http_attrib_copy(a);
    ag_require (ag_http_attrib_eq(a, b), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * eq_02(): ag_http_attrib_eq() test #2.
 */
static void eq_02(void)
{
    printf("ag_http_attrib_eq(): @ctx != @cmp => false ");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("alpha", "beta");
    ag_http_attrib_smart_t *b = ag_http_attrib_new("charlie", "delta");
    ag_require (!ag_http_attrib_eq(b, a), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * gt_01(): ag_http_attrib_gt() test #1.
 */
static void gt_01(void)
{
    printf("ag_http_attrib_gt() @ctx > @cmp => true");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("charlie", "delta");
    ag_http_attrib_smart_t *b = ag_http_attrib_new("alpha", "beta");
    ag_require (ag_http_attrib_gt(a, b), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * gt_02(): ag_http_attrib_gt() test #2.
 */
static void gt_02(void)
{
    printf("ag_http_attrib_gt(): @ctx < @cmp => false");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("charlie", "delta");
    ag_http_attrib_smart_t *b = ag_http_attrib_new("alpha", "beta");
    ag_require (!ag_http_attrib_gt(b, a), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * gt_03(): ag_http_attrib_gt() test #3.
 */
static void gt_03(void)
{
    printf("ag_http_attrib_gt(): @ctx < @cmp => false");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("alpha", "beta");
    ag_http_attrib_smart_t *b = ag_http_attrib_copy(a);
    ag_require (!ag_http_attrib_gt(b, a), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*******************************************************************************
 *                                ACCESSOR TESTS
 */


/*
 * empty_01(): ag_http_attrib_empty() test #1.
 */
static void empty_01(void)
{
    printf("ag_http_attrib_empty(): @ctx empty => true");

    ag_http_attrib_smart_t *a = ag_http_attrib_new_empty("foo");
    ag_require (ag_http_attrib_empty(a), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * empty_02(): ag_http_attrib_empty() test #2.
 */
static void empty_02(void)
{
    printf("ag_http_attrib_empty(): @ctx ~empty => false");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("foo", "bar");
    ag_require (!ag_http_attrib_empty(a), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * objid_01(): ag_http_attrib_objid() test #1.
 */
static void typeid_01(void)
{
    printf("ag_http_attrib_typeid(): @ctx ~null => AG_OBJECT_TYPE_HTTP_ATTRIB");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("foo", "bar");
    ag_require (ag_http_attrib_typeid(a) == AG_OBJECT_TYPE_HTTP_ATTRIB,
            AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * objid_02(): ag_http_attrib_objid() test #2.
 */
static void objid_01(void)
{
    printf("ag_http_attrib_objid(): @ctx ~null => 0");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("foo", "bar");
    ag_require (!ag_http_attrib_objid(a), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * hash_01(): ag_http_attrib_hash() test #1.
 */
static void hash_01(void)
{
    printf("ag_http_attrib_hash(): @ctx ~empty => string hash key=val");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("foo", "bar");
    ag_string_smart_t *s = ag_http_attrib_str(a);
    ag_require (ag_http_attrib_hash(a) == ag_string_hash(s), AG_ERNO_TEST,
            NULL);

    printf("...OK\n");
}


/*
 * hash_02(): ag_http_attrib_hash() test #2.
 */
static void hash_02(void)
{
    printf("ag_http_attrib_hash(): @ctx empty => string hash key");

    ag_http_attrib_smart_t *a = ag_http_attrib_new_empty("foo");
    ag_string_smart_t *s = ag_http_attrib_str(a);
    ag_require (ag_http_attrib_hash(a) == ag_string_hash(s), AG_ERNO_TEST,
            NULL);

    printf("...OK\n");
}


/*
 * sz_01(): ag_http_attrib_sz() test #1.
 */
static void sz_01(void)
{
    printf("ag_http_attrib_sz(): @ctx ~null => string size");

    ag_http_attrib_smart_t *a = ag_http_attrib_new("foo", "bar");
    ag_string_smart_t *s = ag_http_attrib_str(a);
    ag_require (ag_http_attrib_sz(a) == ag_string_sz(s), AG_ERNO_TEST, NULL);
    
    printf("...OK\n");
}


/*
 * len_01(): ag_http_attrib_len() test #1.
 */
static void len_01(void)
{
    printf("ag_http_attrib_len(): @ctx ~empty => value length");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("foo", "bar");
    ag_string_smart_t *v = ag_http_attrib_val(a);
    ag_require (ag_http_attrib_len(a) == ag_string_len(v), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * len_02(): ag_http_attrib_len() test #2.
 */
static void len_02(void)
{
    printf("ag_http_attrib_len(): @ctx empty => 0");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new_empty("foo");
    ag_require (!ag_http_attrib_len(a), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * key_01(): ag_http_attrib_key() test #1.
 */
static void key_01(void)
{
    printf("ag_http_attrib_key(): @ctx ~null => key");

    ag_http_attrib_smart_t *a = ag_http_attrib_new("foo", "bar");
    ag_string_smart_t *k = ag_http_attrib_key(a);
    ag_require (ag_string_eq(k, "foo"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * val_01(): ag_http_attrib_val() test #1.
 */
static void val_01(void)
{
    printf("ag_http_attrib_val(): @ctx ~empty => value");
    
    ag_http_attrib_smart_t *a = ag_http_attrib_new("foo", "bar");
    ag_string_smart_t *v = ag_http_attrib_val(a);
    ag_require (ag_string_eq(v, "bar"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * val_02(): ag_http_attrib_val() test #2.
 */
static void val_02(void)
{
    printf("ag_http_attrib_val(): @ctx ~empty => value URL-decoded");

    ag_http_attrib_smart_t *a = ag_http_attrib_new("foo", "bar foo; x=y%");
    ag_string_smart_t *v = ag_http_attrib_val(a);
    //printf("\n%s\n\n", v);
    ag_require (ag_string_eq(v, "bar foo; x=y%"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * val_03(): ag_http_attrib_val() test #3.
 */
static void val_03(void)
{
    printf("ag_http_attrib_val(): @ctx empty => empty string");

    ag_http_attrib_smart_t *a = ag_http_attrib_new_empty("foo");
    ag_string_smart_t *v = ag_http_attrib_val(a);
    ag_require (!*v, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * str_01(): ag_http_attrib_str() test #1.
 */
static void str_01(void)
{
    printf("ag_http_attrib_str(): @ctx ~empty => key=value");

    ag_http_attrib_smart_t *a = ag_http_attrib_new("foo", "bar");
    ag_string_smart_t *s1 = ag_http_attrib_str(a);

    ag_string_smart_t *k = ag_http_attrib_key(a);
    ag_string_smart_t *v = ag_http_attrib_val(a);
    ag_string_smart_t *s2 = ag_string_new_fmt("%s=%s", k, v);
    
    ag_require (ag_string_eq(s1, s2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * str_02(): ag_http_attrib_str() test #2.
 */
static void str_02(void)
{
    printf("ag_http_attrib_str(): @ctx ~empty => key=value URL-encoded");

    ag_http_attrib_smart_t *a = ag_http_attrib_new("foo", "bar foo; x=y%");
    ag_string_smart_t *s = ag_http_attrib_str(a);
    ag_require (ag_string_eq(s, "foo=bar%20foo%3B%20x%3Dy%25"), AG_ERNO_TEST,
            NULL);

    printf("...OK\n");
}


/*
 * str_03(): ag_http_attrib_str() test #3.
 */
static void str_03(void)
{
    printf("ag_http_attrib_str(): @ctx empty => key");

    ag_http_attrib_smart_t *a = ag_http_attrib_new_empty("foo");
    ag_string_smart_t *k = ag_http_attrib_key(a);
    ag_string_smart_t *s = ag_http_attrib_str(a);
    ag_require (ag_string_eq(s, k), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}




/*******************************************************************************
 *                             TEST SUITE INTERFACE
 */


extern void ag_test_http_attrib(void)
{
    ag_http_attrib_register();

    printf("===============================================================\n");
    printf("Starting HTTP attribute interface test suite...\n\n");

    new_01();
    new_empty_01();
    copy_01();
    dispose_01();
    dispose_02();
    dispose_03();

    cmp_01();
    cmp_02();
    cmp_03();
    lt_01();
    lt_02();
    lt_03();
    eq_01();
    eq_02();
    gt_01();
    gt_02();
    gt_03();

    empty_01();
    empty_02();
    typeid_01();
    objid_01();
    hash_01();
    hash_02();
    sz_01();
    len_01();
    len_02();
    key_01();
    val_01();
    val_02();
    val_03();
    str_01();
    str_02();
    str_03();

    printf("\n");
}

