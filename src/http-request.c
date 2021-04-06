/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Argent---infrastructure for building web services
 * Copyright (C) 2020 Abhishek Chakravarti
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact Abhishek Chakravarti at <abhishek@taranjali.org>.
 ******************************************************************************/


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


AG_OBJECT_DEFINE(ag_http_request, AG_TYPEID_HTTP_REQUEST);

AG_OBJECT_DEFINE_CLONE(ag_http_request,
        const struct payload *p = _p_;
        return payload_new(p->meth, p->type, p->url, p->usr, p->param);
);


AG_OBJECT_DEFINE_RELEASE(ag_http_request,
        struct payload *p = _p_;

        ag_http_client_release(&p->usr);
        ag_http_url_release(&p->url);
        ag_alist_release(&p->param);
);


AG_OBJECT_DEFINE_CMP(ag_http_request,
        AG_AUTO(ag_string) *s1 = ag_object_str(_o1_);
        AG_AUTO(ag_string) *s2 = ag_object_str(_o2_);

        return ag_string_cmp(s1, s2);
);


AG_OBJECT_DEFINE_SZ(ag_http_request,
        const struct payload *p = ag_object_payload(_o_);
        return sizeof *p + ag_http_client_sz(p->usr) + ag_http_url_sz(p->url)
            + ag_alist_sz(p->param);
);


AG_OBJECT_DEFINE_LEN(ag_http_request,
        (void)_o_;
        return 1;
);


AG_OBJECT_DEFINE_HASH(ag_http_request,
        AG_AUTO(ag_string) *s = ag_object_str(_o_);
        return ag_hash_new_str(s);
);


AG_OBJECT_DEFINE_STR(ag_http_request,
        const struct payload *p = ag_object_payload(_o_);

        AG_AUTO(ag_string) *meth = ag_http_method_str(p->meth);
        AG_AUTO(ag_string) *type = ag_http_mime_str(p->type);
        AG_AUTO(ag_string) *url = ag_http_url_str(p->url);
        AG_AUTO(ag_string) *usr = ag_http_client_str(p->usr);
        AG_AUTO(ag_string) *param = ag_alist_str(p->param);

        return ag_string_new_fmt("[%s] %s: url=%s, client=%s, param=%s",
            meth, type, url, usr, param);
);



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

