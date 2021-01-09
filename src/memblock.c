#include "../include/argent.h"

#ifdef __FreeBSD__
#       include <malloc_np.h>
#else
#       include <malloc.h>
#endif
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#ifndef NDEBUG
static inline bool      is_size_valid(size_t );
static inline bool      is_alignment_valid(size_t);
#endif


static inline size_t *
meta_head(const ag_memblock *ctx)
{
        return &((size_t *)ctx)[-2];
}


static inline size_t
meta_sz(const ag_memblock *ctx)
{
        return ((size_t *)ctx)[-1];
}


static inline size_t
meta_refc(const ag_memblock *ctx)
{
        return ((size_t *)ctx)[-2];
}


extern ag_memblock *
ag_memblock_new(size_t sz)
{
        AG_ASSERT (is_size_valid(sz));

        size_t *ctx = malloc(sizeof(size_t) * 2 + sz);
        struct ag_exception_memblock x = { .sz = sz, .align = 0i };
        AG_REQUIRE_OPT (ctx, AG_ERNO_MBLOCK, &x);

        memset(ctx, 0, sz);
        ctx[0] = 1;
        ctx[1] = sz;

        return (ag_memblock *)&(ctx[2]);
}


extern ag_memblock *
ag_memblock_new_align(size_t sz, size_t align)
{
        AG_ASSERT (is_size_valid(sz));
        AG_ASSERT (is_alignment_valid(align));
        
        size_t *ctx;
        (void)posix_memalign((void **)&ctx, align, sizeof(size_t) * 2 + sz);

        struct ag_exception_memblock x = { .sz = sz, .align = align };
        AG_REQUIRE_OPT (ctx, AG_ERNO_MBLOCK, &x);
        
        memset(ctx, 0, sz);
        ctx[0] = 1;
        ctx[1] = sz;

        return (ag_memblock *)&(ctx[2]);
}


extern ag_memblock *
ag_memblock_copy(const ag_memblock *ctx)
{
        ag_memblock *cp = (ag_memblock *)ctx;
        ((size_t *)cp)[-2]++;

        return cp;
}


extern ag_memblock *
ag_memblock_clone(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        size_t sz = meta_sz(ctx);
        ag_memblock *cp = ag_memblock_new(sz);
        memcpy(cp, ctx, sz);

        return cp;
}


extern ag_memblock *
ag_memblock_clone_align(const ag_memblock *ctx, size_t align)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (is_alignment_valid(align));

        size_t sz = meta_sz(ctx);
        ag_memblock *cp = ag_memblock_new_align(sz, align);
        memcpy(cp, ctx, sz);

        return cp;
}


extern void
ag_memblock_release(ag_memblock **ctx)
{
        size_t *hnd;

        if (AG_LIKELY (ctx && (hnd = (size_t *)*ctx))) {
                if (!(--hnd[-2]))
                        free(&hnd[-2]);
                        
                *ctx = NULL;
        }
}


extern enum ag_cmp
ag_memblock_cmp(const ag_memblock *ctx, const ag_memblock *cmp)
{
        return ctx == cmp ? AG_CMP_EQ : memcmp(ctx, cmp, meta_sz(ctx));
}


extern inline bool      ag_memblock_lt(const ag_memblock *,
                            const ag_memblock *);
extern inline bool      ag_memblock_eq(const ag_memblock *,
                            const ag_memblock *);
extern inline bool      ag_memblock_gt(const ag_memblock *,
                            const ag_memblock *);


extern size_t
ag_memblock_sz(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        return meta_sz(ctx);
}

extern size_t
ag_memblock_sz_total(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        return malloc_usable_size(meta_head(ctx));
}




extern size_t
ag_memblock_refc(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        return meta_refc(ctx);
}


extern bool
ag_memblock_aligned(const ag_memblock *ctx, size_t align)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (is_alignment_valid(align));

        return !((uintptr_t)meta_head(ctx) & (align - 1));
}


extern void
ag_memblock_resize(ag_memblock **ctx, size_t sz)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT (is_size_valid(sz));

        ag_memblock *hnd = *ctx;
        size_t oldsz = meta_sz(hnd);

        ag_memblock *cp = ag_memblock_new(sz);
        memcpy(cp, hnd, sz < oldsz ? sz : oldsz);
        
        ag_memblock_release(ctx);
        *ctx = cp;
}


extern void
ag_memblock_resize_align(ag_memblock **ctx, size_t sz, size_t align)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT (is_size_valid(sz));
        AG_ASSERT (is_alignment_valid(align));

        ag_memblock *hnd = *ctx;
        size_t oldsz = meta_sz(hnd);

        ag_memblock *cp = ag_memblock_new_align(sz, align);
        memcpy(cp, hnd, sz < oldsz ? sz : oldsz);
        
        ag_memblock_release(ctx);
        *ctx = cp;
}


extern ag_string *
ag_memblock_str(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        return (ag_string_new_fmt("address = %p, data sz = %lu,"
            " total data = %lu, refc = %lu", (void *)meta_head(ctx),
            meta_sz(ctx), ag_memblock_sz_total(ctx), meta_refc(ctx)));

}


#ifndef NDEBUG
static inline bool
is_size_valid(size_t sz)
{
        return sz;
}
#endif


#ifndef NDEBUG
static inline bool
is_alignment_valid(size_t align)
{
        return align && !(align % 2);

}
#endif

