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


static inline size_t len_sample(void)
{
        AG_AUTO(ag_http_url) *u = sample();
        AG_AUTO(ag_string) *s = ag_http_url_str(u);
        
        return ag_string_len(s);
}


static inline size_t len_sample_2(void)
{
        AG_AUTO(ag_http_url) *u = sample_2();
        AG_AUTO(ag_string) *s = ag_http_url_str(u);
        
        return ag_string_len(s);
}


static inline size_t sz_sample(void)
{
        AG_AUTO(ag_http_url) *u = sample();
        AG_AUTO(ag_string) *s = ag_http_url_str(u);
        
        return ag_string_sz(s);
}


static inline size_t sz_sample_2(void)
{
        AG_AUTO(ag_http_url) *u = sample_2();
        AG_AUTO(ag_string) *s = ag_http_url_str(u);
        
        return ag_string_sz(s);
}


AG_METATEST_OBJECT_COPY(ag_http_url, sample());
AG_METATEST_OBJECT_COPY(ag_http_url, sample_2());

AG_METATEST_OBJECT_CLONE(ag_http_url, sample());
AG_METATEST_OBJECT_CLONE(ag_http_url, sample_2());

AG_METATEST_OBJECT_RELEASE(ag_http_url, sample());
AG_METATEST_OBJECT_RELEASE(ag_http_url, sample_2());


AG_METATEST_OBJECT_CMP(ag_http_url, sample(), sample_2());
AG_METATEST_OBJECT_LT(ag_http_url, sample(), sample_2());
AG_METATEST_OBJECT_EQ(ag_http_url, sample(), sample_2());
AG_METATEST_OBJECT_GT(ag_http_url, sample(), sample_2());


AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, sample());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, sample_2());


AG_METATEST_OBJECT_TYPEID(ag_http_url, sample(), AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, sample_2(), AG_TYPEID_HTTP_URL);


AG_METATEST_OBJECT_UUID(ag_http_url, sample());
AG_METATEST_OBJECT_UUID(ag_http_url, sample_2());


AG_METATEST_OBJECT_REFC(ag_http_url, sample());
AG_METATEST_OBJECT_REFC(ag_http_url, sample_2());


AG_METATEST_OBJECT_LEN(ag_http_url, sample(), len_sample());
AG_METATEST_OBJECT_LEN(ag_http_url, sample_2(), len_sample_2());


AG_METATEST_OBJECT_SZ(ag_http_url, sample(), sz_sample());
AG_METATEST_OBJECT_SZ(ag_http_url, sample_2(), sz_sample_2());


extern ag_test_suite *
test_suite_http_url(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_url interface");
}

