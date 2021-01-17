#include "./object.h"
#include "./test.h"


#define __AG_TEST_SUITE_ID__ 8

/*
HTTP_LOCALHOST_8080_FOO_BAR_HTHhttp://localhost:8080/foo/bar.html
https://localhost:8080/foo/bar.html
*/


static inline ag_http_url *sample(void)
{
        return ag_http_url_new(false, "localhost", "8080",
            "foo/bar.html");
}


static inline ag_http_url *sample_2(void)
{
        return ag_http_url_new(true, "localhost", "8080",
            "foo/bar.html");
}


AG_METATEST_OBJECT_COPY(ag_http_url, sample());


extern ag_test_suite *
test_suite_http_url(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_url interface");
}

