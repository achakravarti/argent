#ifndef __AG_HTTP_CLIENT_METATESTS_H__
#define __AG_HTTP_CLIENT_METATESTS_H__


#define __AG_METATEST_HTTP_CLIENT_STR__(tag, sample, expect)                   \
        AG_TEST_CASE("ag_http_client_" #tag "(): " #sample " => " expect)      \
        {                                                                      \
                AG_AUTO(ag_http_client) *c = sample;                           \
                AG_AUTO(ag_string) *s = ag_http_client_ ## tag(c);             \
                AG_TEST (ag_string_eq(s, expect));                             \
        }


#define AG_METATEST_HTTP_CLIENT_IP(sample, expect)              \
        __AG_METATEST_HTTP_CLIENT_STR__(ip, sample, expect)


#define AG_METATEST_HTTP_CLIENT_PORT(sample, expect)                    \
        AG_TEST_CASE("ag_http_url_client(): " #sample " => " #expect)   \
        {                                                               \
                AG_AUTO(ag_http_client) *c = sample;                    \
                AG_TEST (ag_http_client_port(c) == expect);             \
        }


#define AG_METATEST_HTTP_CLIENT_HOST(sample, expect)            \
        __AG_METATEST_HTTP_CLIENT_STR__(host, sample, expect)


#define AG_METATEST_HTTP_CLIENT_AGENT(sample, expect)           \
        __AG_METATEST_HTTP_CLIENT_STR__(agent, sample, expect)


#define AG_METATEST_HTTP_CLIENT_REFERER(sample, expect)         \
        __AG_METATEST_HTTP_CLIENT_STR__(referer, sample, expect)


#endif /* !__AG_HTTP_CLIENT_METATESTS_H__ */

