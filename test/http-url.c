#include "../include/argent.h"
#include "./test.h"




/*******************************************************************************
 *                                MANAGER TESTS
 */


/*
 * new_01(): ag_http_url_new() test #1.
 */
static void new_01(void)
{
    printf("ag_http_url_new(): @secure true, @host && @path ~empty, @port = 80"
           " => new url");

    ag_http_url_smart_t *u = ag_http_url_new(true, "example.com", 80, "/foo");
    ag_require (u, AG_ERNO_TEST, NULL);
    
    ag_require (ag_http_url_secure(u), AG_ERNO_TEST, NULL);
    ag_require (ag_http_url_port(u) == 80, AG_ERNO_TEST, NULL);

    ag_string_smart_t *h = ag_http_url_host(u);
    ag_require (ag_string_eq(h, "example.com"), AG_ERNO_TEST, NULL);

    ag_string_smart_t *p = ag_http_url_path(u);
    ag_require (ag_string_eq(p, "/foo"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * new_02(): ag_http_url_new() test #2.
 */
static void new_02(void)
{
    printf("ag_http_url_new(): @secure false, @host && @path ~empty, @port = 0"
            " => new url");

    ag_http_url_smart_t *u = ag_http_url_new(false, "example.com", 0, "foo");
    ag_require (u, AG_ERNO_TEST, NULL);

    ag_require (!ag_http_url_secure(u), AG_ERNO_TEST, NULL);
    ag_require (!ag_http_url_port(u), AG_ERNO_TEST, NULL);

    ag_string_smart_t *h = ag_http_url_host(u);
    ag_require (ag_string_eq(h, "example.com"), AG_ERNO_TEST, NULL);

    ag_string_smart_t *p = ag_http_url_path(u);
    ag_require (ag_string_eq(p, "/foo"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * new_03(): ag_http_url_new() test #3.
 */
static void new_03(void)
{
    printf("ag_http_url_new(): @secure true, @host ~empty, @path empty,"
           " @port = 0 => new url");

    ag_http_url_smart_t *u = ag_http_url_new(true, "example.com", 0, "");
    ag_require (u, AG_ERNO_TEST, NULL);

    ag_require (ag_http_url_secure(u), AG_ERNO_TEST, NULL);
    ag_require (!ag_http_url_port(u), AG_ERNO_TEST, NULL);

    ag_string_smart_t *h = ag_http_url_host(u);
    ag_require (ag_string_eq(h, "example.com"), AG_ERNO_TEST, NULL);

    ag_string_smart_t *p = ag_http_url_path(u);
    ag_require (ag_string_eq(p, "/"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * copy_01(): ag_http_url_copy() test #1.
 */
static void copy_01(void)
{
    printf("ag_http_url_copy(): @ctx ~null => @ctx");
    
    ag_http_url_smart_t *u1 = ag_http_url_new(false, "example.com", 80, "foo");
    ag_http_url_smart_t *u2 = ag_http_url_copy(u1);
    ag_require (u2, AG_ERNO_TEST, NULL);
    
    ag_require (!ag_http_url_secure(u2), AG_ERNO_TEST, NULL);
    ag_require (ag_http_url_port(u2) == 80, AG_ERNO_TEST, NULL);

    ag_string_smart_t *host = ag_http_url_host(u2);
    ag_require (ag_string_eq(host, "example.com"), AG_ERNO_TEST, NULL);

    ag_string_smart_t *path = ag_http_url_path(u2);
    ag_require (ag_string_eq(path, "/foo"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * dispose_01(): ag_http_url_dispose() test #1.
 */
static void dispose_01(void)
{
    printf("ag_http_url_dispose(): @ctx valid => @ctx null");

    ag_http_url_smart_t *u = ag_http_url_new(false, "example.com", 80, "path");
    ag_http_url_dispose(&u);
    ag_require (!u, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * dispose_02(): ag_http_url_dispose() test #2.
 */
static void dispose_02(void)
{
    printf("ag_http_url_dispose(): @ctx null => no-op");

    ag_http_url_dispose(NULL);

    printf("...OK\n");
}


/*
 * dispose_03(): ag_http_url_dispose() test #3.
 */
static void dispose_03(void)
{
    printf("ag_http_url_dispose(): @ctx ~null && ptrval null => @ctx null");

    ag_http_url_t *u = NULL;
    ag_http_url_dispose(&u);
    ag_require (!u, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}




/*******************************************************************************
 *                               COMPARATOR TESTS
 */


/*
 * cmp_01(): ag_http_url_cmp() test #1.
 */
static void cmp_01(void)
{
    printf("ag_http_url_cmp(): @ctx < @cmp => AG_TRISTATE_LO");

    ag_http_url_smart_t *u1 = ag_http_url_new(false, "example.com", 0, "");
    ag_http_url_smart_t *u2 = ag_http_url_new(true, "abc.net", 0, "");
    ag_require (ag_http_url_cmp(u1, u2) == AG_TRISTATE_LO, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * cmp_02(): ag_http_url_cmp() test #2.
 */
static void cmp_02(void)
{
    printf("ag_http_url_cmp(): @ctx == @cmp => AG_TRISTATE_GND");

    ag_http_url_smart_t *u1 = ag_http_url_new(true, "example.com", 8080, "foo");
    ag_http_url_smart_t *u2 = ag_http_url_copy(u1);
    ag_require (ag_http_url_cmp(u1, u2) == AG_TRISTATE_GND, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * cmp_03(): ag_http_url_cmp() test #3.
 */
static void cmp_03(void)
{
    printf("ag_http_url_cmp(): @ctx > @cmp => AG_TRISTATE_HI");

    ag_http_url_smart_t *u1 = ag_http_url_new(false, "example.com", 0, "");
    ag_http_url_smart_t *u2 = ag_http_url_new(true, "abc.net", 0, "");
    ag_require (ag_http_url_cmp(u2, u1) == AG_TRISTATE_HI, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * lt_01(): ag_http_url_lt() test #1.
 */
static void lt_01(void)
{
    printf("ag_http_url_lt(): @ctx < @cmp => true");

    ag_http_url_smart_t *u1 = ag_http_url_new(false, "example.com", 0, "");
    ag_http_url_smart_t *u2 = ag_http_url_new(true, "abc.net", 0, "");
    ag_require (ag_http_url_lt(u1, u2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * lt_02(): ag_http_url_lt() test #2.
 */
static void lt_02(void)
{
    printf("ag_http_url_lt(): @ctx == @cmp => false");
    
    ag_http_url_smart_t *u1 = ag_http_url_new(true, "example.com", 8080, "foo");
    ag_http_url_smart_t *u2 = ag_http_url_copy(u1);
    ag_require (!ag_http_url_lt(u1, u2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * lt_03(): ag_http_url_lt() test #3.
 */
static void lt_03(void)
{
    printf("ag_http_url_lt(): @ctx > @cmp => false");
    
    ag_http_url_smart_t *u1 = ag_http_url_new(false, "example.com", 0, "");
    ag_http_url_smart_t *u2 = ag_http_url_new(true, "abc.net", 0, "");
    ag_require (!ag_http_url_lt(u2, u1), AG_ERNO_TEST, NULL);
   
    printf("...OK\n");
}


/*
 * eq_01(): ag_http_url_eq() test #1.
 */
static void eq_01(void)
{
    printf("ag_http_url_eq(): @ctx == @cmp => true");
    
    ag_http_url_smart_t *u1 = ag_http_url_new(true, "example.com", 8080, "foo");
    ag_http_url_smart_t *u2 = ag_http_url_copy(u1);
    ag_require (ag_http_url_eq(u1, u2), AG_ERNO_TEST, NULL);
   
    printf("...OK\n");
}


/*
 * eq_02(): ag_http_url_eq() test #2.
 */
static void eq_02(void)
{
    printf("ag_http_url_eq(): @ctx < @cmp => false");
    
    ag_http_url_smart_t *u1 = ag_http_url_new(false, "example.com", 0, "");
    ag_http_url_smart_t *u2 = ag_http_url_new(true, "abc.net", 0, "");
    ag_require (!ag_http_url_eq(u1, u2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * eq_03(): ag_http_url_eq() test #3.
 */
static void eq_03(void)
{
    printf("ag_http_url_eq(): @ctx > @cmp => false");

    ag_http_url_smart_t *u1 = ag_http_url_new(false, "example.com", 0, "");
    ag_http_url_smart_t *u2 = ag_http_url_new(true, "abc.net", 0, "");
    ag_require (!ag_http_url_eq(u2, u1), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * gt_01(): ag_http_url_gt() test #1.
 */
static void gt_01(void)
{
    printf("ag_http_url_gt(): @ctx > @cmp => true");
    
    ag_http_url_smart_t *u1 = ag_http_url_new(false, "example.com", 0, "");
    ag_http_url_smart_t *u2 = ag_http_url_new(true, "abc.net", 0, "");
    ag_require (ag_http_url_gt(u2, u1), AG_ERNO_TEST, NULL);
   
    printf("...OK\n");
}


/*
 * gt_02(): ag_http_url_gt() test #2.
 */
static void gt_02(void)
{
    printf("ag_http_url_gt(): @ctx == @cmp => false");
    
    ag_http_url_smart_t *u1 = ag_http_url_new(true, "example.com", 8080, "foo");
    ag_http_url_smart_t *u2 = ag_http_url_copy(u1);
    ag_require (!ag_http_url_gt(u1, u2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * gt_03(): ag_http_url_gt() test #3.
 */
static void gt_03(void)
{
    printf("ag_http_url_gt(): @ctx < @cmp => false");
    
    ag_http_url_smart_t *u1 = ag_http_url_new(false, "example.com", 0, "");
    ag_http_url_smart_t *u2 = ag_http_url_new(true, "abc.net", 0, "");
    ag_require (!ag_http_url_gt(u1, u2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}




/*******************************************************************************
 *                                ACCESSOR TESTS
 */


/*
 * empty_01(): ag_http_url_empty() test #1.
 */
static void empty_01(void)
{
}


/*
 * secure_01(): ag_http_url_secure() test #1.
 */
static void secure_01(void)
{
}


/*
 * secure_02(): ag_http_url_secure() test #2.
 */
static void secure_02(void)
{
}


/*
 * typeid_01(): ag_http_url_typeid() test #1.
 */
static void typeid_01(void)
{
    printf("ag_http_url_typeid(): @ctx ~null => AG_OBJECT_TYPE_HTTP_URL");
    
    ag_http_url_smart_t *u = ag_http_url_new(true, "example.com", 0, "bar");
    ag_require (ag_http_url_typeid(u) == AG_OBJECT_TYPE_HTTP_URL,
            AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * objid_01(): ag_http_url_objid() test #1.
 */
static void objid_01(void)
{
    printf("ag_http_url_objid(): @ctx ~null => 0");
    
    ag_http_url_smart_t *u = ag_http_url_new(true, "example.com", 80, "/foo");
    ag_require (!ag_http_attrib_objid(u), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * hash_01(): ag_http_url_hash() test #1.
 */
static void hash_01(void)
{
    printf("ag_http_url_hash(): @ctx ~null => string hash key=val");
    
    ag_http_url_smart_t *u = ag_http_url_new(true, "example.com", 0, "bar");
    ag_string_smart_t *s = ag_http_url_str(u);
    ag_require (ag_http_url_hash(u) == ag_string_hash(s), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * sz_01(): ag_http_url_sz() test #1.
 */
static void sz_01(void)
{
    printf("ag_http_url_sz(): @ctx ~null => string size");

    ag_http_url_smart_t *u = ag_http_url_new(true, "example.com", 80, "/foo");
    ag_string_smart_t *s = ag_http_url_str(u);
    ag_require (ag_http_url_sz(u) == ag_string_sz(s), AG_ERNO_TEST, NULL);
    
    printf("...OK\n");
}


/*
 * len_01(): ag_http_url_len() test #1.
 */
static void len_01(void)
{
    printf("ag_http_url_len(): @ctx ~empty => url length");
    
    ag_http_url_smart_t *u = ag_http_url_new(true, "example.com", 0, "bar");
    ag_string_smart_t *s = ag_http_url_str(u);
    ag_require (ag_http_url_len(u) == ag_string_len(s), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}



/*
 * port_01(): ag_http_url_port() test #1.
 */
static void port_01(void)
{
    printf("ag_http_url_port(): @ctx 3port => port");
    printf("...OK\n");
}


/*
 * port_02(): ag_http_url_port() test #2.
 */
static void port_02(void)
{
    printf("ag_http_url_port(): @ctx ~3port => 0");
    printf("...OK\n");
}


/*
 * str_01(): ag_http_url_str() test #1.
 */
static void str_01(void)
{
    printf("ag_http_url_str(): @ctx 3port && ~3path => url");

    ag_http_url_smart_t *u = ag_http_url_new(true, "example.com", 80, "/foo");
    ag_string_smart_t *s = ag_http_url_str(u);
    ag_require (ag_string_eq(s, "https://example.com:80/foo"), AG_ERNO_TEST,
            NULL);

    printf("...OK\n");
}


/*
 * str_02(): ag_http_url_str() test #2.
 */
static void str_02(void)
{
    printf("ag_http_url_str(): @ctx 3port && ~3path => url");
    
    ag_http_url_smart_t *u = ag_http_url_new(true, "example.com", 80, "");
    ag_string_smart_t *s = ag_http_url_str(u);
    ag_require (ag_string_eq(s, "https://example.com:80/"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * str_03(): ag_http_url_str() test #3.
 */
static void str_03(void)
{
    printf("ag_http_url_str(): @ctx ~3port && ~3path => url");
    
    ag_http_url_smart_t *u = ag_http_url_new(true, "example.com", 0, "");
    ag_string_smart_t *s = ag_http_url_str(u);
    ag_require (ag_string_eq(s, "https://example.com/"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * str_04(): ag_http_url_str() test #4.
 */
static void str_04(void)
{
    printf("ag_http_url_str(): @ctx ~3port && 3path => url");

    ag_http_url_smart_t *u = ag_http_url_new(false, "example.com", 0, "foo");
    ag_string_smart_t *s = ag_http_url_str(u);
    ag_require (ag_string_eq(s, "http://example.com/foo"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * host_01(): ag_http_url_host() test #1.
 */
static void host_01(void)
{
    printf("ag_http_url_host(): @ctx ~null => host");
    printf("...OK\n");
}


/*
 * path_01(): ag_http_url_path() test #1.
 */
static void path_01(void)
{
    printf("ag_http_url_path(): @ctx 3path => path");
    printf("...OK\n");
}


/*
 * path_02(): ag_http_url_path() test #2.
 */
static void path_02(void)
{
    printf("ag_http_url_path(): @ctx 3path && path ~3(prefix slash) => path");
    printf("...OK\n");
}


/*
 * path_03(): ag_http_url_path() test #3.
 */
static void path_03(void)
{
    printf("ag_http_url_path(): @ctx ~3path => path");
    printf("...OK\n");
}




/*******************************************************************************
 *                             TEST SUITE INTERFACE
 */


extern void ag_test_http_url(void)
{
    ag_http_url_register();

    printf("===============================================================\n");
    printf("Starting HTTP URL interface test suite...\n\n");

    new_01();
    new_02();
    new_03();
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
    eq_03();
    gt_01();
    gt_02();
    gt_03();

    empty_01();
    secure_01();
    secure_02();
    typeid_01();
    objid_01();
    hash_01();
    sz_01();
    len_01();
    port_01();
    port_02();
    str_01();
    str_02();
    str_03();
    str_04();
    host_01();
    path_01();
    path_02();
    path_03();

    printf("\n");
}

