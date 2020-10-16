#include "./api.h"
#include <fcgiapp.h>
#include <string.h>
#include <ctype.h>


static ag_threadlocal struct {
    FCGX_Request *req;
    ag_fcgi_handler *cbk;
    ag_string_t *param;
}  *g_fcgi = NULL;


static inline void content_write(const char *mime, const char *fmt, va_list ap)
{
    FCGX_FPrintF(g_fcgi->req->out, "Content-type: %s; charset=UTF-8\r\n"
            "Status: 200 OK\r\n\r\n", mime);
    FCGX_FPrintF(g_fcgi->req->out, fmt, ap);
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


#if 0
// https://stackoverflow.com/questions/2673207
static ag_string_t *param_decode(const char *param)
{
    const char *p = param;
    char *bfr = ag_memblock_new(strlen(p) + 1);

    while (*p) {
        if (param_encoded(p)) {
            *bfr++ = (16 * char_decode(p[1])) + char_decode(p[2]);
            p += 3;
        } else if (*p == '+') {
            *bfr++ = ' ';
            p++;
        } else
            *bfr++ = *p++;
    }

    ag_string_t *ret = ag_string_new(bfr);
    ag_memblock_free((void **) &bfr);

    return ret;
}
#endif


// https://cboard.cprogramming.com/c-programming/13752-how-parse-query_string.html
static inline void param_read(void)
{
    if (!strcmp(getenv("REQUEST_METHOD"), "GET")) {
        ag_memblock_free((void **) &g_fcgi->param);
        ag_string_new(getenv("QUERY_STRING"));
    } else {;
        // TODO
    }
}


extern void ag_fcgi_init(void)
{
    ag_assert (!g_fcgi);
    g_fcgi = ag_memblock_new(sizeof *g_fcgi);

    ag_require (!FCGX_Init(), AG_ERNO_FCGI_INIT, NULL);
    ag_require (!FCGX_InitRequest(g_fcgi->req, 0, 0), AG_ERNO_FCGI_INIT, NULL);
    
    g_fcgi->cbk = NULL;
    g_fcgi->param = NULL;
}


extern void ag_fcgi_exit(void)
{
    if (g_fcgi) {
        ag_string_dispose(&g_fcgi->param);
        ag_memblock_free((void **) &g_fcgi);
    };

}


extern void ag_fcgi_register(ag_fcgi_handler *req)
{
    ag_assert (g_fcgi && req);
    g_fcgi->cbk = req;
}


extern void ag_fcgi_run(void)
{
    ag_assert (g_fcgi);
    while (FCGX_Accept_r(g_fcgi->req) >= 0) {
        ag_assert (getenv("REQUEST_METHOD"));
        param_read();

        ag_assert (g_fcgi->cbk);
        g_fcgi->cbk();

        FCGX_Finish_r(g_fcgi->req);
    }
}


extern ag_string_t *ag_fcgi_env(const char *ev)
{
    ag_assert (g_fcgi);
    const char *env = getenv(ev);

    return env ? ag_string_new(env) : ag_string_new_empty();
}


extern ag_string_t *ag_fcgi_param(const char *key)
{
    ag_assert (g_fcgi && g_fcgi->param && key && *key);
    char *p = strstr(g_fcgi->param, key);

    if (p)
        p += strlen(key);

    if (*p == '=')
        p++;
    else
        return ag_string_new_empty();

    char *val = ag_memblock_new(ag_string_len(g_fcgi->param) + 1);

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





extern void ag_fcgi_write(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    ag_assert (g_fcgi);
    FCGX_FPrintF(g_fcgi->req->out, fmt, ap);
    va_end(ap);
}


extern void ag_fcgi_write_html(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    ag_assert (g_fcgi);
    content_write("text/html", fmt, ap);
    va_end(ap);
}


extern void ag_fcgi_write_json(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    ag_assert (g_fcgi);
    content_write("application/json", fmt, ap);
    va_end(ap);
}

