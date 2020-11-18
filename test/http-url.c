#include "../include/argent.h"
#include "./test.h"




/*******************************************************************************
 *                                MANAGER TESTS
 */


static void new_01(void)
{
    printf("ag_http_url_new(): @secure true, @host && @path && @port ~empty"
           " => new url");

    ag_http_url_smart_t *u = ag_http_url_new(true, "example.com", "80", "foo");
    ag_require (u, AG_ERNO_TEST, NULL);
    
    ag_require (ag_http_url_secure(u), AG_ERNO_TEST, NULL);

    ag_string_smart_t *h = ag_http_url_host(u);
    ag_require (ag_string_eq(h, "example.com"), AG_ERNO_TEST, NULL);

    ag_string_smart_t *p = ag_http_url_path(u);
    ag_require (ag_string_eq(p, "foo"), AG_ERNO_TEST, NULL);

    ag_string_smart_t *port = ag_http_url_port(u);
    ag_require (ag_string_eq(port, "80"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void copy_01(void)
{
    printf("ag_http_url_copy(): @ctx ~null => @ctx");
    
    ag_http_url_smart_t *u1 = ag_http_url_new(false, "example.com", "80", "foo");
    ag_http_url_smart_t *u2 = ag_http_url_copy(u1);
    ag_require (u2, AG_ERNO_TEST, NULL);
    ag_require (!ag_http_url_secure(u1), AG_ERNO_TEST, NULL);

    ag_string_smart_t *host = ag_http_url_host(u2);
    ag_require (ag_string_eq(host, "example.com"), AG_ERNO_TEST, NULL);

    ag_string_smart_t *path = ag_http_url_path(u2);
    ag_require (ag_string_eq(path, "foo"), AG_ERNO_TEST, NULL);

    ag_string_smart_t *port = ag_http_url_port(u2);
    ag_require (ag_string_eq(port, "80"), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void dispose_01(void)
{
    printf("ag_http_url_dispose(): @ctx valid => @ctx null");

    ag_http_url_smart_t *u = ag_http_url_new(false, "example.com", "path",
            "80");
    ag_http_url_dispose(&u);
    ag_require (!u, AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void dispose_02(void)
{
    printf("ag_http_url_dispose(): @ctx null => no-op");

    ag_http_url_dispose(NULL);

    printf("...OK\n");
}

static void dispose_03(void)
{
    printf("ag_http_url_dispose(): @ctx ~null && ptrval null => @ctx null");

    ag_http_url_t *u = NULL;
    ag_http_url_dispose(&u);
    ag_require (!u, AG_ERNO_TEST, NULL);

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

