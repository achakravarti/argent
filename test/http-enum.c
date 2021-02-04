#include "./http.h"
#include "./test.h"




#define __AG_TEST_SUITE_ID__ 8




/*******************************************************************************
 * The test cases for the ag_http_mime enumeration are metaprogrammatically
 * generated through the metatest macros AG_METATEST_HTTP_MIME_PARSE() and
 * AG_METATEST_HTTP_MIME_STR().
 *
 * We generate test cases for each enumerator in the ag_http_mime enumeration,
 * checking whether ag_http_mime_parse() and ag_http_mime_str() work as expected
 * in each case.
 *
 * In the case of the tests for ag_http_mime_parse(), we test against three
 * sample string representations for each enumerator:
 *   - the lowercase form
 *   - the uppercase form
 *   - the proper case form
 *
 * See the following files for more details:
 *   - include/http.h
 *   - test/http.h
 */


AG_METATEST_HTTP_MIME_PARSE("application/x-www-form-urlencoded",
    AG_HTTP_MIME_APPLICATION_FORM);
AG_METATEST_HTTP_MIME_PARSE("APPLICATION/X-WWW-FORM-URLENCODED",
    AG_HTTP_MIME_APPLICATION_FORM);
AG_METATEST_HTTP_MIME_PARSE("Application/X-Www-Form-Urlencoded",
    AG_HTTP_MIME_APPLICATION_FORM);
AG_METATEST_HTTP_MIME_PARSE("application/json", AG_HTTP_MIME_APPLICATION_JSON);
AG_METATEST_HTTP_MIME_PARSE("APPLICATION/JSON", AG_HTTP_MIME_APPLICATION_JSON);
AG_METATEST_HTTP_MIME_PARSE("Application/Json", AG_HTTP_MIME_APPLICATION_JSON);
AG_METATEST_HTTP_MIME_PARSE("application/octet-stream",
    AG_HTTP_MIME_APPLICATION_OCTET);
AG_METATEST_HTTP_MIME_PARSE("APPLICATION/OCTET-STREAM",
    AG_HTTP_MIME_APPLICATION_OCTET);
AG_METATEST_HTTP_MIME_PARSE("Application/Octet-stream",
    AG_HTTP_MIME_APPLICATION_OCTET);
AG_METATEST_HTTP_MIME_PARSE("application/xml", AG_HTTP_MIME_APPLICATION_XML);
AG_METATEST_HTTP_MIME_PARSE("APPLICATION/XML", AG_HTTP_MIME_APPLICATION_XML);
AG_METATEST_HTTP_MIME_PARSE("Application/Xml", AG_HTTP_MIME_APPLICATION_XML);
AG_METATEST_HTTP_MIME_PARSE("multipart/form-data", AG_HTTP_MIME_MULTIPART_FORM);
AG_METATEST_HTTP_MIME_PARSE("MULTIPART/FORM-DATA", AG_HTTP_MIME_MULTIPART_FORM);
AG_METATEST_HTTP_MIME_PARSE("Multipart/fOrm-data", AG_HTTP_MIME_MULTIPART_FORM);
AG_METATEST_HTTP_MIME_PARSE("text/css", AG_HTTP_MIME_TEXT_CSS);
AG_METATEST_HTTP_MIME_PARSE("TEXT/CSS", AG_HTTP_MIME_TEXT_CSS);
AG_METATEST_HTTP_MIME_PARSE("Text/Css", AG_HTTP_MIME_TEXT_CSS);
AG_METATEST_HTTP_MIME_PARSE("text/csv", AG_HTTP_MIME_TEXT_CSV);
AG_METATEST_HTTP_MIME_PARSE("TEXT/CSV", AG_HTTP_MIME_TEXT_CSV);
AG_METATEST_HTTP_MIME_PARSE("Text/Csv", AG_HTTP_MIME_TEXT_CSV);
AG_METATEST_HTTP_MIME_PARSE("text/html", AG_HTTP_MIME_TEXT_HTML);
AG_METATEST_HTTP_MIME_PARSE("TEXT/HTML", AG_HTTP_MIME_TEXT_HTML);
AG_METATEST_HTTP_MIME_PARSE("Text/Html", AG_HTTP_MIME_TEXT_HTML);
AG_METATEST_HTTP_MIME_PARSE("text/javascript", AG_HTTP_MIME_TEXT_JS);
AG_METATEST_HTTP_MIME_PARSE("TEXT/JAVASCRIPT", AG_HTTP_MIME_TEXT_JS);
AG_METATEST_HTTP_MIME_PARSE("Text/Javascript", AG_HTTP_MIME_TEXT_JS);
AG_METATEST_HTTP_MIME_PARSE("text/plain", AG_HTTP_MIME_TEXT_PLAIN);
AG_METATEST_HTTP_MIME_PARSE("TEXT/PLAIN", AG_HTTP_MIME_TEXT_PLAIN);
AG_METATEST_HTTP_MIME_PARSE("Text/Plain", AG_HTTP_MIME_TEXT_PLAIN);
AG_METATEST_HTTP_MIME_PARSE("text/xml", AG_HTTP_MIME_TEXT_XML);
AG_METATEST_HTTP_MIME_PARSE("TEXT/XML", AG_HTTP_MIME_TEXT_XML);
AG_METATEST_HTTP_MIME_PARSE("Text/Xml", AG_HTTP_MIME_TEXT_XML);


AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_APPLICATION_FORM,
    "application/x-www-form-urlencoded");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_APPLICATION_JSON, "application/json");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_APPLICATION_OCTET,
    "application/octet-stream");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_APPLICATION_XML, "application/xml");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_MULTIPART_FORM, "multipart/form-data");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_TEXT_CSS, "text/css");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_TEXT_CSV, "text/csv");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_TEXT_HTML, "text/html");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_TEXT_JS, "text/javascript");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_TEXT_PLAIN, "text/plain");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_TEXT_XML, "text/xml");




/*******************************************************************************
 * The test caes for the ag_http_method enumeration are metaprogrammatically
 * generated through the metatest macros AG_METATEST_HTTP_METHOD_PARSE() and
 * AG_METATEST_HTTP_STR().
 *
 * We generate test cases for each enumerator in the ag_http_method enumeration,
 * checking wheter ag_http_method_parse() and ag_http_method_str() work as
 * expected in each case.
 *
 * In the case of the tests for ag_http_method_parse(), we test against three
 * sample string representations for each enumerator:
 *   - the lowercase form
 *   - the uppercase form
 *   - the proper case form
 *
 * See the following files for more details:
 *   - include/http.h
 *   - test/http.h
 */


AG_METATEST_HTTP_METHOD_PARSE("get", AG_HTTP_METHOD_GET);
AG_METATEST_HTTP_METHOD_PARSE("GET", AG_HTTP_METHOD_GET);
AG_METATEST_HTTP_METHOD_PARSE("Get", AG_HTTP_METHOD_GET);
AG_METATEST_HTTP_METHOD_PARSE("post", AG_HTTP_METHOD_POST);
AG_METATEST_HTTP_METHOD_PARSE("POST", AG_HTTP_METHOD_POST);
AG_METATEST_HTTP_METHOD_PARSE("Post", AG_HTTP_METHOD_POST);
AG_METATEST_HTTP_METHOD_PARSE("PUT", AG_HTTP_METHOD_PUT);
AG_METATEST_HTTP_METHOD_PARSE("put", AG_HTTP_METHOD_PUT);
AG_METATEST_HTTP_METHOD_PARSE("Put", AG_HTTP_METHOD_PUT);
AG_METATEST_HTTP_METHOD_PARSE("PATCH", AG_HTTP_METHOD_PATCH);
AG_METATEST_HTTP_METHOD_PARSE("patch", AG_HTTP_METHOD_PATCH);
AG_METATEST_HTTP_METHOD_PARSE("Patch", AG_HTTP_METHOD_PATCH);
AG_METATEST_HTTP_METHOD_PARSE("DELETE", AG_HTTP_METHOD_DELETE);
AG_METATEST_HTTP_METHOD_PARSE("delete", AG_HTTP_METHOD_DELETE);
AG_METATEST_HTTP_METHOD_PARSE("Delete", AG_HTTP_METHOD_DELETE);


AG_METATEST_HTTP_METHOD_STR(AG_HTTP_METHOD_GET, "GET");
AG_METATEST_HTTP_METHOD_STR(AG_HTTP_METHOD_POST, "POST");
AG_METATEST_HTTP_METHOD_STR(AG_HTTP_METHOD_PUT, "PUT");
AG_METATEST_HTTP_METHOD_STR(AG_HTTP_METHOD_PATCH, "PATCH");
AG_METATEST_HTTP_METHOD_STR(AG_HTTP_METHOD_DELETE, "DELETE");




extern ag_test_suite *
test_suite_http_enum(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http enumeration interfaces");
}

