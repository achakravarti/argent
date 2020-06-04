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


#include "./api.h"




/*******************************************************************************
 *                              TYPE DEFINITIONS
 */


/*
 *      Expansion of the ag_object abstract data type [DM:??].
 */
struct ag_object {
    struct ag_object_method vt;
    size_t refc;
    unsigned id;
    void *payload;
};




/*******************************************************************************
 *                           HELPER IMPLEMENTATION
 */


/*
 *      The copy_default() helper function is the default callback used in case
 *      the client code does not supply a callback to copy the payload of an
 *      object instance.
 */
static inline void *copy_default(const void *payload)
{
    return (void *) payload;
}


/*
 *      The free_default() helper function is the default callback used in case
 *      the client code does not supply a callback to free the payload of an
 *      object instance [DM:??].
 */
static inline void dispose_default(void *payload)
{
    (void) payload;
}


/*
 *      The len_default() helper function is the default callback used in case
 *      the client code does not supply a callback to determine the length of
 *      the payload of an object instance [DM:??].
 */
static inline size_t len_default(const void *payload)
{
    (void) payload;
    return 1;
}


/*
 *      The cmp_default() helper function is the default callback used in case
 *      the client code does not supply a callback to compare two object
 *      instances [DM:??].
 */
static inline enum ag_object_cmp cmp_default(const ag_object *ctx, 
        const ag_object *cmp)
{
    size_t llen = ag_object_len(ctx);
    size_t rlen = ag_object_len(cmp);

    if (llen == rlen)
        return AG_OBJECT_CMP_EQ;

    return llen < rlen ? AG_OBJECT_CMP_LT : AG_OBJECT_CMP_GT;
}


/*
 *      The str_default() helper function is the default callback used in case
 *      the client code does not supply a callback to generate the string
 *      representation of an object [DM:??].
 */
static inline const char *str_default(const ag_object *ctx)
{
    const char *FMT = "object: (id = %u), (len = %lu), (refc = %lu)";
#   define LEN 64

    static ag_threadlocal char bfr[LEN];
    snprintf(bfr, LEN, FMT, ctx->id, ag_object_len(ctx), ctx->refc);

    return bfr;
#   undef LEN
}


/*
 *      The object_new() helper function creates a new object instance [DM:??].
 */
static ag_object *object_new(unsigned id, void *payload,
        const struct ag_object_method *vt)
{
    ag_object *ctx = ag_mempool_new(sizeof *ctx);
    ctx->refc = 1;
    ctx->id = id;
    ctx->payload = payload;
    
    ctx->vt.copy = vt->copy ? vt->copy : copy_default;
    ctx->vt.dispose = vt->dispose? vt->dispose: dispose_default;
    ctx->vt.len = vt->len ? vt->len : len_default;
    ctx->vt.cmp = vt->cmp ? vt->cmp : cmp_default;
    ctx->vt.str = vt->str ? vt->str : str_default;
    
    return ctx;
}


/*
 *      The object_copy() helper function creates a deep copy of an object
 *      instance [DM:??].
 */
static inline void object_copy(ag_object **obj)
{
    ag_object *hnd = *obj;

    if (hnd->refc > 1) {
        ag_object *cp = ag_object_new(hnd->id, hnd->vt.copy(hnd->payload), 
                &hnd->vt);

        ag_object_dispose(obj);
        *obj = cp;
    }
}





/*******************************************************************************
 *                          INTERFACE IMPLEMENTATION
 */


/*
 *      Implementation of the ag_object_new() interface function [DM:??].
 */
extern ag_object *ag_object_new(unsigned id, void *payload,
        const struct ag_object_method *vt)
{
    ag_assert (id && payload && vt);
    return object_new(id, payload, vt);
}


/*
 *      Implementation of the ag_object_new_noid() interface function [DM:??].
 */
extern ag_object *ag_object_new_noid(void *payload,
        const struct ag_object_method *vt)
{
    const unsigned NOID = 0;

    ag_assert (payload && vt);
    return object_new(NOID, payload, vt);
}


/*
 *      Implementation of the ag_object_copy() interface function [DM:??].
 */
extern ag_object *ag_object_copy(const ag_object *ctx)
{
    ag_assert (ctx);
    ag_object *cp = (ag_object *) ctx;
    
    cp->refc++;
    return cp;
}


/*
 *      Implementation of the ag_object_free() interface function [DM:??].
 */
extern void ag_object_dispose(ag_object **ctx)
{
    ag_object *hnd;

    if (ag_likely (ctx && (hnd = *ctx))) {
        if (!--hnd->refc) {
            hnd->vt.dispose(hnd->payload);
            ag_mempool_free(&hnd->payload);
            ag_mempool_free((void **) ctx);
        }
    }
}


/*
 *      Implementation of the ag_object_id() interface function [DM:??].
 */
extern unsigned ag_object_id(const ag_object *ctx)
{
    ag_assert (ctx);
    return ctx->id;
}


/*
 *      Implementation of the ag_object_id_set() interface function [DM:??].
 */
extern void ag_object_id_set(ag_object **ctx, unsigned id)
{
    ag_assert (ctx && *ctx);
    object_copy(ctx);

    ag_assert (id);
    (*ctx)->id = id;
}


/*
 *      Implementation of the ag_object_hash() interface function [DM:??].
 */
extern unsigned ag_object_hash(const ag_object *ctx, size_t len)
{
    ag_assert (ctx && len);
    return ctx->id / len;
}


/*
 *      Implementation of the ag_object_len() interface function [DM:??].
 */
extern size_t ag_object_len(const ag_object *ctx)
{
    ag_assert (ctx);
    return ctx->vt.len(ctx);
}


/*
 *      Implementation of the ag_object_cmp() interface function [DM:??].
 */
extern enum ag_object_cmp ag_object_cmp(const ag_object *ctx, 
        const ag_object *cmp)
{
    ag_assert (ctx);
    return ctx->vt.cmp(ctx, cmp);
}


/*
 *      Declaration of the ag_object_lt() interface function [DM:??].
 */
extern inline bool ag_object_lt(const ag_object *ctx, const ag_object *cmp);


/*
 *      Declaration of the ag_object_eq() interface function [DM:??].
 */
extern inline bool ag_object_eq(const ag_object *ctx, const ag_object *cmp);


/*
 *      Declaration of the ag_object_gt() interface function [DM:??].
 */
extern inline bool ag_object_gt(const ag_object *ctx, const ag_object *cmp);


/*
 *      Implementation of the ag_object_payload() interface function [DM:??].
 */
extern const void *ag_object_payload(const ag_object *ctx)
{
    ag_assert (ctx);
    return ctx->payload;
}


/*
 *      Implementation of the ag_object_payload_mutable() interface function
 *      [DM:??].
 */
extern void *ag_object_payload_mutable(ag_object **ctx)
{
    ag_assert (ctx && *ctx);
    object_copy(ctx);

    return (*ctx)->payload;
}


/*
 *      Implementation of the ag_object_str() interface function [DM:??].
 */
extern const char *ag_object_str(const ag_object *ctx)
{
    ag_assert (ctx);
    return ctx->vt.str(ctx);
}


