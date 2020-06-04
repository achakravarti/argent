/*******************************************************************************
 *                        __   ____   ___  ____  __ _  ____ 
 *                       / _\ (  _ \ / __)(  __)(  ( \(_  _)
 *                      /    \ )   /( (_ \ ) _) /    /  )(  
 *                      \_/\_/(__\_) \___/(____)\_)__) (__)                     
 *
 * Argent Library
 * Copyright (c) 2020 Abhishek Chakravarti <abhishek@taranjali.org>
 *
 * This file is part of the Argent Library. It implements the object v-table
 * interface of the Argent Library.
 *
 * The contents of this file are released under the GPLv3 License. See the
 * accompanying LICENSE file or the generated Developer Manual (section I:?) for 
 * complete licensing details.
 *
 * BY CONTINUING TO USE AND/OR DISTRIBUTE THIS FILE, YOU ACKNOWLEDGE THAT YOU
 * HAVE UNDERSTOOD THESE LICENSE TERMS AND ACCEPT TO BE LEGALLY BOUND BY THEM.
 ******************************************************************************/


#include "./api.h"




/*******************************************************************************
 *                               TYPE DEFINITIONS
 */


                                             /* v-table bucket node [AgDM:??] */
struct node {
    unsigned key;
    struct ag_object_method *val;
    struct node *nxt;
};




/*******************************************************************************
 *                                   GLOBALS
 */


                                    /* number of buckets in v-table [AgDM:??] */
#define VTABLE_BUCKETS 64


                                       /* v-table of object methods [AgDM:??] */
ag_threadlocal struct node **vtable = NULL;




/*******************************************************************************
 *                            HELPER IMPLEMENTATION
 */


                                                /* creates new node [AgDM:??] */
static inline struct node *node_new(unsigned key, 
        const struct ag_object_method *val, struct node *nxt)
{
    struct node *n = ag_mempool_new(sizeof *n);
    n->key = key;
    n->nxt = nxt;

    n->val = ag_mempool_new(sizeof *n->val);
    n->val->copy = val->copy;
    n->val->dispose = val->dispose;
    n->val->sz = val->sz;
    n->val->len = val->len;
    n->val->hash = val->hash;
    n->val->cmp = val->cmp;
    n->val->str = val->str;

    return n;
}


                                                 /* disposes a node [AgDM:??] */
static inline void node_dispose(struct node *n)
{
    ag_mempool_free((void **) &n->val);
    ag_mempool_free((void **) &n);
}




/*******************************************************************************
 *                           INTERFACE IMPLEMENTATION
 */


                       /* implementation of ag_object_vtable_init() [AgDM:??] */
extern void ag_object_vtable_init(void)
{
    if (ag_likely (!vtable))
        vtable = ag_mempool_new(sizeof *vtable * VTABLE_BUCKETS);
}


                       /* implementation of ag_object_vtable_exit() [AgDM:??] */
extern void ag_object_vtable_exit(void)
{
    struct node *n, *nxt;

    for (register size_t i = 0; i < VTABLE_BUCKETS; i++) {
        if ((n = vtable[i])) {
            do {
                nxt = n->nxt;
                node_dispose(n);
                n = nxt;
            } while (n);
        }
    }
}


                     /* implementation of ag_object_vtable_exists() [AgDM:??] */
extern bool ag_object_vtable_exists(unsigned type)
{
    ag_assert (type);
    unsigned hash = type % VTABLE_BUCKETS;

    ag_assert (vtable);
    struct node *n = vtable[hash];

    while (n) {
        if (n->key == type)
            return true;

        n = n->nxt;
    }

    return false;
}


                        /* implementation of ag_object_vtable_get() [AgDM:??] */
extern const struct ag_object_method *ag_object_vtable_get(unsigned type)
{
    ag_assert (type);
    unsigned hash = type % VTABLE_BUCKETS;

    ag_assert (vtable);
    struct node *n = vtable[hash];

    while (n) {
        if (n->key == type)
            return n->val;

        n = n->nxt;
    }

    ag_assert (n);
    return NULL;
}


                        /* implementation of ag_object_vtable_set() [AgDM:??] */
extern void ag_object_vtable_set(unsigned type, 
        const struct ag_object_method *meth)
{
    ag_assert (type);
    unsigned hash = type % VTABLE_BUCKETS;

    ag_assert (vtable && !ag_object_vtable_exists(type));
    struct node *n = node_new(type, meth, vtable[hash]);
    vtable[hash] = n;
}

