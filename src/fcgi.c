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


static inline void param_get(void)
{
    ag_string_dispose(&g_fcgi->param);
    ag_string_new(getenv("QUERY_STRING"));
}


static inline void param_update(const char *bfr)
{
    ag_string_dispose(&g_fcgi->param);
    g_fcgi->param = ag_string_new(bfr);
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

        read += FCGX_GetStr(bfr + read, sz - read, g_fcgi->req->in);
        if (ag_unlikely (!read || (err = FCGX_GetError(g_fcgi->req->in)))) {
            ag_memblock_free((void **) &bfr);
            param_update("");
        }

        ag_require (!err, AG_ERNO_FCGI_PARAM, NULL);
    } while(read == sz);
    
    bfr[read] = '\0';
    param_update(bfr);
    ag_memblock_free((void **) &bfr);
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


// https://cboard.cprogramming.com/c-programming/13752-how-parse-query_string.html
extern void ag_fcgi_run(void)
{
    ag_assert (g_fcgi);
    while (FCGX_Accept_r(g_fcgi->req) >= 0) {
        ag_assert (getenv("REQUEST_METHOD"));
        !strcmp(getenv("REQUEST_METHOD"), "GET") ? param_get : param_post();

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

