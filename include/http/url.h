#if (!defined __ARGENT_HTTP_URL_H__)
#define __ARGENT_HTTP_URL_H__


#include <stdbool.h>
#include "../object.h"


/*******************************************************************************
 *                                   HTTP URL
 */


typedef ag_object_t ag_http_url_t;

#define ag_http_url_smart_t ag_object_smart_t

extern void ag_http_url_register(void);

extern ag_http_url_t *ag_http_url_new(bool secure, const char *host,
        const char *port, const char *path);

inline ag_http_url_t *ag_http_url_copy(const ag_http_url_t *ctx)
{
    return ag_object_copy(ctx);
}

inline void ag_http_url_dispose(ag_http_url_t **ctx)
{
    ag_object_dispose(ctx);
}

inline size_t ag_http_url_typeid(const ag_http_url_t *ctx)
{
    return ag_object_type(ctx);
}

inline size_t ag_http_url_objid(const ag_http_url_t *ctx)
{
    return ag_object_id(ctx);
}

inline size_t ag_http_url_hash(const ag_http_url_t *ctx)
{
    return ag_object_hash(ctx);
}

inline size_t ag_http_url_sz(const ag_http_url_t *ctx)
{
    return ag_object_sz(ctx);
}

inline size_t ag_http_url_len(const ag_http_url_t *ctx)
{
    return ag_object_len(ctx);
}

inline bool ag_http_url_empty(const ag_http_url_t *ctx)
{
    return ag_object_empty(ctx);
}

inline enum ag_tristate ag_http_url_cmp(const ag_http_url_t *ctx,
        const ag_http_url_t *cmp)
{
    return ag_object_cmp(ctx, cmp);
}

inline bool ag_http_url_lt(const ag_http_url_t *ctx, const ag_http_url_t *cmp)
{
    return ag_object_lt(ctx, cmp);
}

inline bool ag_http_url_eq(const ag_http_url_t *ctx, const ag_http_url_t *cmp)
{
    return ag_object_eq(ctx, cmp);
}

inline bool ag_http_url_gt(const ag_http_url_t *ctx, const ag_http_url_t *cmp)
{
    return ag_object_gt(ctx, cmp);
}

inline ag_string_t *ag_http_url_str(const ag_http_url_t *ctx)
{
    return ag_object_str(ctx);
}


extern bool ag_http_url_secure(const ag_http_url_t *ctx);
extern ag_string_t *ag_http_url_host(const ag_http_url_t *ctx);
extern ag_string_t *ag_http_url_port(const ag_http_url_t *ctx);
extern ag_string_t *ag_http_url_path(const ag_http_url_t *ctx);

#endif /* !defined __ARGENT_HTTP_URL_H__ */

