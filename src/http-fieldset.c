#include <string.h>
#include <ctype.h>
#include "../include/argent.h"


struct payload {
    ag_string_t *str;
    char delim;
};

static struct payload *payload_new(const char *str, char delim)
{
    struct payload *p = ag_memblock_new(sizeof *p);

    p->delim = delim;
    p->str = ag_string_new(str);

    return p;
}

static inline void *payload_copy(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return payload_new(p->str, p->delim);
}

static inline void payload_dispose(void *ctx)
{
    struct payload *p = (struct payload *) ctx;
    ag_string_dispose(&p->str);
}

static inline ag_string_t *object_str(const ag_object_t *obj)
{
    const struct payload *p = ag_object_payload(obj);
    return ag_string_copy(p->str);
}

static inline size_t object_sz(const ag_object_t *obj)
{
    const struct payload *p = ag_object_payload(obj);
    return ag_string_sz(p->str);
}

static inline size_t object_len(const ag_object_t *obj)
{
    const struct payload *p = ag_object_payload(obj);
    return ag_string_len(p->str);
}

static inline ag_hash_t object_hash(const ag_object_t *obj)
{
    const struct payload *p = ag_object_payload(obj);
    return ag_string_hash(p->str);
}

static inline enum ag_tristate object_cmp(const ag_object_t *lhs,
        const ag_object_t *rhs)
{
    const struct payload *p1 = ag_object_payload(lhs);
    const struct payload *p2 = ag_object_payload(rhs);
    return ag_string_cmp(p1->str, p2->str);
}


extern void ag_http_fieldset_register(void)
{
    struct ag_object_vtable vt = {
        .copy = &payload_copy,
        .dispose = &payload_dispose,
        .id = NULL,
        .sz = &object_sz,
        .len = &object_len,
        .hash = &object_hash,
        .cmp = &object_cmp,
        .str = &object_str,
    };

    ag_object_register(AG_OBJECT_TYPE_HTTP_BODY, &vt);
}

extern ag_http_fieldset_t *ag_http_fieldset_new(const char *str, char delim)
{
    ag_assert (str && (delim == '&' || delim == ';'));
    return ag_object_new(AG_OBJECT_TYPE_HTTP_BODY, payload_new(str, delim));
}

extern ag_http_fieldset_t *ag_http_fieldset_new_encoded(const char *str,
        char delim)
{
    ag_assert (str && (delim == '&' || delim == ';'));
    return ag_object_new(AG_OBJECT_TYPE_HTTP_BODY, payload_new(str, delim));
}

extern inline ag_http_fieldset_t *ag_http_fieldset_copy(
        const ag_http_fieldset_t *ctx);

extern inline void ag_http_fieldset_dispose(ag_http_fieldset_t **ctx);

extern inline size_t ag_http_fieldset_typeid(const ag_http_fieldset_t *ctx);

extern inline size_t ag_http_fieldset_objid(const ag_http_fieldset_t *ctx);

extern inline size_t ag_http_fieldset_hash(const ag_http_fieldset_t *ctx);

extern inline size_t ag_http_fieldset_sz(const ag_http_fieldset_t *ctx);

extern inline size_t ag_http_fieldset_len(const ag_http_fieldset_t *ctx);

extern inline bool ag_http_fieldset_empty(const ag_http_fieldset_t *ctx);

extern inline enum ag_tristate ag_http_fieldset_cmp(
        const ag_http_fieldset_t *ctx, const ag_http_fieldset_t *cmp);

extern inline bool ag_http_fieldset_lt(const ag_http_fieldset_t *ctx,
        const ag_http_fieldset_t *cmp);

extern inline bool ag_http_fieldset_eq(const ag_http_fieldset_t *ctx,
        const ag_http_fieldset_t *cmp);

extern inline bool ag_http_fieldset_gt(const ag_http_fieldset_t *ctx,
        const ag_http_fieldset_t *cmp);

extern inline ag_string_t *ag_http_fieldset_str(const ag_http_fieldset_t *ctx);

static inline bool url_encoded(const char *str)
{
    return *str == '%' && isxdigit(str[1]) && isxdigit(str[2]);
}

static inline char url_decode(char c)
{
    if (c >= 'a')
        return c - ('a' - 'A');
    else if (c >= 'A')
        return c - ('A' - 10);
    else
        return c - '0';
}


extern ag_string_t *ag_http_fieldset_param(const ag_http_fieldset_t *ctx,
        const char *key)
{
    ag_assert (ctx);
    ag_assert (key && *key);

    const struct payload *p = ag_object_payload(ctx);
    char *k = strstr(p->str, key);
    
    if (k) {
        k += strlen(key);
        
        if (*k == '=')
            k++;
        else
            return ag_string_new_empty();
    } else
        return ag_string_new_empty();
    
    char *val = ag_memblock_new(ag_string_sz(p->str) + 1);
    char *v = val;
    
    while (*k && *k != p->delim) {
        if (url_encoded(k)) {
            *v++ = (16 * url_decode(k[1])) + url_decode(k[2]);
            k += 3;
        } else if (*k == '+') {
            *v++ = ' ';
            k++;
        } else
            *v++ = *k++;
    }

    *v = '\0';
    ag_string_t *ret = ag_string_new(val);
    ag_memblock_free((void **) &val);

    return ret;
}





// https://stackoverflow.com/questions/29414709
static ag_string_t *url_encode(const char *str)
{
    char *bfr = ag_memblock_new((strlen(str) * 3) + 1);
    register size_t n = 0;
    register char c;

    while ((c = *str)) {
        if (c < 33 || c > 126 || strchr("!\"*%'();:@&=+$,/?#[]", *str)) {
            snprintf(bfr + n, 4, "%%%02X", c & 0xff);
            n += 3;
        } else
            bfr[n++] = c;

        str++;
    }

    bfr[n] = '\0';
    return ag_string_new(bfr);
}


extern void ag_http_fieldset_param_add(ag_http_fieldset_t **ctx, 
        const char *param)
{
    ag_assert (ctx && *ctx);
    ag_assert (param && *param);

    struct payload *p = ag_object_payload_mutable(ctx);
    ag_string_smart_t *enc = url_encode(param);
    ag_string_add(&p->str, enc);
}

extern void ag_http_fieldset_param_add_encoded(ag_http_fieldset_t **ctx,
        const char *param)
{
    ag_assert (ctx && *ctx);
    ag_assert (param && *param);

    struct payload *p = ag_object_payload_mutable(ctx);
    ag_string_add(&p->str, param);
}

