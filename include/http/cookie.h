#if (!defined __ARGENT_HTTP_COOKIE_H__)
#define __ARGENT_HTTP_COOKIE_H__


#include "../object.h"
#include "./user.h"


/*******************************************************************************
 *                                 HTTP COOKIE
 */


typedef ag_object_t ag_http_cookie_t;

#define ag_http_cookie_smart_t ag_object_smart_t

extern void ag_http_cookie_register(void);

extern ag_http_cookie_t *ag_http_cookie_new(bool secure, const char *domain,
        const char *path, const char *expires);

inline ag_http_cookie_t *ag_http_cookie_copy(const ag_http_cookie_t *ctx)
{
    return ag_object_copy(ctx);
}

inline void ag_http_cookie_dispose(ag_http_cookie_t **ctx)
{
    ag_object_dispose(ctx);
}

inline size_t ag_http_cookie_typeid(const ag_http_cookie_t *ctx)
{
    return ag_object_type(ctx);
}

inline size_t ag_http_cookie_objid(const ag_http_cookie_t *ctx)
{
    return ag_object_id(ctx);
}

inline size_t ag_http_cookie_hash(const ag_http_cookie_t *ctx)
{
    return ag_object_hash(ctx);
}

inline size_t ag_http_cookie_sz(const ag_http_cookie_t *ctx)
{
    return ag_object_sz(ctx);
}

inline size_t ag_http_cookie_len(const ag_http_cookie_t *ctx)
{
    return ag_object_len(ctx);
}

inline bool ag_http_cookie_empty(const ag_http_cookie_t *ctx)
{
    return ag_object_empty(ctx);
}

inline enum ag_tristate ag_http_cookie_cmp(const ag_http_cookie_t *ctx,
        const ag_http_cookie_t *cmp)
{
    return ag_object_cmp(ctx, cmp);
}

inline bool ag_http_cookie_lt(const ag_http_cookie_t *ctx,
        const ag_http_cookie_t *cmp)
{
    return ag_object_lt(ctx, cmp);
}

inline bool ag_http_cookie_eq(const ag_http_cookie_t *ctx,
        const ag_http_cookie_t *cmp)
{
    return ag_object_eq(ctx, cmp);
}

inline bool ag_http_cookie_gt(const ag_http_cookie_t *ctx,
        const ag_http_cookie_t *cmp)
{
    return ag_object_gt(ctx, cmp);
}

inline ag_string_t *ag_http_cookie_str(const ag_http_cookie_t *ctx)
{
    return ag_object_str(ctx);
}

extern ag_string_t *ag_http_cookie_domain(const ag_http_cookie_t *ctx);

extern ag_string_t *ag_http_cookie_path(const ag_http_cookie_t *ctx);

extern ag_string_t *ag_http_cookie_expires(const ag_http_cookie_t *ctx);

extern bool ag_http_cookie_secure(const ag_http_cookie_t *ctx);

extern ag_string_t *ag_http_cookie_param(const ag_http_cookie_t *ctx,
        const char *key);

extern void ag_http_cookie_param_set(ag_http_cookie_t **ctx, const char *key,
        const char *val);





#endif /* !defined __ARGENT_HTTP_COOKIE_H__ */

