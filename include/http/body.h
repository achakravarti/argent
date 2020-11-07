#if (!defined __ARGENT_HTTP_BODY_H__)
#define __ARGENT_HTTP_BODY_H__


#include "../object.h"
#include "./primitives.h"


typedef ag_object_t ag_http_body_t;
#define ag_http_body_smart_t ag_object_smart_t

extern void ag_http_body_register(void);

extern ag_http_body_t *ag_http_body_new(const char *str, char delim);
extern ag_http_body_t *ag_http_body_new_encoded(const char *str, char delim);

inline ag_http_body_t *ag_http_body_copy(const ag_http_body_t *ctx)
{
    return ag_object_copy(ctx);
}

inline void ag_http_body_dispose(ag_http_body_t **ctx)
{
    ag_object_dispose(ctx);
}

inline size_t ag_http_body_typeid(const ag_http_body_t *ctx)
{
    return ag_object_type(ctx);
}

inline size_t ag_http_body_objid(const ag_http_body_t *ctx)
{
    return ag_object_id(ctx);
}

inline size_t ag_http_body_hash(const ag_http_body_t *ctx)
{
    return ag_object_hash(ctx);
}

inline size_t ag_http_body_sz(const ag_http_body_t *ctx)
{
    return ag_object_sz(ctx);
}

inline size_t ag_http_body_len(const ag_http_body_t *ctx)
{
    return ag_object_len(ctx);
}

inline bool ag_http_body_empty(const ag_http_body_t *ctx)
{
    return ag_object_empty(ctx);
}

inline enum ag_tristate ag_http_body_cmp(const ag_http_body_t *ctx,
        const ag_http_body_t *cmp)
{
    return ag_object_cmp(ctx, cmp);
}

inline bool ag_http_body_lt(const ag_http_body_t *ctx,
        const ag_http_body_t *cmp)
{
    return ag_object_lt(ctx, cmp);
}

inline bool ag_http_body_eq(const ag_http_body_t *ctx,
        const ag_http_body_t *cmp)
{
    return ag_object_eq(ctx, cmp);
}

inline bool ag_http_body_gt(const ag_http_body_t *ctx,
        const ag_http_body_t *cmp)
{
    return ag_object_gt(ctx, cmp);
}

inline ag_string_t *ag_http_body_str(const ag_http_body_t *ctx)
{
    return ag_object_str(ctx);
}

extern ag_string_t *ag_http_body_param(const ag_http_body_t *ctx,
        const char *key);


#endif /* !defined __ARGENT_HTTP_BODY_H__ */

