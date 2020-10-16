#include "./api.h"
#include <fcgiapp.h>
#include <string.h>
#include <ctype.h>


static ag_threadlocal struct {
    FCGX_Request *req;
    ag_fcgi_handler *cbk;
    char *getp;
    char *postp;
}  *g_fcgi = NULL;


static inline void content_write(const char *mime, const char *fmt, va_list ap)
{
    FCGX_FPrintF(g_fcgi->req->out, "Content-type: %s\r\nStatus: 200 OK\r\n\r\n",
            mime);
    FCGX_FPrintF(g_fcgi->req->out, fmt, ap);
}


static inline bool url_encoded(const char *url)
{
    return *url == '%' && isxdigit(url[1]) && isxdigit(url[2]);
}


static inline char url_depercent(const char c)
{
    if (c >= 'a')
        return c - ('a' - 'A');
    else if (c >= 'A')
        return c - ('A' - 10);
    else
        return c - '0';
}


// https://stackoverflow.com/questions/2673207
static ag_string_t *url_decode(const char *url)
{
    ag_assert (url && *url);
    const char *src = url;
    char *dst = ag_memblock_new(strlen(url) + 1);

    while (*src) {
        if (url_encoded(src)) {
            *dst++ = (16 * url_depercent(src[1])) + url_depercent(src[2]);
            src += 3;
        } else if (*src == '+') {
            *dst++ = ' ';
            src++;
        } else
            *dst++ = *src++;
    }

    ag_string_t *ret = ag_string_new(dst);
    ag_memblock_free((void **) &dst);

    return ret;
}




extern void ag_fcgi_init(void)
{
    ag_assert (!g_fcgi);
    g_fcgi = ag_memblock_new(sizeof *g_fcgi);

    ag_require (!FCGX_Init(), AG_ERNO_FCGI_INIT, NULL);
    ag_require (!FCGX_InitRequest(g_fcgi->req, 0, 0), AG_ERNO_FCGI_INIT, NULL);
    
    g_fcgi->cbk = NULL;
    g_fcgi->getp = g_fcgi->postp = NULL;
}


extern void ag_fcgi_exit(void)
{
    if (g_fcgi) {
        ag_memblock_free((void **) &g_fcgi->getp);
        ag_memblock_free((void **) &g_fcgi->postp);
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

