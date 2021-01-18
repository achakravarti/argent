#include "./object.h"
#include "./test.h"


#define __AG_TEST_SUITE_ID__ 8


#define SAMPLE(tag, secure, host, port, path)                           \
        static inline ag_http_url *tag(void)                            \
        {                                                               \
                return ag_http_url_new(secure, host, port, path);       \
        }                                                               \
        static inline size_t tag ## _LEN(void)                          \
        {                                                               \
                AG_AUTO(ag_http_url) *u = tag();                        \
                AG_AUTO(ag_string) *s = ag_http_url_str(u);             \
                return ag_string_len(s);                                \
        }                                                               \
        static inline size_t tag ## _SZ(void)                           \
        {                                                               \
                AG_AUTO(ag_http_url) *u = tag();                        \
                AG_AUTO(ag_string) *s = ag_http_url_str(u);             \
                return ag_string_sz(s);                                 \
        }


SAMPLE(HTTP_LOCALHOST_8080_NOPATH, false, "localhost", 8080, "");
SAMPLE(HTTPS_LOCALHOST_8080_NOPATH, true, "localhost", 8080, "");


AG_METATEST_OBJECT_COPY(ag_http_url, HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_COPY(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH());

AG_METATEST_OBJECT_CLONE(ag_http_url, HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_CLONE(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH());

AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH());


AG_METATEST_OBJECT_CMP(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_LT(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_EQ(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_GT(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    HTTPS_LOCALHOST_8080_NOPATH());


AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH());


AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH(),
    AG_TYPEID_HTTP_URL);


AG_METATEST_OBJECT_UUID(ag_http_url, HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_UUID(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH());


AG_METATEST_OBJECT_REFC(ag_http_url, HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_REFC(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH());


AG_METATEST_OBJECT_LEN(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    HTTP_LOCALHOST_8080_NOPATH_LEN());
AG_METATEST_OBJECT_LEN(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH(),
    HTTPS_LOCALHOST_8080_NOPATH_LEN());


AG_METATEST_OBJECT_SZ(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    HTTP_LOCALHOST_8080_NOPATH_SZ());
AG_METATEST_OBJECT_SZ(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH(),
    HTTPS_LOCALHOST_8080_NOPATH_SZ());



extern ag_test_suite *
test_suite_http_url(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_url interface");
}

