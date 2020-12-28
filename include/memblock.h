#ifndef __ARGENT_MEMBLOCK_H__
#define __ARGENT_MEMBLOCK_H__

#ifdef __cplusplus
        extern "C" {
#endif


#include "./argent.h"



struct ag_mblock_exception {
        size_t sz;
        size_t align;
};


extern void ag_mblock_exception_handler(const struct ag_exception *, void *);

typedef void ag_mblock;

extern ag_mblock *ag_mblock_new(size_t);
extern ag_mblock *ag_mblock_new_align(size_t, size_t);
extern ag_mblock *ag_mblock_copy(const ag_mblock *);
extern ag_mblock *ag_mblock_clone(const ag_mblock *);
extern ag_mblock *ag_mblock_clone_align(const ag_mblock *, size_t);
extern void ag_mblock_release(ag_mblock **);

// warning: don't use with structs containing non-scalar members
// if not equal, comparison based on first differing byte
extern enum ag_cmp ag_mblock_cmp(const ag_mblock *, const ag_mblock *cmp);

inline bool ag_mblock_lt(const ag_mblock *ctx, const ag_mblock *cmp)
{
        return ag_mblock_cmp(ctx, cmp) == AG_CMP_LT;
}

inline bool ag_mblock_eq(const ag_mblock *ctx, const ag_mblock *cmp)
{
        return ag_mblock_cmp(ctx, cmp) == AG_CMP_EQ;
}

inline bool ag_mblock_gt(const ag_mblock *ctx, const ag_mblock *cmp)
{
        return ag_mblock_cmp(ctx, cmp) == AG_CMP_GT;
}

extern size_t ag_mblock_sz(const ag_mblock *);
extern size_t ag_mblock_sz_total(const ag_mblock *);
extern size_t ag_mblock_refc(const ag_mblock *);
extern bool ag_mblock_aligned(const ag_mblock *, size_t);

extern void ag_mblock_resize(ag_mblock **, size_t);
extern void ag_mblock_resize_align(ag_mblock **, size_t, size_t);
extern ag_string *ag_mblock_str(const ag_mblock *);


#ifdef __cplusplus
        }
#endif

#endif /* !__ARGENT_MEMBLOCK_H__ */

