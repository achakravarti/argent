#ifndef __ARGENT_MBLOCK_H__
#define __ARGENT_MBLOCK_H__

#ifdef __cplusplus
        extern "C" {
#endif

#include "./argent.h"

#include <stdbool.h>
#include <stddef.h>

enum ag_cmp {
        AG_CMP_LT = -1,
        AG_CMP_EQ,
        AG_CMP_GT
};


typedef void ag_mblock;


extern ag_mblock *ag_mblock_new(size_t);
extern ag_mblock *ag_mblock_new_align(size_t, size_t);
extern ag_mblock *ag_mblock_copy(const ag_mblock *);
extern void ag_mblock_retain(ag_mblock *);
extern void ag_mblock_release(ag_mblock *);
extern void ag_mblock_dispose(ag_mblock **);

extern enum ag_cmp ag_mblock_cmp(const ag_mblock *, const ag_mblock *);
extern bool ag_mblock_lt(const ag_mblock *, const ag_mblock *);
extern bool ag_mblock_eq(const ag_mblock *, const ag_mblock *);
extern bool ag_mblock_gt(const ag_mblock *, const ag_mblock *);

extern size_t ag_mblock_sz(const ag_mblock *);
extern size_t ag_mblock_refc(const ag_mblock *);
extern size_t ag_mblock_alignment(const ag_mblock *);

extern void ag_mblock_resize(ag_mblock **);
extern char *ag_mblock_str(const ag_mblock *);


#ifdef __cplusplus
        }
#endif

#endif /* !__ARGENT_MBLOCK_H__ */

