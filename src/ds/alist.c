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

#include "../argent.h"


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

AG_OBJECT_DEFINE(ag_alist, AG_TYPEID_ALIST);

AG_OBJECT_DEFINE_CLONE(ag_alist,
        const struct payload *p = _p_;
        return payload_new(p->head);
);

AG_OBJECT_DEFINE_RELEASE(ag_alist,
        struct payload *p = _p_;
        register struct node *n = p->head;

        while (n)
                n = node_release(n);
);

AG_OBJECT_DEFINE_CMP(ag_alist,
        const struct payload *p1 = ag_object_payload(_o1_);
        const struct payload *p2 = ag_object_payload(_o2_);

        if (AG_UNLIKELY (!p1->len))
                return !p2->len ? AG_CMP_EQ : AG_CMP_LT;

        if (AG_UNLIKELY (!p2->len))
                return !p1->len ? AG_CMP_EQ : AG_CMP_GT;

        size_t lim = p1->len < p2->len ? p1->len : p2->len;
        register const struct node *n = p1->head;
        register const struct node *n2 = p2->head;
        register enum ag_cmp chk;

        for (register size_t i = 0; i < lim; i++) {
                if ((chk = ag_field_cmp(n->attr, n2->attr)))
                        return chk;

                n = n->nxt;
                n2 = n2->nxt;
        }

        if (p1->len == p2->len)
                return AG_CMP_EQ;
        else
                return p1->len < p2->len ? AG_CMP_LT : AG_CMP_GT;
);

AG_OBJECT_DEFINE_VALID(ag_alist,
        const struct payload *p = ag_object_payload(_o_);
        register const struct node *n = p->head;

        if (AG_UNLIKELY (!n))
                return false;

        while (n && ag_field_valid(n->attr))
                n = n->nxt;

        return !n;
);

AG_OBJECT_DEFINE_SZ(ag_alist,
        const struct payload *p = ag_object_payload(_o_);
        return p->sz;
);

AG_OBJECT_DEFINE_LEN(ag_alist,
        const struct payload *p = ag_object_payload(_o_);
        return p->len;
);

AG_OBJECT_DEFINE_HASH(ag_alist,
        const struct payload *p = ag_object_payload(_o_);
        return p->hash;
);

AG_OBJECT_DEFINE_STR(ag_alist,
        const struct payload *p = ag_object_payload(_o_);
        struct node *n = p->head;

        ag_string *s = ag_string_new_empty();
        ag_string *s2 = ag_string_new_empty();
        ag_string *s3 = ag_string_new_empty();

        for (register size_t i = 0; i < p->len; i++) {
                ag_string_release(&s);
                s = ag_field_str(n->attr);

                ag_string_release(&s2);
                s2 = ag_string_new_fmt("(%s)", s);

                ag_string_release(&s);
                s = ag_string_clone(s3);

                ag_string_release(&s3);
                s3 = *s ? ag_string_new_fmt("%s %s", s, s2) : ag_string_new(s2);

                n = n->nxt;
        }

        ag_string_release(&s);
        s = ag_string_new_fmt("(%s)", s3);
        
        ag_string_release(&s2);
        ag_string_release(&s3);
        return s;
);



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


extern ag_alist *
ag_alist_parse(const char *src, const char *sep, const char *delim)
{
        AG_ASSERT_PTR (src);
        AG_ASSERT_STR (sep);
        AG_ASSERT_STR (delim);

        ag_alist *a = ag_alist_new_empty();

        if (AG_UNLIKELY (!*src))
                return a;

        ag_string *s = ag_string_new(src);

        if (!ag_string_has(s, delim)) {
                AG_AUTO(ag_field) *f = ag_field_parse(s, sep);
                ag_alist_push(&a, f);
                ag_string_release(&s);

                return a;
        }

        ag_string *l = ag_string_split(s, delim);
        ag_string *r = ag_string_split_right(s, delim);

        AG_AUTO(ag_field) *f = ag_field_parse(l, sep);
        ag_alist_push(&a, f);

        while (*r) {
                ag_string_release(&s);
                s = ag_string_copy(r);

                ag_string_release(&l);
                l = ag_string_split(s, delim);

                AG_AUTO(ag_field) *f = ag_field_parse(*l ? l : s, sep);
                ag_alist_push(&a, f);

                ag_string_release(&r);
                r = ag_string_split_right(s, delim);
        }

        ag_string_release(&s);
        ag_string_release(&l);
        ag_string_release(&r);

        return a;
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
                flag = map(n->attr, in, out);
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

