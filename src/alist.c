/*-
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Argent - infrastructure for building web services
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
 */

#include "../include/argent.h"


struct node {
        ag_field        *attr; /* node field */
        struct node     *nxt; /* next node  */
};


struct payload {
        struct node     *head; /* start of list             */
        struct node     *tail; /* end of list               */
        struct node     *itr;  /* current iterator position */
        size_t           len;  /* number of items           */
        size_t           sz;   /* cumulative size           */
        ag_hash          hash; /* cumulative hash           */
};


static inline struct node       *node_new(const ag_field *);
static inline struct node       *node_release(struct node *);


static struct payload   *payload_new(const struct node *);
static void              payload_push(struct payload *, const ag_field *);


static ag_memblock *virt_clone(const ag_memblock *);
static void         virt_release(ag_memblock *);
static enum ag_cmp  virt_cmp(const ag_object *, const ag_object *);
static bool         virt_valid(const ag_object *);
static size_t       virt_sz(const ag_object *);
static size_t       virt_len(const ag_object *);
static ag_hash      virt_hash(const ag_object *);
static ag_string   *virt_str(const ag_object *);


AG_OBJECT_DEFINE(ag_alist)
{
        struct ag_object_vtable vt = {
                .clone = virt_clone, .release = virt_release, .cmp = virt_cmp,
                .valid = virt_valid, .sz = virt_sz,           .len = virt_len,
                .hash = virt_hash,   .str = virt_str,
        };

        ag_object_registry_set(AG_TYPEID_ALIST, &vt);
}


extern ag_alist *
ag_alist_new(const ag_field *attr)
{
        AG_ASSERT_PTR (attr);

        struct payload *p = payload_new(NULL);
        payload_push(p, attr);

        return ag_object_new(AG_TYPEID_ALIST, p);
}


extern ag_alist *
ag_alist_new_array(const ag_field **attr, size_t len)
{
        AG_ASSERT_PTR (attr);
        AG_ASSERT (len);

        struct payload *p = payload_new(NULL);

        for (register size_t i = 0; i < len; i++)
                payload_push(p, attr[i]);
        
        return ag_object_new(AG_TYPEID_ALIST, p);
}


extern ag_alist *
ag_alist_new_empty(void)
{
        return ag_object_new(AG_TYPEID_ALIST, payload_new(NULL));
}


extern bool
ag_alist_has(const ag_alist *ctx, const ag_field *attr)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (attr);

        const struct payload *p = ag_object_payload(ctx);
        register const struct node *n = p->head;

        while (n) {
                if (ag_field_eq(n->attr, attr))
                        return true;

                n = n->nxt;
        }

        return false;
}


extern bool
ag_alist_has_key(const ag_alist *ctx, const ag_value *key)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (key);

        ag_value *k;
        const struct payload *p = ag_object_payload(ctx);
        register const struct node *n = p->head;

        while (n) {
                k = ag_field_key(n->attr);
                if (ag_value_eq(k, key)) {
                        ag_value_release(&k);
                        return true;
                }

                ag_value_release(&k);
                n = n->nxt;
        }

        return false;
}


extern bool
ag_alist_has_val(const ag_alist *ctx, const ag_value *val)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (val);

        ag_value *v;
        const struct payload *p = ag_object_payload(ctx);
        register const struct node *n = p->head;

        while (n) {
                v = ag_field_val(n->attr);
                if (ag_value_eq(v, val)) {
                        ag_value_release(&v);
                        return true;
                }

                ag_value_release(&v);
                n = n->nxt;
        }

        return false;
}


extern ag_field *
ag_alist_get(const ag_alist *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (!ag_alist_empty(ctx));

        const struct payload *p = ag_object_payload(ctx);
        return ag_field_copy(p->itr->attr);
}


extern ag_field *
ag_alist_get_at(const ag_alist *ctx, size_t idx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (!ag_alist_empty(ctx));
        AG_ASSERT (idx >= 1 && idx <= ag_alist_len(ctx));

        const struct payload *p = ag_object_payload(ctx);
        register const struct node *n = p->head;

        for (register size_t i = 1; i < idx; i++)
                n = n->nxt;

        return ag_field_copy(n->attr);
}


extern ag_value *
ag_alist_val(const ag_alist *ctx, const ag_value *key)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (key);

        ag_value *k;
        const struct payload *p = ag_object_payload(ctx);
        register const struct node *n = p->head;

        while (n) {
                k = ag_field_key(n->attr);
                if (ag_value_eq(k, key)) {
                        ag_value_release(&k);
                        return ag_field_val(n->attr);
                }

                ag_value_release(&k);
                n = n->nxt;
        }

        AG_ASSERT (false);
        return NULL;
}


extern void
ag_alist_map(const ag_alist *ctx, ag_alist_iterator *map, void *in, void *out)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (map);

        const struct payload *p = ag_object_payload(ctx);
        register const struct node *n = p->head;
        register bool flag = true;

        while (n && flag) {
                flag = map(ag_field_copy(n->attr), in, out);
                n = n->nxt;
        }
}


extern void
ag_alist_set(ag_alist **ctx, const ag_field *attr)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT_PTR (attr);
        AG_ASSERT (!ag_alist_empty(*ctx));

        struct payload *p = ag_object_payload_mutable(ctx);
        ag_field_release(&p->itr->attr);
        p->itr->attr = ag_value_copy(attr);
}


extern void
ag_alist_set_at(ag_alist **ctx, const ag_field *attr, size_t idx)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT_PTR (attr);
        AG_ASSERT (!ag_alist_empty(*ctx));
        AG_ASSERT (idx >= 1 && idx <= ag_alist_len(*ctx));

        struct payload *p = ag_object_payload_mutable(ctx);
        register struct node *n = p->head;

        for (register size_t i = 1; i < idx; i++)
                n = n->nxt;

        ag_field_release(&n->attr);
        n->attr = ag_value_copy(attr);
}


extern void
ag_alist_val_set(ag_alist **ctx, const ag_value *key, const ag_value *val)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT_PTR (key);
        AG_ASSERT_PTR (val);

        ag_value *k;
        struct payload *p = ag_object_payload_mutable(ctx);
        register struct node *n = p->head;

        while (n) {
                k = ag_field_key(n->attr);
                if (ag_value_eq(k, key)) {
                        ag_value_release(&k);
                        return ag_field_val_set(&n->attr, val);
                }

                ag_value_release(&k);
                n = n->nxt;
        }
}


extern void
ag_alist_map_mutable(ag_alist **ctx, ag_alist_iterator_mutable *map, void *in,
    void *out)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT_PTR (map);

        struct payload *p = ag_object_payload_mutable(ctx);
        register struct node *n = p->head;
        register bool flag = true;

        while (n && flag) {
                flag = map(&n->attr, in, out);
                n = n->nxt;
        }
}


extern void
ag_alist_start(ag_alist **ctx)
{
        AG_ASSERT_PTR (ctx && *ctx);

        struct payload *p = ag_object_payload_mutable(ctx);
        p->itr = p->head;
}


extern bool
ag_alist_next(ag_alist **ctx)
{
        AG_ASSERT_PTR (ctx && *ctx);

        struct payload *p = ag_object_payload_mutable(ctx);

        if (AG_LIKELY (p->itr)) {
                p->itr = p->itr->nxt;
                return p->itr->nxt;
        }

        return false;
}


extern void
ag_alist_push(ag_alist **ctx, const ag_field *attr)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT_PTR (attr);

        struct payload *p = ag_object_payload_mutable(ctx);
        payload_push(p, attr);
}

        
static inline struct node*
node_new(const ag_field *val)
{
        AG_ASSERT_PTR (val);

        struct node *n = ag_memblock_new(sizeof *n);
        n->attr = ag_field_copy(val);
        n->nxt = NULL;

        return n;
}

        
static inline struct node*
node_release(struct node *ctx)
{
        AG_ASSERT_PTR (ctx);

        struct node *nxt = ctx->nxt;
        void *ptr = ctx;

        ag_field_release(&ctx->attr);
        ag_memblock_release(&ptr);

        return nxt;
}

        
static struct payload *
payload_new(const struct node *head)
{
        struct payload *p = ag_memblock_new(sizeof *p);
        p->len = p->sz = p->hash = 0;
        p->itr = NULL;

        if (!head) {
                p->head = p->tail = NULL;
                return p;
        }

        register const struct node *n = head;
        while (n) {
                payload_push(p, n->attr);
                n = n->nxt;
        }

        return p;
}

        
static void
payload_push(struct payload *ctx, const ag_field *attr)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (attr);

        struct node *n = node_new(attr);

        if (AG_LIKELY (ctx->tail))
                ctx->tail->nxt = n;
        else
                ctx->head = n;

        ctx->tail = n;
        ctx->len++;
        ctx->sz += ag_field_sz(attr);
        ctx->hash += ag_field_hash(attr);
}


        
static ag_memblock *
virt_clone(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ctx;
        return payload_new(p->head);
}


static void
virt_release(ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        struct payload *p = ctx;
        register struct node *n = p->head;

        while (n)
                n = node_release(n);
}


static enum ag_cmp
virt_cmp(const ag_object *ctx, const ag_object *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_ALIST);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_ALIST);

        const struct payload *p = ag_object_payload(ctx);
        const struct payload *p2 = ag_object_payload(cmp);

        if (AG_UNLIKELY (!p->len))
                return !p2->len ? AG_CMP_EQ : AG_CMP_LT;

        if (AG_UNLIKELY (!p2->len))
                return !p->len ? AG_CMP_EQ : AG_CMP_GT;

        size_t lim = p->len < p2->len ? p->len : p2->len;
        register const struct node *n = p->head;
        register const struct node *n2 = p2->head;
        register enum ag_cmp chk;

        for (register size_t i = 0; i < lim; i++) {
                if ((chk = ag_field_cmp(n->attr, n2->attr)))
                        return chk;

                n = n->nxt;
                n2 = n2->nxt;
        }

        if (p->len == p2->len)
                return AG_CMP_EQ;
        else
                return p->len < p2->len ? AG_CMP_LT : AG_CMP_GT;
}


static bool
virt_valid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_ALIST);

        const struct payload *p = ag_object_payload(ctx);
        register const struct node *n = p->head;

        if (AG_UNLIKELY (!n))
                return false;

        while (n && ag_field_valid(n->attr))
                n = n->nxt;

        return !n;
}


static size_t
virt_sz(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_ALIST);

        const struct payload *p = ag_object_payload(ctx);
        return p->sz;
}


static size_t
virt_len(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_ALIST);

        const struct payload *p = ag_object_payload(ctx);
        return p->len;
}


static ag_hash
virt_hash(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_ALIST);

        const struct payload *p = ag_object_payload(ctx);
        return p->hash;
}


static ag_string
*virt_str(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_ALIST);

        return ag_string_new_fmt("list len = %lu", ag_object_len(ctx));
}


        
