#include "../include/argent.h"


struct ag_obj {
        ag_typeid  typeid;  /* Object type ID */
        ag_uuid   *uuid;    /* Object ID      */
        ag_mblock *payload; /* Object payload */
};


static inline const struct ag_obj_vtable *vtable_get(const ag_obj *ctx)
{
        return ag_obj_registry_get(ctx->typeid);
}


extern inline bool ag_obj_lt(const ag_obj *, const ag_obj *);
extern inline bool ag_obj_eq(const ag_obj *, const ag_obj *);
extern inline bool ag_obj_gt(const ag_obj *, const ag_obj *);
extern inline bool ag_obj_empty(const ag_obj *);


extern ag_obj *ag_obj_new(ag_typeid typeid, ag_mblock *payload)
{
        AG_ASSERT_PTR (payload);

        ag_obj *ctx = ag_mblock_new(sizeof *ctx);
        
        ctx->uuid    = ag_uuid_new();
        ctx->typeid  = typeid;
        ctx->payload = payload;

        return ctx;
}


extern ag_obj *ag_obj_copy(const ag_obj *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_mblock_copy(ctx);
}


extern ag_obj *ag_obj_clone(const ag_obj *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_obj_new(ctx->typeid, vtable_get(ctx)->clone(ctx->payload));
}


extern void ag_obj_release(ag_obj **ctx)
{
        ag_obj *o;

        if (AG_LIKELY (ctx && (o = *ctx))) {
                vtable_get(o)->release(o->payload);

                ag_mblock *m = o;
                ag_mblock_release(&m);
                *ctx = m;
        }
}


extern enum ag_cmp ag_obj_cmp(const ag_obj *ctx, const ag_obj *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        
        return vtable_get(ctx)->cmp(ctx, cmp);
}


extern ag_typeid ag_obj_typeid(const ag_obj *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ctx->typeid;
}


extern ag_uuid *ag_obj_uuid(const ag_obj *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_uuid_copy(ctx->uuid);
}


extern bool ag_obj_valid(const ag_obj *ctx)
{
        AG_ASSERT_PTR (ctx);

        return vtable_get(ctx)->valid(ctx);
}


extern size_t ag_obj_sz(const ag_obj *ctx)
{
        AG_ASSERT_PTR (ctx);

        return vtable_get(ctx)->sz(ctx);
}


extern size_t ag_obj_refc(const ag_obj *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_mblock_refc(ctx);
}


extern size_t ag_obj_len(const ag_obj *ctx)
{
        AG_ASSERT_PTR (ctx);
        
        return vtable_get(ctx)->len(ctx);
}


extern size_t ag_obj_hash(const ag_obj *ctx)
{
        AG_ASSERT_PTR (ctx);
        
        return vtable_get(ctx)->hash(ctx);
}


extern ag_str *ag_obj_str(const ag_obj *ctx)
{
        AG_ASSERT_PTR (ctx);

        return vtable_get(ctx)->str(ctx);
}


extern const ag_mblock *ag_obj_payload(const ag_obj *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ctx->payload;
}


extern ag_mblock *ag_obj_payload_mutable(ag_obj **ctx)
{
        AG_ASSERT_PTR (ctx && *ctx);

        ag_obj *o = *ctx;
        ag_mblock *m = o;

        if (ag_mblock_refc(m) > 1) {
                ag_mblock_release(&m);
                *ctx = ag_obj_clone(o);
        }

        return (*ctx)->payload;
}

