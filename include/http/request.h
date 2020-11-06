#if (!defined __ARGENT_HTTP_REQUEST_H__)
#define __ARGENT_HTTP_REQUEST_H__


#include "./object.h"
#include "./primitives.h"


typedef ag_object_t ag_http_request_t;
#define ag_http_request_smart_t ag_object_smart_t
extern void ag_http_request_register(void);


extern ag_http_request_t *ag_http_request_new(enum ag_http_method meth,
        enum ag_http_mime mime, const char *referer, const char *param);

inline ag_http_request_t *ag_http_request_copy(const ag_http_request_t *ctx)
{
    return ag_object_copy(ctx);
}

inline void ag_http_request_dispose(ag_http_request_t **ctx)
{
    ag_object_dispose(ctx);
}

inline size_t ag_http_request_typeid(const ag_http_request_t *ctx)
{
    return ag_object_type(ctx);
}

inline size_t ag_http_request_objid(const ag_http_request_t *ctx)
{
    return ag_object_id(ctx);
}

inline size_t ag_http_request_hash(const ag_http_request_t *ctx)
{
    return ag_object_hash(ctx);
}

inline size_t ag_http_request_sz(const ag_http_request_t *ctx)
{
    return ag_object_sz(ctx);
}

inline size_t ag_http_request_len(const ag_http_request_t *ctx)
{
    return ag_object_len(ctx);
}

inline bool ag_http_request_empty(const ag_http_request_t *ctx)
{
    return ag_object_empty(ctx);
}

inline enum ag_tristate ag_http_request_cmp(const ag_http_request_t *ctx,
        const ag_http_request_t *cmp)
{
    return ag_object_cmp(ctx, cmp);
}

inline bool ag_http_request_lt(const ag_http_request_t *ctx,
        const ag_http_request_t *cmp)
{
    return ag_object_lt(ctx, cmp);
}

inline bool ag_http_request_eq(const ag_http_request_t *ctx,
        const ag_http_request_t *cmp)
{
    return ag_object_eq(ctx, cmp);
}

inline bool ag_http_request_gt(const ag_http_request_t *ctx,
        const ag_http_request_t *cmp)
{
    return ag_object_gt(ctx, cmp);
}

inline ag_string_t *ag_http_request_str(const ag_http_request_t *ctx)
{
    return ag_object_str(ctx);
}

extern enum ag_http_method ag_http_request_method(void);

extern enum ag_http_mime ag_http_request_mime(void);

extern ag_string_t *ag_http_request_referer(void);

extern ag_string_t *ag_http_request_param(const char *key);


#endif /* !defined __ARGENT_HTTP_REQUEST_H__ */

