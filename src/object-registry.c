#include "../include/argent.h"


struct vector {
        struct ag_object_vtable *vt;
        size_t cap;
};


static struct vector *g_argent;
static struct vector *g_client;


/*
 * Prototypes for the helper functions for selecting the appropriate vector and
 * determining the vector index according to type ID.
 */
static inline struct vector *typeid_vector(ag_typeid);
static inline size_t         typeid_index(ag_typeid);


/* Prototypes to manage managing vectors */
static inline struct vector *vector_new(size_t);
static void                  vector_release(struct vector **);
static inline void           vector_resize(struct vector *, size_t);


/* Prototypes for vector accessor and mutator */
static inline const struct ag_object_vtable *vector_get(const struct vector *,
                                                        size_t);
static void vector_set(struct vector *, size_t, const struct ag_object_vtable *);


/* Prototypes for the default callback functions */
static ag_memblock *def_clone(const ag_memblock *);
static void         def_release(ag_memblock *);
static enum ag_cmp  def_cmp(const ag_object *, const ag_object *);
static bool         def_valid(const ag_object *);
static size_t       def_sz(const ag_object *);
static size_t       def_len(const ag_object *);
static size_t       def_hash(const ag_object *);
static ag_string   *def_str(const ag_object *);


extern void ag_object_registry_init(void)
{
        g_argent = vector_new(sizeof(size_t));
        g_client = vector_new(sizeof(size_t));
}


extern void ag_object_registry_exit(void)
{
        vector_release(&g_argent);
        vector_release(&g_client);
}


extern const struct ag_object_vtable *ag_object_registry_get(ag_typeid typeid)
{
        return vector_get(typeid_vector(typeid), typeid_index(typeid));
}


extern void ag_object_registry_set(ag_typeid typeid,
                                const struct ag_object_vtable *vt)
{
        vector_set(typeid_vector(typeid), typeid_index(typeid), vt);
}


static inline struct vector *typeid_vector(ag_typeid typeid)
{
        return typeid < AG_TYPEID_OBJECT ? g_argent : g_client;
}


static inline size_t typeid_index(ag_typeid typeid)
{
        return typeid < AG_TYPEID_OBJECT ? AG_TYPEID_OBJECT - typeid : typeid;
}


static inline struct vector *vector_new(size_t cap)
{
        struct vector *ctx = ag_memblock_new(sizeof *ctx);
        ctx->vt = ag_memblock_new(sizeof *ctx->vt * cap);
        ctx->cap = cap;

        return ctx;
}


static void vector_release(struct vector **ctx)
{
        struct vector *v;

        if (AG_LIKELY (ctx && (v = *ctx))) {
                ag_memblock *m = v->vt;
                ag_memblock_release(&m);

                m = v;
                ag_memblock_release(&m);
                *ctx = m;
        }
}


static inline void vector_resize(struct vector *ctx, size_t cap)
{
        ag_memblock *m = ctx->vt;
        ag_memblock_resize(&m, cap);
}


static inline const struct ag_object_vtable *vector_get(const struct vector *ctx,
                                                     size_t idx)
{
        return &ctx->vt[idx];
}


static void vector_set(struct vector *ctx, size_t idx,
                       const struct ag_object_vtable *vt)
{
        register size_t cap = ctx->cap;

        if (cap < idx) {
                while (cap < idx)
                        cap *= 2;

                vector_resize(ctx, cap);
        }

        struct ag_object_vtable *dst = &ctx->vt[idx];
        dst->clone   = vt->clone   ? vt->clone   : def_clone;
        dst->release = vt->release ? vt->release : def_release;
        dst->cmp     = vt->cmp     ? vt->cmp     : def_cmp;
        dst->valid   = vt->valid   ? vt->valid   : def_valid;
        dst->sz      = vt->sz      ? vt->sz      : def_sz;
        dst->len     = vt->len     ? vt->len     : def_len;
        dst->hash    = vt->hash    ? vt->hash    : def_hash;
        dst->str     = vt->str     ? vt->str     : def_str;
}


static ag_memblock *def_clone(const ag_memblock *ctx)
{
        return ag_memblock_clone(ctx);
}


// we don't do anything because ag_object_release() takes care of releasing the
// memory allocated to the payload
static void def_release(ag_memblock *ctx)
{
        (void)ctx;
}


static enum ag_cmp def_cmp(const ag_object *ctx, const ag_object *cmp)
{
        return ag_memblock_cmp(ctx, cmp);
}


static bool def_valid(const ag_object *ctx)
{
        return ctx;
}


static size_t def_sz(const ag_object *ctx)
{
        return ag_memblock_sz(ctx) + ag_memblock_sz(ag_object_payload(ctx));
}


static size_t def_len(const ag_object *ctx)
{
        (void)ctx;
        return 1;
}


static ag_hash def_hash(const ag_object *ctx)
{
        AG_AUTO(ag_uuid) *u = ag_object_uuid(ctx);
        return ag_uuid_hash(u);
}


static ag_string *def_str(const ag_object *ctx)
{
        AG_AUTO(ag_uuid) *u    = ag_object_uuid(ctx);
        AG_AUTO(ag_string)  *ustr = ag_uuid_str(u);
        AG_AUTO(ag_string)  *mstr = ag_memblock_str(ctx);

        return ag_string_new_fmt("typeid = %d, uuid = %s, %s", ag_object_typeid(ctx),
                              ustr, mstr);
}

