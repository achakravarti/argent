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


typedef ag_object_t ag_url_t;
#define ag_url_smart_t ag_object_smart_t
extern void ag_url_register(void);

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

