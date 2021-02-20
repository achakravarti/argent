#include "../include/argent.h"
#include <fcgiapp.h>


static void plugin_release(void *hnd)
{
        AG_ASSERT_PTR (hnd);

        ag_plugin *p = hnd;
        ag_plugin_release(&p);
}


static AG_THREADLOCAL struct {
        FCGX_Request    *req;
        ag_registry     *reg;
} *g_http = NULL;


extern void
ag_http_server_init(void)
{
        AG_ASSERT (!g_http);

        g_http = ag_memblock_new(sizeof *g_http);
        g_http->reg = ag_registry_new(plugin_release);

        AG_REQUIRE (!FCGX_Init(), AG_ERNO_HTTP);
        AG_REQUIRE (!FCGX_InitRequest(g_http->req, 0, 0), AG_ERNO_HTTP);
}


extern void
ag_http_server_exit(void)
{
        if (AG_UNLIKELY (!g_http))
                return;

        ag_registry_release(&g_http->reg);

        ag_memblock *m = g_http;
        ag_memblock_release(&m);
}


extern void
ag_http_server_register(const char *path, const ag_plugin *plg)
{
        AG_ASSERT_STR (path);
        AG_ASSERT_PTR (plg);
        AG_ASSERT_PTR (g_http);

        ag_registry_push(g_http->reg, ag_hash_new_str(path),
            ag_plugin_copy(plg));
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

