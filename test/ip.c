#include "../include/argent.h"
#include "./test.h"




static inline ag_ip_t *sample_localhost(void)
{
    return ag_ip_new(127, 0, 0, 1);
}


static inline ag_ip_t *sample_private(void)
{
    return ag_ip_parse("192.168.1.4");
}




/*******************************************************************************
 *                                MANAGER TESTS
 */


/*
 * new_01(): ag_ip_new() test #1.
 */
static void new_01(void)
{
    printf("ag_ip_new():");

    ag_ip_smart_t *ip = sample_localhost();
    ag_test (ip);

    ag_test (ag_ip_octet(ip, 1) == 127);
    ag_test (!ag_ip_octet(ip, 2));
    ag_test (!ag_ip_octet(ip, 3));
    ag_test (ag_ip_octet(ip, 4) == 1);

    printf("...OK\n");
}


/*
 * parse_01(): ag_ip_parse() test #1.
 */
static void parse_01(void)
{
    printf("ag_ip_parse():");

    ag_ip_smart_t *ip = ag_ip_parse("127.0.0.1");
    ag_test (ip);
    
    ag_test (ag_ip_octet(ip, 1) == 127);
    ag_test (!ag_ip_octet(ip, 2));
    ag_test (!ag_ip_octet(ip, 3));
    ag_test (ag_ip_octet(ip, 4) == 1);

    printf("...OK\n");
}


/*
 * copy_01(): ag_ip_copy() test #1.
 */
static void copy_01(void)
{
    printf("ag_ip_copy():");

    ag_ip_smart_t *ip1 = sample_localhost();
    ag_ip_smart_t *ip2 = ag_ip_copy(ip1);

    ag_test (ag_ip_octet(ip1, 1) == ag_ip_octet (ip2, 1));
    ag_test (ag_ip_octet(ip1, 2) == ag_ip_octet (ip2, 2));
    ag_test (ag_ip_octet(ip1, 3) == ag_ip_octet (ip2, 3));
    ag_test (ag_ip_octet(ip1, 4) == ag_ip_octet (ip2, 4));

    printf("...OK\n");
}


/*
 * dispose_01(): ag_ip_dispose() test #1.
 */
static void dispose_01(void)
{
    printf("ag_ip_dispose(): @ctx valid => @ctx null");

    ag_ip_smart_t *ip = sample_localhost();
    ag_ip_dispose(&ip);
    ag_test (!ip);

    printf("...OK\n");
}


/*
 * dispose_02(): ag_ip_dispose() test #2.
 */
static void dispose_02(void)
{
    printf("ag_ip_dispose(): @ctx null => no-op");

    ag_ip_dispose(NULL);

    printf("...OK\n");
}


/*
 * dispose_03(): ag_ip_dispose() test #3.
 */
static void dispose_03(void)
{
    printf("ag_ip_dispose(): @ctx ~null && ptrval null => @ctx null");
    
    ag_ip_smart_t *ip = NULL;
    ag_ip_dispose(&ip);
    ag_test (!ip);

    printf("...OK\n");
}




/*******************************************************************************
 *                               COMPARATOR TESTS
 */


/*
 * cmp_01(): ag_ip_cmp() test #1.
 */
static void cmp_01(void)
{
    printf("ag_ip_cmp(): @ctx < @cmp => AG_TRISTATE_LO");

    ag_ip_smart_t *ip1 = sample_localhost();
    ag_ip_smart_t *ip2 = sample_private();
    ag_test (ag_ip_cmp(ip1, ip2) == AG_TRISTATE_LO);

    printf("...OK\n");
}


/*
 * cmp_02(): ag_ip_cmp() test #2.
 */
static void cmp_02(void)
{
    printf("ag_ip_cmp(): @ctx == @cmp => AG_TRISTATE_GND");

    ag_ip_smart_t *ip1 = sample_localhost();
    ag_ip_smart_t *ip2 = ag_ip_copy(ip1);
    ag_test (ag_ip_cmp(ip1, ip2) == AG_TRISTATE_GND);

    printf("...OK\n");
}


/*
 * cmp_03(): ag_ip_cmp() test #3.
 */
static void cmp_03(void)
{
    printf("ag_ip_cmp(): @ctx > @cmp => AG_TRISTATE_HI");

    ag_ip_smart_t *ip1 = sample_localhost();
    ag_ip_smart_t *ip2 = sample_private();
    ag_test (ag_ip_cmp(ip2, ip1) == AG_TRISTATE_HI);

    printf("...OK\n");
}


/*
 * lt_01(): ag_ip_lt() test #1.
 */
static void lt_01(void)
{
    printf("ag_ip_lt(): @ctx < @cmp => true");

    ag_ip_smart_t *ip1 = sample_localhost();
    ag_ip_smart_t *ip2 = sample_private();
    ag_test (ag_ip_lt(ip1, ip2));

    printf("...OK\n");
}


/*
 * lt_02(): ag_ip_lt() test #2.
 */
static void lt_02(void)
{
    printf("ag_ip_lt(): @ctx == @cmp => false");

    ag_ip_smart_t *ip1 = sample_localhost();
    ag_ip_smart_t *ip2 = ag_ip_copy(ip1);
    ag_test (!ag_ip_lt(ip1, ip2));

    printf("...OK\n");
}


/*
 * lt_03(): ag_ip_lt() test #3.
 */
static void lt_03(void)
{
    printf("ag_ip_lt(): @ctx > @cmp => false");

    ag_ip_smart_t *ip1 = sample_localhost();
    ag_ip_smart_t *ip2 = sample_private();
    ag_test (!ag_ip_lt(ip2, ip1));

    printf("...OK\n");
}


/*
 * eq_01(): ag_ip_eq() test #1.
 */
static void eq_01(void)
{
    printf("ag_ip_eq(): @ctx == @cmp => true");

    ag_ip_smart_t *ip1 = sample_localhost();
    ag_ip_smart_t *ip2 = ag_ip_copy(ip1);
    ag_test (ag_ip_eq(ip1, ip2));

    printf("...OK\n");
}


/*
 * eq_02(): ag_ip_eq() test #2.
 */
static void eq_02(void)
{
    printf("ag_ip_eq(): @ctx < @cmp => false");

    ag_ip_smart_t *ip1 = sample_localhost();
    ag_ip_smart_t *ip2 = sample_private();
    ag_test (!ag_ip_eq(ip1, ip2));

    printf("...OK\n");
}


/*
 * eq_03(): ag_ip_eq() test #3.
 */
static void eq_03(void)
{
    printf("ag_ip_eq(): @ctx > @cmp => false");

    ag_ip_smart_t *ip1 = sample_localhost();
    ag_ip_smart_t *ip2 = sample_private();
    ag_test (!ag_ip_eq(ip2, ip1));

    printf("...OK\n");
}


/*
 * gt_01(): ag_ip_gt() test #1.
 */
static void gt_01(void)
{
    printf("ag_ip_gt(): @ctx > @cmp => true");

    ag_ip_smart_t *ip1 = sample_localhost();
    ag_ip_smart_t *ip2 = sample_private();
    ag_test (ag_ip_gt(ip2, ip1));

    printf("...OK\n");
}


/*
 * gt_02(): ag_ip_gt() test #2.
 */
static void gt_02(void)
{
    printf("ag_ip_gt(): @ctx == @cmp => false");

    ag_ip_smart_t *ip1 = sample_localhost();
    ag_ip_smart_t *ip2 = ag_ip_copy(ip1);
    ag_test (!ag_ip_gt(ip1, ip2));

    printf("...OK\n");
}


/*
 * gt_03(): ag_ip_gt() test #3.
 */
static void gt_03(void)
{
    printf("ag_ip_gt(): @ctx < @cmp => false");

    ag_ip_smart_t *ip1 = sample_localhost();
    ag_ip_smart_t *ip2 = sample_private();
    ag_test (!ag_ip_gt(ip1, ip2));

    printf("...OK\n");
}




/*******************************************************************************
 *                                ACCESSOR TESTS
 */


/*
 * empty_01(): ag_ip_empty() test #1.
 */
static void empty_01(void)
{
    printf("ag_ip_empty(): @ctx ~null => false");

    ag_ip_smart_t *ip = sample_localhost();
    ag_test (!ag_ip_empty(ip));

    printf("...OK\n");
}


/*
 * typeid_01(): ag_ip_typeid() test #1.
 */
static void typeid_01(void)
{
    printf("ag_ip_typeid(): @ctx ~null => AG_OBJECT_TYPE_IP");
    
    ag_ip_smart_t *ip = sample_private();
    ag_test (ag_ip_typeid(ip) == AG_OBJECT_TYPE_IP);

    printf("...OK\n");
}


/*
 * objid_01(): ag_ip_objid() test #1.
 */
static void objid_01(void)
{
    printf("ag_ip_objid(): @ctx ~null => 0");
    
    ag_ip_smart_t *ip = sample_localhost();
    ag_test (!ag_ip_objid(ip));

    printf("...OK\n");
}


/*
 * len_01(): ag_ip_len() test #1.
 */
static void len_01(void)
{
    printf("ag_ip_len(): @ctx ~null => str length");
    
    ag_ip_smart_t *ip = sample_private();
    ag_string_smart_t *s = ag_ip_str(ip);
    ag_test (ag_ip_len(ip) == ag_string_len(s));

    printf("...OK\n");
}


/*
 * sz_01(): ag_ip_sz() test #1.
 */
static void sz_01(void)
{
    printf("ag_ip_sz(): @ctx ~null => 4 * uint8_t sz");

    ag_ip_smart_t *ip = sample_localhost();
    ag_test (ag_ip_sz(ip) == 4 * sizeof(uint8_t));

    printf("...OK\n");
}


/*
 * hash_01(): ag_ip_hash() test #1.
 */
static void hash_01(void)
{
    printf("ag_ip_hash(): @ctx ~null => str hash");

    ag_ip_smart_t *ip = sample_private();
    ag_string_smart_t *s = ag_ip_str(ip);
    ag_test (ag_ip_hash(ip) == ag_hash_new_str(s));

    printf("...OK\n");
}


/*
 * octet_01(): ag_ip_octet() test #1.
 */
static void octet_01(void)
{
    printf("ag_ip_octet(): @ctx ~null => octet 1");
    
    ag_ip_smart_t *ip = sample_localhost();
    ag_test (ag_ip_octet(ip, 1) == 127);

    printf("...OK\n");
}


/*
 * octet_02(): ag_ip_octet() test #2.
 */
static void octet_02(void)
{
    printf("ag_ip_octet(): @ctx ~null => octet 2");
    
    ag_ip_smart_t *ip = sample_private();
    ag_test (ag_ip_octet(ip, 2) == 168);

    printf("...OK\n");
}


/*
 * octet_03(): ag_ip_octet() test #3.
 */
static void octet_03(void)
{
    printf("ag_ip_octet(): @ctx ~null => octet 3");
    
    ag_ip_smart_t *ip = sample_localhost();
    ag_test (!ag_ip_octet(ip, 3));

    printf("...OK\n");
}


/*
 * octet_04(): ag_ip_octet() test #4.
 */
static void octet_04(void)
{
    printf("ag_ip_octet(): @ctx ~null => octet 4");

    ag_ip_smart_t *ip = sample_private();
    ag_test (ag_ip_octet(ip, 4) == 4);

    printf("...OK\n");
}


/*
 * str_01(): ag_ip_str() test #1.
 */
static void str_01(void)
{
    printf("ag_ip_str(): @ctx ~null => str representation");

    ag_ip_smart_t *ip = sample_localhost();
    ag_string_smart_t *s = ag_ip_str(ip);
    ag_test (ag_string_eq(s, "127.0.0.1"));

    printf("...OK\n");
}




/*******************************************************************************
 *                             TEST SUITE INTERFACE
 */


extern void ag_test_ip(void)
{
    ag_ip_register();

    printf("===============================================================\n");
    printf("Starting IP interface test suite...\n\n");

    new_01();
    parse_01();
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
    octet_01();
    octet_02();
    octet_03();
    octet_04();
    str_01();
}

