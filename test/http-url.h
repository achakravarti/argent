#ifndef __AG_HTTP_URL_METATESTS_H__
#define __AG_HTTP_URL_METATESTS_H__


#define AG_METATEST_HTTP_URL_HOST(sample, expect)                       \
        AG_TEST_CASE("ag_http_url_host(): " #sample " => " expect)      \
        {                                                               \
                AG_AUTO(ag_http_url) *u = sample;                       \
                AG_AUTO(ag_string) *h = ag_http_url_host(u);            \
                AG_TEST (ag_string_eq(h, expect));                      \
        }


#define AG_METATEST_HTTP_URL_SECURE(sample, expect)                     \
        AG_TEST_CASE("ag_http_url_secure(): " #sample " => " #expect)   \
        {                                                               \
                AG_AUTO(ag_http_url) *u = sample;                       \
                AG_TEST (ag_http_url_secure(u) == expect);              \
        }


#define AG_METATEST_HTTP_URL_PORT(sample, expect)                       \
        AG_TEST_CASE("ag_http_url_port(): " #sample " => " #expect)     \
        {                                                               \
                AG_AUTO(ag_http_url) *u = sample;                       \
                AG_TEST (ag_http_url_port(u) == expect);                \
        }


#endif /* !__AG_HTTP_URL_METATESTS_H__ */

