#if (!defined __ARGENT_HTTP_RESPONSE_H__)
#define __ARGENT_HTTP_RESPONSE_H__


#include "../object.h"
#include "./primitives.h"


/*******************************************************************************
 *                                HTTP RESPONSE
 */


typedef ag_object_t ag_http_response_t;
#define ag_http_response_smart_t ag_object_smart_t
extern void ag_http_response_register(void);

extern ag_http_response_t *ag_http_response_new(enum ag_http_mime type, 
        enum ag_http_status code);

inline ag_http_response_t *ag_http_response_copy(const ag_http_response_t *ctx)
{
    return ag_object_copy(ctx);
}


inline void ag_http_response_dispose(ag_http_response_t **ctx)
{
    ag_object_dispose(ctx);
}

inline size_t ag_http_response_typeid(const ag_http_response_t *ctx)
{
    return ag_object_type(ctx);
}

inline size_t ag_http_response_objid(const ag_http_response_t *ctx)
{
    return ag_object_id(ctx);
}

inline size_t ag_http_response_hash(const ag_http_response_t *ctx)
{
    return ag_object_hash(ctx);
}

inline size_t ag_http_response_sz(const ag_http_response_t *ctx)
{
    return ag_object_sz(ctx);
}

inline size_t ag_http_response_len(const ag_http_response_t *ctx)
{
    return ag_object_len(ctx);
}

inline bool ag_http_response_empty(const ag_http_response_t *ctx)
{
    return ag_object_empty(ctx);
}

inline enum ag_tristate ag_http_response_cmp(const ag_http_response_t *ctx, 
        const ag_http_response_t *cmp)
{
    return ag_object_cmp(ctx, cmp);
}

inline bool ag_http_response_lt(const ag_http_response_t *ctx,
        const ag_http_response_t *cmp)
{
    return ag_object_lt(ctx, cmp);
}

inline bool ag_http_response_eq(const ag_http_response_t *ctx,
        const ag_http_response_t *cmp)
{
    return ag_object_eq(ctx, cmp);
}

inline bool ag_http_response_gt(const ag_http_response_t *ctx,
        const ag_http_response_t *cmp)
{
    return ag_object_gt(ctx, cmp);
}

inline ag_string_t *ag_http_response_str(const ag_http_response_t *ctx)
{
    return ag_object_str(ctx);
}


extern void ag_http_response_add(ag_http_response_t **ctx,
        const char *str);

extern void ag_http_response_add_file(ag_http_response_t **ctx,
        const char *fpath);

extern void ag_http_response_flush(ag_http_response_t **ctx);

extern ag_string_t *ag_http_response_header(const ag_http_response_t *ctx);

extern ag_string_t *ag_http_response_body(const ag_http_response_t *ctx);

#endif /* !defined __ARGENT_HTTP_RESPONSE_H__ */

