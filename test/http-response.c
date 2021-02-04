#include "./http.h" 
#include "./object.h"
#include "./test.h"




#define __AG_TEST_SUITE_ID__ 13




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



extern ag_test_suite *
test_suite_http_response(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_response interface");
}
