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


static ag_memblock *virt_clone(const ag_memblock *);
static void         virt_release(ag_memblock *);
static enum ag_cmp  virt_cmp(const ag_object *, const ag_object *);
static bool         virt_valid(const ag_object *);
static size_t       virt_sz(const ag_object *);
static size_t       virt_len(const ag_object *);
static ag_hash      virt_hash(const ag_object *);
static ag_string   *virt_str(const ag_object *);


AG_OBJECT_DEFINE(ag_list)
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
}


extern bool
ag_alist_has_key(const ag_alist *ctx, const ag_value *key)
{
}


extern bool
ag_alist_has_val(const ag_alist *ctx, const ag_value *val)
{
}


extern ag_field *
ag_alist_get(const ag_alist *ctx)
{
}


extern ag_field *
ag_alist_get_at(const ag_alist *ctx, size_t idx)
{
}


extern ag_value *
ag_alist_val(const ag_alist *ctx, const ag_value *idx)
{
}


extern void
ag_alist_map(const ag_alist *ctx, ag_alist_iterator *map, void *opt)
{
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
}


extern bool
ag_alist_next(ag_alist **ctx)
{
}


extern void
ag_alist_push(ag_alist **ctx, const ag_field *attr)
{
}

