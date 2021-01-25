#include "../include/argent.h"


struct payload {
        enum ag_http_mime        type;
        enum ag_http_method      meth;
        ag_http_client          *usr;
        ag_http_url             *url;
        ag_alist                *param;
};


static struct payload   *payload_new(enum ag_http_method, enum ag_http_mime,
                            const ag_http_url *, const ag_http_client *,
                            const ag_alist *);


static ag_memblock      *virt_clone(const ag_memblock *);
static void              virt_release(ag_memblock *);
static enum ag_cmp       virt_cmp(const ag_object *, const ag_object *);
static bool              virt_valid(const ag_object *);
static size_t            virt_sz(const ag_object *);
static size_t            virt_len(const ag_object *);
static ag_hash           virt_hash(const ag_object *);
static ag_string        *virt_str(const ag_object *);


AG_OBJECT_DEFINE(ag_http_request)
{
        struct ag_object_vtable vt = {
                .clone = virt_clone, .release = virt_release, .cmp = virt_cmp,
                .valid = virt_valid, .sz = virt_sz,           .len = virt_len,
                .hash = virt_hash,   .str = virt_str,
        };

        ag_object_registry_set(AG_TYPEID_HTTP_REQUEST, &vt);
}


extern ag_http_request *
ag_http_request_new(enum ag_http_method meth, enum ag_http_mime type,
    const ag_http_url *url, const ag_http_client *usr, const ag_alist *param)
{
        AG_ASSERT_PTR (url);
        AG_ASSERT_PTR (usr);
        AG_ASSERT_PTR (param);

        return ag_object_new(AG_TYPEID_HTTP_REQUEST,
            payload_new(meth, type, url, usr, param));
}


extern enum ag_http_mime
ag_http_request_mime(const ag_http_request *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return p->type;
}


extern enum ag_http_method
ag_http_request_method(const ag_http_request *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return p->meth;
}


extern ag_http_client *
ag_http_request_client(const ag_http_request *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_http_client_copy(p->usr);
}


extern ag_http_url *
ag_http_request_url(const ag_http_request *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_http_url_copy(p->url);
}


extern ag_alist *
ag_http_request_param(const ag_http_request *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_alist_copy(p->param);
}


static struct payload *
payload_new(enum ag_http_method meth, enum ag_http_mime type,
    const ag_http_url *url, const ag_http_client *usr, const ag_alist *param)
{
        AG_ASSERT_PTR (url);
        AG_ASSERT_PTR (usr);
        AG_ASSERT_PTR (param);

        struct payload *p = ag_memblock_new(sizeof *p);

        p->meth = meth;
        p->type = type;
        p->url = ag_http_url_copy(url);
        p->usr = ag_http_client_copy(usr);
        p->param = ag_alist_copy(param);

        return p;
}


static ag_memblock *
virt_clone(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ctx;
        return payload_new(p->meth, p->type, p->url, p->usr, p->param);
}


static void
virt_release(ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        struct payload *p = ctx;

        ag_http_client_release(&p->usr);
        ag_http_url_release(&p->url);
        ag_alist_release(&p->param);
}


static enum ag_cmp
virt_cmp(const ag_object *ctx, const ag_object *cmp)
{
        AG_ASSERT_PTR (ctx);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        AG_AUTO(ag_string) *s2 = virt_str(cmp);

        return ag_string_cmp(s, s2);
}


static bool
virt_valid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        (void)ctx;
        return true;
}


static size_t
virt_sz(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return sizeof *p + ag_http_client_sz(p->usr) + ag_http_url_sz(p->url)
            + ag_alist_sz(p->param);
}


static size_t
virt_len(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        (void)ctx;
        return 1;
}


static ag_hash
virt_hash(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        return ag_hash_new_str(s);
}


static ag_string *
virt_str(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);

        AG_AUTO(ag_string) *meth = ag_http_method_str(p->meth);
        AG_AUTO(ag_string) *type = ag_http_mime_str(p->type);
        AG_AUTO(ag_string) *url = ag_http_url_str(p->url);
        AG_AUTO(ag_string) *usr = ag_http_client_str(p->usr);
        AG_AUTO(ag_string) *param = ag_alist_str(p->param);

        return ag_string_new_fmt("[%s] %s: url=%s, client=%s, param=%s",
            meth, type, url, usr, param);
}
