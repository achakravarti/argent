#include "../include/argent.h"


struct ag_object {
        ag_typeid  typeid;  /* Object type ID */
        ag_uuid   *uuid;    /* Object ID      */
        ag_memblock *payload; /* Object payload */
};


static inline const struct ag_object_vtable *vtable_get(const ag_object *ctx)
{
        return ag_object_registry_get(ctx->typeid);
}


extern inline bool ag_object_lt(const ag_object *, const ag_object *);
extern inline bool ag_object_eq(const ag_object *, const ag_object *);
extern inline bool ag_object_gt(const ag_object *, const ag_object *);
extern inline bool ag_object_empty(const ag_object *);


extern ag_object *ag_object_new(ag_typeid typeid, ag_memblock *payload)
{
        AG_ASSERT_PTR (payload);

        ag_object *ctx = ag_memblock_new(sizeof *ctx);
        
        ctx->uuid    = ag_uuid_new();
        ctx->typeid  = typeid;
        ctx->payload = payload;

        return ctx;
}


extern ag_object *ag_object_copy(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_memblock_copy(ctx);
}


extern ag_object *ag_object_clone(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_object_new(ctx->typeid, vtable_get(ctx)->clone(ctx->payload));
}


extern void ag_object_release(ag_object **ctx)
{
        ag_object    *o;
        ag_memblock *m;

        if (AG_LIKELY (ctx && (o = *ctx))) {
                if (ag_memblock_refc(o) == 1) {
                        ag_uuid_release(&o->uuid);
                        vtable_get(o)->release(o->payload);

                        m = o->payload;
                        ag_memblock_release(&m);
                }

                m = o;
                ag_memblock_release(&m);
                *ctx = m;
        }
}


extern enum ag_cmp ag_object_cmp(const ag_object *ctx, const ag_object *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        
        return vtable_get(ctx)->cmp(ctx, cmp);
}


extern ag_typeid ag_object_typeid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ctx->typeid;
}


extern ag_uuid *ag_object_uuid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_uuid_copy(ctx->uuid);
}


extern bool ag_object_valid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return vtable_get(ctx)->valid(ctx);
}


extern size_t ag_object_sz(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return vtable_get(ctx)->sz(ctx);
}


extern size_t ag_object_refc(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_memblock_refc(ctx);
}


extern size_t ag_object_len(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        
        return vtable_get(ctx)->len(ctx);
}


extern ag_hash ag_object_hash(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);
        
        return vtable_get(ctx)->hash(ctx);
}


extern ag_string *ag_object_str(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return vtable_get(ctx)->str(ctx);
}


extern const ag_memblock *ag_object_payload(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ctx->payload;
}


extern ag_memblock *ag_object_payload_mutable(ag_object **ctx)
{
        AG_ASSERT_PTR (ctx && *ctx);

        ag_object *o = *ctx;
        ag_memblock *m = o;

        if (ag_memblock_refc(m) > 1) {
                ag_memblock_release(&m);
                *ctx = ag_object_clone(o);
        }

        return (*ctx)->payload;
}

