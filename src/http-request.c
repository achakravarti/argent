#include "../include/argent.h"
#include <string.h>

struct payload {
    enum ag_http_method meth;
    enum ag_http_mime mime;
    ag_string_t *referer;
    ag_string_t *param;
};


static struct payload *payload_new(enum ag_http_method meth,
        enum ag_http_mime mime, const char *referer, const char *param)
{
    struct payload *p = ag_memblock_new(sizeof *p);

    p->meth = meth;
    p->mime = mime;
    p->referer = ag_string_new(referer);
    p->param = ag_string_new(param);

    return p;
}


static inline void *method_copy(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return payload_new(p->meth, p->mime, p->referer, p->param);
}


static inline void method_dispose(void *ctx)
{
    struct payload *p = (struct payload *) ctx;
    ag_string_dispose(&p->referer);
    ag_string_dispose(&p->param);
}


static inline ag_string_t *method_str(const ag_object_t *obj)
{
    const struct payload *p = ag_object_payload(obj);
    return ag_string_copy(p->param);
}


static inline size_t method_sz(const ag_object_t *obj)
{
    ag_string_smart_t *s = method_str(obj);
    return ag_string_sz(s);
}


static inline size_t method_len(const ag_object_t *obj)
{
    ag_string_smart_t *s = method_str(obj);
    return ag_string_len(s);
}


static enum ag_tristate method_cmp(const ag_object_t *lhs,
        const ag_object_t *rhs)
{
    ag_string_smart_t *s1 = method_str(lhs);
    ag_string_smart_t *s2 = method_str(rhs);
    return ag_string_cmp(s1, s2);
}


static inline ag_hash_t method_hash(const ag_object_t *obj)
{
    ag_string_smart_t *s = method_str(obj);
    return ag_string_hash(s);
}


extern void ag_http_request_register(void)
{
    struct ag_object_vtable vt = {
        .copy = &method_copy,
        .dispose = &method_dispose,
        .id = NULL,
        .sz = &method_sz,
        .len = &method_len,
        .hash = &method_hash,
        .cmp = &method_cmp,
        .str = &method_str,
    };

    ag_object_register(AG_OBJECT_TYPE_HTTP_REQUEST, &vt);
}


extern ag_http_request_t *ag_http_request_new(enum ag_http_method meth,
        enum ag_http_mime mime, const char *referer, const char *param)
{
    ag_assert (referer && param);
    return ag_object_new(AG_OBJECT_TYPE_HTTP_REQUEST, payload_new(meth, mime,
                referer, param));
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
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);

    ag_assert (key && *key);
    char *c = strstr(p->param, key);

    if (c) {
        c += strlen(key);

        if (*c == '=')
            c++;
        else
            return ag_string_new_empty();
    } else
        return ag_string_new_empty();

    char *val = ag_memblock_new(ag_string_sz(p->param) + 1);
    char *v = val;
    while (*c && *c != '&')
        *v++ = *c++;

    *v = '\0';
    ag_string_smart_t *ret = ag_string_new(val);
    ag_string_t *ret1 = ag_string_url_decode(ret);
    ag_memblock_free((void **) &val);

    return ret1;
}

