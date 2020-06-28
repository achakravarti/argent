/*******************************************************************************
 *                        __   ____   ___  ____  __ _  ____ 
 *                       / _\ (  _ \ / __)(  __)(  ( \(_  _)
 *                      /    \ )   /( (_ \ ) _) /    /  )(  
 *                      \_/\_/(__\_) \___/(____)\_)__) (__)                     
 *
 * Argent Library
 * Copyright (c) 2020 Abhishek Chakravarti <abhishek@taranjali.org>
 *
 * This file is part of the Argent Library. It implements the object model
 * interface of the Argent Library.
 *
 * The contents of this file are released under the GPLv3 License. See the
 * accompanying LICENSE file or the generated Developer Manual (section I:?) for 
 * complete licensing details.
 *
 * BY CONTINUING TO USE AND/OR DISTRIBUTE THIS FILE, YOU ACKNOWLEDGE THAT YOU
 * HAVE UNDERSTOOD THESE LICENSE TERMS AND ACCEPT TO BE LEGALLY BOUND BY THEM.
 ******************************************************************************/


#include <math.h>
#include "./api.h"


/*******************************************************************************
 *                            V-TABLE NODE INTERNALS
 */

                                             /* v-table bucket node [AgDM:??] */
struct vtable_node {
    size_t key;
    struct ag_object_vtable *val;
    struct vtable_node *nxt;
};

                                                /* creates new node [AgDM:??] */
static struct vtable_node *vtable_node_new(size_t key, 
        const struct ag_object_vtable *val, struct vtable_node *nxt)
{
    struct vtable_node *n = ag_memblock_new(sizeof *n);
    n->key = key;
    n->nxt = nxt;

    n->val = ag_memblock_new(sizeof *n->val);
    n->val->copy = val->copy;
    n->val->dispose = val->dispose;
    n->val->id = val->id;
    n->val->sz = val->sz;
    n->val->len = val->len;
    n->val->hash = val->hash;
    n->val->cmp = val->cmp;
    n->val->str = val->str;

    return n;
}

                                                 /* disposes a node [AgDM:??] */
static inline void vtable_node_dispose(struct vtable_node *n)
{
    ag_memblock_free((ag_memblock_t **) &n->val);
    ag_memblock_free((ag_memblock_t **) &n);
}


/*******************************************************************************
 *                              V-TABLE INTERNALS
 */

                                       /* v-table of object methods [AgDM:??] */
static ag_threadlocal struct {
    size_t len;
    struct vtable_node **bkt;
} *vtable = NULL;

                                        /* gets hash of object type [AgDM:??] */
static inline size_t vtable_hash(size_t type)
{
    return type % vtable->len;
}

                         /* checks if methods for object type exist [AgDM:??] */
static bool vtable_exists(size_t type)
{
    struct vtable_node *n = vtable->bkt[vtable_hash(type)];
    while (n) {
        if (n->key == type)
            return true;

        n = n->nxt;
    }

    return false;
}

                        /* gets methods of object type from v-table [AgDM:??] */
static const struct ag_object_vtable *vtable_get(size_t type)
{
    struct vtable_node *n = vtable->bkt[vtable_hash(type)];
    while (n) {
        if (n->key == type)
            return n->val;

        n = n->nxt;
    }

    return NULL;
}

                                     /* sets methods of object type [AgDM:??] */
static void vtable_set(size_t type, const struct ag_object_vtable *vt)
{
    size_t h = vtable_hash(type);
    struct vtable_node *n = vtable_node_new(type, vt, vtable->bkt[h]);
    vtable->bkt[h] = n;
}


/*******************************************************************************
 *                               OBJECT INTERNALS
 */

                                          /* expansion of ag_object [AgDM:??] */
struct ag_object_t {
    size_t refc;
    size_t type;
    ag_memblock_t *payload;
};

                                             /* default copy method [AgDM:??] */
static inline ag_memblock_t *object_method_copy(const ag_memblock_t *payload)
{
    return ag_memblock_copy(payload);
}

                                          /* default dispose method [AgDM:??] */
static inline void object_method_dispose(ag_memblock_t *payload)
{
    (void) payload;
}

                                               /* default ID method [AgDM:??] */
static inline size_t object_method_id(const ag_object_t *obj)
{
    (void) obj;
    return 0;
}

                                             /* default size method [AgDM:??] */
static inline size_t object_method_sz(const ag_object_t *obj)
{
    return ag_memblock_sz(obj->payload);
}

                                           /* default length method [AgDM:??] */
static inline size_t object_method_len(const ag_object_t *obj)
{
    (void) obj;
    return 1;
}

                                             /* default hash method [AgDM:??] */
static inline size_t object_method_hash(const ag_object_t *obj)
{
    return (ag_object_id(obj) * (size_t) 2654435761) % (size_t) pow(2, 32);
}

                                       /* default comparison method [AgDM:??] */
static inline enum ag_tristate object_method_cmp(const ag_object_t *ctx, 
        const ag_object_t *cmp)
{
    size_t llen = ag_object_len(ctx);
    size_t rlen = ag_object_len(cmp);

    if (llen == rlen)
        return AG_TRISTATE_GND;

    return llen < rlen ? AG_TRISTATE_LO : AG_TRISTATE_HI;
}

                                           /* default string method [AgDM:??] */
static inline ag_string_t *object_method_str(const ag_object_t *ctx)
{
    (void) ctx;
    return ag_string_new("object");
}

                                              /* creates new object [AgDM:??] */
static ag_object_t *object_new(size_t type, ag_memblock_t *payload)
{
    ag_object_t *ctx = ag_memblock_new(sizeof *ctx);
    ctx->refc = 1;
    ctx->type = type;
    ctx->payload = payload;
    
    return ctx;
}


/*******************************************************************************
 *                               OBJECT EXTERNALS
 */

                                /* declaration of ag_object_empty() [AgDM:??] */
extern inline bool ag_object_empty(const ag_object_t *ctx);

                                   /* declaration of ag_object_lt() [AgDM:??] */
extern inline bool ag_object_lt(const ag_object_t *ctx, const ag_object_t *cmp);

                                   /* declaration of ag_object_eq() [AgDM:??] */
extern inline bool ag_object_eq(const ag_object_t *ctx, const ag_object_t *cmp);

                                   /* declaration of ag_object_gt() [AgDM:??] */
extern inline bool ag_object_gt(const ag_object_t *ctx, const ag_object_t *cmp);

                              /* implementation of ag_object_init() [AgDM:??] */
extern void ag_object_init(size_t len)
{
    if (ag_likely (!vtable)) {
        ag_assert (len);
        vtable = ag_memblock_new(sizeof *vtable);

        vtable->bkt = ag_memblock_new(sizeof *vtable->bkt * len);
        vtable->len = len;
    }
}

                              /* implementation of ag_object_exit() [AgDM:??] */
extern void ag_object_exit(void)
{
    if (ag_unlikely (!vtable))
        return;

    struct vtable_node *n, *nxt;
    for (register size_t i = 0; i < vtable->len; i++) {
        if ((n = vtable->bkt[i])) {
            do {
                nxt = n->nxt;
                vtable_node_dispose(n);
                n = nxt;
            } while (n);
        }
    }
}

                          /* implementation of ag_object_register() [AgDM:??] */
extern void ag_object_register(size_t type, const struct ag_object_vtable *vt)
{
    struct ag_object_vtable vtbl = {
        .copy = vt->copy ? vt->copy : object_method_copy,
        .dispose = vt->dispose? vt->dispose: object_method_dispose,
        .id = vt->id ? vt->id : object_method_id,
        .sz = vt->sz ? vt->sz : object_method_sz,
        .len = vt->len ? vt->len : object_method_len,
        .hash = vt->hash ? vt->hash : object_method_hash,
        .cmp = vt->cmp ? vt->cmp : object_method_cmp,
        .str = vt->str ? vt->str : object_method_str
    };

    ag_assert (vtable && type && !vtable_exists(type));
    vtable_set(type, &vtbl);
}

                               /* implementation of ag_object_new() [AgDM:??] */
extern ag_object_t *ag_object_new(size_t type, ag_memblock_t *payload)
{
    ag_assert (type && payload);
    return object_new(type, payload);
}

                              /* implementation of ag_object_copy() [AgDM:??] */
extern ag_object_t *ag_object_copy(const ag_object_t *ctx)
{
    ag_assert (ctx);
    ag_object_t *cp = (ag_object_t *) ctx;
    
    cp->refc++;
    return cp;
}

                           /* implementation of ag_object_dispose() [AgDM:??] */
extern void ag_object_dispose(ag_object_t **ctx)
{
    ag_object_t *hnd;

    if (ag_likely (ctx && (hnd = *ctx))) {
        if (!--hnd->refc) {
            ag_assert (vtable);
            vtable_get(hnd->type)->dispose(hnd->payload);

            ag_memblock_free(&hnd->payload);
            ag_memblock_free((ag_memblock_t **) ctx);
        }
    }
}

                              /* implementation of ag_object_type() [AgDM:??] */
extern size_t ag_object_type(const ag_object_t *ctx)
{
    ag_assert (ctx);
    return ctx->type;
}

                              /* implementation of ag_object_refc() [AgDM:??] */
extern size_t ag_object_refc(const ag_object_t *ctx)
{
    ag_assert (ctx);
    return ctx->refc;
}

                                /* implementation of ag_object_id() [AgDM:??] */
extern size_t ag_object_id(const ag_object_t *ctx)
{
    ag_assert (ctx && vtable);
    return vtable_get(ctx->type)->id(ctx);
}

                              /* implementation of ag_object_hash() [AgDM:??] */
extern size_t ag_object_hash(const ag_object_t *ctx)
{
    ag_assert (ctx && vtable);
    return vtable_get(ctx->type)->hash(ctx);
}

                                /* implementation of ag_object_sz() [AgDM:??] */
extern size_t ag_object_sz(const ag_object_t *ctx)
{
    ag_assert (ctx && vtable);
    return vtable_get(ctx->type)->sz(ctx);
}

                               /* implementation of ag_object_len() [AgDM:??] */
extern size_t ag_object_len(const ag_object_t *ctx)
{
    ag_assert (ctx && vtable);
    return vtable_get(ctx->type)->len(ctx);
}

                               /* implementation of ag_object_cmp() [AgDM:??] */
extern enum ag_tristate ag_object_cmp(const ag_object_t *ctx, 
        const ag_object_t *cmp)
{
    ag_assert (ctx && vtable && cmp 
            && ag_object_type(ctx) == ag_object_type(cmp));
    return vtable_get(ctx->type)->cmp(ctx, cmp);
}

                           /* implementation of ag_object_payload() [AgDM:??] */
extern const ag_memblock_t *ag_object_payload(const ag_object_t *ctx)
{
    ag_assert (ctx);
    return ctx->payload;
}

                   /* implementation of ag_object_payload_mutable() [AgDM:??] */
extern ag_memblock_t *ag_object_payload_mutable(ag_object_t **ctx)
{
    ag_assert (ctx && *ctx);
    ag_object_t *hnd = *ctx;

    if (hnd->refc > 1) {
        hnd->refc--;
        *ctx = object_new(hnd->type, vtable_get(hnd->type)->copy(hnd->payload));
    }

    return (*ctx)->payload;
}

                               /* implementation of ag_object_str() [AgDM:??] */
extern ag_string_t *ag_object_str(const ag_object_t *ctx)
{
    ag_assert (ctx && vtable);
    return vtable_get(ctx->type)->str(ctx);
}

