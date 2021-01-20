#ifndef __AG_HTTP_CLIENT_METATESTS_H__
#define __AG_HTTP_CLIENT_METATESTS_H__


#define AG_METATEST_HTTP_CLIENT_IP(sample, expect)                      \
        AG_TEST_CASE("ag_http_client_ip(): " #sample " => " expect)     \
        {                                                               \
                AG_AUTO(ag_http_client) *c = sample;                    \
                AG_AUTO(ag_string) *i = ag_http_client_ip(c);           \
                AG_TEST (ag_string_eq(i, expect));                      \
        }


#define AG_METATEST_HTTP_CLIENT_PORT(sample, expect)                    \
        AG_TEST_CASE("ag_http_client_port(): " #sample " => " #expect)  \
        {                                                               \
                AG_AUTO(ag_http_client) *c = sample;                    \
                AG_TEST (ag_http_client_port(c) == expect);             \
        }


#define AG_METATEST_HTTP_CLIENT_HOST(sample, expect)                    \
        AG_TEST_CASE("ag_http_client_host(): " #sample " => " expect)   \
        {                                                               \
                AG_AUTO(ag_http_client) *c = sample;                    \
                AG_AUTO(ag_string) *h = ag_http_client_host(c);         \
                AG_TEST (ag_string_eq(h, expect));                      \
        }


#define AG_METATEST_HTTP_CLIENT_AGENT(sample, expect)                   \
        AG_TEST_CASE("ag_http_client_agent(): " #sample " => " expect)  \
        {                                                               \
                AG_AUTO(ag_http_client) *c = sample;                    \
                AG_AUTO(ag_string) *a = ag_http_client_agent(c);        \
                AG_TEST (ag_string_eq(a, expect));                      \
        }


#define AG_METATEST_HTTP_CLIENT_REFERER(sample, expect)                        \
        AG_TEST_CASE("ag_http_client_referer(): " #sample " => " expect)       \
        {                                                                      \
                AG_AUTO(ag_http_client) *c = sample;                           \
                AG_AUTO(ag_string) *r = ag_http_client_referer(c);             \
                AG_TEST (ag_string_eq(r, expect));                             \
        }


#endif /* !__AG_HTTP_CLIENT_METATESTS_H__ */

