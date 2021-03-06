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
#include <dlfcn.h>


struct payload {
        ag_string       *dso;
        ag_string       *sym;
        void            *hnd;
};

static struct payload   *payload_new(const char *, const char *);


AG_OBJECT_DEFINE(ag_plugin, AG_TYPEID_PLUGIN);

AG_OBJECT_DEFINE_CLONE(ag_plugin,
        const struct payload *p = _p_;
        return payload_new(p->dso, p->sym);
);

AG_OBJECT_DEFINE_RELEASE(ag_plugin,
        struct payload *p = _p_;
        ag_string_release(&p->dso);
        ag_string_release(&p->sym);
        dlclose(p->hnd);
);

AG_OBJECT_DEFINE_CMP(ag_plugin,
        AG_AUTO(ag_string) *s1 = ag_object_str(_o1_);
        AG_AUTO(ag_string) *s2 = ag_object_str(_o2_);

        return ag_string_cmp(s1, s2);
);

AG_OBJECT_DEFINE_SZ(ag_plugin,
        (void)_o_;
        return sizeof(struct payload);
);

AG_OBJECT_DEFINE_HASH(ag_plugin,
        AG_AUTO(ag_string) *s = ag_object_str(_o_);
        return ag_hash_new_str(s);
);

AG_OBJECT_DEFINE_STR(ag_plugin,
        const struct payload *p = ag_object_payload(_o_);
        return ag_string_new_fmt("%s::%s()", p->dso, p->sym);
);


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

        dlerror();

        const struct payload *p = ag_object_payload(ctx);
        void *hnd = dlsym(p->hnd, p->sym);
        char *err = dlerror();

        if (AG_UNLIKELY (err)) {
                fprintf(stderr, "failed to load plugin: %s\n", err);
                ag_log_err("failed to load plugin: %s", err);

                exit(EXIT_FAILURE);
        }

        return hnd;
}

        
static struct payload *
payload_new(const char *dso, const char *sym)
{
        AG_ASSERT_PTR (dso);
        AG_ASSERT_STR (sym);

        dlerror();

        struct payload *p = ag_memblock_new(sizeof *p);
        p->dso = ag_string_new(dso);
        p->sym = ag_string_new(sym);
        p->hnd = dlopen(*dso ? dso : NULL, RTLD_LAZY | RTLD_GLOBAL);

        if (AG_UNLIKELY (!p->hnd)) {
                char *err = dlerror();
                fprintf(stderr, "failed to open plugin DSO: %s\n", err);
                ag_log_err("failed to open plugin DSO: %s\n", err);

                exit(EXIT_FAILURE);
        }

        return p;
}

