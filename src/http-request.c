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

