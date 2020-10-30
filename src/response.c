#include "./api.h"


struct payload {
    enum ag_http_mime type;
    enum ag_http_status code;
    ag_string_t *body;
};


static struct payload *payload_new(enum ag_http_mime type,
        enum ag_http_status code, const char *body)
{
    struct payload *p = ag_memblock_new(sizeof *p);

    p->type = type;
    p->code = code;
    p->body = ag_string_new(body);

    return p;
}


static inline void *method_copy(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return payload_new(p->type, p->code, p->body);
}


static inline void method_dispose(void *ctx)
{
    struct payload *p = (struct payload *) ctx;
    ag_string_dispose(&p->body);
}


static inline ag_string_t *method_str(const ag_object_t *obj)
{
    const struct payload *p = ag_object_payload(obj);
    return ag_string_new_fmt("Content-type: %s; charset=UTF-8\r\n"
            "Status: %s\r\n\r\n%s", ag_http_mime_str(p->type),
            ag_http_status_str(p->code), p->body);
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


extern void ag_response_register(void)
{
    struct ag_object_vtable vt = {
        .copy = NULL,
        .dispose = NULL,
        .id = NULL,
        .sz = &method_sz,
        .len = &method_len,
        .hash = NULL,
        .cmp = &method_cmp,
        .str = &method_str 
    };

    ag_object_register(AG_OBJECT_TYPE_RESPONSE, &vt);
}


extern ag_response_t *ag_response_new(enum ag_http_mime type, 
        enum ag_http_status code)
{
    ag_string_smart_t *head = ag_string_new_fmt("Content-type: %s;"
            " charset=UTF-8\r\nStatus: %s\r\n\r\n", ag_http_mime_str(type),
            ag_http_status_str(code));
    
    return ag_object_new(AG_OBJECT_TYPE_RESPONSE, payload_new(type, code, ""));
}


extern inline ag_response_t *ag_response_copy(const ag_response_t *ctx);


extern inline void ag_response_dispose(ag_response_t **ctx);

extern inline size_t ag_response_typeid(const ag_response_t *ctx);

extern inline size_t ag_response_objid(const ag_response_t *ctx);

extern inline size_t ag_response_hash(const ag_response_t *ctx);

extern inline size_t ag_response_sz(const ag_response_t *ctx);

extern inline size_t ag_response_len(const ag_response_t *ctx);

extern inline bool ag_response_empty(const ag_response_t *ctx);

extern inline enum ag_tristate ag_response_cmp(const ag_response_t *ctx, 
        const ag_response_t *cmp);

extern inline bool ag_response_lt(const ag_response_t *ctx, 
        const ag_response_t *cmp);

extern inline bool ag_response_eq(const ag_response_t *ctx, 
        const ag_response_t *cmp);

extern inline bool ag_response_gt(const ag_response_t *ctx, 
        const ag_response_t *cmp);

extern inline ag_string_t *ag_response_str(const ag_response_t *ctx);


extern void ag_response_add(ag_response_t **ctx, const char *str)
{
    ag_assert (ctx);
    struct payload *p = ag_object_payload_mutable(ctx);
    ag_string_add(&p->body, str);
}


extern void ag_response_add_file(ag_response_t **ctx, const char *fpath)
{
    ag_assert (ctx);
    struct payload *p = ag_object_payload_mutable(ctx);

    ag_assert (fpath && *fpath);
    FILE *file = fopen(fpath, "r");
    ag_require (file, AG_ERNO_HTTP_FILE, NULL);

    
    char bfr[1024] = "";
    while (fgets(bfr, sizeof(bfr), file))
        ag_string_add(&p->body, bfr);

    fclose(file);
}


extern void ag_response_flush(ag_response_t **ctx)
{
    ag_assert (ctx);
    struct payload *p = ag_object_payload_mutable(ctx);

    ag_string_dispose(&p->body);
    p->body = ag_string_new_empty();
}


extern ag_string_t *ag_response_header(const ag_response_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);

    return ag_string_new_fmt("Content-type: %s; charset=UTF-8\r\n"
            "Status: %s\r\n\r\n", ag_http_mime_str(p->type),
            ag_http_status_str(p->code));
}


extern ag_string_t *ag_response_body(const ag_response_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->body);
}

