#include "../include/argent.h"
#include "./test.h"




/*******************************************************************************
 *                                MANAGER TESTS
 */


/*
 * new_01(): ag_http_user_new() test #1.
 */
static void new_01(void)
{
    printf("ag_http_user_new(): @agent ~empty && @ip ~empty && host ~empty"
            " && ip > 0 => new user");

    ag_ip_smart_t *ip1 = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip1, 80,
            "example.com");
    ag_require (u, AG_ERNO_TEST, NULL);
    ag_require (ag_http_user_port(u) == 80, AG_ERNO_TEST, NULL);

    ag_string_smart_t *a = ag_http_user_agent(u);
    ag_require (ag_string_eq(a, "mozilla"), AG_ERNO_TEST, NULL);

    ag_ip_smart_t *ip2 = ag_http_user_ip(u);
    ag_test (ag_ip_eq(ip1, ip2));

    ag_string_smart_t *h = ag_http_user_host(u);
    ag_require (ag_string_eq(h, "example.com"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * new_02(): ag_http_user_new() test #2.
 */
static void new_02(void)
{
    printf("ag_http_user_new(): @agent ~empty && @ip ~empty && host ~empty"
            " && ~ip => new user");

    ag_ip_smart_t *ip1 = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip1, 0,
            "example.com");
    ag_require (u, AG_ERNO_TEST, NULL);
    ag_require (!ag_http_user_port(u), AG_ERNO_TEST, NULL);

    ag_string_smart_t *a = ag_http_user_agent(u);
    ag_require (ag_string_eq(a, "mozilla"), AG_ERNO_TEST, NULL);

    ag_ip_smart_t *ip2 = ag_http_user_ip(u);
    ag_test (ag_ip_eq(ip1, ip2));

    ag_string_smart_t *h = ag_http_user_host(u);
    ag_require (ag_string_eq(h, "example.com"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * new_03(): ag_http_user_new() test #3.
 */
static void new_03(void)
{
    printf("ag_http_user_new(): @agent ~empty && @ip ~empty && host empty"
            " && ip > 0 => new user");

    ag_ip_smart_t *ip1 = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip1, 80,
            "");
    ag_require (u, AG_ERNO_TEST, NULL);
    ag_require (ag_http_user_port(u) == 80, AG_ERNO_TEST, NULL);

    ag_string_smart_t *a = ag_http_user_agent(u);
    ag_require (ag_string_eq(a, "mozilla"), AG_ERNO_TEST, NULL);

    ag_ip_smart_t *ip2 = ag_http_user_ip(u);
    ag_test (ag_ip_eq(ip1, ip2));

    ag_string_smart_t *h = ag_http_user_host(u);
    ag_require (ag_string_eq(h, ""), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * copy_01(): ag_http_user_copy() test #1.
 */
static void copy_01(void)
{
    printf("ag_http_user_copy(): @ctx ~null => @ctx");
    
    ag_ip_smart_t *ip1 = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u1 = ag_http_user_new("mozilla", ip1, 80,
            "example.com");
    ag_http_user_smart_t *u2 = ag_http_user_copy(u1);
    ag_require (u2, AG_ERNO_TEST, NULL);
    ag_require (ag_http_user_port(u2) == 80, AG_ERNO_TEST, NULL);

    ag_string_smart_t *a = ag_http_user_agent(u2);
    ag_require (ag_string_eq(a, "mozilla"), AG_ERNO_TEST, NULL);

    ag_ip_smart_t *ip2 = ag_http_user_ip(u2);
    ag_test (ag_ip_eq(ip1, ip2));

    ag_string_smart_t *h = ag_http_user_host(u2);
    ag_require (ag_string_eq(h, "example.com"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * dispose_01(): ag_http_user_dispose() test #1.
 */
static void dispose_01(void)
{
    printf("ag_http_user_dispose(): @ctx ~null && ptrval ~null => @ctx null");
   

    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_http_user_dispose(&u);
    ag_require (1u, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * dispose_02(): ag_http_user_dispose() test #2.
 */
static void dispose_02(void)
{
    printf("ag_http_user_dispose(): @ctx null => no-op");

    ag_http_user_dispose(NULL);

    printf("...OK\n");
}


/*
 * dispose_03(): ag_http_user_dispose() test #3.
 */
static void dispose_03(void)
{
    printf("ag_http_user_dispose(): @ctx ~null && ptrval null => @ctx null");

    ag_http_user_t *u = NULL;
    ag_http_user_dispose(&u);
    ag_require (!u, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}




/*******************************************************************************
 *                               COMPARATOR TESTS
 */


/*
 * cmp_01(): ag_http_user_cmp() test #1.
 */
static void cmp_01(void)
{
    printf("ag_http_user_cmp(): @ctx < @cmp => AG_TRISTATE_LO");

    ag_ip_smart_t *ip1 = ag_ip_new(192, 168, 1, 4);
    ag_ip_smart_t *ip2 = ag_ip_new(192, 168, 1, 5);

    ag_http_user_smart_t *u1 = ag_http_user_new("mozilla", ip1, 80,
            "example.com");
    ag_http_user_smart_t *u2 = ag_http_user_new("mozilla", ip2, 80,
            "example.com");
    ag_require (ag_http_user_cmp(u1, u2) == AG_TRISTATE_LO, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * cmp_02(): ag_http_user_cmp() test #2.
 */
static void cmp_02(void)
{
    printf("ag_http_user_cmp(): @ctx == @cmp => AG_TRISTATE_GND");

    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u1 = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_http_user_smart_t *u2 = ag_http_user_copy(u1);
    ag_require (ag_http_user_cmp(u1, u2) == AG_TRISTATE_GND, AG_ERNO_TEST,
            NULL);

    printf("...OK\n");
}


/*
 * cmp_03(): ag_http_user_cmp() test #3.
 */
static void cmp_03(void)
{
    printf("ag_http_user_cmp(): @ctx > @cmp => AG_TRISTATE_HI");
    
    ag_ip_smart_t *ip1 = ag_ip_new(192, 168, 1, 4);
    ag_ip_smart_t *ip2 = ag_ip_new(192, 168, 1, 5);

    ag_http_user_smart_t *u1 = ag_http_user_new("mozilla", ip1, 80,
            "example.com");
    ag_http_user_smart_t *u2 = ag_http_user_new("mozilla", ip2, 80,
            "example.com");
    ag_require (ag_http_user_cmp(u2, u1) == AG_TRISTATE_HI, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * lt_01(): ag_http_user_lt() test #1.
 */
static void lt_01(void)
{
    printf("ag_http_user_lt(): @ctx < @cmp => true");

    ag_ip_smart_t *ip1 = ag_ip_new(192, 168, 1, 4);
    ag_ip_smart_t *ip2 = ag_ip_new(192, 168, 1, 5);

    ag_http_user_smart_t *u1 = ag_http_user_new("mozilla", ip1, 80,
            "example.com");
    ag_http_user_smart_t *u2 = ag_http_user_new("mozilla", ip2, 80,
            "example.com");
    ag_require (ag_http_user_lt(u1, u2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * lt_02(): ag_http_user_lt() test #2.
 */
static void lt_02(void)
{
    printf("ag_http_user_lt(): @ctx == @cmp => false");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u1 = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_http_user_smart_t *u2 = ag_http_user_copy(u1);
    ag_require (!ag_http_user_lt(u1, u2), AG_ERNO_TEST,
            NULL);

    printf("...OK\n");
}


/*
 * lt_03(): ag_http_user_lt() test #3.
 */
static void lt_03(void)
{
    printf("ag_http_user_lt(): @ctx > @cmp => false");

    ag_ip_smart_t *ip1 = ag_ip_new(192, 168, 1, 4);
    ag_ip_smart_t *ip2 = ag_ip_new(192, 168, 1, 5);

    ag_http_user_smart_t *u1 = ag_http_user_new("mozilla", ip1, 80,
            "example.com");
    ag_http_user_smart_t *u2 = ag_http_user_new("mozilla", ip2, 80,
            "example.com");
    ag_require (!ag_http_user_lt(u2, u1), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * eq_01(): ag_http_user_eq() test #1.
 */
static void eq_01(void)
{
    printf("ag_http_user_eq(): @ctx == @cmp => true");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u1 = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_http_user_smart_t *u2 = ag_http_user_copy(u1);
    ag_require (ag_http_user_eq(u1, u2), AG_ERNO_TEST,
            NULL);

    printf("...OK\n");
}


/*
 * eq_02(): ag_http_user_eq() test #2.
 */
static void eq_02(void)
{
    printf("ag_http_user_eq(): @ctx < @cmp => false");
    
    ag_ip_smart_t *ip1 = ag_ip_new(192, 168, 1, 4);
    ag_ip_smart_t *ip2 = ag_ip_new(192, 168, 1, 5);

    ag_http_user_smart_t *u1 = ag_http_user_new("mozilla", ip1, 80,
            "example.com");
    ag_http_user_smart_t *u2 = ag_http_user_new("mozilla", ip2, 80,
            "example.com");
    ag_require (!ag_http_user_eq(u1, u2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * eq_03(): ag_http_user_eq() test #3.
 */
static void eq_03(void)
{
    printf("ag_http_user_eq(): @ctx > @cmp => false");
    
    ag_ip_smart_t *ip1 = ag_ip_new(192, 168, 1, 4);
    ag_ip_smart_t *ip2 = ag_ip_new(192, 168, 1, 5);

    ag_http_user_smart_t *u1 = ag_http_user_new("mozilla", ip1, 80,
            "example.com");
    ag_http_user_smart_t *u2 = ag_http_user_new("mozilla", ip2, 80,
            "example.com");
    ag_require (!ag_http_user_eq(u2, u1), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * gt_01(): ag_http_user_gt() test #1.
 */
static void gt_01(void)
{
    printf("ag_http_user_gt(): @ctx < @cmp => true");
    
    ag_ip_smart_t *ip1 = ag_ip_new(192, 168, 1, 4);
    ag_ip_smart_t *ip2 = ag_ip_new(192, 168, 1, 5);

    ag_http_user_smart_t *u1 = ag_http_user_new("mozilla", ip1, 80,
            "example.com");
    ag_http_user_smart_t *u2 = ag_http_user_new("mozilla", ip2, 80,
            "example.com");
    ag_require (ag_http_user_gt(u2, u1), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * gt_02(): ag_http_user_gt() test #2.
 */
static void gt_02(void)
{
    printf("ag_http_user_gt(): @ctx == @cmp => false");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u1 = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_http_user_smart_t *u2 = ag_http_user_copy(u1);
    ag_require (!ag_http_user_gt(u1, u2), AG_ERNO_TEST,
            NULL);

    printf("...OK\n");
}


/*
 * gt_03(): ag_http_user_gt() test #3.
 */
static void gt_03(void)
{
    printf("ag_http_user_gt(): @ctx > @cmp => false");
    
    ag_ip_smart_t *ip1 = ag_ip_new(192, 168, 1, 4);
    ag_ip_smart_t *ip2 = ag_ip_new(192, 168, 1, 5);

    ag_http_user_smart_t *u1 = ag_http_user_new("mozilla", ip1, 80,
            "example.com");
    ag_http_user_smart_t *u2 = ag_http_user_new("mozilla", ip2, 80,
            "example.com");
    ag_require (!ag_http_user_gt(u1, u2), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}




/*******************************************************************************
 *                                ACCESSOR TESTS
 */


/*
 * empty_01(): ag_http_user_empty() test #1.
 */
static void empty_01(void)
{
    printf("ag_http_user_empty(): @ctx ~null => false");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_require (!ag_http_user_empty(u), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * empty_02(): ag_http_user_empty() test #2.
 */
static void empty_02(void)
{
    printf("ag_http_user_empty(): @ctx ~3port => false");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 0,
            "example.com");
    ag_require (!ag_http_user_empty(u), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * empty_03(): ag_http_user_empty() test #3.
 */
static void empty_03(void)
{
    printf("ag_http_user_empty(): @ctx ~host => false");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "");
    ag_require (!ag_http_user_empty(u), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * typeid_01(): ag_http_user_typeid() test #1.
 */
static void typeid_01(void)
{
    printf("ag_http_user_typeid(): @ctx ~null => AG_OBJECT_TYPE_HTTP_USER");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_require (ag_http_user_typeid(u) == AG_OBJECT_TYPE_HTTP_USER,
            AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * objid_01(): ag_http_user_objid() test #1.
 */
static void objid_01(void)
{
    printf("ag_http_user_objid(): @ctx ~null => 0");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_require (!ag_http_user_objid(u), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * len_01(): ag_http_user_len() test #1.
 */
static void len_01(void)
{
    printf("ag_http_user_len(): @ctx ~null => string length");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_string_smart_t *s = ag_http_user_str(u);
    ag_require (ag_http_user_len(u) == ag_string_len(s), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * sz_01(): ag_http_user_sz() test #1.
 */
static void sz_01(void)
{
    printf("ag_http_user_sz(): @ctx ~null => sigma component size");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "example.com");

    ag_string_smart_t *a = ag_http_user_agent(u);
    ag_ip_smart_t *i = ag_http_user_ip(u);
    ag_string_smart_t *h = ag_http_user_host(u);
    size_t p = ag_http_user_port(u);
    size_t sz = ag_string_sz(a) + ag_ip_sz(i) + ag_string_sz(h) 
        + sizeof p;
    ag_require (sz == ag_http_user_sz(u), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * hash_01(): ag_http_user_hash() test #1.
 */
static void hash_01(void)
{
    printf("ag_http_user_hash(): @ctx ~null => string hash");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_string_smart_t *s = ag_http_user_str(u);
    ag_require (ag_http_user_hash(u) == ag_string_hash(s), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}



/*
 * str_01(): ag_http_user_str() test #1.
 */
static void str_01(void)
{
    printf("ag_http_user_str(): @ctx ~null => str representation");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_string_smart_t *s = ag_http_user_str(u);
    ag_require (ag_string_eq(s, 
            "agent=mozilla; ip=192.168.1.4; host=example.com; port=80"),
            AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * str_02(): ag_http_user_str() test #2.
 */
static void str_02(void)
{
    printf("ag_http_user_str(): @ctx ~host => str representation");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "");
    ag_string_smart_t *s = ag_http_user_str(u);
    ag_require (ag_string_eq(s, 
            "agent=mozilla; ip=192.168.1.4; host=; port=80"),
            AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * str_03(): ag_http_user_str() test #3.
 */
static void str_03(void)
{
    printf("ag_http_user_str(): @ctx ~port => str representation");

    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 0,
            "example.com");
    ag_string_smart_t *s = ag_http_user_str(u);
    ag_require (ag_string_eq(s, 
            "agent=mozilla; ip=192.168.1.4; host=example.com; port=0"),
            AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * agent_01(): ag_http_user_agent() test #1.
 */
static void agent_01(void)
{
    printf("ag_http_user_agent(): @ctx ~null => agent");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_string_smart_t *a = ag_http_user_agent(u);
    ag_require (ag_string_eq(a, "mozilla"), AG_ERNO_TEST, NULL);
    
    printf("...OK\n");
}


/*
 * ip_01(): ag_http_user_ip() test #1.
 */
static void ip_01(void)
{
    printf("ag_http_user_ip(): @ctx ~null => ip");
    
    ag_ip_smart_t *ip1 = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip1, 80,
            "example.com");
    
    ag_ip_smart_t *ip2 = ag_http_user_ip(u);
    ag_test (ag_ip_eq(ip1, ip2));

    printf("...OK\n");
}


/*
 * port_01(): ag_http_user_port() test #1.
 */
static void port_01(void)
{
    printf("ag_http_user_port(): @ctx 3port => port");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_require (ag_http_user_port(u) == 80, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * port_02(): ag_http_user_port() test #2.
 */
static void port_02(void)
{
    printf("ag_http_user_port(): @ctx ~3port => 0");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 0,
            "example.com");
    ag_require (!ag_http_user_port(u), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * host_01(): ag_http_user_host() test #1.
 */
static void host_01(void)
{
    printf("ag_http_user_host(): @ctx 3host => host");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "example.com");
    ag_string_smart_t *h = ag_http_user_host(u);
    ag_require (ag_string_eq(h, "example.com"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}


/*
 * host_02(): ag_http_user_host() test #2.
 */
static void host_02(void)
{
    printf("ag_http_user_host(): @ctx ~3host => empty string");
    
    ag_ip_smart_t *ip = ag_ip_new(192, 168, 1, 4);
    ag_http_user_smart_t *u = ag_http_user_new("mozilla", ip, 80,
            "");
    ag_string_smart_t *h = ag_http_user_host(u);
    ag_require (ag_string_eq(h, ""), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}




/*******************************************************************************
 *                             TEST SUITE INTERFACE
 */


extern void ag_test_http_user(void)
{
    ag_http_user_register();
    
    printf("===============================================================\n");
    printf("Starting HTTP user interface test suite...\n\n");

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
    empty_02();
    empty_03();
    typeid_01();
    objid_01();
    len_01();
    sz_01();
    hash_01();
    str_01();
    str_02();
    str_03();
    agent_01();
    ip_01();
    port_01();
    port_02();
    host_01();
    host_02();
    
    printf("\n");
}

