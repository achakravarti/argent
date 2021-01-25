#include "../include/argent.h"


struct payload {
        enum ag_http_mime        type;
        enum ag_http_method      meth;
        ag_http_client          *usr;
        ag_http_url             *url;
        ag_alist                *param;
};


extern ag_http_request *
ag_http_request_new(enum ag_http_method meth, enum ag_http_mime type,
    const ag_http_url *url, const ag_http_client *usr, const char *body)
{
        AG_ASSERT_PTR (url);
        AG_ASSERT_PTR (usr);
        AG_ASSERT_PTR (body);
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

