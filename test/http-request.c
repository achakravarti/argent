#include "./object.h"
#include "./test.h"


#define __AG_TEST_SUITE_ID__ 12


#define SAMPLE_CLIENT(tag, ip, port, host, agent, referer)                     \
        static inline ag_http_client *CLIENT_ ## tag(void)                     \
        {                                                                      \
                return ag_http_client_new(ip, port, host, agent, referer);     \
        }


#define SAMPLE_URL(tag, secure, host, port, path)                       \
        static inline ag_http_url *URL_ ## tag(void)                    \
        {                                                               \
                return port ? ag_http_url_new(secure, host, port, path) \
                    : ag_http_url_new_noport(secure, host, path);       \
        }                                                               \


#define SAMPLE_FIELD(tag, key, val)                                     \
        static inline ag_field *FIELD_ ## tag(void)                     \
        {                                                               \
                AG_AUTO(ag_value) *k = ag_value_new_string(key);        \
                AG_AUTO(ag_value) *v = ag_value_new_string(val);        \
                return ag_field_new(k, v);                              \
        }



SAMPLE_CLIENT(CLIENT0, "", 0, "", "", "");
SAMPLE_CLIENT(CLIENT1, "192.168.0.1", 0, "host.com", "mozilla", "google.com");
SAMPLE_CLIENT(CLIENT2, "192.168.1.1", 40, "domain.com", "webkit", "");


SAMPLE_URL(HTTP_LOCALHOST_8080_NOPATH, false, "127.0.0.1", 8080, "");
SAMPLE_URL(HTTPS_LOCALHOST_8080_NOPATH, true, "127.0.0.1", 8080, "");
SAMPLE_URL(HTTP_LOCALHOST_8080_FOO, false, "localhost", 8080, "foo");
SAMPLE_URL(HTTPS_LOCALHOST_8080_FOO, true, "localhost", 8080, "foo");
SAMPLE_URL(HTTPS_DOMAIN_FOO, true, "www.domain.com", 0, "/foo");
SAMPLE_URL(HTTPS_DOMAIN_FOO_BAR, true, "www.domain.com", 0, "foo/bar");


SAMPLE_FIELD(KEYVAL, "key", "val");
SAMPLE_FIELD(FOOBAR, "foo", "bar");
SAMPLE_FIELD(FOO, "foo", "");


extern ag_test_suite *
test_suite_http_request(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_request interface");
}

