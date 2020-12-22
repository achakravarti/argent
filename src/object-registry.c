#include "../include/argent.h"


static ag_mblock *def_clone(const ag_mblock *);
static void def_release(ag_mblock *);
static enum ag_cmp def_cmp(const ag_obj *, const ag_obj *);
static bool def_valid(const ag_obj *);
static size_t def_len(const ag_obj *);
static size_t def_hash(const ag_obj *);
static ag_str *def_str(const ag_obj *);


extern void ag_obj_registry_init(size_t len)
{
        (void)len;
}


extern void ag_obj_registry_exit(void)
{
}


extern const struct ag_obj_vtable *ag_obj_registry_get(size_t typeid)
{
        (void)typeid;

        static struct ag_obj_vtable vt = {
                .vt_clone   = &def_clone,
                .vt_release = &def_release,
                .vt_cmp     = &def_cmp,
                .vt_valid   = &def_valid,
                .vt_len     = &def_len,
                .vt_hash    = &def_hash,
                .vt_str     = &def_str,
        };

        return &vt;
}


extern void ag_obj_registry_set(size_t typeid, const struct ag_obj_vtable *vt)
{
        (void)typeid;
        (void)vt;
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

