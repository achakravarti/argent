#define __AG_TEST_SUITE_ID__ 9


#include "./object.h"
#include "./test.h"


#define SAMPLE(tag, ip, port, host, agent, referer)                            \
        static inline ag_http_client *tag(void)                                \
        {                                                                      \
                return ag_http_client_new(ip, port, host, agent, referer);     \
        }                                                                      \
        static inline size_t tag ## _LEN(void)                                 \
        {                                                                      \
                AG_AUTO(ag_http_client) *c = tag();                            \
                AG_AUTO(ag_string) *s = ag_http_client_str(c);                 \
                return ag_string_len(s);                                       \
        }                                                                      \
        static inline size_t tag ## _SZ(void)                                  \
        {                                                                      \
                AG_AUTO(ag_http_client) *c = tag();                            \
                AG_AUTO(ag_string) *s = ag_http_client_str(c);                 \
                return ag_string_sz(s);                                        \
        }                                                                      \
        static inline size_t tag ## _HASH(void)                                \
        {                                                                      \
                AG_AUTO(ag_http_client) *c = tag();                            \
                AG_AUTO(ag_string) *s = ag_http_client_str(c);                 \
                return ag_hash_new_str(s);                                     \
        }


SAMPLE(CLIENT, "192.168.0.1", 0, "host.com", "mozilla", "google.com");


extern ag_test_suite *
test_suite_http_client(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_client interface");
}

