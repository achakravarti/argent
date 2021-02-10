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
};


static struct payload   *payload_new(const char *, const char *);


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

        void *dso = dlopen(p->dso, RTLD_NOW);
        if (!dso) {
                fputs(dlerror(), stderr);
                exit(1);
        }

        void *sym = dlsym(dso, p->sym);
        char *err = dlerror();
        if (err) {
                fputs(err, stderr);
                exit(1);
        }

        return sym;
}

        
static struct payload *
payload_new(const char *dso, const char *sym)
{
        AG_ASSERT_PTR (dso);
        AG_ASSERT_STR (sym);

        struct payload *p = ag_memblock_new(sizeof *p);
        p->dso = ag_string_new(dso);
        p->sym = ag_string_new(sym);

        return p;
}

