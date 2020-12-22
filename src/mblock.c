#include "../include/argent.h"

#include <malloc.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#ifndef NDEBUG
        static inline bool is_size_valid(size_t );
        static inline bool is_alignment_valid(size_t);
#endif


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


extern void ag_mblock_exception_handler(const struct ag_exception *ex,
                void *opt)
{
        struct ag_mblock_exception *x = (struct ag_mblock_exception *) opt;

        printf("[!] %d [%s(), %s:%lu]: %s\n", ex->erno, ex->func, ex->file,
                        ex->line, ag_exception_registry_msg(ex->erno));
        
        ag_log_err("%d [%s(), %s:%lu]: %s", ex->erno, ex->func, ex->file,
                        ex->line, ag_exception_registry_msg(ex->erno));

        if (x->align) {
                printf("[!] requested %lu bytes alignmed to %lu bytes\n",
                                x->sz, x->align);
                ag_log_err("requested %lu bytes aligned to %lu bytes", x->sz,
                                x->align);
        } else {
                printf("[!] requested %lu bytes\n", x->sz);
                ag_log_err("requested %lu bytes", x->sz);
        }

        ag_exit(EXIT_FAILURE);
}


extern ag_mblock *ag_mblock_new(size_t sz)
{
        AG_ASSERT (is_size_valid(sz));

        struct ag_mblock_exception x = {
                .sz = sz,
                .align = 0,
        };

        size_t *ctx = malloc(sizeof(size_t) * 2 + sz);
        AG_REQUIRE_OPT (ctx, AG_ERNO_MBLOCK, &x);

        memset(ctx, 0, sz);
        ctx[0] = 1;
        ctx[1] = sz;

        return (ag_mblock *) &(ctx[2]);
}


extern ag_mblock *ag_mblock_new_align(size_t sz, size_t align)
{
        AG_ASSERT (is_size_valid(sz));
        AG_ASSERT (is_alignment_valid(align));
        
        struct ag_mblock_exception x = {
                .sz = sz,
                .align = align,
        };

        size_t *ctx;
        (void) posix_memalign((void **)&ctx, align, sizeof(size_t) * 2 + sz);
        AG_REQUIRE_OPT (ctx, AG_ERNO_MBLOCK, &x);
        
        memset(ctx, 0, sz);
        ctx[0] = 1;
        ctx[1] = sz;

        return (ag_mblock *)&(ctx[2]);
}


extern ag_mblock *ag_mblock_copy(const ag_mblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        size_t sz = meta_sz(ctx);
        ag_mblock *cp = ag_mblock_new(sz);
        memcpy(cp, ctx, sz);

        return cp;
}


extern ag_mblock *ag_mblock_copy_align(const ag_mblock *ctx, size_t align)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (is_alignment_valid(align));

        size_t sz = meta_sz(ctx);
        ag_mblock *cp = ag_mblock_new_align(sz, align);
        memcpy(cp, ctx, sz);

        return cp;
}


extern void ag_mblock_dispose(ag_mblock **ctx)
{
        ag_mblock *hnd;

        if (AG_LIKELY (ctx && (hnd = *ctx))) {
                free(meta_head(hnd));
                *ctx = NULL;
        }

}

extern void ag_mblock_release(ag_mblock **ctx)
{
        size_t *hnd;

        if (AG_LIKELY (ctx && (hnd = (size_t *)*ctx))) {
                if (!(--hnd[-2]))
                        free(&hnd[-2]);
                        
                *ctx = NULL;
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
        AG_ASSERT_PTR (ctx);

        return meta_sz(ctx);
}

extern size_t ag_mblock_sz_total(const ag_mblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        return malloc_usable_size(meta_head(ctx));
}




extern size_t ag_mblock_refc(const ag_mblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        return meta_refc(ctx);
}


extern bool ag_mblock_aligned(const ag_mblock *ctx, size_t align)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (is_alignment_valid(align));

        return !((uintptr_t)meta_head(ctx) & (align - 1));
}


extern void ag_mblock_retain(ag_mblock *ctx)
{
        AG_ASSERT_PTR (ctx);
        
        ((size_t *) ctx)[-2]++;
}




extern void ag_mblock_resize(ag_mblock **ctx, size_t sz)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT (is_size_valid(sz));

        ag_mblock *hnd = *ctx;
        size_t oldsz = meta_sz(hnd);

        ag_mblock *cp = ag_mblock_new(sz);
        memcpy(cp, hnd, sz < oldsz ? sz : oldsz);
        
        ag_mblock_dispose(ctx);
        *ctx = cp;
}


extern void ag_mblock_resize_align(ag_mblock **ctx, size_t sz, size_t align)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT (is_size_valid(sz));
        AG_ASSERT (is_alignment_valid(align));

        ag_mblock *hnd = *ctx;
        size_t oldsz = meta_sz(hnd);

        ag_mblock *cp = ag_mblock_new_align(sz, align);
        memcpy(cp, hnd, sz < oldsz ? sz : oldsz);
        
        ag_mblock_dispose(ctx);
        *ctx = cp;
}


extern ag_str *ag_mblock_str(const ag_mblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_str_new_fmt("address = %p, data sz = %lu, total data = %lu,"
                              " refc = %lu", (void *)meta_head(ctx),
                              meta_sz(ctx), ag_mblock_sz_total(ctx),
                              meta_refc(ctx));

}


#ifndef NDEBUG
static inline bool is_size_valid(size_t sz)
{
        return sz;
}
#endif


#ifndef NDEBUG
static inline bool is_alignment_valid(size_t align)
{
        return align && !(align % 2);

}
#endif

