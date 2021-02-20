#include "../include/argent.h"
#include <fcgiapp.h>


struct entry {
        ag_string       *path;
        ag_string       *dso;
        ag_string       *sym;
};

static void entry_release(void *hnd)
{
        AG_ASSERT (hnd);

        struct entry *e = hnd;
        ag_string_release(&e->path);
        ag_string_release(&e->dso);
        ag_string_release(&e->sym);
}


static AG_THREADLOCAL struct {
        FCGX_Request    *req;
        ag_registry     *reg;
} *g_http = NULL;

#define HTTP_LISTENSOCK_FILENO  0
#define HTTP_LISTENSOCK_FLAGS   0


extern void
ag_http_server_init(void)
{
        AG_ASSERT (!g_http);

        g_http = ag_memblock_new(sizeof *g_http);
        g_http->reg = ag_registry_new(entry_release);

        AG_REQUIRE (!FCGX_Init(), AG_ERNO_HTTP);
        AG_REQUIRE (!FCGX_InitRequest(g_http->req, HTTP_LISTENSOCK_FILENO,
            HTTP_LISTENSOCK_FLAGS), AG_ERNO_HTTP);
}


extern void
ag_http_server_exit(void)
{
        if (AG_UNLIKELY (!g_http))
                return;

        ag_registry_release(&g_http->reg);
}


extern void
ag_http_server_register(const char *path, const char *sym)
{
        AG_ASSERT_STR (path);
        AG_ASSERT_STR (sym);
        AG_ASSERT_PTR (g_http);

        struct entry *e = ag_memblock_new(sizeof *e);
        e->path = ag_string_new(path);
        e->sym = ag_string_new(sym);
        e->dso = NULL;

        ag_registry_push(g_http->reg, ag_hash_new_str(path), e);
}

extern void
ag_http_server_register_dso(const char *path, const char *dso, const char *sym)
{
        AG_ASSERT_STR (path);
        AG_ASSERT_STR (dso);
        AG_ASSERT_STR (sym);
        AG_ASSERT_PTR (g_http);

        struct entry *e = ag_memblock_new(sizeof *e);
        e->path = ag_string_new(path);
        e->sym = ag_string_new(sym);
        e->dso = ag_string_new(dso);

        ag_registry_push(g_http->reg, ag_hash_new_str(path), e);
}


static const char *
http_env(const char *key)
{
        AG_ASSERT_PTR (g_http);
        AG_ASSERT_STR (key);

        const char *v = FCGX_GetParam(key, g_http->req->envp);
        return v ? v : "";
}


static enum ag_http_method
http_parse_method(void)
{
        AG_ASSERT_PTR (g_http);

        return ag_http_method_parse(http_env("REQUEST_METHOD"));
}


static ag_http_url *
http_parse_url(void)
{
        AG_ASSERT_PTR (g_http);

        AG_AUTO(ag_string) *s = ag_string_new(http_env("HTTPS"));
        AG_AUTO(ag_string) *s2 = ag_string_lower(s);
        bool secure = ag_string_eq(s2, "on");

        const char *host = http_env("SERVER_NAME");

        AG_AUTO(ag_string) *p = ag_string_new(http_env("REQUEST_URI"));
        AG_AUTO(ag_string) *p2 = ag_string_split(p, "?");
        AG_AUTO(ag_string) *path = ag_string_split(p2, "#");

        ag_uint port;

        return port ? ag_http_url_new(secure, host, port, path)
            : ag_http_url_new_noport(secure, host, path);
}


extern void
ag_http_server_run(void)
{
        AG_ASSERT_PTR (g_http);

        while (FCGX_Accept_r(g_http->req) >= 0) {
                /*
                enum ag_http_method m = http_parse_method();
                enum ag_http_mime t = http_parse_mime();

                AG_AUTO(ag_http_client) *c = http_parse_client();
                AG_AUTO(ag_http_url) *u = http_parse_url();
                AG_AUTO(ag_alist) *p = ag_alist_new_empty();

                AG_AUTO(ag_http_request) *r = ag_http_request_new(m, t, u, c, p);
                */

                FCGX_Finish_r(g_http->req);
        }
}

