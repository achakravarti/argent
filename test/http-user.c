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

    ag_http_user_smart_t *u = ag_http_user_new("mozilla", "192.168.1.4", 80,
            "example.com");
    ag_require (u, AG_ERNO_TEST, NULL);
    ag_require (ag_http_user_port(u) == 80, AG_ERNO_TEST, NULL);

    ag_string_smart_t *a = ag_http_user_agent(u);
    ag_require (ag_string_eq(a, "mozilla"), AG_ERNO_TEST, NULL);

    ag_string_smart_t *i = ag_http_user_ip(u);
    ag_require (ag_string_eq(i, "192.168.1.4"), AG_ERNO_TEST, NULL);

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
    printf("...OK\n");
}


/*
 * new_03(): ag_http_user_new() test #3.
 */
static void new_03(void)
{
    printf("ag_http_user_new(): @agent ~empty && @ip ~empty && host empty"
            " && ip > 0 => new user");
    printf("...OK\n");
}


/*
 * copy_01(): ag_http_user_copy() test #1.
 */
static void copy_01(void)
{
    printf("ag_http_user_copy(): @ctx ~null => @ctx");
    printf("...OK\n");
}


/*
 * dispose_01(): ag_http_user_dispose() test #1.
 */
static void dispose_01(void)
{
    printf("ag_http_user_dispose(): @ctx ~null && ptrval ~null => @ctx null");

    printf("...OK\n");
}


/*
 * dispose_02(): ag_http_user_dispose() test #2.
 */
static void dispose_02(void)
{
    printf("ag_http_user_dispose(): @ctx null => no-op");
    printf("...OK\n");
}


/*
 * dispose_03(): ag_http_user_dispose() test #3.
 */
static void dispose_03(void)
{
    printf("ag_http_user_dispose(): @ctx ~null && ptrval null => @ctx null");
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
    printf("ag_http_user_cmp():");
    printf("...OK\n");
}


/*
 * cmp_02(): ag_http_user_cmp() test #2.
 */
static void cmp_02(void)
{
    printf("ag_http_user_cmp():");
    printf("...OK\n");
}


/*
 * cmp_03(): ag_http_user_cmp() test #3.
 */
static void cmp_03(void)
{
    printf("ag_http_user_cmp():");
    printf("...OK\n");
}


/*
 * lt_01(): ag_http_user_lt() test #1.
 */
static void lt_01(void)
{
    printf("ag_http_user_lt():");
    printf("...OK\n");
}


/*
 * lt_02(): ag_http_user_lt() test #2.
 */
static void lt_02(void)
{
    printf("ag_http_user_lt():");
    printf("...OK\n");
}


/*
 * lt_03(): ag_http_user_lt() test #3.
 */
static void lt_03(void)
{
    printf("ag_http_user_lt():");
    printf("...OK\n");
}


/*
 * eq_01(): ag_http_user_eq() test #1.
 */
static void eq_01(void)
{
    printf("ag_http_user_eq():");
    printf("...OK\n");
}


/*
 * eq_02(): ag_http_user_eq() test #2.
 */
static void eq_02(void)
{
    printf("ag_http_user_eq():");
    printf("...OK\n");
}


/*
 * eq_03(): ag_http_user_eq() test #3.
 */
static void eq_03(void)
{
    printf("ag_http_user_eq():");
    printf("...OK\n");
}


/*
 * gt_01(): ag_http_user_gt() test #1.
 */
static void gt_01(void)
{
    printf("ag_http_user_gt():");
    printf("...OK\n");
}


/*
 * gt_02(): ag_http_user_gt() test #2.
 */
static void gt_02(void)
{
    printf("ag_http_user_gt():");
    printf("...OK\n");
}


/*
 * gt_03(): ag_http_user_gt() test #3.
 */
static void gt_03(void)
{
    printf("ag_http_user_gt():");
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
    printf("ag_http_user_empty():");
    printf("...OK\n");
}


/*
 * typeid_01(): ag_http_user_typeid() test #1.
 */
static void typeid_01(void)
{
    printf("ag_http_user_typeid():");
    printf("...OK\n");
}


/*
 * objid_01(): ag_http_user_objid() test #1.
 */
static void objid_01(void)
{
    printf("ag_http_user_objid():");
    printf("...OK\n");
}


/*
 * len_01(): ag_http_user_len() test #1.
 */
static void len_01(void)
{
    printf("ag_http_user_len():");
    printf("...OK\n");
}


/*
 * sz_01(): ag_http_user_sz() test #1.
 */
static void sz_01(void)
{
    printf("ag_http_user_sz():");
    printf("...OK\n");
}


/*
 * hash_01(): ag_http_user_hash() test #1.
 */
static void hash_01(void)
{
    printf("ag_http_user_hash():");
    printf("...OK\n");
}



/*
 * str_01(): ag_http_user_str() test #1.
 */
static void str_01(void)
{
    printf("ag_http_user_str():");
    printf("...OK\n");
}


/*
 * str_02(): ag_http_user_str() test #2.
 */
static void str_02(void)
{
    printf("ag_http_user_str():");
    printf("...OK\n");
}


/*
 * str_03(): ag_http_user_str() test #3.
 */
static void str_03(void)
{
    printf("ag_http_user_str():");
    printf("...OK\n");
}


/*
 * agent_01(): ag_http_user_agent() test #1.
 */
static void agent_01(void)
{
    printf("ag_http_user_agent():");
    printf("...OK\n");
}


/*
 * ip_01(): ag_http_user_ip() test #1.
 */
static void ip_01(void)
{
    printf("ag_http_user_ip():");
    printf("...OK\n");
}


/*
 * port_01(): ag_http_user_port() test #1.
 */
static void port_01(void)
{
    printf("ag_http_user_port():");
    printf("...OK\n");
}


/*
 * port_02(): ag_http_user_port() test #2.
 */
static void port_02(void)
{
    printf("ag_http_user_port():");
    printf("...OK\n");
}


/*
 * host_01(): ag_http_user_host() test #1.
 */
static void host_01(void)
{
    printf("ag_http_user_host():");
    printf("...OK\n");
}


/*
 * host_02(): ag_http_user_host() test #2.
 */
static void host_02(void)
{
    printf("ag_http_user_host():");
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

