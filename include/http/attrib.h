#if (!defined __ARGENT_HTTP_ATTRIB_H__)
#define __ARGENT_HTTP_ATTRIB_H__

#include "../object.h"


typedef ag_object_t ag_http_attrib_t;
#define ag_http_attrib_smart_t ag_object_smart_t
extern void ag_http_attrib_register(void);


extern ag_http_attrib_t *ag_http_attrib_new(const char *name, const char *val);
extern ag_http_attrib_t *ag_http_attrib_new_nameonly(const char *name);


inline ag_http_attrib_t *ag_http_attrib_copy(const ag_http_attrib_t *ctx)
{
    return ag_object_copy(ctx);
}

inline void ag_http_attrib_dispose(ag_http_attrib_t **ctx)
{
    ag_object_dispose(ctx);
}

inline enum ag_tristate ag_http_attrib_cmp(const ag_http_attrib_t *ctx,
        const ag_http_attrib_t *cmp)
{
    return ag_object_cmp(ctx, cmp);
}

inline bool ag_http_attrib_lt(const ag_http_attrib_t *ctx,
        const ag_http_attrib_t *cmp)
{
    return ag_object_lt(ctx, cmp);
}

inline bool ag_http_attrib_eq(const ag_http_attrib_t *ctx,
        const ag_http_attrib_t *cmp)
{
    return ag_object_eq(ctx, cmp);
}

inline bool ag_http_attrib_gt(const ag_http_attrib_t *ctx,
        const ag_http_attrib_t *cmp)
{
    return ag_object_gt(ctx, cmp);
}

inline bool ag_http_attrib_empty(const ag_http_attrib_t *ctx)
{
    return ag_object_empty(ctx);
}

extern bool ag_http_attrib_nameonly(const ag_http_attrib_t *ctx);

inline size_t ag_http_attrib_typeid(const ag_http_attrib_t *ctx)
{
    return ag_object_type(ctx);
}

inline size_t ag_http_attrib_objid(const ag_http_attrib_t *ctx)
{
    return ag_object_id(ctx);
}

inline size_t ag_http_attrib_hash(const ag_http_attrib_t *ctx)
{
    return ag_object_hash(ctx);
}

inline size_t ag_http_attrib_sz(const ag_http_attrib_t *ctx)
{
    return ag_object_sz(ctx);
}

inline size_t ag_http_attrib_len(const ag_http_attrib_t *ctx)
{
    return ag_object_len(ctx);
}

extern ag_string_t *ag_http_attrib_name(const ag_http_attrib_t *ctx);

extern ag_string_t *ag_http_attrib_value(const ag_http_attrib_t *ctx);

inline ag_string_t *ag_http_attrib_str(const ag_http_attrib_t *ctx)
{
    return ag_object_str(ctx);
}

#endif /* !defined __ARGENT_HTTP_ATTRIB_H__ */

