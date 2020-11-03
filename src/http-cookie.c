#include "./api.h"
#include <string.h>
#include <ctype.h>




/*******************************************************************************
 *                              PAYLOAD INTERFNALS
 */


struct payload {
    bool secure;
    ag_string_t *domain;
    ag_string_t *path;
    ag_string_t *expires;
    ag_string_t *param;
};


static struct payload *payload_new(bool secure, const char *domain,
        const char *path, const char *expires, const char *param)
{
    struct payload *p = ag_memblock_new(sizeof *p);

    p->secure = secure;
    p->domain = ag_string_new(domain);
    p->path = ag_string_new(path);
    p->expires = ag_string_new(expires);
    p->param = ag_string_new(param);

    return p;
}




/*******************************************************************************
 *                              V-TABLE INTERNALS
 */


static inline void *vt_copy(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return payload_new(p->secure, p->domain, p->path, p->expires, p->param);
}


static inline void vt_dispose(void *ctx)
{
    struct payload *p = (struct payload *) ctx;

    ag_string_dispose(&p->domain);
    ag_string_dispose(&p->path);
    ag_string_dispose(&p->expires);
    ag_string_dispose(&p->param);
}




/*******************************************************************************
 *                            URL ENCODING/DECODING
 */


static inline bool url_encoded(const char *param)
{
    return *param == '%' && isxdigit(param[1]) && isxdigit(param[2]);
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


// https://stackoverflow.com/questions/29414709
static ag_string_t *url_encode(const char *str)
{
    ag_string_t *ret = ag_string_new_empty();

    register int c;
    char bfr[4];
    while ((c = *str)) {
        if (c < 33 || c > 126 || strchr("!\"*%'();:@&=+$,/?#[]", *str)) {
            snprintf(bfr, 4, "%%%02X", c & 0xff);
            bfr[3] = '\0';
        } else {
            bfr[0] = c;
            bfr[1] = '\0';
        }

        ag_string_add_cstr(&ret, bfr);
        str++;
    }

    return ret;
}



/*******************************************************************************
 *                              MANAGER EXTERNALS
 */


extern void ag_http_cookie_register(void)
{
    struct ag_object_vtable vt = {
        .copy = &vt_copy,
        .dispose = &vt_dispose,
        .id = NULL,
        .sz = NULL,
        .len = NULL,
        .hash = NULL,
        .cmp = NULL,
        .str = NULL
    };

    ag_object_register(AG_OBJECT_TYPE_HTTP_COOKIE, &vt);
}


extern ag_http_cookie_t *ag_http_cookie_new(bool secure, const char *domain,
        const char *path, const char *expires)
{
    ag_assert (domain && path && expires);
    return ag_object_new(AG_OBJECT_TYPE_HTTP_COOKIE, payload_new(secure, domain,
                path, expires, ""));
}


extern inline ag_http_cookie_t *ag_http_cookie_copy(const ag_http_user_t *ctx);


extern inline void ag_http_cookie_dispose(ag_http_cookie_t **ctx);




/*******************************************************************************
 *                             COMPARATOR EXTERNALS
 */


extern inline enum ag_tristate ag_http_cookie_cmp(const ag_http_cookie_t *ctx,
        const ag_http_cookie_t *cmp);


extern inline bool ag_http_cookie_lt(const ag_http_cookie_t *ctx,
        const ag_http_cookie_t *cmp);


extern inline bool ag_http_cookie_eq(const ag_http_cookie_t *ctx,
        const ag_http_cookie_t *cmp);


extern inline bool ag_http_cookie_gt(const ag_http_cookie_t *ctx,
        const ag_http_cookie_t *cmp);




/*******************************************************************************
 *                              ACCESSOR EXTERNALS
 */


extern inline size_t ag_http_cookie_typeid(const ag_http_cookie_t *ctx);


extern inline size_t ag_http_cookie_objid(const ag_http_cookie_t *ctx);


extern inline size_t ag_http_cookie_hash(const ag_http_cookie_t *ctx);


extern inline size_t ag_http_cookie_sz(const ag_http_cookie_t *ctx);


extern inline size_t ag_http_cookie_len(const ag_http_cookie_t *ctx);


extern inline bool ag_http_cookie_empty(const ag_http_cookie_t *ctx);


extern inline ag_string_t *ag_http_cookie_str(const ag_http_cookie_t *ctx);


extern ag_string_t *ag_http_cookie_domain(const ag_http_user_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return p->domain;
}


extern ag_string_t *ag_http_cookie_path(const ag_http_user_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return p->path;
}


extern ag_string_t *ag_http_cookie_expires(const ag_http_user_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return p->expires;
}


extern bool ag_http_cookie_secure(const ag_http_user_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);
    return p->secure;
}


extern ag_string_t *ag_http_cookie_param(const ag_http_cookie_t *ctx,
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

    char *val =ag_memblock_new(ag_string_sz(p->param) + 1);
    char *v = val;

    while (*c && *c != '&') {
        if (url_encoded(c)) {
            *v++ = (16 * url_decode(c[1])) + url_decode(c[2]);
            c += 3;
        } else if (*c == '+') {
            *v++ = ' ';
            c++;
        } else
            *v++ = *c++;
    }

    *v = '\0';
    ag_string_t *ret = ag_string_new(val);
    ag_memblock_free((void **) &val);

    return ret;
}


extern void ag_http_cookie_param_set(ag_http_cookie_t **ctx, const char *key,
        const char *val)
{
    ag_assert (ctx);
    struct payload *p = ag_object_payload_mutable(ctx);

    if (*p->param)
        ag_string_add(&p->param, ";");

    ag_assert (key && *key);
    ag_string_add(&p->param, key);
    ag_string_add(&p->param, "=");

    ag_assert (val);
    ag_string_smart_t *enc = url_encode(val);
    ag_string_add(&p->param, enc);
}

