#include "../include/argent.h"
#include "./test.h"




static inline ag_ip_t *sample_localhost(void)
{
    return ag_ip_new(127, 0, 0, 1);
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
}

