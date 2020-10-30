#include "./api.h"


/*******************************************************************************
 *                              METHOD ENUMERATION
 */


static ag_threadlocal const char *g_method[] = {
    "GET",
    "POST",
    "PUT",
    "PATCH",
    "DELETE",
};


extern const char *ag_http_method_str(enum ag_http_method meth)
{
    return g_method[meth];
}


extern enum ag_http_method ag_http_method_parse(const char *str)
{
    ag_string_smart_t *method = ag_string_new(str);
    ag_string_upper(&method);

    for (register int i = 0; i < __AG_HTTP_METHOD_LEN; i++) {
        if (ag_string_eq(method, g_method[i]))
            return i;
    }

    return AG_HTTP_METHOD_GET;
}


/*******************************************************************************
 *                               MIME ENUMERATION
 */


static ag_threadlocal const char *g_mime[] = {
    "application/x-www-form-urlencoded",
    "application/json",
    "application/octet-stream",
    "application/xml",
    "multipart/form-data",
    "text/css",
    "text/csv",
    "text/html",
    "text/javascript",
    "text/plain",
    "text/xml",
};


extern const char *ag_http_mime_str(enum ag_http_mime type)
{
    return g_mime[type];
}


extern enum ag_http_mime ag_http_mime_parse(const char *str)
{
    ag_string_smart_t *mime = ag_string_new(str);
    ag_string_lower(&mime);

    for (register int i = 0; i < __AG_HTTP_MIME_LEN; i++) {
        if (ag_string_eq(mime, g_mime[i]))
            return i;
    }

    return AG_HTTP_MIME_TEXT_PLAIN;
}




/*******************************************************************************
 *                              STATUS ENUMERATION
 */


static ag_threadlocal const char *g_status[] = {
    "200 OK",
    "201 Created",
    "202 Accepted",
    "204 No Content",
    "301 Moved Permanently",
    "302 Found",
    "303 See Other",
    "304 Not Modified",
    "307 Temporary Redirect",
    "400 Bad Request",
    "401 Unauthorized",
    "403 Forbidden",
    "404 Not Found",
    "405 Method Not Allowed",
    "406 Not Acceptable",
    "412 Precondition Failed",
    "415 Unsupported Media Type",
    "500 Internal Server Error",
    "501 Not Implemented",
};


extern const char *ag_http_status_str(enum ag_http_status code)
{
    ag_assert (code >= AG_HTTP_STATUS_200_OK && code < __AG_HTTP_STATUS_LEN);
    return g_status[code];
}


extern enum ag_http_status ag_http_status_parse(const char *str)
{
    ag_string_smart_t *status = ag_string_new(str);
    ag_string_proper(&status);

    for (register int i = AG_HTTP_STATUS_200_OK; i < __AG_HTTP_MIME_LEN; i++) {
        if (ag_string_eq(status, g_status[i]))
            return i;
    }

    return AG_HTTP_STATUS_501_NOT_IMPLEMENTED;
}
