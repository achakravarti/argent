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


struct map_iterator {
        ag_alist_iterator *map;
        void *opt;
};

static ag_memblock *virt_clone(const ag_memblock *);
static void         virt_release(ag_memblock *);
static enum ag_cmp  virt_cmp(const ag_object *, const ag_object *);
static bool         virt_valid(const ag_object *);
static size_t       virt_sz(const ag_object *);
static size_t       virt_len(const ag_object *);
static ag_hash      virt_hash(const ag_object *);
static ag_string   *virt_str(const ag_object *);


static bool     map_has(const ag_value *, void *, void *);
static bool     map_has_key(const ag_value *, void *, void *);
static bool     map_has_val(const ag_value *, void *, void *);
static bool     map_val(const ag_value *, void *, void *);
static bool     map_map(const ag_value *, void *, void *);


AG_OBJECT_DEFINE(ag_alist)
{
        struct ag_object_vtable vt = {
                .clone = virt_clone, .release = virt_release, .cmp = virt_cmp,
                .valid = virt_valid, .sz = virt_sz,           .len = virt_len,
                .hash = virt_hash,   .str = virt_str,
        };

        ag_object_registry_set(AG_TYPEID_LIST, &vt);
}


extern ag_alist *
ag_alist_new(const ag_field *attr)
{
        AG_ASSERT_PTR (attr);

        AG_AUTO(ag_value) *v = ag_value_new_object(attr);
        ag_list *l = ag_list_new();
        ag_list_push(&l, v);

        return ag_object_new(AG_TYPEID_ALIST, l);
}


extern ag_alist *
ag_alist_new_array(const ag_field **attr, size_t len)
{
        AG_ASSERT_PTR (attr);
        AG_ASSERT (len);

        ag_value *v;
        ag_list *l = ag_list_new();

        for (size_t i = 0; i < len; i++) {
                v = ag_value_new_object(attr[i]);
                ag_list_push(&l, v);
                ag_value_release(&v);
        }

        return ag_object_new(AG_TYPEID_ALIST, l);
}


extern ag_alist *
ag_alist_new_empty(void)
{
        return ag_object_new(AG_TYPEID_ALIST, ag_list_new());
}


extern bool
ag_alist_has(const ag_alist *ctx, const ag_field *attr)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (attr);

        bool chk;
        const ag_list *p = ag_object_payload(ctx);
        ag_list_map(p, map_has, (void *)attr, &chk);

        return chk;
}


extern bool
ag_alist_has_key(const ag_alist *ctx, const ag_value *key)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (key);

        bool chk;
        const ag_list *p = ag_object_payload(ctx);
        ag_list_map(p, map_has_key, (void *)key, &chk);

        return chk;
}


extern bool
ag_alist_has_val(const ag_alist *ctx, const ag_value *val)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (val);

        bool chk;
        const ag_list *p = ag_object_payload(ctx);
        ag_list_map(p, map_has_val, (void *)val, &chk);

        return chk;
}


extern ag_field *
ag_alist_get(const ag_alist *ctx)
{
        AG_ASSERT_PTR (ctx);

        const ag_list *p = ag_object_payload(ctx);
        AG_AUTO(ag_value) *v = ag_list_get(p);
        const ag_field *f = ag_value_object(v);

        return ag_field_copy(f);
}


extern ag_field *
ag_alist_get_at(const ag_alist *ctx, size_t idx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (idx);

        const ag_list *p = ag_object_payload(ctx);
        AG_AUTO(ag_value) *v = ag_list_get_at(p, idx);
        const ag_field *f = ag_value_object(v);

        return ag_field_copy(f);
}


extern ag_value *
ag_alist_val(const ag_alist *ctx, const ag_value *key)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (key);

        ag_value *val;
        const ag_list *p = ag_object_payload(ctx);
        ag_list_map(p, map_val, (void *)key, &val);

        return val;
}


extern void
ag_alist_map(const ag_alist *ctx, ag_alist_iterator *map, void *in, void *out)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (map);

        struct map_iterator m = {.map = map, .opt = in};
        const ag_list *p = ag_object_payload(ctx);
        ag_list_map(p, map_map, &m, out);
}


extern void
ag_alist_set(ag_alist **ctx, const ag_field *attr)
{
}


extern void
ag_alist_set_at(ag_alist **ctx, const ag_field *attr, size_t idx)
{
}


extern void
ag_alist_val_set(ag_alist **ctx, const ag_value *key, const ag_value *val)
{
}


extern void
ag_alist_map_mutable(ag_alist **ctx, ag_alist_iterator_mutable *map, void *opt)
{
}


extern void
ag_alist_start(ag_alist **ctx)
{
        AG_ASSERT_PTR (ctx && *ctx);
        
        ag_list *p = ag_object_payload_mutable(ctx);
        ag_list_start(&p);
}


extern bool
ag_alist_next(ag_alist **ctx)
{
        AG_ASSERT_PTR (ctx && *ctx);

        ag_list *p = ag_object_payload_mutable(ctx);
        return ag_list_next(&p);
}


extern void
ag_alist_push(ag_alist **ctx, const ag_field *attr)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT_PTR (attr);

        AG_AUTO(ag_value) *v = ag_value_new_object(attr);
        ag_list *p = ag_object_payload_mutable(ctx);
        ag_list_push(&p, v);
}

        
static bool
map_has(const ag_value *itr, void *in, void *out)
{
        AG_ASSERT_PTR (itr);
        AG_ASSERT_PTR (in);
        AG_ASSERT_PTR (out);

        const ag_field *ctx = ag_value_object(itr);
        const ag_field *cmp = in;
        bool *chk = out;

        *chk = ag_field_eq(ctx, cmp);
        return !*chk;
}


static bool
map_has_key(const ag_value *itr, void *in, void *out)
{
        AG_ASSERT_PTR (itr);
        AG_ASSERT_PTR (in);
        AG_ASSERT_PTR (out);

        const ag_field *ctx = ag_value_object(itr);
        const ag_value *key = in;
        bool *chk = out;

        AG_AUTO(ag_value) *k = ag_field_key(ctx);
        *chk = ag_value_eq(key, k);

        return !*chk;
}


static bool
map_has_val(const ag_value *itr, void *in, void *out)
{
        AG_ASSERT_PTR (itr);
        AG_ASSERT_PTR (in);
        AG_ASSERT_PTR (out);

        const ag_field *ctx = ag_value_object(itr);
        const ag_value *val = in;
        bool *chk = out;

        AG_AUTO(ag_value) *v = ag_field_val(ctx);
        *chk = ag_value_eq(val, v);

        return !*chk;
}


static bool
map_val(const ag_value *itr, void *in, void *out)
{
        AG_ASSERT_PTR (itr);
        AG_ASSERT_PTR (in);
        AG_ASSERT_PTR (out);

        const ag_field *ctx = ag_value_object(itr);
        const ag_value *key = in;
        ag_value **val = out;

        AG_AUTO(ag_value) *k = ag_field_key(ctx);

        if (ag_value_eq(key, k)) {
                *val = ag_field_val(ctx);
                return false;
        }

        return true;
}


static bool
map_map(const ag_value *itr, void *in, void *out)
{
        AG_ASSERT_PTR (itr);
        AG_ASSERT_PTR (in);

        const ag_field *ctx = ag_value_object(itr);
        struct map_iterator *inp = in;

        return inp->map(ctx, inp->opt, out);
}

