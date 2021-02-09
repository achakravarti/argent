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
#include <dlfcn.h>


struct payload {
        ag_string       *dso;
        ag_string       *sym;
        void            *hnd;
};

static struct payload   *payload_new(const char *, const char *);


static ag_memblock      *virt_clone(const ag_memblock *);
static void              virt_release(ag_memblock *);
static enum ag_cmp       virt_cmp(const ag_object *, const ag_object *);
static bool              virt_valid(const ag_object *);
static size_t            virt_sz(const ag_object *);
static size_t            virt_len(const ag_object *);
static ag_hash           virt_hash(const ag_object *);
static ag_string        *virt_str(const ag_object *);


AG_OBJECT_DEFINE(ag_plugin)
{
        struct ag_object_vtable vt = {
                .clone = virt_clone, .release = virt_release, .cmp = virt_cmp,
                .valid = virt_valid, .sz = virt_sz,           .len = virt_len,
                .hash = virt_hash,   .str = virt_str,
        };

        ag_object_registry_push(AG_TYPEID_PLUGIN, &vt);
}



extern ag_plugin *
ag_plugin_new(const char *dso, const char *sym)
{
        AG_ASSERT_STR (dso);
        AG_ASSERT_STR (sym);

        return ag_object_new(AG_TYPEID_PLUGIN, payload_new(dso, sym));
}

extern ag_plugin *
ag_plugin_new_local(const char *sym)
{
        AG_ASSERT_STR (sym);
        
        return ag_object_new(AG_TYPEID_PLUGIN, payload_new("", sym));
}

extern ag_string *
ag_plugin_dso(const ag_plugin *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->dso);
}

extern ag_string *
ag_plugin_sym(const ag_plugin *ctx)
{
        AG_ASSERT_PTR (ctx);
        
        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->sym);
}

extern void *
ag_plugin_hnd(const ag_plugin *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        void *hnd = dlsym(p->hnd, p->sym);
        char *err = dlerror();

        if (err) {
                fputs(err, stderr);
                exit(1);
        }

        return hnd;
}

        
static struct payload *
payload_new(const char *dso, const char *sym)
{
        AG_ASSERT_PTR (dso);
        AG_ASSERT_STR (sym);

        struct payload *p = ag_memblock_new(sizeof *p);
        p->dso = ag_string_new(dso);
        p->sym = ag_string_new(sym);
        p->hnd = dlopen(p->dso, RTLD_NOW);

        if (!p->hnd) {
                fputs(dlerror(), stderr);
                exit(1);
        }

        return p;
}


static ag_memblock *
virt_clone(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ctx;
        return payload_new(p->dso, p->sym);
}


static void
virt_release(ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        struct payload *p = ctx;
        ag_string_release(&p->dso);
        ag_string_release(&p->sym);
        dlclose(p->hnd);
}


static enum ag_cmp       
virt_cmp(const ag_object *ctx, const ag_object *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_PLUGIN);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_PLUGIN);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        AG_AUTO(ag_string) *s2 = virt_str(cmp);

        return ag_string_cmp(s, s2);
}


static bool              
virt_valid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_PLUGIN);

        (void)ctx;
        return true;
}


static size_t            
virt_sz(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_PLUGIN);

        (void)ctx;
        return sizeof(struct payload);
}


static size_t            
virt_len(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_PLUGIN);

        (void)ctx;
        return 1;
}


static ag_hash           
virt_hash(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_PLUGIN);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        return ag_hash_new_str(s);
}


static ag_string *
virt_str(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_PLUGIN);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_new_fmt("%s::%s()", p->dso, p->sym);
}

