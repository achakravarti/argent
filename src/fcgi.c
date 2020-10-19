#include "./api.h"
#include <fcgiapp.h>
#include <string.h>
#include <ctype.h>


static ag_threadlocal struct {
    FCGX_Request *req;
    ag_http_handler *cbk;
    ag_string_t *param;
}  *g_http = NULL;


static inline void response_head(enum ag_http_mime type, 
        enum ag_http_status code)
{
    static const char *status[] = {
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

    static const char *mime[] = {
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

    FCGX_FPrintF(g_http->req->out, "Content-type: %s; charset=UTF-8\r\n"
            "Status: %s\r\n\r\n", mime[type], status[code]);
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
    ag_string_new(getenv("QUERY_STRING"));
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


extern void ag_http_init(void)
{
    ag_assert (!g_http);
    g_http = ag_memblock_new(sizeof *g_http);

    ag_require (!FCGX_Init(), AG_ERNO_HTTP_INIT, NULL);
    ag_require (!FCGX_InitRequest(g_http->req, 0, 0), AG_ERNO_HTTP_INIT, NULL);
    
    g_http->cbk = NULL;
    g_http->param = NULL;
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
    while (FCGX_Accept_r(g_http->req) >= 0) {
        ag_http_method() == AG_HTTP_METHOD_GET ? param_get : param_post();

        ag_assert (g_http->cbk);
        g_http->cbk();

        FCGX_Finish_r(g_http->req);
    }
}


extern ag_string_t *ag_http_env(const char *ev)
{
    ag_assert (g_http);
    const char *env = getenv(ev);

    return env ? ag_string_new(env) : ag_string_new_empty();
}


extern enum ag_http_method ag_http_method(void)
{
    ag_assert (g_http);
    const char *env = getenv("REQUEST_METHOD");
    ag_require (env, AG_ERNO_HTTP_METHOD, NULL);

    ag_string_smart_t *meth = ag_string_new(env);
    ag_string_lower(&meth);

    if (ag_string_eq(meth, "get"))
        return AG_HTTP_METHOD_GET;

    if (ag_string_eq(meth, "post"))
        return AG_HTTP_METHOD_POST;

    if (ag_string_eq(meth, "put"))
        return AG_HTTP_METHOD_PUT;

    if (ag_string_eq(meth, "patch"))
        return AG_HTTP_METHOD_PATCH;

    if (ag_string_eq(meth, "delete"))
        return AG_HTTP_METHOD_DELETE;

    ag_require (0, AG_ERNO_HTTP_METHOD, NULL);
    return AG_HTTP_METHOD_GET;
}


extern enum ag_http_mime ag_http_type(void)
{
    ag_assert (g_http);
    const char *env = getenv("CONTENT_TYPE");
    ag_require (env, AG_ERNO_HTTP_TYPE, NULL);

    ag_string_smart_t *type = ag_string_new(env);
    ag_string_lower(&type);

    if (ag_string_eq(type, "application/x-www-form-urlencoded"))
        return AG_HTTP_MIME_APPLICATION_FORM;

    if (ag_string_eq(type, "application/json"))
        return AG_HTTP_MIME_APPLICATION_JSON;

    if (ag_string_eq(type, "application/xml"))
        return AG_HTTP_MIME_APPLICATION_XML;

    if (ag_string_eq(type, "multipart/form-data"))
        return AG_HTTP_MIME_MULTIPART_FORM;

    if (ag_string_eq(type, "text/plain"))
        return AG_HTTP_MIME_TEXT_PLAIN;

    ag_require (0, AG_ERNO_HTTP_TYPE, NULL);
    return AG_HTTP_MIME_TEXT_PLAIN;
}


extern ag_string_t *ag_http_param(const char *key)
{
    ag_assert (g_http && g_http->param && key && *key);
    char *p = strstr(g_http->param, key);

    if (p)
        p += strlen(key);

    if (*p == '=')
        p++;
    else
        return ag_string_new_empty();

    char *val = ag_memblock_new(ag_string_len(g_http->param) + 1);

    while (*p && *p != '&') {
        if (param_encoded(p)) {
            *val++ = (16 * param_decode(p[1])) + param_decode(p[2]);
            p += 3;
        } else if (*p == '+') {
            *val++ = ' ';
            p++;
        } else
            *val++ = *p++;
    }

    *val = '\0';
    ag_string_t *ret = ag_string_new(val);
    ag_memblock_free((void **) &val);

    return ret;
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


extern void ag_http_respond_file(enum ag_http_mime type,
        enum ag_http_status code, const char *fpath)
{
    ag_assert (fpath && *fpath);
    FILE *file = fopen(fpath, "r");
    ag_require (file, AG_ERNO_HTTP_FILE, NULL);

    ag_assert (g_http);
    response_head(type, code);
    
    register char c;
    do {
        c = (char) fgetc(file);
        FCGX_FPrintF(g_http->req->out,"%c", c);
    } while (c != EOF);

    fclose(file);
}

