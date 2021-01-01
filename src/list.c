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
 * Define the __ag_list_register__() utility function. This is a special
 * function that is *not* part of the public list interface. It is invoked by
 * the Argent manager to register the dynamic dispatch callback functions for
 * the list type with the object registry.
 */


extern void
__ag_list_register__(void)
{
        struct ag_object_vtable vt = {
                .clone = virt_clone, .release = virt_release, .cmp = virt_cmp,
                .valid = virt_valid, .sz = virt_sz,           .len = virt_len,
                .hash = virt_hash,   .str = virt_str,
        };

        ag_object_registry_set(AG_TYPEID_LIST, &vt);
}


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
 * (ag_memblock **) in the call to ag_memblock_release() in order to avoid
 * potential undefined behaviour. 
 *
 * We know that there will always be only one given instance of a particular
 * node, i.e., there will be no shallow copies of the node. Hence, we can safely
 * release the value encapsulated by the node without checking the reference
 * count.
 */


static inline struct node*
node_release(struct node *ctx)
{
        AG_ASSERT_PTR (ctx);
        
        struct node *nxt = ctx->nxt;
        void *ptr = ctx;
        
        ag_value_release(&ctx->val);
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
 * called by ab_object_clone() when ag_list_clone() is invoked. We create a new
 * list using the contextual list as a reference.
 */


static ag_memblock *
virt_clone(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = (const struct payload *)ctx;
        return payload_new(p->head);
}


/*
 * Define the virt_release() dynamic dispatch callback function. This function
 * is called by ag_object_release() when ag_list_release() is invoked. We simply
 * iterate through each of the nodes in the list and release the node through
 * node_release(). node_release() takes care of releasing the encapsulated value
 * and returns NULL when it reaches the tail node.
 *
 * We don't need to check whether the reference count has fallen to 1 before
 * performing the cleanup operation because ag_object_release() takes care of
 * that before invoking this callback.
 */


static void
virt_release(ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        struct payload *p = (struct payload *)ctx;
        register struct node *n = p->head;

        while (n)
                n = node_release(n);
}


/*
 * Define the virt_cmp() dynamic dispatch callback function. This function is
 * called by ag_object_cmp() when ag_list_cmp() is invoked. We perform a
 * lexicographical comparison of the two lists. Although we're not considering
 * it now, it's important to keep in mind that sorting can affect the result of
 * this comparison. See also https://stackoverflow.com/questions/13052857/.
 */


static enum ag_cmp
virt_cmp(const ag_object *ctx, const ag_object *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_LIST);
        //AG_ASSERT (ag_list_type(ctx) == ag_list_type(cmp));

        const struct payload *p = ag_object_payload(ctx);
        const struct payload *p2 = ag_object_payload(cmp);

        size_t lim = p->len < p2->len ? p->len : p2->len;
        register const struct node *n = p->head;
        register const struct node *n2 = p2->head;
        register enum ag_cmp chk;

        AG_ASSERT (ag_value_type(n->val) == ag_value_type(n2->val));

        for (register size_t i = 0; i < lim; i++) {
                if ((chk = ag_value_cmp(n->val, n2->val)))
                        return chk;

                n = n->nxt;
                n2 = n2->nxt;
        }

        if (p->len == p2->len)
                return AG_CMP_EQ;
        else
                return p->len < p2->len ? AG_CMP_LT : AG_CMP_GT;
}


/*
 * Define the virt_valid() dynamic dispatch callback function. This function is
 * called by ag_object_valid() when ag_list_valid() is invoked. We consider a
 * list to be valid if (a) it's not empty, and (b) each of the values contained
 * within it is valid.
 *
 * In our implementation, we first check if the list is not empty, and then
 * iterate through each of its values, checking if each is valid. If we reach
 * the end of the list successfully in this manner, the iterator will be NULL
 * since it'll have crossed the tail.
 */


static bool
virt_valid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        const struct payload *p = ag_object_payload(ctx);
        register const struct node *n = p->head;

        if (AG_UNLIKELY (!n))
                return false;

        while (n && ag_value_valid(n->val))
                n = n->nxt;

        return !n;
}


/*
 * Define the virt_sz() dynamic dispatch callback function. This function is
 * called by ag_object_sz() when ag_list_sz() is invoked. The size of list is
 * the cumulative size of all the values contained within it.
 */


static size_t
virt_sz(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        const struct payload *p = ag_object_payload(ctx);
        return p->sz;
}


/*
 * Define the virt_len() dynamic dispatch callback function. This function is
 * called by ag_object_len() when ag_list_len() is invoked. The length of a list
 * is the number of values contained within it.
 */


static size_t
virt_len(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        const struct payload *p = ag_object_payload(ctx);
        return p->len;
}


/*
 * Define the virt_hash() dynamic dispatch callback function. This function is
 * called by ag_object_hash() when ag_list_hash() is invoked. The hash of a list
 * is the cumulative hash of all the values contained within it.
 */


static ag_hash
virt_hash(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);
        
        const struct payload *p = ag_object_payload(ctx);
        return p->hash;
}


/*
 * Define the virt_str() dynamic dispatch callback function. This function is
 * called by ag_object_str() when ag_list_str() is invoked.
 *
 * TODO: Improve definition
 */


static ag_string
*virt_str(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_string_new_fmt("list len = %lu", ag_object_len(ctx));
}

