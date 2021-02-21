#include "../include/argent.h"
#include <fcgiapp.h>


static void plugin_release(void *hnd)
{
        AG_ASSERT_PTR (hnd);

        ag_plugin *p = hnd;
        ag_plugin_release(&p);
}


static AG_THREADLOCAL struct {
        struct ag_http_env       env;
        FCGX_Request            *req;
        ag_registry             *reg;
} *g_http = NULL;


extern void
ag_http_server_init(void)
{
        AG_ASSERT (!g_http);

        g_http = ag_memblock_new(sizeof *g_http);
        g_http->reg = ag_registry_new(plugin_release);

        g_http->env.document_root = "";
        g_http->env.http_cookie = "";
        g_http->env.http_host = "";
        g_http->env.http_referer = "";
        g_http->env.http_user_agent = "";
        g_http->env.https = "";
        g_http->env.path = "";
        g_http->env.query_string = "";
        g_http->env.remote_addr = "";
        g_http->env.remote_user = "";
        g_http->env.request_method = "";
        g_http->env.request_uri = "";
        g_http->env.script_filename = "";
        g_http->env.script_name = "";
        g_http->env.server_admin = "";
        g_http->env.server_name = "";
        g_http->env.server_port = "";
        g_http->env.server_software = "";

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

        
static inline const char *
env_read(const char *key)
{
        AG_ASSERT_PTR (g_http);
        AG_ASSERT_STR (key);

        const char *v = FCGX_GetParam(key, g_http->req->envp);
        return v ? v : "";
}



extern const struct ag_http_env *
ag_http_server_env(void)
{
        AG_ASSERT_PTR (g_http);
        
        g_http->env.document_root = env_read("DOCUMENT_ROOT");
        g_http->env.http_cookie = env_read("HTTP_COOKIE");
        g_http->env.http_host = env_read("HTTP_HOST");
        g_http->env.http_referer = env_read("HTTP_REFERER");
        g_http->env.http_user_agent = env_read("HTTP_USER_AGENT");
        g_http->env.https = env_read("HTTPS");
        g_http->env.path = env_read("PATH");
        g_http->env.query_string = env_read("QUERY_STRING");
        g_http->env.remote_addr = env_read("REMOTE_ADDR");
        g_http->env.remote_user = env_read("REMOTE_USER");
        g_http->env.request_method = env_read("REQUEST_METHOD");
        g_http->env.request_uri = env_read("REQUEST_URI");
        g_http->env.script_filename = env_read("SCRIPT_FILENAME");
        g_http->env.script_name = env_read("SCRIPT_NAME");
        g_http->env.server_admin = env_read("SERVER_ADMIN");
        g_http->env.server_name = env_read("SERVER_NAME");
        g_http->env.server_port = env_read("SERVER_PORT");
        g_http->env.server_software = env_read("SERVER_SOFTWARE");

        return &g_http->env;
}


extern void
ag_http_server_register(const char *path, const ag_plugin *plug)
{
        AG_ASSERT_STR (path);
        AG_ASSERT_PTR (plug);
        AG_ASSERT_PTR (g_http);

        ag_registry_push(g_http->reg, ag_hash_new_str(path),
            ag_plugin_copy(plug));
}



static enum ag_http_method
http_parse_method(void)
{
        AG_ASSERT_PTR (g_http);

        return ag_http_method_parse(env_read("REQUEST_METHOD"));
}


static ag_http_url *
http_parse_url(void)
{
        AG_ASSERT_PTR (g_http);

        AG_AUTO(ag_string) *s = ag_string_new(env_read("HTTPS"));
        AG_AUTO(ag_string) *s2 = ag_string_lower(s);
        bool secure = ag_string_eq(s2, "on");

        const char *host = env_read("SERVER_NAME");

        AG_AUTO(ag_string) *p = ag_string_new(env_read("REQUEST_URI"));
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
                const struct ag_http_env *e = ag_http_server_env();

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

