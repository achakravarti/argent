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
                AG_AUTO(ag_string) *ks = ag_string_new(key);            \
                AG_AUTO(ag_string) *vs = ag_string_new(val);            \
                AG_AUTO(ag_value) *k = ag_value_new_string(ks);         \
                AG_AUTO(ag_value) *v = ag_value_new_string(vs);         \
                return ag_field_new(k, v);                              \
        }


#define SAMPLE_REQUEST(tag, method, mime, url, client, param)           \
        static inline ag_http_request *REQUEST_ ## tag(void)            \
        {                                                               \
                AG_AUTO(ag_http_url) *u = url;                          \
                AG_AUTO(ag_http_client) *c = client;                    \
                AG_AUTO(ag_alist) *p = param;                           \
                return ag_http_request_new(method, mime, u, c, p);      \
        }


SAMPLE_CLIENT(0, "", 0, "", "", "");
SAMPLE_CLIENT(1, "192.168.0.1", 0, "host.com", "mozilla", "google.com");
SAMPLE_CLIENT(2, "192.168.1.1", 40, "domain.com", "webkit", "");


SAMPLE_URL(HTTP_LOCALHOST_8080_NOPATH, false, "127.0.0.1", 8080, "");
SAMPLE_URL(HTTPS_LOCALHOST_8080_NOPATH, true, "127.0.0.1", 8080, "");
SAMPLE_URL(HTTP_LOCALHOST_8080_FOO, false, "localhost", 8080, "foo");
SAMPLE_URL(HTTPS_LOCALHOST_8080_FOO, true, "localhost", 8080, "foo");
SAMPLE_URL(HTTPS_DOMAIN_FOO, true, "www.domain.com", 0, "/foo");
SAMPLE_URL(HTTPS_DOMAIN_FOO_BAR, true, "www.domain.com", 0, "foo/bar");


SAMPLE_FIELD(KEYVAL, "key", "val");
SAMPLE_FIELD(FOOBAR, "foo", "bar");
SAMPLE_FIELD(FOO, "foo", "");


static inline ag_list *param_empty(void)
{
        return ag_alist_new_empty();
}


static inline ag_list *param_single(void)
{
        AG_AUTO(ag_field) *f = FIELD_KEYVAL();
        return ag_alist_new(f);
}


static ag_list *param_array(void)
{
        AG_AUTO(ag_field) *f1 = FIELD_KEYVAL();
        AG_AUTO(ag_field) *f2 = FIELD_FOOBAR();
        AG_AUTO(ag_field) *f3 = FIELD_FOO();

        const ag_field *f[] = {f1, f2, f3};
        return ag_alist_new_array(f, 3);
}


SAMPLE_REQUEST(GET0, AG_HTTP_METHOD_GET, AG_HTTP_MIME_TEXT_HTML,
    URL_HTTP_LOCALHOST_8080_NOPATH(), CLIENT_0(), param_empty());
SAMPLE_REQUEST(GET1, AG_HTTP_METHOD_GET, AG_HTTP_MIME_TEXT_PLAIN,
    URL_HTTPS_LOCALHOST_8080_NOPATH(), CLIENT_1(), param_single());
SAMPLE_REQUEST(GET2, AG_HTTP_METHOD_GET, AG_HTTP_MIME_TEXT_CSS,
    URL_HTTP_LOCALHOST_8080_FOO(), CLIENT_2(), param_array());


AG_METATEST_OBJECT_COPY(ag_http_request, REQUEST_GET0());
AG_METATEST_OBJECT_COPY(ag_http_request, REQUEST_GET1());
AG_METATEST_OBJECT_COPY(ag_http_request, REQUEST_GET2());


extern ag_test_suite *
test_suite_http_request(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_request interface");
}

