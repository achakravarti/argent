#include "../include/argent.h"


struct vector {
        struct ag_obj_vtable *vt;
        size_t cap;
};


static struct vector *g_argent;
static struct vector *g_client;


/*
 * Prototypes for the helper functions for selecting the appropriate vector and
 * determining the vector index according to type ID.
 */
static inline struct vector *typeid_vector(int);
static inline size_t typeid_index(int);


/* Prototypes to manage managing vectors */
static inline struct vector *vector_new(size_t);
static void vector_release(struct vector **);
static inline void vector_resize(struct vector *, size_t);


/* Prototypes for vector accessor and mutator */
static inline const struct ag_obj_vtable *vector_get(const struct vector *,
                                                     size_t);
static void vector_set(struct vector *, size_t, const struct ag_obj_vtable *);


/* Prototypes for the default callback functions */
static ag_mblock *def_clone(const ag_mblock *);
static void def_release(ag_mblock *);
static enum ag_cmp def_cmp(const ag_obj *, const ag_obj *);
static bool def_valid(const ag_obj *);
static size_t def_len(const ag_obj *);
static size_t def_hash(const ag_obj *);
static ag_str *def_str(const ag_obj *);


extern void ag_obj_registry_init(void)
{
        g_argent = vector_new(sizeof(size_t));
        g_client = vector_new(sizeof(size_t));
}


extern void ag_obj_registry_exit(void)
{
        vector_release(&g_argent);
        vector_release(&g_client);
}


extern const struct ag_obj_vtable *ag_obj_registry_get(int typeid)
{
        return vector_get(typeid_vector(typeid), typeid_index(typeid));
}


extern void ag_obj_registry_set(int typeid, const struct ag_obj_vtable *vt)
{
        vector_set(typeid_vector(typeid), typeid_index(typeid), vt);
}


static inline struct vector *typeid_vector(int typeid)
{
        return typeid < 0 ? g_argent : g_client;
}


static inline size_t typeid_index(int typeid)
{
        return typeid < 0 ? 0 - typeid : typeid;
}


static inline struct vector *vector_new(size_t cap)
{
        struct vector *ctx = ag_mblock_new(cap);
        ctx->vt = ag_mblock_new(sizeof *ctx->vt * cap);
        ctx->cap = cap;

        return ctx;
}


static void vector_release(struct vector **ctx)
{
        struct vector *v;

        if (AG_LIKELY (ctx && (v = *ctx))) {
                ag_mblock *m = v->vt;
                ag_mblock_release(&m);

                m = v;
                ag_mblock_release(&m);
                *ctx = m;
        }
}


static inline void vector_resize(struct vector *ctx, size_t cap)
{
        ag_mblock *m = ctx->vt;
        ag_mblock_resize(&m, cap);
}


static inline const struct ag_obj_vtable *vector_get(const struct vector *ctx,
                                                     size_t idx)
{
        return &ctx->vt[idx];
}


static void vector_set(struct vector *ctx, size_t idx,
                       const struct ag_obj_vtable *vt)
{
        register size_t cap = ctx->cap;

        while (cap < idx)
                cap *= 2;

        vector_resize(ctx, cap);

        struct ag_obj_vtable *dst = &ctx->vt[idx];
        dst->vt_clone   = vt->vt_clone   ? vt->vt_clone   : def_clone;
        dst->vt_release = vt->vt_release ? vt->vt_release : def_release;
        dst->vt_cmp     = vt->vt_cmp     ? vt->vt_cmp     : def_cmp;
        dst->vt_valid   = vt->vt_valid   ? vt->vt_valid   : def_valid;
        dst->vt_len     = vt->vt_len     ? vt->vt_len     : def_len;
        dst->vt_hash    = vt->vt_hash    ? vt->vt_hash    : def_hash;
        dst->vt_str     = vt->vt_str     ? vt->vt_str     : def_str;
}


static ag_mblock *def_clone(const ag_mblock *ctx)
{
        return ag_mblock_clone(ctx);
}


static void def_release(ag_mblock *ctx)
{
        ag_mblock_release(&ctx);
}


static enum ag_cmp def_cmp(const ag_obj *ctx, const ag_obj *cmp)
{
        return ag_mblock_cmp(ctx, cmp);
}


static bool def_valid(const ag_obj *ctx)
{
        return ctx;
}


static size_t def_len(const ag_obj *ctx)
{
        (void)ctx;
        return 1;
}


static size_t def_hash(const ag_obj *ctx)
{
        (void)ctx;
        return 1;
}


static ag_str *def_str(const ag_obj *ctx)
{
        return ag_mblock_str(ctx);
}

