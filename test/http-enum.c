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



AG_METATEST_HTTP_STATUS_PARSE("200 (OK)", AG_HTTP_STATUS_200_OK);
AG_METATEST_HTTP_STATUS_PARSE("200 (ok)", AG_HTTP_STATUS_200_OK);
AG_METATEST_HTTP_STATUS_PARSE("200 (Ok)", AG_HTTP_STATUS_200_OK);


AG_METATEST_HTTP_STATUS_PARSE("201 (CREATED)", AG_HTTP_STATUS_201_CREATED);
AG_METATEST_HTTP_STATUS_PARSE("201 (created)", AG_HTTP_STATUS_201_CREATED);
AG_METATEST_HTTP_STATUS_PARSE("201 (Created)", AG_HTTP_STATUS_201_CREATED);


AG_METATEST_HTTP_STATUS_PARSE("202 (ACCEPTED)", AG_HTTP_STATUS_202_ACCEPTED);
AG_METATEST_HTTP_STATUS_PARSE("202 (accepted)", AG_HTTP_STATUS_202_ACCEPTED);
AG_METATEST_HTTP_STATUS_PARSE("202 (Accepted)", AG_HTTP_STATUS_202_ACCEPTED);


AG_METATEST_HTTP_STATUS_PARSE("204 (NO CONTENT)",
    AG_HTTP_STATUS_204_NO_CONTENT);
AG_METATEST_HTTP_STATUS_PARSE("204 (no content)",
    AG_HTTP_STATUS_204_NO_CONTENT);
AG_METATEST_HTTP_STATUS_PARSE("204 (No Content)",
    AG_HTTP_STATUS_204_NO_CONTENT);


AG_METATEST_HTTP_STATUS_PARSE("301 (MOVED PERMANENTLY)",
    AG_HTTP_STATUS_301_MOVED_PERMANENTLY);
AG_METATEST_HTTP_STATUS_PARSE("301 (moved permanently)",
    AG_HTTP_STATUS_301_MOVED_PERMANENTLY);
AG_METATEST_HTTP_STATUS_PARSE("301 (Moved Permanently)",
    AG_HTTP_STATUS_301_MOVED_PERMANENTLY);


AG_METATEST_HTTP_STATUS_PARSE("302 (FOUND)", AG_HTTP_STATUS_302_FOUND);
AG_METATEST_HTTP_STATUS_PARSE("302 (found)", AG_HTTP_STATUS_302_FOUND);
AG_METATEST_HTTP_STATUS_PARSE("302 (Found)", AG_HTTP_STATUS_302_FOUND);


AG_METATEST_HTTP_STATUS_PARSE("303 (SEE OTHER)", AG_HTTP_STATUS_303_SEE_OTHER);
AG_METATEST_HTTP_STATUS_PARSE("303 (see other)", AG_HTTP_STATUS_303_SEE_OTHER);
AG_METATEST_HTTP_STATUS_PARSE("303 (See Other)", AG_HTTP_STATUS_303_SEE_OTHER);


AG_METATEST_HTTP_STATUS_PARSE("304 (NOT MODIFIED)",
    AG_HTTP_STATUS_304_NOT_MODIFIED);
AG_METATEST_HTTP_STATUS_PARSE("304 (not modified)",
    AG_HTTP_STATUS_304_NOT_MODIFIED);
AG_METATEST_HTTP_STATUS_PARSE("304 (Not Modified)",
    AG_HTTP_STATUS_304_NOT_MODIFIED);


AG_METATEST_HTTP_STATUS_PARSE("307 (TEMPORARY REDIRECT)",
    AG_HTTP_STATUS_307_TEMPORARY_REDIRECT);
AG_METATEST_HTTP_STATUS_PARSE("307 (temporary redirect)",
    AG_HTTP_STATUS_307_TEMPORARY_REDIRECT);
AG_METATEST_HTTP_STATUS_PARSE("307 (Temporary Redirect)",
    AG_HTTP_STATUS_307_TEMPORARY_REDIRECT);


AG_METATEST_HTTP_STATUS_PARSE("400 (BAD REQUEST)",
    AG_HTTP_STATUS_400_BAD_REQUEST);
AG_METATEST_HTTP_STATUS_PARSE("400 (bad request)",
    AG_HTTP_STATUS_400_BAD_REQUEST);
AG_METATEST_HTTP_STATUS_PARSE("400 (Bad Request)",
    AG_HTTP_STATUS_400_BAD_REQUEST);


AG_METATEST_HTTP_STATUS_PARSE("401 (UNAUTHORIZED)",
    AG_HTTP_STATUS_401_UNAUTHORIZED);
AG_METATEST_HTTP_STATUS_PARSE("401 (unauthorized)",
    AG_HTTP_STATUS_401_UNAUTHORIZED);
AG_METATEST_HTTP_STATUS_PARSE("401 (Unauthorized)",
    AG_HTTP_STATUS_401_UNAUTHORIZED);


AG_METATEST_HTTP_STATUS_PARSE("403 (FORBIDDEN)", AG_HTTP_STATUS_403_FORBIDDEN);
AG_METATEST_HTTP_STATUS_PARSE("403 (forbidden)", AG_HTTP_STATUS_403_FORBIDDEN);
AG_METATEST_HTTP_STATUS_PARSE("403 (Forbidden)", AG_HTTP_STATUS_403_FORBIDDEN);


AG_METATEST_HTTP_STATUS_PARSE("404 (NOT FOUND)", AG_HTTP_STATUS_404_NOT_FOUND);
AG_METATEST_HTTP_STATUS_PARSE("404 (not found)", AG_HTTP_STATUS_404_NOT_FOUND);
AG_METATEST_HTTP_STATUS_PARSE("404 (Not Found)", AG_HTTP_STATUS_404_NOT_FOUND);


AG_METATEST_HTTP_STATUS_PARSE("405 (METHOD NOT ALLOWED)",
    AG_HTTP_STATUS_405_METHOD_NOT_ALLOWED);
AG_METATEST_HTTP_STATUS_PARSE("405 (method not allowed)",
    AG_HTTP_STATUS_405_METHOD_NOT_ALLOWED);
AG_METATEST_HTTP_STATUS_PARSE("405 (Method Not Allowed)",
    AG_HTTP_STATUS_405_METHOD_NOT_ALLOWED);


AG_METATEST_HTTP_STATUS_PARSE("406 (NOT ACCEPTABLE)",
    AG_HTTP_STATUS_406_NOT_ACCEPTABLE);
AG_METATEST_HTTP_STATUS_PARSE("406 (not acceptable)",
    AG_HTTP_STATUS_406_NOT_ACCEPTABLE);
AG_METATEST_HTTP_STATUS_PARSE("406 (Not Acceptable)",
    AG_HTTP_STATUS_406_NOT_ACCEPTABLE);


AG_METATEST_HTTP_STATUS_PARSE("412 (PRECONDITION FAILED)",
    AG_HTTP_STATUS_412_PRECONDITION_FAILED);
AG_METATEST_HTTP_STATUS_PARSE("412 (precondition failed)",
    AG_HTTP_STATUS_412_PRECONDITION_FAILED);
AG_METATEST_HTTP_STATUS_PARSE("412 (Precondition Failed)",
    AG_HTTP_STATUS_412_PRECONDITION_FAILED);


AG_METATEST_HTTP_STATUS_PARSE("415 (UNSUPPORTED MEDIA TYPE)",
    AG_HTTP_STATUS_415_UNSUPPORTED_MEDIA_TYPE);
AG_METATEST_HTTP_STATUS_PARSE("415 (unsupported media type)",
    AG_HTTP_STATUS_415_UNSUPPORTED_MEDIA_TYPE);
AG_METATEST_HTTP_STATUS_PARSE("415 (Unsupported Media Type)",
    AG_HTTP_STATUS_415_UNSUPPORTED_MEDIA_TYPE);


AG_METATEST_HTTP_STATUS_PARSE("500 (INTERNAL SERVER ERROR)",
    AG_HTTP_STATUS_500_INTERNAL_SERVER_ERROR);
AG_METATEST_HTTP_STATUS_PARSE("500 (internal server error)",
    AG_HTTP_STATUS_500_INTERNAL_SERVER_ERROR);
AG_METATEST_HTTP_STATUS_PARSE("500 (Internal Server Error)",
    AG_HTTP_STATUS_500_INTERNAL_SERVER_ERROR);


AG_METATEST_HTTP_STATUS_PARSE("501 (NOT IMPLEMENTED)",
    AG_HTTP_STATUS_501_NOT_IMPLEMENTED);
AG_METATEST_HTTP_STATUS_PARSE("501 (not implemented)",
    AG_HTTP_STATUS_501_NOT_IMPLEMENTED);
AG_METATEST_HTTP_STATUS_PARSE("501 (Not Implemented)",
    AG_HTTP_STATUS_501_NOT_IMPLEMENTED);


AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_200_OK, "200 (OK)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_201_CREATED, "201 (Created)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_202_ACCEPTED, "202 (Accepted)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_204_NO_CONTENT, "204 (No Content)");


AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_301_MOVED_PERMANENTLY,
    "301 (Moved Permanently)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_302_FOUND, "302 (Found)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_303_SEE_OTHER, "303 (See Other)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_304_NOT_MODIFIED,
    "304 (Not Modified)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_307_TEMPORARY_REDIRECT,
    "307 (Temporary Redirect)");


AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_400_BAD_REQUEST,
    "400 (Bad Request)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_401_UNAUTHORIZED,
    "401 (Unauthorized)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_403_FORBIDDEN,
    "403 (Forbidden)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_404_NOT_FOUND,
    "404 (Not Found)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_405_METHOD_NOT_ALLOWED,
    "405 (Method Not Allowed)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_406_NOT_ACCEPTABLE,
    "406 (Not Acceptable)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_412_PRECONDITION_FAILED, 
    "412 (Precondition Failed)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_415_UNSUPPORTED_MEDIA_TYPE,
    "415 (Unsupported Media Type)");


AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_500_INTERNAL_SERVER_ERROR,
    "500 (Internal Server Error)");
AG_METATEST_HTTP_STATUS_STR(AG_HTTP_STATUS_501_NOT_IMPLEMENTED,
    "501 (Not Implemented)");




extern ag_test_suite *
test_suite_http_enum(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http enumeration interfaces");
}

