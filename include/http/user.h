#if (!defined __ARGENT_HTTP_USER_H__)
#define __ARGENT_HTTP_USER_H__


#include "../object.h"


/*******************************************************************************
 *                                  HTTP USER
 */


typedef ag_object_t ag_http_user_t;

#define ag_http_user_smart_t ag_object_smart_t

extern void ag_http_user_register(void);

extern ag_http_user_t *ag_http_user_new(const char *agent, const char *ip,
        const char *host, const char *port);

inline ag_http_user_t *ag_http_user_copy(const ag_http_user_t *ctx)
{
    return ag_object_copy(ctx);
}

inline void ag_http_user_dispose(ag_http_user_t **ctx)
{
    ag_object_dispose(ctx);
}

inline size_t ag_http_user_typeid(const ag_http_user_t *ctx)
{
    return ag_object_type(ctx);
}

inline size_t ag_http_user_objid(const ag_http_user_t *ctx)
{
    return ag_object_id(ctx);
}

inline size_t ag_http_user_hash(const ag_http_user_t *ctx)
{
    return ag_object_hash(ctx);
}

inline size_t ag_http_user_sz(const ag_http_user_t *ctx)
{
    return ag_object_sz(ctx);
}

inline size_t ag_http_user_len(const ag_http_user_t *ctx)
{
    return ag_object_len(ctx);
}

inline bool ag_http_user_empty(const ag_http_user_t *ctx)
{
    return ag_object_empty(ctx);
}

inline enum ag_tristate ag_http_user_cmp(const ag_http_user_t *ctx,
        const ag_http_user_t *cmp)
{
    return ag_object_cmp(ctx, cmp);
}

inline bool ag_http_user_lt(const ag_http_user_t *ctx,
        const ag_http_user_t *cmp)
{
    return ag_object_lt(ctx, cmp);
}

inline bool ag_http_user_eq(const ag_http_user_t *ctx,
        const ag_http_user_t *cmp)
{
    return ag_object_eq(ctx, cmp);
}

inline bool ag_http_user_gt(const ag_http_user_t *ctx,
        const ag_http_user_t *cmp)
{
    return ag_object_gt(ctx, cmp);
}

inline ag_string_t *ag_http_user_str(const ag_http_user_t *ctx)
{
    return ag_object_str(ctx);
}

extern ag_string_t *ag_http_user_agent(const ag_http_user_t *ctx);

extern ag_string_t *ag_http_user_ip(const ag_http_user_t *ctx);

extern ag_string_t *ag_http_user_host(const ag_http_user_t *ctx);

extern ag_string_t *ag_http_user_port(const ag_http_user_t *ctx);


#endif /* !defined __ARGENT_HTTP_USER_H__ */

