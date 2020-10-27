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


static inline size_t method_sz(const ag_object_t *obj)
{
    const struct payload *p = ag_object_payload(obj);
    return sizeof p->secure + ag_string_sz(p->host) + ag_string_sz(p->port) 
            + ag_string_sz(p->path);
}


static inline size_t method_len(const ag_object_t *obj)
{
    const struct payload *p = ag_object_payload(obj);
    return ag_string_len(p->host) + ag_string_len(p->port) 
            + ag_string_len(p->path);
}


extern void ag_url_register(void)
{
    struct ag_object_vtable vt = {
        .copy = &method_copy,
        .dispose = &method_dispose,
        .id = NULL,
        .sz = &method_sz,
        .len = &method_len,
        .hash = NULL,
        .cmp = NULL,
        .str = NULL
    };

    ag_object_register(AG_OBJECT_TYPE_URL, &vt);
}


extern ag_url_t *ag_url_new(bool secure, const char *host, const char *port,
        const char *path)
{
    ag_assert (host && port && path);
    return ag_object_new(AG_OBJECT_TYPE_URL, payload_new(secure, host, port,
                path));
}


extern inline ag_url_t *ag_url_copy(const ag_url_t *ctx);
extern inline void ag_url_dispose(ag_url_t **ctx);
extern inline size_t ag_url_typeid(const ag_url_t *ctx);
extern inline size_t ag_url_objid(const ag_url_t *ctx);
extern inline size_t ag_url_hash(const ag_url_t *ctx);
extern inline size_t ag_url_sz(const ag_url_t *ctx);
extern inline size_t ag_url_len(const ag_url_t *ctx);
extern inline bool ag_url_empty(const ag_url_t *ctx);
extern inline enum ag_tristate ag_url_cmp(const ag_url_t *ctx, 
        const ag_url_t *cmp);
extern inline bool ag_url_lt(const ag_url_t *ctx, const ag_url_t *cmp);
extern inline bool ag_url_eq(const ag_url_t *ctx, const ag_url_t *cmp);
extern inline bool ag_url_gt(const ag_url_t *ctx, const ag_url_t *cmp);
extern inline const char *ag_url_str(const ag_url_t *ctx);


extern bool ag_url_secure(const ag_url_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return p->secure;
}


extern ag_string_t *ag_url_host(const ag_url_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->host);
}


extern ag_string_t *ag_url_port(const ag_url_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->port);
}


extern ag_string_t *ag_url_path(const ag_url_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->path);
}

