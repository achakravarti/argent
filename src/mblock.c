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
        char *bfr = malloc(vsnprintf(NULL, 0, fmt, args) + 1);
        va_end(args);

        va_start(args, fmt);
        (void) vsprintf(bfr, fmt, args);
        va_end(args);

        return bfr;
}


extern ag_mblock *ag_mblock_new(size_t sz)
{
        size_t *ctx = malloc(sz + sizeof(size_t));
        
        memset(ctx, 0, sz);
        ctx[0] = 1;

        return (void *)&ctx[1];
}


extern ag_mblock *ag_mblock_new_align(size_t sz, size_t align)
{
        size_t *ctx;
        (void) posix_memalign(&ctx, align, sz);

        memset(ctx, 0, sz);
        ctx[0] = 1;

        return (void *)&ctx[1];
}


extern ag_mblock *ag_mblock_copy(const ag_mblock *ctx)
{

        size_t sz = malloc_usable_size((void *)ctx);
        ag_mblock *cp = ag_mblock_new(sz);
        memcpy(cp, ctx, sz);

        return cp;
}


extern void ag_mblock_retain(ag_mblock *ctx)
{
        ((size_t *)ctx)[-1]++;
}


extern void ag_mblock_release(ag_mblock *ctx)
{
        ((size_t *)ctx)[-1]--;
}


extern void ag_mblock_dispose(ag_mblock **ctx)
{
       if (ctx && *ctx) {
               free(*ctx);
               *ctx = NULL;
       } 
}

extern enum ag_cmp ag_mblock_cmp(const ag_mblock *ctx, const ag_mblock *cmp)
{
        return memcmp(ctx, cmp, malloc_usable_size((void *)ctx));
}

extern inline bool ag_mblock_lt(const ag_mblock *, const ag_mblock *);

extern inline bool ag_mblock_eq(const ag_mblock *, const ag_mblock *);

extern inline bool ag_mblock_gt(const ag_mblock *, const ag_mblock *);

extern size_t ag_mblock_sz(const ag_mblock *restrict ctx)
{
        return malloc_usable_size((void *)ctx);
}


extern size_t ag_mblock_refc(const ag_mblock *ctx)
{
        return ((size_t *)ctx)[-1];
}


extern bool ag_mblock_aligned(const ag_mblock *restrict ctx, size_t align)
{
        return !((uintptr_t)ctx & (align - 1));
}


extern void ag_mblock_resize(ag_mblock **ctx, size_t sz)
{
        *ctx = realloc(*ctx, sz);
}


extern char *ag_mblock_str(const ag_mblock *ctx)
{
        return str_new_fmt("address = %p, size = %lu, refcount = %lu",
                        (void *)ctx, malloc_usable_size((void *)ctx),
                        ((size_t *)ctx)[-1]);

}

