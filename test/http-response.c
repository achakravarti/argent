#include "./http.h" 
#include "./object.h"
#include "./test.h"




#define __AG_TEST_SUITE_ID__ 12

        
static inline size_t    sample_len(ag_http_response *);
static inline size_t    sample_sz(ag_http_response *);
static inline ag_hash   sample_hash(ag_http_response *);




AG_SAMPLE_HTTP_RESPONSE_EMPTY(HTML_200_EMPTY, AG_HTTP_MIME_TEXT_HTML,
    AG_HTTP_STATUS_200_OK);
AG_SAMPLE_HTTP_RESPONSE(JSON_201, AG_HTTP_MIME_APPLICATION_JSON,
    AG_HTTP_STATUS_201_CREATED, "{key:foo, val:bar}");




AG_METATEST_OBJECT_COPY(ag_http_response, HTML_200_EMPTY());
AG_METATEST_OBJECT_COPY(ag_http_response, JSON_201());


AG_METATEST_OBJECT_CLONE(ag_http_response, HTML_200_EMPTY());
AG_METATEST_OBJECT_CLONE(ag_http_response, JSON_201());


AG_METATEST_OBJECT_RELEASE(ag_http_response, HTML_200_EMPTY());
AG_METATEST_OBJECT_RELEASE(ag_http_response, JSON_201());


AG_METATEST_OBJECT_CMP(ag_http_response, JSON_201(), HTML_200_EMPTY());
AG_METATEST_OBJECT_LT(ag_http_response, JSON_201(), HTML_200_EMPTY());
AG_METATEST_OBJECT_EQ(ag_http_response, JSON_201(), HTML_200_EMPTY());
AG_METATEST_OBJECT_GT(ag_http_response, JSON_201(), HTML_200_EMPTY());


AG_METATEST_OBJECT_EMPTY(ag_http_response, HTML_200_EMPTY());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_response, JSON_201());


AG_METATEST_OBJECT_VALID(ag_http_response, HTML_200_EMPTY());
AG_METATEST_OBJECT_VALID(ag_http_response, JSON_201());
       

AG_METATEST_OBJECT_TYPEID(ag_http_response, HTML_200_EMPTY(),
    AG_TYPEID_HTTP_RESPONSE);
AG_METATEST_OBJECT_TYPEID(ag_http_response, JSON_201(),
    AG_TYPEID_HTTP_RESPONSE);


AG_METATEST_OBJECT_UUID(ag_http_response, HTML_200_EMPTY());
AG_METATEST_OBJECT_UUID(ag_http_response, JSON_201());


AG_METATEST_OBJECT_REFC(ag_http_response, HTML_200_EMPTY());
AG_METATEST_OBJECT_REFC(ag_http_response, JSON_201());


AG_METATEST_OBJECT_LEN(ag_http_response, HTML_200_EMPTY(),
    sample_len(HTML_200_EMPTY()));
AG_METATEST_OBJECT_LEN(ag_http_response, JSON_201(), sample_len(JSON_201()));


AG_METATEST_OBJECT_SZ(ag_http_response, HTML_200_EMPTY(),
    sample_sz(HTML_200_EMPTY()));
AG_METATEST_OBJECT_SZ(ag_http_response, JSON_201(), sample_sz(JSON_201()));


AG_METATEST_OBJECT_HASH(ag_http_response, HTML_200_EMPTY(),
    sample_hash(HTML_200_EMPTY()));
AG_METATEST_OBJECT_HASH(ag_http_response, JSON_201(), sample_hash(JSON_201()));


AG_METATEST_OBJECT_STR(ag_http_response, HTML_200_EMPTY(),
    "Content-type: text/html; charset=UTF-8\r\nStatus: 200 (OK)\r\n\r\n");
AG_METATEST_OBJECT_STR(ag_http_response, JSON_201(), 
    "Content-type: application/json; charset=UTF-8\r\nStatus: 201 (Created)"
    "\r\n\r\n{key:foo, val:bar}");





extern ag_test_suite *
test_suite_http_response(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_response interface");
}


static inline size_t
sample_len(ag_http_response *ctx)
{
        AG_AUTO(ag_http_response) *r = ctx;
        AG_AUTO(ag_string) *s = ag_http_response_body(r);

        return ag_string_len(s);
}


static inline size_t
sample_sz(ag_http_response *ctx)
{
        AG_AUTO(ag_http_response) *r = ctx;
        AG_AUTO(ag_string) *s = ag_http_response_body(r);

        return ag_string_sz(s);
}


static inline ag_hash
sample_hash(ag_http_response *ctx)
{
        AG_AUTO(ag_http_response) *r = ctx;
        AG_AUTO(ag_string) *s = ag_http_response_str(r);

        return ag_hash_new_str(s);
}

