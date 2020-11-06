#include "../include/argent.h"

struct payload {
    enum ag_http_method meth;
    enum ag_http_mime mime;
    ag_string_t *referer;
    ag_string_t *param;
};


extern void ag_http_request_register(void)
{
}


extern ag_http_request_t *ag_http_request_new(enum ag_http_method meth,
        enum ag_http_mime mime, const char *referer, const char *param)
{
}


extern inline ag_http_request_t *ag_http_request_copy(
        const ag_http_request_t *ctx);

extern inline void ag_http_request_dispose(ag_http_request_t **ctx);

extern inline size_t ag_http_request_typeid(const ag_http_request_t *ctx);

extern inline size_t ag_http_request_objid(const ag_http_request_t *ctx);

extern inline size_t ag_http_request_hash(const ag_http_request_t *ctx);

extern inline size_t ag_http_request_sz(const ag_http_request_t *ctx);

extern inline size_t ag_http_request_len(const ag_http_request_t *ctx);

extern inline bool ag_http_request_empty(const ag_http_request_t *ctx);

extern inline enum ag_tristate ag_http_request_cmp(const ag_http_request_t *ctx,
        const ag_http_request_t *cmp);

extern inline bool ag_http_request_lt(const ag_http_request_t *ctx,
        const ag_http_request_t *cmp);

extern inline bool ag_http_request_eq(const ag_http_request_t *ctx,
        const ag_http_request_t *cmp);

extern inline bool ag_http_request_gt(const ag_http_request_t *ctx,
        const ag_http_request_t *cmp);

extern inline ag_string_t *ag_http_request_str(const ag_http_request_t *ctx);


extern enum ag_http_method ag_http_request_method(const ag_http_request_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return p->meth;
}


extern enum ag_http_mime ag_http_request_mime(const ag_http_request_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return p->mime;
}


extern ag_string_t *ag_http_request_referer(const ag_http_request_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->referer);
}


extern ag_string_t *ag_http_request_param(const ag_http_request_t *ctx,
        const char *key)
{
    ag_assert (ctx && key && *key);
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->param);
}

