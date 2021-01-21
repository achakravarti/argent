#define __AG_TEST_SUITE_ID__ 9


#include "./http-client.h"
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


SAMPLE(CLIENT0, "", 0, "", "", "");
SAMPLE(CLIENT1, "192.168.0.1", 0, "host.com", "mozilla", "google.com");
SAMPLE(CLIENT2, "192.168.1.1", 40, "domain.com", "webkit", "");


AG_METATEST_OBJECT_COPY(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_COPY(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_COPY(ag_http_client, CLIENT2());


AG_METATEST_OBJECT_CLONE(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_CLONE(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_CLONE(ag_http_client, CLIENT2());


AG_METATEST_OBJECT_RELEASE(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_RELEASE(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_RELEASE(ag_http_client, CLIENT2());


AG_METATEST_OBJECT_CMP(ag_http_client, CLIENT1(), CLIENT0());
AG_METATEST_OBJECT_CMP(ag_http_client, CLIENT2(), CLIENT0());
AG_METATEST_OBJECT_CMP(ag_http_client, CLIENT1(), CLIENT2());


AG_METATEST_OBJECT_LT(ag_http_client, CLIENT1(), CLIENT0());
AG_METATEST_OBJECT_LT(ag_http_client, CLIENT2(), CLIENT0());
AG_METATEST_OBJECT_LT(ag_http_client, CLIENT1(), CLIENT2());


AG_METATEST_OBJECT_EQ(ag_http_client, CLIENT1(), CLIENT0());
AG_METATEST_OBJECT_EQ(ag_http_client, CLIENT2(), CLIENT0());
AG_METATEST_OBJECT_EQ(ag_http_client, CLIENT1(), CLIENT2());


AG_METATEST_OBJECT_GT(ag_http_client, CLIENT1(), CLIENT0());
AG_METATEST_OBJECT_GT(ag_http_client, CLIENT2(), CLIENT0());
AG_METATEST_OBJECT_GT(ag_http_client, CLIENT1(), CLIENT2());


AG_METATEST_OBJECT_EMPTY_NOT(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_client, CLIENT2());


AG_METATEST_OBJECT_VALID(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_VALID(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_VALID(ag_http_client, CLIENT2());


AG_METATEST_OBJECT_TYPEID(ag_http_client, CLIENT0(), AG_TYPEID_HTTP_CLIENT);
AG_METATEST_OBJECT_TYPEID(ag_http_client, CLIENT1(), AG_TYPEID_HTTP_CLIENT);
AG_METATEST_OBJECT_TYPEID(ag_http_client, CLIENT2(), AG_TYPEID_HTTP_CLIENT);


AG_METATEST_OBJECT_UUID(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_UUID(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_UUID(ag_http_client, CLIENT2());


AG_METATEST_OBJECT_REFC(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_REFC(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_REFC(ag_http_client, CLIENT2());


AG_METATEST_OBJECT_LEN(ag_http_client, CLIENT1(), CLIENT1_LEN());
AG_METATEST_OBJECT_LEN(ag_http_client, CLIENT2(), CLIENT2_LEN());


AG_METATEST_OBJECT_SZ(ag_http_client, CLIENT1(), CLIENT1_SZ());
AG_METATEST_OBJECT_SZ(ag_http_client, CLIENT2(), CLIENT2_SZ());


AG_METATEST_OBJECT_HASH(ag_http_client, CLIENT0(), CLIENT0_HASH());
AG_METATEST_OBJECT_HASH(ag_http_client, CLIENT1(), CLIENT1_HASH());
AG_METATEST_OBJECT_HASH(ag_http_client, CLIENT2(), CLIENT2_HASH());


AG_METATEST_OBJECT_STR(ag_http_client, CLIENT0(),
    "[] host=, agent=, referer=");
AG_METATEST_OBJECT_STR(ag_http_client, CLIENT1(),
    "[192.168.0.1] host=host.com, agent=mozilla, referer=google.com");
AG_METATEST_OBJECT_STR(ag_http_client, CLIENT2(),
    "[192.168.1.1:40] host=domain.com, agent=webkit, referer=");


AG_METATEST_HTTP_CLIENT_IP(CLIENT0(), "");
AG_METATEST_HTTP_CLIENT_IP(CLIENT1(), "192.168.0.1");
AG_METATEST_HTTP_CLIENT_IP(CLIENT2(), "192.168.1.1");


AG_METATEST_HTTP_CLIENT_PORT(CLIENT0(), 0);
AG_METATEST_HTTP_CLIENT_PORT(CLIENT1(), 0);
AG_METATEST_HTTP_CLIENT_PORT(CLIENT2(), 40);


AG_METATEST_HTTP_CLIENT_HOST(CLIENT0(), "");
AG_METATEST_HTTP_CLIENT_HOST(CLIENT1(), "host.com");
AG_METATEST_HTTP_CLIENT_HOST(CLIENT2(), "domain.com");


AG_METATEST_HTTP_CLIENT_AGENT(CLIENT0(), "");
AG_METATEST_HTTP_CLIENT_AGENT(CLIENT1(), "mozilla");
AG_METATEST_HTTP_CLIENT_AGENT(CLIENT2(), "webkit");


AG_METATEST_HTTP_CLIENT_REFERER(CLIENT0(), "");
AG_METATEST_HTTP_CLIENT_REFERER(CLIENT1(), "google.com");
AG_METATEST_HTTP_CLIENT_REFERER(CLIENT2(), "");


extern ag_test_suite *
test_suite_http_client(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_client interface");
}

