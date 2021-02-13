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


static inline void      *mem_new(size_t);
static inline void       mem_release(void **);


struct node {
        void            *data;
        struct node     *next;
};

static inline struct node       *node_new(void *);
static inline void               node_release(struct node **, 
                                    ag_registry_release_cbk *);


struct ag_registry {
        size_t                    len;  /* number of buckets     */
        struct node             **buck; /* buckets of node lists */
        ag_registry_release_cbk  *disp; /* cleanup callback      */
};


static inline void *
mem_new(size_t sz)
{
        AG_ASSERT (sz);

        void *ctx = malloc(sz);

        if (AG_UNLIKELY (!ctx)) {
                printf("[!] failed to allocate memory for registry\n");
                exit(EXIT_FAILURE);
        }

        memset(ctx, 0, sz);
        return ctx;
}

static inline void
mem_release(void **hnd)
{
        if (AG_LIKELY (hnd && *hnd)) {
                free(*hnd);
                *hnd = NULL;
        }
}


static inline struct node *
node_new(void *data)
{
        AG_ASSERT_PTR (data);

        struct node *n = mem_new(sizeof *n);
        n->data = data;
        n->next = NULL;

        return n;
}

static inline void
node_release(struct node **hnd, ag_registry_release_cbk *disp)
{
        struct node *n;

        if (AG_LIKELY (hnd && (n = *hnd))) {
                disp(n->data);
                void *m = &n;
                mem_release(m);
        }
}

