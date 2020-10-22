#include "./api.h"
#include <fcgiapp.h>
#include <string.h>
#include <ctype.h>




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




/*******************************************************************************
 *                              REQUEST INTERNALS
 */


static ag_threadlocal struct {
    FCGX_Request *req;
    ag_http_handler *cbk;
    ag_string_t *param;
    bool hdr;
}  *g_http = NULL;


static inline ag_string_t *request_env(const char *key)
{
    const char *env = FCGX_GetParam(key, g_http->req->envp);
    return env ? ag_string_new(env) : ag_string_new_empty();
}


static inline bool param_encoded(const char *param)
{
    return *param == '%' && isxdigit(param[1]) && isxdigit(param[2]);
}


static inline char param_decode(const char c)
{
    if (c >= 'a')
        return c - ('a' - 'A');
    else if (c >= 'A')
        return c - ('A' - 10);
    else
        return c - '0';
}


static inline void param_get(void)
{
    ag_string_dispose(&g_http->param);
    g_http->param = request_env("QUERY_STRING");
}


static inline void param_update(const char *bfr)
{
    ag_string_dispose(&g_http->param);
    g_http->param = ag_string_new(bfr);
}

// https://blog.ijun.org/2013/01/nginx-with-fastcgi-and-c.html
static inline void param_post(void)
{
    size_t sz = 1024;
    char *bfr = ag_memblock_new(sz);

    size_t read = 0;
    int err = 0;

    do {
        sz = sz << 1;
        ag_memblock_resize((void **) &bfr, sz);

        read += FCGX_GetStr(bfr + read, sz - read, g_http->req->in);
        if (ag_unlikely (!read || (err = FCGX_GetError(g_http->req->in)))) {
            ag_memblock_free((void **) &bfr);
            param_update("");
        }

        ag_require (!err, AG_ERNO_HTTP_PARAM, NULL);
    } while(read == sz);
    
    bfr[read] = '\0';
    param_update(bfr);
    ag_memblock_free((void **) &bfr);
}




/*******************************************************************************
 *                              RESPONSE INTERNALS
 */


static inline void response_head(enum ag_http_mime type, 
        enum ag_http_status code)
{
    FCGX_FPrintF(g_http->req->out, "Content-type: %s; charset=UTF-8\r\n"
            "Status: %s\r\n\r\n", g_mime[type], g_status[code]);
}




/*******************************************************************************
 *                                 HTTP SERVER
 */


extern void ag_http_init(void)
{
    ag_assert (!g_http);
    g_http = ag_memblock_new(sizeof *g_http);
    g_http->req = ag_memblock_new(sizeof *g_http->req);

    ag_require (!FCGX_Init(), AG_ERNO_HTTP_INIT, NULL);
    ag_require (!FCGX_InitRequest(g_http->req, 0, 0), AG_ERNO_HTTP_INIT, NULL);
    
    g_http->cbk = NULL;
    g_http->param = NULL;
    g_http->hdr = false;
}


extern void ag_http_exit(void)
{
    if (g_http) {
        ag_string_dispose(&g_http->param);
        ag_memblock_free((void **) &g_http);
    };
}


extern void ag_http_register(ag_http_handler *req)
{
    ag_assert (g_http && req);
    g_http->cbk = req;
}


// https://cboard.cprogramming.com/c-programming/13752-how-parse-query_string.html
extern void ag_http_run(void)
{
    ag_assert (g_http);
    enum ag_http_method meth;

    while (FCGX_Accept_r(g_http->req) >= 0) {
        meth = ag_http_request_method();
        if (meth == AG_HTTP_METHOD_GET || meth == AG_HTTP_METHOD_DELETE)
            param_get();
        else
            param_post();

        ag_assert (g_http->cbk);
        g_http->cbk();

        FCGX_Finish_r(g_http->req);
    }
}


extern void ag_http_respond(enum ag_http_mime type, enum ag_http_status code,
        const char *fmt, ...)
{
    ag_assert (g_http && fmt && *fmt);
    response_head(type, code);
    
    va_list ap;
    va_start(ap, fmt);
    FCGX_FPrintF(g_http->req->out, fmt, ap);
    va_end(ap);
}


extern enum ag_http_method ag_http_request_method(void)
{
    ag_assert (g_http);
    ag_string_smart_t *env = request_env("REQUEST_METHOD");
    ag_string_upper(&env);

    return ag_http_method_parse(env);
}


extern enum ag_http_mime ag_http_request_type(void)
{
    ag_assert (g_http);
    ag_string_smart_t *env = request_env("CONTENT_TYPE");
    ag_string_lower(&env);

    return ag_http_mime_parse(env);
}


extern ag_string_t *ag_http_request_referer(void)
{
    ag_assert (g_http);
    return request_env("HTTP_REFERER");
}

extern ag_string_t *ag_http_request_user_agent(void)
{
    ag_assert (g_http);
    return request_env("HTTP_USER_AGENT");
}

extern ag_string_t *ag_http_request_user_ip(void)
{
    ag_assert (g_http);
    return request_env("REMOTE_ADDR");
}

extern ag_string_t *ag_http_request_user_host(void)
{
    ag_assert (g_http);
    return request_env("REMOTE_HOST");
}

extern ag_string_t *ag_http_request_user_port(void)
{
    ag_assert (g_http);
    return request_env("REMOTE_PORT");
}


extern bool ag_http_request_url_secure(void)
{
    ag_assert (g_http);
    ag_string_smart_t *https = request_env("HTTPS");
    ag_string_lower(&https);

    return ag_string_eq(https, "on");
}

extern ag_string_t *ag_http_request_url_host(void)
{
    ag_assert (g_http);
    return request_env("SERVER_NAME");
}

extern ag_string_t *ag_http_request_url_port(void)
{
    ag_assert (g_http);
    return request_env("SERVER_PORT");
}

extern ag_string_t *ag_http_request_url_path(void)
{
    ag_assert (g_http);
    return request_env("REQUEST_URI");
}


extern ag_string_t *ag_http_request_param(const char *key)
{
    ag_assert (g_http && g_http->param && key && *key);
    char *p = strstr(g_http->param, key);

    if (p) {
        p += strlen(key);
        
        if (*p == '=')
            p++;
        else
            return ag_string_new_empty();
    } else
        return ag_string_new_empty();

    char *val = ag_memblock_new(ag_string_sz(g_http->param) + 1);
    char *v = val;

    while (*p && *p != '&') {
        if (param_encoded(p)) {
            *v++ = (16 * param_decode(p[1])) + param_decode(p[2]);
            p += 3;
        } else if (*p == '+') {
            *v++ = ' ';
            p++;
        } else
            *v++ = *p++;
    }

    *v = '\0';
    ag_string_t *ret = ag_string_new(val);
    ag_memblock_free((void **) &val);

    return ret;
}


extern void ag_http_response_begin(enum ag_http_mime type, 
        enum ag_http_status code)
{
    ag_assert (g_http);
    FCGX_FPrintF(g_http->req->out, "Content-type: %s; charset=UTF-8\r\n"
            "Status: %s\r\n\r\n", g_mime[type], g_status[code]);
    
    g_http->hdr = true;
}


extern void ag_http_response_string(const char *str)
{
    ag_assert (g_http && g_http->hdr);
    FCGX_FPrintF(g_http->req->out, str);
}


extern void ag_http_response_file(const char *fpath)
{
    ag_assert (fpath && *fpath);
    FILE *file = fopen(fpath, "r");
    ag_require (file, AG_ERNO_HTTP_FILE, NULL);

    ag_assert (g_http && g_http->hdr);
    register char c;

    do {
        c = (char) fgetc(file);
        FCGX_FPrintF(g_http->req->out,"%c", c);
    } while (c != EOF);

    fclose(file);
}

