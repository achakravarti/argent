#include "./api.h"


struct payload {
    bool secure;
    ag_string_t *host;
    ag_string_t *port;
    ag_string_t *path;
};


static struct payload *payload_new(bool secure, const char *host, 
        const char *port, const char *path)
{
    struct payload *p = ag_memblock_new(sizeof *p);

    p->secure = secure;
    p->host = ag_string_new(host);
    p->port = ag_string_new(port);
    p->path = ag_string_new(path);

    return p;
}


static inline void *method_copy(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return payload_new(p->secure, p->host, p->port, p->path);
}


static void method_dispose(void *ctx)
{
    struct payload *p = (struct payload *) ctx;

    ag_string_dispose(&p->host);
    ag_string_dispose(&p->port);
    ag_string_dispose(&p->path);
}


static inline ag_string_t *method_str(const ag_object_t *obj)
{
    const struct payload *p = ag_object_payload(obj);

    if (*p->port) {
        return ag_string_new_fmt("http%s://%s:%s%s", p->secure ? "s" : "",
                p->host, p->port, p->path);
    } else {
        return ag_string_new_fmt("http%s://%s%s", p->secure ? "s" : "", p->host,
                p->path);
    }
}


static inline size_t method_sz(const ag_object_t *obj)
{
    ag_string_smart_t *s = method_str(obj);
    return ag_string_sz(s);
}


static inline size_t method_len(const ag_object_t *obj)
{
    ag_string_smart_t *s = method_str(obj);
    return ag_string_len(s);
}


static enum ag_tristate method_cmp(const ag_object_t *lhs, 
        const ag_object_t *rhs)
{
    ag_string_smart_t *s1 = method_str(lhs);
    ag_string_smart_t *s2 = method_str(rhs);
    return ag_string_cmp(s1, s2);
}


extern void ag_http_url_register(void)
{
    struct ag_object_vtable vt = {
        .copy = &method_copy,
        .dispose = &method_dispose,
        .id = NULL,
        .sz = &method_sz,
        .len = &method_len,
        .hash = NULL,
        .cmp = &method_cmp,
        .str = &method_str
    };

    ag_object_register(AG_OBJECT_TYPE_URL, &vt);
}


extern ag_http_url_t *ag_http_url_new(bool secure, const char *host, const char *port,
        const char *path)
{
    ag_assert (host && port && path);
    return ag_object_new(AG_OBJECT_TYPE_URL, payload_new(secure, host, port,
                path));
}


extern inline ag_http_url_t *ag_http_url_copy(const ag_http_url_t *ctx);
extern inline void ag_http_url_dispose(ag_http_url_t **ctx);
extern inline size_t ag_http_url_typeid(const ag_http_url_t *ctx);
extern inline size_t ag_http_url_objid(const ag_http_url_t *ctx);
extern inline size_t ag_http_url_hash(const ag_http_url_t *ctx);
extern inline size_t ag_http_url_sz(const ag_http_url_t *ctx);
extern inline size_t ag_http_url_len(const ag_http_url_t *ctx);
extern inline bool ag_http_url_empty(const ag_http_url_t *ctx);
extern inline enum ag_tristate ag_http_url_cmp(const ag_http_url_t *ctx, 
        const ag_http_url_t *cmp);
extern inline bool ag_http_url_lt(const ag_http_url_t *ctx, const ag_http_url_t *cmp);
extern inline bool ag_http_url_eq(const ag_http_url_t *ctx, const ag_http_url_t *cmp);
extern inline bool ag_http_url_gt(const ag_http_url_t *ctx, const ag_http_url_t *cmp);
extern inline ag_string_t *ag_http_url_str(const ag_http_url_t *ctx);


extern bool ag_http_url_secure(const ag_http_url_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return p->secure;
}


extern ag_string_t *ag_http_url_host(const ag_http_url_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->host);
}


extern ag_string_t *ag_http_url_port(const ag_http_url_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->port);
}


extern ag_string_t *ag_http_url_path(const ag_http_url_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->path);
}

