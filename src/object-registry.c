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


static ag_registry *g_argent;
static ag_registry *g_client;

static void     vt_release(void *);


/* Prototypes for the default callback functions */
static ag_memblock *def_clone(const ag_memblock *);
static void         def_release(ag_memblock *);
static enum ag_cmp  def_cmp(const ag_object *, const ag_object *);
static bool         def_valid(const ag_object *);
static size_t       def_sz(const ag_object *);
static size_t       def_len(const ag_object *);
static size_t       def_hash(const ag_object *);
static ag_string   *def_str(const ag_object *);


extern void
ag_object_registry_init(void)
{
        g_argent = ag_registry_new(vt_release);
        g_client = ag_registry_new(vt_release);
}


extern void
ag_object_registry_exit(void)
{
        ag_registry_release(&g_argent);
        ag_registry_release(&g_client);
}


extern const struct ag_object_vtable *
ag_object_registry_get(ag_typeid typeid)
{
        ag_registry *r = typeid < 0 ? g_argent : g_client;
        ag_hash h = ag_hash_new(typeid);

        struct ag_object_vtable *v = ag_registry_get(r, h);
        return v;
}


extern void
ag_object_registry_push(ag_typeid typeid, const struct ag_object_vtable *vt)
{
        struct ag_object_vtable *v = ag_memblock_new(sizeof *v);
        v->clone = vt->clone ? vt->clone : def_clone;
        v->release = vt->release ? vt->release : def_release;
        v->cmp = vt->cmp ? vt->cmp : def_cmp;
        v->valid = vt->valid ? vt->valid : def_valid;
        v->sz = vt->sz ? vt->sz : def_sz;
        v->len = vt->len ? vt->len : def_len;
        v->hash = vt->hash ? vt->hash : def_hash;
        v->str = vt->str ? vt->str : def_str;

        ag_registry *r = typeid < 0 ? g_argent : g_client;
        ag_hash h = ag_hash_new(typeid);
        ag_registry_push(r, h, v);
}


static void
vt_release(void *hnd)
{
        ag_memblock_release(&hnd);
}


static ag_memblock *
def_clone(const ag_memblock *ctx)
{
        return ag_memblock_clone(ctx);
}


// we don't do anything because ag_object_release() takes care of releasing the
// memory allocated to the payload
static void
def_release(ag_memblock *ctx)
{
        (void)ctx;
}


static enum ag_cmp
def_cmp(const ag_object *ctx, const ag_object *cmp)
{
        return ag_memblock_cmp(ctx, cmp);
}


static bool
def_valid(const ag_object *ctx)
{
        return ctx;
}


static size_t
def_sz(const ag_object *ctx)
{
        return ag_memblock_sz(ctx) + ag_memblock_sz(ag_object_payload(ctx));
}


static size_t
def_len(const ag_object *ctx)
{
        (void)ctx;
        return 1;
}


static ag_hash
def_hash(const ag_object *ctx)
{
        AG_AUTO(ag_uuid) *u = ag_object_uuid(ctx);
        return ag_uuid_hash(u);
}


static ag_string *
def_str(const ag_object *ctx)
{
        AG_AUTO(ag_uuid) *u = ag_object_uuid(ctx);
        AG_AUTO(ag_string)  *ustr = ag_uuid_str(u);
        AG_AUTO(ag_string)  *mstr = ag_memblock_str(ctx);

        return ag_string_new_fmt("typeid = %d, uuid = %s, %s",
            ag_object_typeid(ctx), ustr, mstr);
}

