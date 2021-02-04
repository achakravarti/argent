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
        enum ag_http_mime        mime;
        enum ag_http_status      status;
        ag_string               *body;
};

static struct payload   *payload_new(enum ag_http_mime, enum ag_http_status,
                            const char *);




static ag_memblock      *virt_clone(const ag_memblock *);
static void              virt_release(ag_memblock *);
static enum ag_cmp       virt_cmp(const ag_object *, const ag_object *);
static bool              virt_valid(const ag_object *);
static size_t            virt_sz(const ag_object *);
static size_t            virt_len(const ag_object *);
static ag_hash           virt_hash(const ag_object *);
static ag_string        *virt_str(const ag_object *);




AG_OBJECT_DEFINE(ag_http_response)
{
        struct ag_object_vtable vt = {
                .clone = virt_clone, .release = virt_release, .cmp = virt_cmp,
                .valid = virt_valid, .sz = virt_sz,           .len = virt_len,
                .hash = virt_hash,   .str = virt_str,
        };

        ag_object_registry_set(AG_TYPEID_HTTP_RESPONSE, &vt);
}




extern ag_http_response *
ag_http_response_new(enum ag_http_mime mime, enum ag_http_status status, 
    const char *body)
{
        AG_ASSERT_STR (body);

        return ag_object_new(AG_TYPEID_HTTP_RESPONSE,
            payload_new(mime, status, body));
}




extern ag_http_response *
ag_http_response_new_file(enum ag_http_mime mime, enum ag_http_status status,
    const char *path)
{
        AG_ASSERT_STR (path);

        FILE *file = fopen(path, "r");
        
        char bfr[1024] = "";
        ag_string *tmp = NULL;
        ag_string *body = ag_string_new_empty();

        while (fgets(bfr, sizeof(bfr), file)) {
                tmp = ag_string_new_fmt("%s%s", body, bfr);
                ag_string_release(&body);
                body = ag_string_clone(tmp);
                ag_string_release(&tmp);
        }

        struct payload *p = payload_new(mime, status, body);

        fclose(file);
        ag_string_release(&body);

        return ag_object_new(AG_TYPEID_HTTP_RESPONSE, p);
}




extern ag_http_response *
ag_http_response_new_empty(enum ag_http_mime mime, enum ag_http_status status)
{
        return ag_object_new(AG_TYPEID_HTTP_RESPONSE,
            payload_new(mime, status, ""));
}




extern ag_string *
ag_http_response_header(const ag_http_response *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);

        return ag_string_new_fmt("Content-type: %s; charset=UTF-8\r\n"
            "Status: %s\r\n\r\n", ag_http_mime_str(p->mime),
            ag_http_status_str(p->status));
}




extern ag_string *
ag_http_response_body(const ag_http_response *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->body);
}




extern void
ag_http_response_add(ag_http_response **ctx, const char *body)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT_STR (body);

        struct payload *p = ag_object_payload_mutable(ctx);
        ag_string *add = ag_string_new_fmt("%s%s", p->body, body);

        ag_string_release(&p->body);
        p->body = add;
}




extern void
ag_http_response_add_file(ag_http_response **ctx, const char *path)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT_STR (path);

        FILE *file = fopen(path, "r");
        
        char bfr[1024] = "";
        ag_string *tmp = NULL;
        struct payload *p = ag_object_payload_mutable(ctx);

        while (fgets(bfr, sizeof(bfr), file)) {
                tmp = ag_string_new_fmt("%s%s", p->body, bfr);
                ag_string_release(&p->body);
                p->body = ag_string_clone(tmp);
                ag_string_release(&tmp);
        }

        fclose(file);
}




extern void
ag_http_response_flush(ag_http_response **ctx)
{
        AG_ASSERT_PTR (ctx && *ctx);

        struct payload *p = ag_object_payload_mutable(ctx);

        ag_string_release(&p->body);
        p->body = ag_string_new_empty();
}




static struct payload *
payload_new(enum ag_http_mime mime, enum ag_http_status status,
    const char *body)
{
        AG_ASSERT_PTR (body);

        struct payload *p = ag_memblock_new(sizeof *p);

        p->mime = mime;
        p->status = status;
        p->body = ag_string_new(body);

        return p;
}


static ag_memblock *
virt_clone(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ctx;
        return payload_new(p->mime, p->status, p->body);
}




static void 
virt_release(ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        struct payload *p = ctx;
        ag_string_release(&p->body);
}




static enum ag_cmp
virt_cmp(const ag_object *ctx, const ag_object *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_HTTP_RESPONSE);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_HTTP_RESPONSE);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        AG_AUTO(ag_string) *s2 = virt_str(cmp);

        return ag_string_cmp(s, s2);
}




static bool              
virt_valid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_HTTP_RESPONSE);

        (void)ctx;
        return true;
}




static size_t            
virt_sz(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_HTTP_RESPONSE);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_sz(p->body);
}




static size_t            
virt_len(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_HTTP_RESPONSE);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_len(p->body);
}




static ag_hash           
virt_hash(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_HTTP_RESPONSE);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        return ag_hash_new_str(s);
}




static ag_string *
virt_str(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_HTTP_RESPONSE);

        const struct payload *p = ag_object_payload(ctx);
        AG_AUTO(ag_string) *m = ag_http_mime_str(p->mime);
        AG_AUTO(ag_string) *s = ag_http_status_str(p->status);

        return ag_string_new_fmt("Content-type: %s; charset=UTF-8\r\n"
            "Status: %s\r\n\r\n%s", m, s);
}

