#include "../include/argent.h"


#if (defined __GNUC__ || defined __clang__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wattributes"
#endif




/*******************************************************************************
 *                              PAYLOAD INTERNALS
 */


/*
 * struct payload: object payload for HTTP URL.
 */
struct payload {
    bool secure;
    ag_uint port;
    ag_string_t *host;
    ag_string_t *path;
};


/*
 * payload_new(): create new HTTP URL payload.
 *
 * @hsecure: HTTPS protocol used.
 * @host: URL host.
 * @port: URL port.
 * @path: URL path.
 *
 * Return: new payload.
 */
static struct payload *payload_new(bool secure, const char *host, ag_uint port,
        const char *path)
{
    struct payload *p = ag_memblock_new(sizeof *p);

    p->secure = secure;
    p->port = port;
    p->host = ag_string_new(host);

    char c = *path;
    if (c) {
        p->path = c == '/' ? ag_string_new(path) 
            : ag_string_new_fmt("/%s", path);
    } else
        p->path = ag_string_new("/");

    return p;
}


/*
 * payload_copy(): make deep copy of HTTP URL payload.
 *
 * @ctx: contextual payload.
 *
 * Return: copy of @ctx.
 */
static inline void *payload_copy(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return payload_new(p->secure, p->host, p->port, p->path);
}


/*
 * payload_dispose(): free up HTTP URL payload resources.
 *
 * @ctx: contextual payload.
 */
static void payload_dispose(void *ctx)
{
    struct payload *p = (struct payload *) ctx;

    ag_string_dispose(&p->host);
    ag_string_dispose(&p->path);
}




/*******************************************************************************
 *                               OBJECT INTERNALS
 */


/*
 * object_str(): get HTTP URL string representation.
 *
 * @ctx: contextual object.
 *
 * Return: string representation of @ctx.
 */
static inline ag_string_t *object_str(const ag_object_t *ctx)
{
    const struct payload *p = ag_object_payload(ctx);

    if (p->port) {
        return ag_string_new_fmt("http%s://%s:%u%s", p->secure ? "s" : "",
                p->host, p->port, p->path);
    } else {
        return ag_string_new_fmt("http%s://%s%s", p->secure ? "s" : "", p->host,
                p->path);
    }
}


/*
 * object_cmp(): compare two HTTP URL objects.
 *
 * @ctx: contextual object.
 * @cmp: comparison object.
 *
 * Return: AG_TRISTATE_LO  - @ctx < @cmp,
 *         AG_TRISTATE_GND - @ctx == @cmp,
 *         AG_TRISTATE_HI  - @ctx > @cmp.
 */
static inline enum ag_tristate object_cmp(const ag_object_t *ctx, 
        const ag_object_t *cmp)
{
    ag_string_smart_t *s1 = object_str(ctx);
    ag_string_smart_t *s2 = object_str(cmp);
    return ag_string_cmp(s1, s2);
}


/*
 * object_sz(): get HTTP URL object size.
 *
 * @ctx: contextual object.
 *
 * Return: size of @ctx.
 */
static inline size_t object_sz(const ag_object_t *ctx)
{
    ag_string_smart_t *s = object_str(ctx);
    return ag_string_sz(s);
}


/*
 * object_len(): get HTTP URL object length.
 *
 * @ctx: contextual object.
 *
 * Return: length of @ctx.
 */
static inline size_t object_len(const ag_object_t *ctx)
{
    ag_string_smart_t *s = object_str(ctx);
    return ag_string_len(s);
}


/*
 * object_hash(): get HTTP URL object hash.
 *
 * @ctx: contextual object.
 *
 * Return: hash of @ctx.
 */
static inline ag_hash_t object_hash(const ag_object_t *ctx)
{
    ag_string_smart_t *s = object_str(ctx);
    return ag_string_hash(s);
}




/*******************************************************************************
 *                              MANAGER EXTERNALS
 */


extern void ag_http_url_register(void)
{
    struct ag_object_vtable vt = {
        .copy = &payload_copy,
        .dispose = &payload_dispose,
        .id = NULL,
        .sz = &object_sz,
        .len = &object_len,
        .hash = &object_hash,
        .cmp = &object_cmp,
        .str = &object_str
    };

    ag_object_register(AG_OBJECT_TYPE_HTTP_URL, &vt);
}


extern ag_http_url_t *ag_http_url_new(bool secure, const char *host,
        ag_uint port, const char *path)
{
    ag_assert (host && *host && port < 65535 && path);

    return ag_object_new(AG_OBJECT_TYPE_HTTP_URL, payload_new(secure, host,
                port, path));
}


extern inline ag_http_url_t *ag_http_url_copy(const ag_http_url_t *ctx);


extern inline void ag_http_url_dispose(ag_http_url_t **ctx);




/*******************************************************************************
 *                             COMPARATOR EXTERNALS
 */


extern inline enum ag_tristate ag_http_url_cmp(const ag_http_url_t *ctx, 
        const ag_http_url_t *cmp);


extern inline bool ag_http_url_lt(const ag_http_url_t *ctx,
        const ag_http_url_t *cmp);


extern inline bool ag_http_url_eq(const ag_http_url_t *ctx,
        const ag_http_url_t *cmp);


extern inline bool ag_http_url_gt(const ag_http_url_t *ctx,
        const ag_http_url_t *cmp);




/*******************************************************************************
 *                              ACCESSOR EXTERNALS
 */


extern inline bool ag_http_url_empty(const ag_http_url_t *ctx);


extern bool ag_http_url_secure(const ag_http_url_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return p->secure;
}


extern inline size_t ag_http_url_typeid(const ag_http_url_t *ctx);


extern inline size_t ag_http_url_objid(const ag_http_url_t *ctx);


extern inline size_t ag_http_url_hash(const ag_http_url_t *ctx);


extern inline size_t ag_http_url_sz(const ag_http_url_t *ctx);


extern inline size_t ag_http_url_len(const ag_http_url_t *ctx);


extern ag_uint ag_http_url_port(const ag_http_url_t *ctx)
{
    ag_assert (ctx);

    const struct payload *p = ag_object_payload(ctx);
    return p->port;
}


extern inline ag_string_t *ag_http_url_str(const ag_http_url_t *ctx);


extern ag_string_t *ag_http_url_host(const ag_http_url_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->host);
}


extern ag_string_t *ag_http_url_path(const ag_http_url_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->path);
}




#if (defined __GNUC__ || defined __clang__)
#   pragma GCC diagnostic pop
#endif

