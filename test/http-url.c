#include "../include/argent.h"
#include "./test.h"




/*******************************************************************************
 *                                MANAGER TESTS
 */


static void new_01(void)
{
    printf("ag_http_url_new(): @secure true, host && path && port ~empty"
           " => new url");
    printf("...OK\n");
}

static void copy_01(void)
{
    printf("ag_http_url_new(): @secure true, host && path && port ~empty"
           " => new url");
    printf("...OK\n");
}

static void dispose_01(void)
{
    printf("ag_http_url_dispose(): @secure true, host && path && port ~empty"
           " => new url");
    printf("...OK\n");
}

static void dispose_02(void)
{
    printf("ag_http_url_dispose(): @secure true, host && path && port ~empty"
           " => new url");
    printf("...OK\n");
}

static void dispose_03(void)
{
    printf("ag_http_url_dispose(): @secure true, host && path && port ~empty"
           " => new url");
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
    copy_01();
    dispose_01();
    dispose_02();
    dispose_03();
    
    printf("\n");
}

