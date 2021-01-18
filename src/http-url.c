#include "../include/argent.h"

#include <inttypes.h>


struct payload {
        bool             secure;
        ag_uint          port;
        ag_string       *host;
        ag_string       *path;
};


static struct payload   *payload_new(bool, const char *, ag_uint, const char *);


static ag_memblock      *virt_clone(const ag_memblock *);
static void              virt_release(ag_memblock *);
static enum ag_cmp       virt_cmp(const ag_object *, const ag_object *);
static bool              virt_valid(const ag_object *);
static size_t            virt_sz(const ag_object *);
static size_t            virt_len(const ag_object *);
static ag_hash           virt_hash(const ag_object *);
static ag_string        *virt_str(const ag_object *);


AG_OBJECT_DEFINE(ag_http_url)
{
        struct ag_object_vtable vt = {
                .clone = virt_clone, .release = virt_release, .cmp = virt_cmp,
                .valid = virt_valid, .sz = virt_sz,           .len = virt_len,
                .hash = virt_hash,   .str = virt_str,
        };

        ag_object_registry_set(AG_TYPEID_HTTP_URL, &vt);
}


extern ag_http_url *
ag_http_url_new(bool secure, const char *host, const char *port,
    const char *path)
{
        AG_ASSERT_STR (host);
        AG_ASSERT_PTR (port);
        AG_ASSERT_PTR (path);

        ag_uint p = *port ? strtoumax(port, NULL, 10) : 0;
        AG_ASSERT (p < 65535);

        return ag_object_new(AG_TYPEID_HTTP_URL,
            payload_new(secure, host, p, path));
}


extern bool              
ag_http_url_secure(const ag_http_url *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return p->secure;
}


extern ag_string *
ag_http_url_host(const ag_http_url *ctx)
{
        AG_ASSERT_PTR (ctx);
        
        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->host);
}


extern ag_uint           
ag_http_url_port(const ag_http_url *ctx)
{
        AG_ASSERT_PTR (ctx);
        
        const struct payload *p = ag_object_payload(ctx);
        return p->port;
}


extern ag_string *
ag_http_url_path(const ag_http_url *ctx)
{
        AG_ASSERT_PTR (ctx);
        
        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->path);
}


static struct payload *
payload_new(bool secure, const char *host, ag_uint port, const char *path)
{
        AG_ASSERT_STR (host);
        AG_ASSERT_PTR (path);
        AG_ASSERT (port < 65535);

        struct payload *p = ag_memblock_new(sizeof *p);

        p->secure = secure;
        p->port = port;
        p->host = ag_string_new(host);

        char c = *path;
        if (c) {
                p->path = c == '/'
                    ? ag_string_new(path) : ag_string_new_fmt("/%s", path);
        } else
                p->path = ag_string_new("/");

        return p;
}


static ag_memblock *
virt_clone(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ctx;
        return payload_new(p->secure, p->host, p->port, p->path);
}


static void
virt_release(ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        struct payload *p = ctx;
        ag_string_release(&p->host);
        ag_string_release(&p->path);
}


static enum ag_cmp
virt_cmp(const ag_object *ctx, const ag_object *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        AG_AUTO(ag_string) *s2 = virt_str(cmp);

        return ag_string_cmp(s, s2);
}


static bool
virt_valid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        
        AG_AUTO(ag_string) *s = virt_str(ctx);
        return ag_string_len(s);
}


static size_t
virt_sz(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        return ag_string_sz(s);
}


static size_t
virt_len(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        return ag_string_len(s);
}


static ag_hash
virt_hash(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        return ag_hash_new_str(s);
}


static ag_string *
virt_str(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);

        if (p->port) {
                return ag_string_new_fmt("http%s://%s:%u%s",
                    p->secure ? "s" : "", p->host, p->port, p->path);
        } else {
                return ag_string_new_fmt("http%s://%s%s",
                    p->secure ? "s" : "", p->host, p->path);
        }
}
