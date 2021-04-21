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


#include "../../include/argent.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>


struct ag_object {
        ag_typeid        typeid;  /* Object type ID */
        ag_uuid         *uuid;    /* Object ID      */
        ag_memblock     *payload; /* Object payload */
};


static inline const struct ag_object_vtable *
vtable_get(const ag_object *ctx)
{
        return ag_object_registry_get(ctx->typeid);
}


extern inline bool ag_object_lt(const ag_object *, const ag_object *);
extern inline bool ag_object_eq(const ag_object *, const ag_object *);
extern inline bool ag_object_gt(const ag_object *, const ag_object *);
extern inline bool ag_object_empty(const ag_object *);


extern ag_object *
ag_object_new(ag_typeid typeid, ag_memblock *payload)
{
        AG_ASSERT_PTR (payload);

        ag_object *ctx = ag_memblock_new(sizeof *ctx);
        
        ctx->uuid    = ag_uuid_new();
        ctx->typeid  = typeid;
        ctx->payload = payload;

        return ctx;
}


extern ag_object *
ag_object_copy(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_memblock_copy(ctx);
}


extern ag_object *
ag_object_clone(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_object_new(ctx->typeid, vtable_get(ctx)->clone(ctx->payload));
}


extern void
ag_object_release(ag_object **ctx)
{
        ag_object    *o;
        ag_memblock *m;

        if (AG_LIKELY (ctx && (o = *ctx))) {
                if (ag_memblock_refc(o) == 1) {
                        ag_uuid_release(&o->uuid);
                        vtable_get(o)->release(o->payload);

                        m = o->payload;
                        ag_memblock_release(&m);
                }

                m = o;
                ag_memblock_release(&m);
                *ctx = m;
        }
}


extern enum ag_cmp
ag_object_cmp(const ag_object *ctx, const ag_object *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        
        return vtable_get(ctx)->cmp(ctx, cmp);
}


extern ag_typeid
ag_object_typeid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ctx->typeid;
}


extern ag_uuid *
ag_object_uuid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_uuid_copy(ctx->uuid);
}


extern bool
ag_object_valid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return vtable_get(ctx)->valid(ctx);
}


extern size_t
ag_object_sz(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return vtable_get(ctx)->sz(ctx);
}


extern size_t
ag_object_refc(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_memblock_refc(ctx);
}


extern size_t
ag_object_len(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        
        return vtable_get(ctx)->len(ctx);
}


extern ag_hash
ag_object_hash(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        
        return vtable_get(ctx)->hash(ctx);
}


extern ag_string *
ag_object_str(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return vtable_get(ctx)->str(ctx);
}


extern ag_string *
ag_object_json(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return vtable_get(ctx)->json(ctx);
}


extern const ag_memblock *
ag_object_payload(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ctx->payload;
}


extern ag_memblock *
ag_object_payload_mutable(ag_object **ctx)
{
        AG_ASSERT_PTR (ctx && *ctx);

        ag_object *o = *ctx;
        ag_memblock *m = o;

        if (ag_memblock_refc(m) > 1) {
                ag_memblock_release(&m);
                *ctx = ag_object_clone(o);
        }

        return (*ctx)->payload;
}

static void *
sym_load(void *dso, const char *type, const char *meth)
{
        AG_ASSERT_STR (type);
        AG_ASSERT_STR (meth);

        AG_AUTO(ag_string) *sym = ag_string_new_fmt("__%s_%s__", type, meth);
        return dlsym(dso, sym);
}


extern void
__ag_object_register__(const char *type, ag_typeid tid)
{
        AG_ASSERT_STR (type);

        void *dso = dlopen(NULL, RTLD_NOW);
        if (AG_UNLIKELY (!dso)) {
                fputs(dlerror(), stderr);
                fputs("\n", stderr);
                exit(1);
        }

        struct ag_object_vtable vt;
        vt.clone = sym_load(dso, type, "clone");
        vt.release = sym_load(dso, type, "release");
        vt.cmp = sym_load(dso, type, "cmp");
        vt.valid = sym_load(dso, type, "valid");
        vt.sz = sym_load(dso, type, "sz");
        vt.len = sym_load(dso, type, "len");
        vt.hash = sym_load(dso, type, "hash");
        vt.str = sym_load(dso, type, "str");
        vt.json = sym_load(dso, type, "json");

        ag_object_registry_push(tid, &vt);
        dlclose(dso);
}

