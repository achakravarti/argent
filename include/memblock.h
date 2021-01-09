#ifndef __ARGENT_MEMBLOCK_H__
#define __ARGENT_MEMBLOCK_H__

#ifdef __cplusplus
        extern "C" {
#endif


#include "./string.h"


typedef void ag_memblock;

extern ag_memblock *ag_memblock_new(size_t);
extern ag_memblock *ag_memblock_new_align(size_t, size_t);
extern ag_memblock *ag_memblock_copy(const ag_memblock *);
extern ag_memblock *ag_memblock_clone(const ag_memblock *);
extern ag_memblock *ag_memblock_clone_align(const ag_memblock *, size_t);
extern void ag_memblock_release(ag_memblock **);

// warning: don't use with structs containing non-scalar members
// if not equal, comparison based on first differing byte
extern enum ag_cmp ag_memblock_cmp(const ag_memblock *, const ag_memblock *cmp);

inline bool ag_memblock_lt(const ag_memblock *ctx, const ag_memblock *cmp)
{
        return ag_memblock_cmp(ctx, cmp) == AG_CMP_LT;
}

inline bool ag_memblock_eq(const ag_memblock *ctx, const ag_memblock *cmp)
{
        return ag_memblock_cmp(ctx, cmp) == AG_CMP_EQ;
}

inline bool ag_memblock_gt(const ag_memblock *ctx, const ag_memblock *cmp)
{
        return ag_memblock_cmp(ctx, cmp) == AG_CMP_GT;
}

extern size_t ag_memblock_sz(const ag_memblock *);
extern size_t ag_memblock_sz_total(const ag_memblock *);
extern size_t ag_memblock_refc(const ag_memblock *);
extern bool ag_memblock_aligned(const ag_memblock *, size_t);

extern void ag_memblock_resize(ag_memblock **, size_t);
extern void ag_memblock_resize_align(ag_memblock **, size_t, size_t);
extern ag_string *ag_memblock_str(const ag_memblock *);


#ifdef __cplusplus
        }
#endif

#endif /* !__ARGENT_MEMBLOCK_H__ */

