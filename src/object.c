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
 *                               TYPE DEFINITIONS
 */


                                          /* expansion of ag_object [AgDM:??] */
struct ag_object_t {
    unsigned refc;
    unsigned type;
    unsigned id;
    ag_memblock_t *payload;
};


                                             /* v-table bucket node [AgDM:??] */
struct node {
    unsigned key;
    struct ag_object_method *val;
    struct node *nxt;
};




/*******************************************************************************
 *                                   GLOBALS
 */


                                       /* v-table of object methods [AgDM:??] */
static ag_threadlocal struct {
    size_t len;
    struct node **bkt;
} *vtable = NULL;




/*******************************************************************************
 *                            HELPER IMPLEMENTATION
 */


                                                /* creates new node [AgDM:??] */
static struct node *node_new(unsigned key, const struct ag_object_method *val, 
        struct node *nxt)
{
    struct node *n = ag_memblock_new(sizeof *n);
    n->key = key;
    n->nxt = nxt;

    n->val = ag_memblock_new(sizeof *n->val);
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
    ag_memblock_free((ag_memblock_t **) &n->val);
    ag_memblock_free((ag_memblock_t **) &n);
}


                                        /* gets hash of object type [AgDM:??] */
static inline size_t type_hash(unsigned type)
{
    return type % vtable->len;
}


                         /* checks if methods for object type exist [AgDM:??] */
static bool vtable_exists(unsigned type)
{
    struct node *n = vtable->bkt[type_hash(type)];
    while (n) {
        if (n->key == type)
            return true;

        n = n->nxt;
    }

    return false;
}


                        /* gets methods of object type from v-table [AgDM:??] */
static const struct ag_object_method *vtable_get(unsigned type)
{
    struct node *n = vtable->bkt[type_hash(type)];

    while (n) {
        if (n->key == type)
            return n->val;

        n = n->nxt;
    }

    return NULL;
}


                                     /* sets methods of object type [AgDM:??] */
static void vtable_set(unsigned type, const struct ag_object_method *meth)
{
    unsigned h = type_hash(type);
    struct node *n = node_new(type, meth, vtable->bkt[h]);
    vtable->bkt[h] = n;
}


                                             /* default copy method [AgDM:??] */
static inline ag_memblock_t *copy_default(const ag_memblock_t *payload)
{
    return (ag_memblock_t *) payload;
}


                                          /* default dispose method [AgDM:??] */
static inline void dispose_default(ag_memblock_t *payload)
{
    (void) payload;
}


                                             /* default size method [AgDM:??] */
static inline size_t sz_default(const ag_memblock_t *payload)
{
    (void) payload;
    return 0;
}


                                           /* default length method [AgDM:??] */
static inline size_t len_default(const ag_memblock_t *payload)
{
    (void) payload;
    return 0;
}


                                             /* default hash method [AgDM:??] */
static inline size_t hash_default(const ag_object_t *obj)
{
    return ((size_t) obj->id * (size_t) 2654435761) % (size_t) pow(2, 32);
}


                                       /* default comparison method [AgDM:??] */
static inline enum ag_object_cmp cmp_default(const ag_object_t *ctx, 
        const ag_object_t *cmp)
{
    size_t llen = ag_object_len(ctx);
    size_t rlen = ag_object_len(cmp);

    if (llen == rlen)
        return AG_OBJECT_CMP_EQ;

    return llen < rlen ? AG_OBJECT_CMP_LT : AG_OBJECT_CMP_GT;
}


                                           /* default string method [AgDM:??] */
static inline const char *str_default(const ag_object_t *ctx)
{
    const char *FMT = "object: (id = %u), (len = %lu), (refc = %lu)";
#   define LEN 64

    static ag_threadlocal char bfr[LEN];
    snprintf(bfr, LEN, FMT, ctx->id, ag_object_len(ctx), ctx->refc);

    return bfr;
#   undef LEN
}


                                              /* creates new object [AgDM:??] */
static ag_object_t *object_new(unsigned type, unsigned id, 
        ag_memblock_t *payload)
{
    ag_object_t *ctx = ag_memblock_new(sizeof *ctx);
    ctx->refc = 1;
    ctx->type = type;
    ctx->id = id;
    ctx->payload = payload;
    
    return ctx;
}


                                       /* performs deep copy object [AgDM:??] */
static inline void object_copy(ag_object_t **obj)
{
    ag_object_t *hnd = *obj;

    if (hnd->refc > 1) {
        ag_object_t *cp = ag_object_new(hnd->type, hnd->id, 
                vtable_get(hnd->type)->copy(hnd->payload));

        ag_object_dispose(obj);
        *obj = cp;
    }
}




/*******************************************************************************
 *                        INLINE INTERFACE DECLARATIONS
 */


                                /* declaration of ag_object_empty() [AgDM:??] */
extern inline bool ag_object_empty(const ag_object_t *ctx);


                                   /* declaration of ag_object_lt() [AgDM:??] */
extern inline bool ag_object_lt(const ag_object_t *ctx, const ag_object_t *cmp);


                                   /* declaration of ag_object_eq() [AgDM:??] */
extern inline bool ag_object_eq(const ag_object_t *ctx, const ag_object_t *cmp);


                                   /* declaration of ag_object_gt() [AgDM:??] */
extern inline bool ag_object_gt(const ag_object_t *ctx, const ag_object_t *cmp);




/*******************************************************************************
 *                           INTERFACE IMPLEMENTATION
 */


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

    struct node *n, *nxt;
    for (register size_t i = 0; i < vtable->len; i++) {
        if ((n = vtable->bkt[i])) {
            do {
                nxt = n->nxt;
                node_dispose(n);
                n = nxt;
            } while (n);
        }
    }
}


                          /* implementation of ag_object_register() [AgDM:??] */
extern void ag_object_register(unsigned type, 
        const struct ag_object_method *meth)
{
    struct ag_object_method m = {
        .copy = meth->copy ? meth->copy : copy_default,
        .dispose = meth->dispose? meth->dispose: dispose_default,
        .sz = meth->sz ? meth->sz : sz_default,
        .len = meth->len ? meth->len : len_default,
        .hash = meth->hash ? meth->hash : hash_default,
        .cmp = meth->cmp ? meth->cmp : cmp_default,
        .str = meth->str ? meth->str : str_default
    };

    ag_assert (vtable && type && !vtable_exists(type));
    vtable_set(type, &m);
}


                               /* implementation of ag_object_new() [AgDM:??] */
extern ag_object_t *ag_object_new(unsigned type, unsigned id, 
        ag_memblock_t *payload)
{
    ag_assert (type && id && payload);
    return object_new(type, id, payload);
}


                          /* implementation of ag_object_new_noid() [AgDM:??] */
extern ag_object_t *ag_object_new_noid(unsigned type, ag_memblock_t *payload)
{
    const unsigned NOID = 0;

    ag_assert (type && payload);
    return object_new(type, NOID, payload);
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
extern unsigned ag_object_type(const ag_object_t *ctx)
{
    ag_assert (ctx);
    return ctx->type;
}


                                /* implementation of ag_object_id() [AgDM:??] */
extern unsigned ag_object_id(const ag_object_t *ctx)
{
    ag_assert (ctx);
    return ctx->id;
}


                            /* implementation of ag_object_id_set() [AgDM:??] */
extern void ag_object_id_set(ag_object_t **ctx, unsigned id)
{
    ag_assert (ctx && *ctx);
    object_copy(ctx);

    ag_assert (id);
    (*ctx)->id = id;
}


                              /* implementation of ag_object_hash() [AgDM:??] */
extern unsigned ag_object_hash(const ag_object_t *ctx)
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
extern enum ag_object_cmp ag_object_cmp(const ag_object_t *ctx, 
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
    object_copy(ctx);
    return (*ctx)->payload;
}


                               /* implementation of ag_object_str() [AgDM:??] */
extern const char *ag_object_str(const ag_object_t *ctx)
{
    ag_assert (ctx && vtable);
    return vtable_get(ctx->type)->str(ctx);
}

