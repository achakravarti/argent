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


/*
 * Define the node of a list. Our lists are singly-linked, so we only need to
 * maintain a pointer to the next node.
 */


struct node {
        ag_value        *val; /* node value */
        struct node     *nxt; /* next node  */
};


/*
 * Define the object payload of a list. We choose to keep a pointer to the last
 * node in order to speed up push operations. Again, in order to avoid having to
 * iterate through the entire list, we maintain the length, cumulative size, and
 * cumulative hash of the list. 
 */


struct payload {
        struct node     *head; /* start of list             */
        struct node     *tail; /* end of list               */
        struct node     *itr;  /* current iterator position */
        size_t           len;  /* number of items           */
        size_t           sz;   /* cumulative size           */
        ag_hash          hash; /* cumulative hash           */
};


/*
 * Declare the prototypes for the node helper functions. node_new() helps create
 * a new node, and node_release() helps destroy an existing one.
 */


static inline struct node       *node_new(const ag_value *);
static inline struct node       *node_release(struct node *);
                                


/*
 * Declare the prototypes for the payload helper functions. payload_new() helps
 * create a new payload instance, either empty or filled with the values cloned
 * from another payload instance. payload_push() helps push a new node to the
 * end of the list.
 */


static struct payload   *payload_new(const struct node *);
static void              payload_push(struct payload *, const ag_value *);


/*
 * Declare the proptotypes of the dynamic dispatch callbacks for ag_list. We are
 * providing callbacks for all polymorphic object functions.
 */


static ag_memblock *virt_clone(const ag_memblock *);
static void         virt_release(ag_memblock *);
static enum ag_cmp  virt_cmp(const ag_object *, const ag_object *);
static bool         virt_valid(const ag_object *);
static size_t       virt_sz(const ag_object *);
static size_t       virt_len(const ag_object *);
static ag_hash      virt_hash(const ag_object *);
static ag_string   *virt_str(const ag_object *);


/*
 * Declare the prototypes for the inline functions of the ag_list interface.
 * These inline functions are the aliases of their object counterparts.
 */


extern inline ag_list           *ag_list_copy(const ag_list *);
extern inline ag_list           *ag_list_clone(const ag_list *);
extern inline void               ag_list_release(ag_list **);
extern inline enum ag_cmp        ag_list_cmp(const ag_list *, const ag_list *);
extern inline bool               ag_list_lt(const ag_list *, const ag_list *);
extern inline bool               ag_list_eq(const ag_list *, const ag_list *);
extern inline bool               ag_list_gt(const ag_list *, const ag_list *);
extern inline bool               ag_list_empty(const ag_list *);
extern inline ag_typeid          ag_list_typeid(const ag_list *);
extern inline ag_uuid           *ag_list_uuid(const ag_list *);
extern inline bool               ag_list_valid(const ag_list *);
extern inline size_t             ag_list_sz(const ag_list *);
extern inline size_t             ag_list_refc(const ag_list *);
extern inline size_t             ag_list_len(const ag_list *);
extern inline ag_hash            ag_list_hash(const ag_list *);
extern inline ag_string         *ag_list_str(const ag_list *);


/*
 * Define the ag_list_new() interface function.
 */


extern ag_list *
ag_list_new(void)
{
}


/*
 * Define the ag_list_get() interface function.
 */


extern ag_value *
ag_list_get(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
}


/*
 * Define the ag_list_get_at() interface function.
 */


extern ag_value *
ag_list_get_at(const ag_list *ctx, size_t idx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (idx >= 1 && idx <= ag_list_len(ctx));
}


/*
 * Define the ag_list_map() interface function.
 */


extern void
ag_list_map(const ag_list *ctx, ag_list_iterator *itr, void *opt)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (itr);
}


/*
 * Define the ag_list_set() interface function.
 */


extern void
ag_list_set(ag_list **ctx, const ag_value *val)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT_PTR (val);
}


/*
 * Define the ag_list_set_at() interface function.
 */


extern void
ag_list_set_at(ag_list **ctx, size_t idx, const ag_value *val)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT_PTR (val);
        AG_ASSERT (idx >= 1 && idx <= ag_list_len(*ctx));
}


/*
 * Define the ag_list_map_mutable_list() interface function.
 */


extern void
ag_list_map_mutable(ag_list **ctx, ag_list_iterator_mutable *itr, void *opt)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT_PTR (itr);
}


/*
 * Define the ag_list_start() interface function.
 */


extern void
ag_list_start(ag_list **ctx)
{
        AG_ASSERT_PTR (ctx && *ctx);
}


/*
 * Define the ag_list_next() interface function.
 */


extern bool
ag_list_next(ag_list **ctx)
{
        AG_ASSERT_PTR (ctx && *ctx);
}


/*
 * Define the ag_list_push() interface function.
 */


extern void
ag_list_push(ag_list **ctx, const ag_value *val)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT_PTR (val);
}


/*
 * Define the node_new() helper function. The node_new() function is responsible
 * for creating a new node on the heap. By default, the pointer to the next node
 * will always be NULL when a new node is created. Since we're performing a
 * shallow copy using ag_value_copy(), creating a new node is relatively
 * inexpensive.
 */


static inline struct node*
node_new(const ag_value *val)
{
        AG_ASSERT_PTR (val);

        struct node *n = ag_memblock_new(sizeof *n);
        n->val = ag_value_copy(val);
        n->nxt = NULL;

        return n;
}


/*
 * Define the node_new() helper function. We release the heap memory allocated
 * to a node and return a pointer to the next node (which may be NULL). By
 * returning a pointer to the next node, we make it easier to iterate through
 * the list with this function. Note that we're taking care to avoid casting to
 * (void **) in the call to ag_memblock_release() in order to avoid potential
 * undefined behaviour.
 */


static inline struct node*
node_release(struct node *ctx)
{
        AG_ASSERT_PTR (ctx);

        struct node *nxt = ctx->nxt;
        void *ptr = ctx;
        ag_memblock_release(&ptr);

        return nxt;
}


/*
 * Define the payload_new() helper function. This function is responsible for
 * creating a new payload instance, either empty or populated with the same
 * values as in another payload. In either case, the internal iterator is reset,
 * and the counters are updated accordingly.
 */


static struct payload*
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
                payload_push(p, n->val);
                n = n->nxt;
        }

        return p;
}


/*
 * Define the payload_push() helper function. This function is responsible for
 * pushing a new value encapsulated as a node into the list. There are two
 * possible scenarios where this function is called: When the list is empty, and
 * when the list is not empty. Of these two possibilities, the latter is much
 * more likely.
 *
 * When the list is empty, we set the pushed value to the start of the list.
 * Otherwise, in the case where the list is not empty, we push the value to the
 * end of the list. In either case, we update the tail pointer and the counters.
 */


static void
payload_push(struct payload *ctx, const ag_value *val)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (val);

        struct node *n = node_new(val);

        if (AG_LIKELY (ctx->tail))
                ctx->tail->nxt = n;
        else
                ctx->head = n;

        ctx->tail = n;
        ctx->len++;
        ctx->sz += ag_value_sz(val);
        ctx->hash += ag_value_hash(val);
}


/*
 * Define the virt_clone() dynamic dispatch callback function. This function is
 * called by ab_object_clone() when ag_list_clone() is invoked.
 */


static ag_memblock *
virt_clone(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);
}


/*
 * Define the virt_release() dynamic dispatch callback function. This function
 * is called by ag_object_release() when ag_list_release() is invoked.
 */


static void
virt_release(ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);
}


/*
 * Define the virt_cmp() dynamic dispatch callback function. This function is
 * called by ag_object_cmp() when ag_list_cmp() is invoked.
 */


static enum ag_cmp
virt_cmp(const ag_object *ctx, const ag_object *cmp)
{
        AG_ASSERT_PTR (ctx);
}


/*
 * Define the virt_valid() dynamic dispatch callback function. This function is
 * called by ag_object_valid() when ag_list_valid() is invoked.
 */


static bool
virt_valid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
}


/*
 * Define the virt_sz() dynamic dispatch callback function. This function is
 * called by ag_object_sz() when ag_list_sz() is invoked.
 */


static size_t
virt_sz(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
}


/*
 * Define the virt_len() dynamic dispatch callback function. This function is
 * called by ag_object_len() when ag_list_len() is invoked.
 */


static size_t
virt_len(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
}


/*
 * Define the virt_hash() dynamic dispatch callback function. This function is
 * called by ag_object_hash() when ag_list_hash() is invoked.
 */


static ag_hash
virt_hash(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
}


/*
 * Define the virt_str() dynamic dispatch callback function. This function is
 * called by ag_object_str() when ag_list_str() is invoked.
 */


static ag_string
*virt_str(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
}

