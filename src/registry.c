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

#include <stdlib.h>
#include <string.h>


struct node {
        ag_hash          key;   /* node key  */
        void            *data;  /* node data */
        struct node     *next;  /* next node */
};

static inline struct node       *node_new(ag_hash, void *);
static inline void               node_release(struct node *, 
                                    ag_registry_release_cbk *);

/* Definition of the ag_registry ADT. */

struct ag_registry {
        size_t                    len;  /* number of buckets     */
        struct node             **buck; /* buckets of node lists */
        ag_registry_release_cbk  *disp; /* cleanup callback      */
};


#define MEM_CHECK(p) do {                                               \
        if (AG_UNLIKELY (!(p))) {                                       \
                printf("[!] failed to allocate memory for registry");   \
                ag_log_err("failed to allocate memory for registry");   \
        }                                                               \
} while (0)

/* 
 * ag_registry_new() creates a new registry instance. The registry is
 * implemented as a hash map of generic data. The number of buckets is set to be
 * the native word size in bits. The callback to dispose the contained data is 
 * passed through the first parameter.
 */

extern ag_registry *
ag_registry_new(ag_registry_release_cbk *disp)
{
        AG_ASSERT_PTR (disp);

        ag_registry *r = malloc(sizeof *r);
        MEM_CHECK (r);

        r->len = sizeof(size_t) * 8;
        r->disp = disp;

        size_t sz = sizeof *r->buck * r->len;
        r->buck = malloc(sz);
        MEM_CHECK (r->buck);
        memset(r->buck, 0, sz);

        return r;
}


extern void
ag_registry_release(ag_registry **hnd)
{
        ag_registry *r;

        if (AG_LIKELY (hnd && (r = *hnd))) {
                for (register size_t i = 0; i < r->len; i++) {
                        register struct node *n = r->buck[i];

                        while (n) {
                                register struct node *n2 = n->next;
                                node_release(n, r->disp);
                                n = n2;
                        }
                }

                free(r->buck);
                free(r);
        }
}


extern void *
ag_registry_get(const ag_registry *hnd, ag_hash key)
{
        AG_ASSERT_PTR (hnd);

        register struct node *n = hnd->buck[key % hnd->len];

        while (n) {
                if (n->key == key)
                        return n->data;

                n = n->next;
        }

        return NULL;
}


extern void
ag_registry_push(ag_registry *hnd, ag_hash key, void *data)
{
        AG_ASSERT_PTR (hnd);
        AG_ASSERT_PTR (data);

        register struct node *n = hnd->buck[key % hnd->len];

        if (n) {
                while (n && n->next)
                        n = n->next;

                n->next = node_new(key, data);
        } else
                hnd->buck[key % hnd->len] = node_new(key, data);
}


static inline struct node *
node_new(ag_hash key, void *data)
{
        AG_ASSERT_PTR (data);

        struct node *n = malloc(sizeof *n);
        MEM_CHECK(n);

        n->key = key;
        n->data = data;
        n->next = NULL;

        return n;
}

static inline void
node_release(struct node *hnd, ag_registry_release_cbk *disp)
{
        AG_ASSERT_PTR (hnd);

        disp(hnd->data);
        free(hnd);
}

