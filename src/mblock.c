#include "../include/argent.h"

#include <malloc.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


/*
 * str_new_fmt(): create new dynamic formatted string.
 *
 * @fmt: formatted static source string.
 * @...: format arguments.
 *
 * Return: new dynamic formatted string.
 */
static char *str_new_fmt(const char *fmt, ...)
{
        va_list args;

        va_start(args, fmt);
        char *bfr = ag_mblock_new(vsnprintf(NULL, 0, fmt, args) + 1);
        va_end(args);

        va_start(args, fmt);
        (void) vsprintf(bfr, fmt, args);
        va_end(args);

        return bfr;
}


static inline size_t *meta_head(const ag_mblock *ctx)
{
        return &((size_t *)ctx)[-2];
}


static inline size_t meta_sz(const ag_mblock *ctx)
{
        return ((size_t *)ctx)[-1];
}


static inline size_t meta_refc(const ag_mblock *ctx)
{
        return ((size_t *)ctx)[-2];
}


static inline void meta_refc_retain(ag_mblock *ctx)
{
        ((size_t *)ctx)[-2]++;
}


static inline void meta_refc_release(ag_mblock *ctx)
{
        ((size_t *)ctx)[-2]--;
}


extern ag_mblock *ag_mblock_new(size_t sz)
{
        size_t *ctx = malloc(sizeof(size_t) * 2 + sz);
        
        memset(ctx, 0, sz);
        ctx[0] = 1;
        ctx[1] = sz;

        return (ag_mblock *) &(ctx[2]);
}


extern ag_mblock *ag_mblock_new_align(size_t sz, size_t align)
{
        size_t *ctx;
        (void) posix_memalign((void **)&ctx, align, sizeof(size_t) * 2 + sz);

        memset(ctx, 0, sz);
        ctx[0] = 1;
        ctx[1] = sz;

        return (ag_mblock *) &(ctx[2]);
}


extern ag_mblock *ag_mblock_copy(const ag_mblock *ctx)
{
        size_t *hnd = (ag_mblock *)ctx;
        meta_refc_retain(hnd);
        return hnd;
}


extern ag_mblock *ag_mblock_copy_deep(const ag_mblock *ctx)
{
        size_t sz = meta_sz(ctx);
        ag_mblock *cp = ag_mblock_new(sz);
        memcpy(cp, ctx, sz);

        return cp;
}


extern ag_mblock *ag_mblock_copy_deep_align(const ag_mblock *ctx, size_t align)
{
        size_t sz = meta_sz(ctx);
        ag_mblock *cp = ag_mblock_new_align(sz, align);
        memcpy(cp, ctx, sz);

        return cp;
}


extern void ag_mblock_free(ag_mblock **ctx)
{
        ag_mblock *hnd;

        if (AG_LIKELY (ctx && (hnd = *ctx))) {
                meta_refc_release(hnd);

                if (!meta_refc(hnd)) {
                        free(meta_head(hnd));
                        *ctx = NULL;
                }
        }

}

extern enum ag_cmp ag_mblock_cmp(const ag_mblock *ctx, const ag_mblock *cmp)
{
        return ctx == cmp ? AG_CMP_EQ : memcmp(ctx, cmp, meta_sz(ctx));
}

extern inline bool ag_mblock_lt(const ag_mblock *, const ag_mblock *);

extern inline bool ag_mblock_eq(const ag_mblock *, const ag_mblock *);

extern inline bool ag_mblock_gt(const ag_mblock *, const ag_mblock *);


extern size_t ag_mblock_sz(const ag_mblock *ctx)
{
        return meta_sz(ctx);
}

extern size_t ag_mblock_sz_total(const ag_mblock *ctx)
{
        return malloc_usable_size(meta_head(ctx));
}




extern size_t ag_mblock_refc(const ag_mblock *ctx)
{
        return meta_refc(ctx);
}


extern bool ag_mblock_aligned(const ag_mblock *ctx, size_t align)
{
        return !((uintptr_t)meta_head(ctx) & (align - 1));
}


extern void ag_mblock_resize(ag_mblock **ctx, size_t sz)
{
        ag_mblock *hnd = *ctx;
        size_t oldsz = meta_sz(hnd);

        ag_mblock *cp = ag_mblock_new(sz);
        memcpy(cp, hnd, sz < oldsz ? sz : oldsz);
        
        ag_mblock_free(ctx);
        *ctx = cp;
}


extern void ag_mblock_resize_align(ag_mblock **ctx, size_t sz, size_t align)
{
        ag_mblock *hnd = *ctx;
        size_t oldsz = meta_sz(hnd);

        ag_mblock *cp = ag_mblock_new_align(sz, align);
        memcpy(cp, hnd, sz < oldsz ? sz : oldsz);
        
        ag_mblock_free(ctx);
        *ctx = cp;
}


extern char *ag_mblock_str(const ag_mblock *ctx)
{
        return str_new_fmt("address = %p, data sz = %lu, total data = %lu,"
                        " refc = %lu", (void *)meta_head(ctx), meta_sz(ctx),
                        ag_mblock_sz_total(ctx), meta_refc(ctx));

}

