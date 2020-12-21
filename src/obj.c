#include "../include/argent.h"


struct ag_obj {
        size_t     ob_objid;   /* Object ID      */
        size_t     ob_typeid;  /* Object type ID */
        ag_mblock *ob_payload; /* Object payload */
};


static inline const struct ag_obj_vtable *vtable_get(const ag_obj *ctx)
{
        return ag_obj_registry_get(ctx->ob_typeid);
}


extern inline bool ag_obj_lt(const ag_obj *, const ag_obj *);
extern inline bool ag_obj_eq(const ag_obj *, const ag_obj *);
extern inline bool ag_obj_gt(const ag_obj *, const ag_obj *);
extern inline bool ag_obj_empty(const ag_obj *);


extern ag_obj *ag_obj_new(size_t typeid, ag_mblock *payload)
{
        ag_obj *ctx = ag_mblock_new(sizeof *ctx);
        
        ctx->ob_objid = 1;
        ctx->ob_typeid = typeid;
        ctx->ob_payload = payload;

        return ctx;
}


extern ag_obj *ag_obj_copy(const ag_obj *ctx)
{
        ag_obj *cp = (ag_obj *)ctx;
        ag_mblock_retain(cp);
        
        return cp;
}


extern ag_obj *ag_obj_clone(const ag_obj *ctx)
{
        //return ag_mblock_copy(ctx);
        //return vtable_get(ctx)->vt_clone(ctx);

        return ag_obj_new(ctx->ob_typeid,
                          vtable_get(ctx)->vt_clone(ctx->ob_payload));
}


extern void ag_obj_release(ag_obj **ctx)
{
        ag_obj *o = *ctx;
        vtable_get(o)->vt_release(o->ob_payload);

        ag_mblock *m = o;
        ag_mblock_release(&m);
        *ctx = m;
}


extern enum ag_cmp ag_obj_cmp(const ag_obj *ctx, const ag_obj *cmp)
{
        //return ag_mblock_cmp(ctx, cmp);
        return vtable_get(ctx)->vt_cmp(ctx, cmp);
}


extern bool ag_obj_valid(const ag_obj *ctx)
{
        //return ctx;
        return vtable_get(ctx)->vt_valid(ctx);
}


extern size_t ag_obj_sz(const ag_obj *ctx)
{
        return ag_mblock_sz(ctx) + ag_mblock_sz(ctx->ob_payload);
}


extern size_t ag_obj_refc(const ag_obj *ctx)
{
        return ag_mblock_refc(ctx);
}


extern size_t ag_obj_len(const ag_obj *ctx)
{
        //return 1;
        return vtable_get(ctx)->vt_len(ctx);
}


extern size_t ag_obj_hash(const ag_obj *ctx)
{
        //return 1;
        return vtable_get(ctx)->vt_hash(ctx);
}


extern ag_str *ag_obj_str(const ag_obj *ctx)
{
        //return ag_mblock_str(ctx);
        return vtable_get(ctx)->vt_str(ctx);
}


extern const ag_mblock *ag_obj_payload(const ag_obj *ctx)
{
        return ctx->ob_payload;
}


extern ag_mblock *ag_obj_payload_mutable(ag_obj **ctx)
{
        ag_obj *o = *ctx;
        ag_mblock *m = o;

        if (ag_mblock_refc(m) > 1) {
                ag_mblock_release(&m);
                *ctx = ag_obj_clone(o);
        }

        return (*ctx)->ob_payload;
}


