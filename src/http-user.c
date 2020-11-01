#include "./api.h"




/*******************************************************************************
 *                              PAYLOAD INTERFNALS
 */


struct payload {
    ag_string_t *agent;
    ag_string_t *ip;
    ag_string_t *host;
    ag_string_t *port;
};


static struct payload *payload_new(const char *agent, const char *ip,
        const char *host, const char *port)
{
    struct payload *p = ag_memblock_new(sizeof *p);

    p->agent = ag_string_new(agent);
    p->ip = ag_string_new(ip);
    p->host = ag_string_new(host);
    p->port = ag_string_new(port);

    return p;
}




/*******************************************************************************
 *                              V-TABLE INTERNALS
 */


static inline void *vt_copy(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return payload_new(p->agent, p->ip, p->host, p->port);
}


static inline void vt_dispose(void *ctx)
{
    struct payload *p = (struct payload *) ctx;

    ag_string_dispose(&p->agent);
    ag_string_dispose(&p->ip);
    ag_string_dispose(&p->host);
    ag_string_dispose(&p->port);
}


static inline size_t vt_sz(const ag_object_t *obj)
{
    const struct payload *p = ag_object_payload(obj);
    return ag_string_sz(p->agent) + ag_string_sz(p->ip) + ag_string_sz(p->host)
        + ag_string_sz(p->port);
}


static inline ag_string_t *vt_str(const ag_object_t *obj)
{
    const struct payload *p = ag_object_payload(obj);
    return ag_string_new_fmt("agent=%s; ip=%s; host=%s; port=%s", p->agent,
            p->ip, p->host, p->port);
}


static inline enum ag_tristate vt_cmp(const ag_object_t *lhs, 
        const ag_object_t *rhs)
{
    ag_string_smart_t *s1 = ag_object_str(lhs);
    ag_string_smart_t *s2 = ag_object_str(rhs);
    return ag_string_cmp(s1, s2);
}




/*******************************************************************************
 *                              MANAGER EXTERNALS
 */


extern void ag_http_user_register(void)
{
    struct ag_object_vtable vt = {
        .copy = &vt_copy,
        .dispose = &vt_dispose,
        .id = NULL,
        .sz = &vt_sz,
        .len = NULL,
        .hash = NULL,
        .cmp = &vt_cmp,
        .str = &vt_str
    };

    ag_object_register(AG_OBJECT_TYPE_HTTP_USER, &vt);
}


extern ag_http_user_t *ag_http_user_new(const char *agent, const char *ip,
        const char *host, const char *port)
{
    ag_assert (agent && ip && host && port);
    return ag_object_new(AG_OBJECT_TYPE_HTTP_USER, payload_new(agent, ip, host,
                port));
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


extern inline size_t ag_http_user_typeid(const ag_http_user_t *ctx);


extern inline size_t ag_http_user_objid(const ag_http_user_t *ctx);


extern inline size_t ag_http_user_hash(const ag_http_user_t *ctx);


extern inline size_t ag_http_user_sz(const ag_http_user_t *ctx);


extern inline size_t ag_http_user_len(const ag_http_user_t *ctx);


extern inline bool ag_http_user_empty(const ag_http_user_t *ctx);


extern inline ag_string_t *ag_http_user_str(const ag_http_user_t *ctx);


extern ag_string_t *ag_http_user_agent(const ag_http_user_t *ctx)
{
    const struct payload *p = ag_object_payload(ctx);
    return p->agent;
}


extern ag_string_t *ag_http_user_ip(const ag_http_user_t *ctx)
{
    const struct payload *p = ag_object_payload(ctx);
    return p->ip;
}


extern ag_string_t *ag_http_user_host(const ag_http_user_t *ctx)
{
    const struct payload *p = ag_object_payload(ctx);
    return p->host;
}


extern ag_string_t *ag_http_user_port(const ag_http_user_t *ctx)
{
    const struct payload *p = ag_object_payload(ctx);
    return p->port;
}

