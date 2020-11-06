#if (!defined __ARGENT_HTTP_PRIMITIVES_H__)
#define __ARGENT_HTTP_PRIMITIVES_H__


/*******************************************************************************
 *                                 HTTP METHOD
 */



enum ag_http_method {
    AG_HTTP_METHOD_GET,
    AG_HTTP_METHOD_POST,
    AG_HTTP_METHOD_PUT,
    AG_HTTP_METHOD_PATCH,
    AG_HTTP_METHOD_DELETE,
    __AG_HTTP_METHOD_LEN,
};


extern const char *ag_http_method_str(enum ag_http_method meth);
extern enum ag_http_method ag_http_method_parse(const char *str);


/*******************************************************************************
 *                                 HTTP STATUS
 */


// https://restfulapi.net/http-status-codes/
enum ag_http_status {
    AG_HTTP_STATUS_200_OK,
    AG_HTTP_STATUS_201_CREATED,
    AG_HTTP_STATUS_202_ACCEPTED,
    AG_HTTP_STATUS_204_NO_CONTENT,
    AG_HTTP_STATUS_301_MOVED_PERMANENTLY,
    AG_HTTP_STATUS_302_FOUND,
    AG_HTTP_STATUS_303_SEE_OTHER,
    AG_HTTP_STATUS_304_NOT_MODIFIED,
    AG_HTTP_STATUS_307_TEMPORARY_REDIRECT,
    AG_HTTP_STATUS_400_BAD_REQUEST,
    AG_HTTP_STATUS_401_UNAUTHORIZED,
    AG_HTTP_STATUS_403_FORBIDDEN,
    AG_HTTP_STATUS_404_NOT_FOUND,
    AG_HTTP_STATUS_405_METHOD_NOT_ALLOWED,
    AG_HTTP_STATUS_406_NOT_ACCEPTABLE,
    AG_HTTP_STATUS_412_PRECONDITION_FAILED,
    AG_HTTP_STATUS_415_UNSUPPORTED_MEDIA_TYPE,
    AG_HTTP_STATUS_500_INTERNAL_SERVER_ERROR,
    AG_HTTP_STATUS_501_NOT_IMPLEMENTED,
    __AG_HTTP_STATUS_LEN,
};


extern const char *ag_http_status_str(enum ag_http_status code);
extern enum ag_http_status ag_http_status_parse(const char *str);


/*******************************************************************************
 *                                  HTTP MIME
 */


// https://github.com/cujojs/rest/blob/master/docs/mime.md
// https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
enum ag_http_mime {
    AG_HTTP_MIME_APPLICATION_FORM,
    AG_HTTP_MIME_APPLICATION_JSON,
    AG_HTTP_MIME_APPLICATION_OCTET,
    AG_HTTP_MIME_APPLICATION_XML,
    AG_HTTP_MIME_MULTIPART_FORM,
    AG_HTTP_MIME_TEXT_CSS,
    AG_HTTP_MIME_TEXT_CSV,
    AG_HTTP_MIME_TEXT_HTML,
    AG_HTTP_MIME_TEXT_JS,
    AG_HTTP_MIME_TEXT_PLAIN,
    AG_HTTP_MIME_TEXT_XML,
    __AG_HTTP_MIME_LEN,
};

extern const char *ag_http_mime_str(enum ag_http_mime type);
extern enum ag_http_mime ag_http_mime_parse(const char *str);

#endif /* !defined __ARGENT_HTTP_PRIMITIVES_H__ */

