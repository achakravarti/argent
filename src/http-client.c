#include "../include/argent.h"


struct payload {
        ag_uint          port;
        ag_string       *ip;
        ag_string       *host;
        ag_string       *agent;
        ag_string       *referer;
};


static struct payload   *payload_new(const char *, ag_uint, const char *,
                            const char *, const char *);


static ag_memblock      *virt_clone(const ag_memblock *);
static void              virt_release(ag_memblock *);
static enum ag_cmp       virt_cmp(const ag_object *, const ag_object *);
static bool              virt_valid(const ag_object *);
static size_t            virt_sz(const ag_object *);
static size_t            virt_len(const ag_object *);
static ag_hash           virt_hash(const ag_object *);
static ag_string        *virt_str(const ag_object *);


AG_OBJECT_DEFINE(ag_http_client)
{
        struct ag_object_vtable vt = {
                .clone = virt_clone, .release = virt_release, .cmp = virt_cmp,
                .valid = virt_valid, .sz = virt_sz,           .len = virt_len,
                .hash = virt_hash,   .str = virt_str,
        };

        ag_object_registry_set(AG_TYPEID_HTTP_CLIENT, &vt);
}


extern ag_http_client *
ag_http_client_new(const char *ip, ag_uint port, const char *host,
    const char *agent, const char *referer)
{
        AG_ASSERT_PTR (ip);
        AG_ASSERT_PTR (host);
        AG_ASSERT_PTR (agent);
        AG_ASSERT_PTR (referer);
        AG_ASSERT (port < 65535);

        return ag_object_new(AG_TYPEID_HTTP_CLIENT,
            payload_new(ip, port, host, agent, referer));
}


extern ag_string *
ag_http_client_ip(const ag_http_client *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->ip);
}


extern ag_uint
ag_http_client_port(const ag_http_client *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return p->port;
}


extern ag_string *
ag_http_client_host(const ag_http_client *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->host);
}


extern ag_string *
ag_http_client_agent(const ag_http_client *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->agent);
}


extern ag_string *
ag_http_client_referer(const ag_http_client *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->referer);
}


static struct payload *
payload_new(const char *ip, ag_uint port, const char *host, const char *agent,
    const char *referer)
{
        AG_ASSERT_PTR (ip);
        AG_ASSERT_PTR (host);
        AG_ASSERT_PTR (agent);
        AG_ASSERT_PTR (referer);
        AG_ASSERT (port < 65535);

        struct payload *p = ag_memblock_new(sizeof *p);

        p->port = port;
        p->ip = ag_string_new(ip);
        p->host = ag_string_new(host);
        p->agent = ag_string_new(agent);
        p->referer = ag_string_new(referer);

        return p;
}


static ag_memblock *
virt_clone(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ctx;
        return payload_new(p->ip, p->port, p->host, p->agent, p->referer);
}


static void
virt_release(ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        struct payload *p = ctx;
        ag_string_release(&p->ip);
        ag_string_release(&p->host);
        ag_string_release(&p->agent);
        ag_string_release(&p->referer);
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

        (void)ctx;
        return true;
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

        if (p->port)
                return ag_string_new_fmt(
                   "[%s:%lu] host=%s, agent=%s, referer=%s",
                    p->ip, p->port, p->host, p->agent, p->referer);
        else
                return ag_string_new_fmt("[%s] host=%s, agent=%s, referer=%s",
                    p->ip, p->host, p->agent, p->referer);
}

