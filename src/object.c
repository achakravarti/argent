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


/*
 *      Expansion of the ag_object abstract data type [DM:??].
 */
struct ag_object {
    unsigned refc;
    unsigned type;
    unsigned id;
    ag_memblock_t *payload;
};




/*******************************************************************************
 *                            HELPER IMPLEMENTATION
 */


                                             /* default copy method [AgDM:??] */
static inline void *copy_default(const ag_memblock_t *payload)
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
    return 1;
}


                                             /* default hash method [AgDM:??] */
static inline size_t hash_default(const ag_object *obj)
{
    return ((size_t) obj->id * (size_t) 2654435761) % (size_t) pow(2, 32);
}


                                       /* default comparison method [AgDM:??] */
static inline enum ag_object_cmp cmp_default(const ag_object *ctx, 
        const ag_object *cmp)
{
    size_t llen = ag_object_len(ctx);
    size_t rlen = ag_object_len(cmp);

    if (llen == rlen)
        return AG_OBJECT_CMP_EQ;

    return llen < rlen ? AG_OBJECT_CMP_LT : AG_OBJECT_CMP_GT;
}


                                           /* default string method [AgDM:??] */
static inline const char *str_default(const ag_object *ctx)
{
    const char *FMT = "object: (id = %u), (len = %lu), (refc = %lu)";
#   define LEN 64

    static ag_threadlocal char bfr[LEN];
    snprintf(bfr, LEN, FMT, ctx->id, ag_object_len(ctx), ctx->refc);

    return bfr;
#   undef LEN
}


                                              /* creates new object [AgDM:??] */
static ag_object *object_new(unsigned type, unsigned id, ag_memblock_t *payload)
{
    ag_object *ctx = ag_memblock_new(sizeof *ctx);
    ctx->refc = 1;
    ctx->type = type;
    ctx->id = id;
    ctx->payload = payload;
    
    return ctx;
}


                                       /* performs deep copy object [AgDM:??] */
static inline void object_copy(ag_object **obj)
{
    ag_object *hnd = *obj;

    if (hnd->refc > 1) {
        ag_object *cp = ag_object_new(hnd->type, hnd->id, 
                ag_object_vtable_get(hnd->type)->copy(hnd->payload));

        ag_object_dispose(obj);
        *obj = cp;
    }
}




/*******************************************************************************
 *                        INLINE INTERFACE DECLARATIONS
 */


                                /* declaration of ag_object_empty() [AgDM:??] */
extern inline bool ag_object_empty(const ag_object *ctx);


                                   /* declaration of ag_object_lt() [AgDM:??] */
extern inline bool ag_object_lt(const ag_object *ctx, const ag_object *cmp);


                                   /* declaration of ag_object_eq() [AgDM:??] */
extern inline bool ag_object_eq(const ag_object *ctx, const ag_object *cmp);


                                   /* declaration of ag_object_gt() [AgDM:??] */
extern inline bool ag_object_gt(const ag_object *ctx, const ag_object *cmp);




/*******************************************************************************
 *                           INTERFACE IMPLEMENTATION
 */


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

    ag_object_vtable_set(type, &m);
}


                               /* implementation of ag_object_new() [AgDM:??] */
extern ag_object *ag_object_new(unsigned type, unsigned id, 
        ag_memblock_t *payload)
{
    ag_assert (type && id && payload);
    return object_new(type, id, payload);
}


                          /* implementation of ag_object_new_noid() [AgDM:??] */
extern ag_object *ag_object_new_noid(unsigned type, ag_memblock_t *payload)
{
    const unsigned NOID = 0;

    ag_assert (type && payload);
    return object_new(type, NOID, payload);
}


                              /* implementation of ag_object_copy() [AgDM:??] */
extern ag_object *ag_object_copy(const ag_object *ctx)
{
    ag_assert (ctx);
    ag_object *cp = (ag_object *) ctx;
    
    cp->refc++;
    return cp;
}


                           /* implementation of ag_object_dispose() [AgDM:??] */
extern void ag_object_dispose(ag_object **ctx)
{
    ag_object *hnd;

    if (ag_likely (ctx && (hnd = *ctx))) {
        if (!--hnd->refc) {
            ag_object_vtable_get(hnd->type)->dispose(hnd->payload);
            ag_memblock_free(&hnd->payload);
            ag_memblock_free((ag_memblock_t **) ctx);
        }
    }
}


                              /* implementation of ag_object_type() [AgDM:??] */
extern unsigned ag_object_type(const ag_object *ctx)
{
    ag_assert (ctx);
    return ctx->type;
}


                                /* implementation of ag_object_id() [AgDM:??] */
extern unsigned ag_object_id(const ag_object *ctx)
{
    ag_assert (ctx);
    return ctx->id;
}


                            /* implementation of ag_object_id_set() [AgDM:??] */
extern void ag_object_id_set(ag_object **ctx, unsigned id)
{
    ag_assert (ctx && *ctx);
    object_copy(ctx);

    ag_assert (id);
    (*ctx)->id = id;
}


                              /* implementation of ag_object_hash() [AgDM:??] */
extern unsigned ag_object_hash(const ag_object *ctx)
{
    ag_assert (ctx);
    return ag_object_vtable_get(ctx->id)->hash(ctx);
}


                                /* implementation of ag_object_sz() [AgDM:??] */
extern size_t ag_object_sz(const ag_object *ctx)
{
    ag_assert (ctx);
    return ag_object_vtable_get(ctx->id)->sz(ctx);
}


                               /* implementation of ag_object_len() [AgDM:??] */
extern size_t ag_object_len(const ag_object *ctx)
{
    ag_assert (ctx);
    return ag_object_vtable_get(ctx->id)->len(ctx);
}


                               /* implementation of ag_object_cmp() [AgDM:??] */
extern enum ag_object_cmp ag_object_cmp(const ag_object *ctx, 
        const ag_object *cmp)
{
    ag_assert (ctx && ag_object_type(ctx) == ag_object_type(cmp));
    return ag_object_vtable_get(ctx->id)->cmp(ctx, cmp);
}


                           /* implementation of ag_object_payload() [AgDM:??] */
extern const ag_memblock_t *ag_object_payload(const ag_object *ctx)
{
    ag_assert (ctx);
    return ctx->payload;
}


                   /* implementation of ag_object_payload_mutable() [AgDM:??] */
extern ag_memblock_t *ag_object_payload_mutable(ag_object **ctx)
{
    ag_assert (ctx && *ctx);
    object_copy(ctx);
    return (*ctx)->payload;
}


                               /* implementation of ag_object_str() [AgDM:??] */
extern const char *ag_object_str(const ag_object *ctx)
{
    ag_assert (ctx);
    return ag_object_vtable_get(ctx->id)->str(ctx);
}

