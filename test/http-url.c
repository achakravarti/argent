#include "../include/argent.h"
#include "./test.h"




/*******************************************************************************
 *                                MANAGER TESTS
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

static void dispose_01(void)
{
    printf("ag_http_url_dispose(): @ctx valid => @ctx null");

    ag_http_url_smart_t *u = ag_http_url_new(false, "example.com", 80, "path");
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
    new_02();
    new_03();
    copy_01();
    dispose_01();
    dispose_02();
    dispose_03();
    
    printf("\n");
}

