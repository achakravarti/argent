#include "../include/argent.h"


extern ag_http_request *
ag_http_request_new(enum ag_http_method meth, enum ag_http_mime type,
    const ag_http_url *url, const ag_http_client *usr, const char *body)
{
}


extern enum ag_http_mime
ag_http_request_mime(const ag_http_request *ctx)
{
}


extern enum ag_http_method       
ag_http_request_method(const ag_http_request *ctx)
{
}


extern ag_http_client *
ag_http_request_client(const ag_http_request *ctx)
{
}


extern ag_http_url *
ag_http_request_url(const ag_http_request *ctx)
{
}


extern ag_alist *
ag_http_request_param(const ag_http_request *ctx)
{
}

