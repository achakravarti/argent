#include "../argent.h"

#include <fcgiapp.h>


static void plugin_release(void *hnd)
{
        AG_ASSERT_PTR (hnd);

        ag_plugin *p = hnd;
        ag_plugin_release(&p);
}


static AG_THREADLOCAL struct {
        struct ag_http_env       env;
        FCGX_Request             cgi;
        ag_registry             *reg;
        ag_http_request         *req;
} *g_http = NULL;


extern void
ag_http_server_init(void)
{
        AG_ASSERT (!g_http);

        g_http = ag_memblock_new(sizeof *g_http);
        g_http->reg = ag_registry_new(plugin_release);
        g_http->req = NULL;
        g_http->env = (const struct ag_http_env){'\0'};
        
        AG_REQUIRE (!FCGX_Init(), AG_ERNO_HTTP);
        AG_REQUIRE (!FCGX_InitRequest(&g_http->cgi, 0, 0), AG_ERNO_HTTP);
}


extern void
ag_http_server_exit(void)
{
        if (AG_UNLIKELY (!g_http))
                return;

        ag_registry_release(&g_http->reg);
        ag_http_request_release(&g_http->req);

        ag_memblock *m = g_http;
        ag_memblock_release(&m);
}

        
static inline const char *
env_read(const char *key)
{
        AG_ASSERT_PTR (g_http);
        AG_ASSERT_STR (key);

        const char *v = FCGX_GetParam(key, g_http->cgi.envp);
        ag_log_debug("%s = %s", key, v ? v : "(empty)");

        return v ? v : "";
}



extern const struct ag_http_env *
ag_http_server_env(void)
{
        AG_ASSERT_PTR (g_http);

        g_http->env.auth_type = env_read("AUTH_TYPE");
        g_http->env.content_length = env_read("CONTENT_LENGTH");
        g_http->env.content_type = env_read("CONTENT_TYPE");
        g_http->env.document_root = env_read("DOCUMENT_ROOT");
        g_http->env.gateway_interface = env_read("GATEWAY_INTEFACE");
        g_http->env.http_accept = env_read("HTTP_ACCEPT");
        g_http->env.http_accept_charset = env_read("HTTP_ACCEPT_CHARSET");
        g_http->env.http_accept_encoding = env_read("HTTP_ACCEPT_ENCODING");
        g_http->env.http_accept_language = env_read("HTTP_ACCEPT_LANGUAGE");
        g_http->env.http_cookie = env_read("HTTP_COOKIE");
        g_http->env.http_from = env_read("HTTP_FROM");
        g_http->env.http_host = env_read("HTTP_HOST");
        g_http->env.http_referer = env_read("HTTP_REFERER");
        g_http->env.http_user_agent = env_read("HTTP_USER_AGENT");
        g_http->env.https = env_read("HTTPS");
        g_http->env.path = env_read("PATH");
        g_http->env.path_info = env_read("PATH_INFO");
        g_http->env.path_translated = env_read("PATH_TRANSLATED");
        g_http->env.query_string = env_read("QUERY_STRING");
        g_http->env.remote_addr = env_read("REMOTE_ADDR");
        g_http->env.remote_host = env_read("REMOTE_HOST");
        g_http->env.remote_ident = env_read("REMOTE_IDENT");
        g_http->env.remote_port = env_read("REMOTE_PORT");
        g_http->env.remote_user = env_read("REMOTE_USER");
        g_http->env.request_method = env_read("REQUEST_METHOD");
        g_http->env.request_uri = env_read("REQUEST_URI");
        g_http->env.script_filename = env_read("SCRIPT_FILENAME");
        g_http->env.script_name = env_read("SCRIPT_NAME");
        g_http->env.server_admin = env_read("SERVER_ADMIN");
        g_http->env.server_name = env_read("SERVER_NAME");
        g_http->env.server_port = env_read("SERVER_PORT");
        g_http->env.server_protocol = env_read("SERVER_PROTOCOL");
        g_http->env.server_software = env_read("SERVER_SOFTWARE");

        return &g_http->env;
}


extern const ag_http_request *
ag_http_server_request(void)
{
        AG_ASSERT_PTR (g_http);
        AG_ASSERT_PTR (g_http->req);

        return g_http->req;
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


extern void
ag_http_server_respond(const ag_http_response *resp)
{
        AG_ASSERT_PTR (g_http);

        AG_AUTO(ag_string) *s = ag_http_response_str(resp);
        FCGX_FPrintF(g_http->cgi.out, s);
}


static void
default_http_handler(const ag_http_request *req)
{
        AG_ASSERT_PTR (req);

        const char *msg = "<h1>[!] request handler not found!</h1><p>%s</p>";
        AG_AUTO(ag_string) *s = ag_http_request_str(req);
        AG_AUTO(ag_string) *s2 = ag_string_new_fmt(msg, s);

        AG_AUTO(ag_http_response) *r = ag_http_response_new(
            AG_HTTP_MIME_TEXT_HTML, AG_HTTP_STATUS_404_NOT_FOUND, s2);
        ag_http_server_respond(r);
}


static inline ag_string *
param_get(void)
{
        AG_ASSERT_PTR (g_http);

        AG_AUTO(ag_string) *s = ag_string_new(g_http->env.query_string);
        return ag_string_url_decode(s);
}


static ag_string *
param_post(void)
{
        AG_ASSERT_PTR (g_http);

        size_t sz = 1024;
        char *bfr = ag_memblock_new(sz);

        size_t read = 0;
        int err = 0;

        do {
                sz = sz << 1;
                ag_memblock *m = bfr;
                ag_memblock_resize(&m, sz);

                read += FCGX_GetStr(bfr + read, sz - read, g_http->cgi.in);
                if (AG_UNLIKELY (!read || 
                    (err = FCGX_GetError(g_http->cgi.in))))
                        ag_memblock_release(&m);

                AG_REQUIRE (!err, AG_ERNO_HTTP);
        } while(read == sz);

        bfr[read] = '\0';
        AG_AUTO(ag_string) *s = ag_string_new(bfr); 
   
        ag_memblock *m = bfr; 
        ag_memblock_release(&m);

        return ag_string_url_decode(s);
}


static void
srv_req(void)
{
        AG_ASSERT_PTR (g_http);

        const struct ag_http_env *e = ag_http_server_env();

        enum ag_http_method m = ag_http_method_parse(e->request_method);
        enum ag_http_mime t = *e->content_type 
            ? ag_http_mime_parse(e->content_type) : AG_HTTP_MIME_TEXT_PLAIN;

        AG_AUTO(ag_http_url) *u = ag_http_url_parse_env(e);
        AG_AUTO(ag_http_client) *c = ag_http_client_parse_env(e);

        AG_AUTO(ag_string) *s = (m == AG_HTTP_METHOD_GET ||
            m == AG_HTTP_METHOD_DELETE) ? param_get() : param_post();
        AG_AUTO(ag_alist) *p = ag_alist_parse(s, "=", "&");

        ag_http_request_release(&g_http->req);
        g_http->req = ag_http_request_new(m, t, u, c, p);
}


static void
srv_resp(void)
{
        AG_ASSERT_PTR (g_http);

        AG_AUTO(ag_http_url) *u = ag_http_request_url(g_http->req);
        AG_AUTO(ag_string) *p = ag_http_url_path(u);
        ag_hash h = ag_hash_new_str(p);

        const ag_plugin *plg = ag_registry_get(g_http->reg, h);
        ag_http_handler *hnd = ag_plugin_hnd(plg);

        if (AG_LIKELY (hnd))
                hnd(g_http->req);
        else
                default_http_handler(g_http->req);
}


extern void
ag_http_server_run(void)
{
        AG_ASSERT_PTR (g_http);

        while (FCGX_Accept_r(&g_http->cgi) >= 0) {
                srv_req();
                srv_resp();
                FCGX_Finish_r(&g_http->cgi);
        }
}

