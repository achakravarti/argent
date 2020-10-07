#include "./api.h"
#include <fcgiapp.h>


static ag_threadlocal FCGX_Request  *g_request = NULL;


static ag_threadlocal ag_fcgi_handler *g_handler = NULL;


extern void ag_fcgi_init(void)
{
    ag_assert (!g_request);
    g_request = ag_memblock_new(sizeof *g_request);

    ag_require (!FCGX_Init(), AG_ERNO_FCGI_INIT, NULL);
    ag_require (!FCGX_InitRequest(g_request, 0, 0), AG_ERNO_FCGI_INIT, NULL);
}


extern void ag_fcgi_exit(void)
{
    ag_memblock_free((void **) &g_request);
}


extern void ag_fcgi_register(ag_fcgi_handler *req)
{
    ag_assert (g_request && req);
    g_handler = req; 
}


extern void ag_fcgi_run(void)
{
    ag_assert (g_request && g_handler);
    while (FCGX_Accept_r(g_request) >= 0) {
        g_handler();
        FCGX_Finish_r(g_request);
    }
}


extern void ag_fcgi_write(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    FCGX_FPrintF(g_request->out, fmt, ap);
    va_end(ap);
}


static inline void content_write(const char *mime, const char *fmt, va_list ap)
{
    FCGX_FPrintF(g_request->out, "Content-type: %s\r\nStatus: 200 OK\r\n\r\n",
            mime);
    FCGX_FPrintF(g_request->out, fmt, ap);
}


extern void ag_fcgi_write_html(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    content_write("text/html", fmt, ap);
    va_end(ap);
}


extern void ag_fcgi_write_json(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    content_write("application/json", fmt, ap);
    va_end(ap);
}

