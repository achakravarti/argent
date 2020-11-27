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
    ag_uint port;
    ag_string_t *agent;
    ag_string_t *host;
    ag_ip_t *ip;
};


/*
 * payload_new(): create new HTTP user payload.
 *
 * @agent: user agent
 * @ip   : user IP address
 * @port : user port number.
 * @host : user hostname.
 *
 * Return: new payload.
 */
static struct payload *payload_new(const char *agent, const ag_ip_t *ip,
        ag_uint port, const char *host)
{
    struct payload *p = ag_memblock_new(sizeof *p);

    p->agent = ag_string_new(agent);
    p->host = ag_string_new(host);
    p->ip = ag_ip_copy(ip);
    p->port = port;

    return p;
}


/*
 * payload_copy(): make deep copy of HTTP user payload.
 *
 * @ctx: contextual payload.
 *
 * Return: copy of @ctx.
 */
static inline void *payload_copy(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return payload_new(p->agent, p->ip, p->port, p->host);
}


/*
 * payload_dispose(): free up HTTP user payload resources.
 *
 * @ctx: contextual payload.
 */
static inline void payload_dispose(void *ctx)
{
    struct payload *p = (struct payload *) ctx;

    ag_string_dispose(&p->agent);
    ag_string_dispose(&p->host);
    ag_ip_dispose(&p->ip);
}




/*******************************************************************************
 *                               OBJECT INTERNALS
 */


/*
 * object_sz(): get HTTP user object size.
 *
 * @ctx: contextual object.
 *
 * Return size of @ctx.
 */
static inline size_t object_sz(const ag_object_t *ctx)
{
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_sz(p->agent) + ag_ip_sz(p->ip) + ag_string_sz(p->host)
        + sizeof p->port;
}


/*
 * object_str(): get HTTP user object string representation.
 *
 * @ctx: contextual object.
 *
 * Return: string representation of @ctx.
 */
static inline ag_string_t *object_str(const ag_object_t *ctx)
{
    const struct payload *p = ag_object_payload(ctx);
    ag_string_smart_t *ip = ag_ip_str(p->ip);

    return ag_string_new_fmt("agent=%s; ip=%s; host=%s; port=%d", p->agent, ip,
            p->host, p->port);
}


/*
 * object_len(): get HTTP user object length.
 *
 * @ctx: contextual object.
 *
 * Return length of @ctx.
 */
static inline size_t object_len(const ag_object_t *ctx)
{
    ag_string_smart_t *s = object_str(ctx);
    return ag_string_len(s);
}


/*
 * object_hash(): get HTTP user object hash.
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


/*
 * object_cmp(): compare two HTTP user objects.
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
    ag_string_smart_t *s1 = ag_object_str(ctx);
    ag_string_smart_t *s2 = ag_object_str(cmp);
    return ag_string_cmp(s1, s2);
}




/*******************************************************************************
 *                              MANAGER EXTERNALS
 */


extern void ag_http_user_register(void)
{
    struct ag_object_vtable vt = {
        .copy = &payload_copy,
        .dispose = &payload_dispose,
        .id = NULL,
        .sz = &object_sz,
        .len = &object_len,
        .hash = object_hash,
        .cmp = &object_cmp,
        .str = &object_str
    };

    ag_object_register(AG_OBJECT_TYPE_HTTP_USER, &vt);
}


extern ag_http_user_t *ag_http_user_new(const char *agent, const ag_ip_t *ip,
        ag_uint port, const char *host)
{
    ag_assert (agent && ip && host && port < 65535);
    return ag_object_new(AG_OBJECT_TYPE_HTTP_USER, payload_new(agent, ip, port,
                host));
}


extern inline ag_http_user_t *ag_http_user_copy(const ag_http_user_t *ctx);


extern inline void ag_http_user_dispose(ag_http_user_t **ctx);




/*******************************************************************************
 *                             COMPARATOR EXTERNALS
 */


extern inline enum ag_tristate ag_http_user_cmp(const ag_http_user_t *ctx,
        const ag_http_user_t *cmp);


extern inline bool ag_http_user_lt(const ag_http_user_t *ctx,
        const ag_http_user_t *cmp);


extern inline bool ag_http_user_eq(const ag_http_user_t *ctx,
        const ag_http_user_t *cmp);


extern inline bool ag_http_user_gt(const ag_http_user_t *ctx,
        const ag_http_user_t *cmp);




/*******************************************************************************
 *                              ACCESSOR EXTERNALS
 */


extern inline bool ag_http_user_empty(const ag_http_user_t *ctx);


extern inline size_t ag_http_user_typeid(const ag_http_user_t *ctx);


extern inline size_t ag_http_user_objid(const ag_http_user_t *ctx);


extern inline size_t ag_http_user_len(const ag_http_user_t *ctx);


extern inline size_t ag_http_user_sz(const ag_http_user_t *ctx);


extern inline size_t ag_http_user_hash(const ag_http_user_t *ctx);


extern inline ag_string_t *ag_http_user_str(const ag_http_user_t *ctx);


extern ag_string_t *ag_http_user_agent(const ag_http_user_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->agent);
}


extern ag_ip_t *ag_http_user_ip(const ag_http_user_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return ag_ip_copy(p->ip);
}


extern ag_uint ag_http_user_port(const ag_http_user_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return p->port;
}


extern ag_string_t *ag_http_user_host(const ag_http_user_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->host);
}




#if (defined __GNUC__ || defined __clang__)
#   pragma GCC diagnostic pop
#endif

