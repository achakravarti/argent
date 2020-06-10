#include "./api.h"

#define METADATA_OFFSET 1
#define METADATA_INDEX 0
#define ALIGNMENT_MAX 255

extern inline const void *ag_mblock_data(const ag_mblock_t *bfr);

extern inline void *ag_mblock_data_mutable(ag_mblock_t *bfr);

static inline void alignment_set(ag_mblock_t *bfr, size_t align)
{
    ((char *) bfr)[METADATA_INDEX] = align;
}


extern ag_mblock_t *ag_mblock_new(size_t sz)
{
    ag_assert (sz);
    ag_mblock_t *bfr = ag_mempool_new(sz + METADATA_OFFSET);
    
    alignment_set(bfr, 0);
    return bfr;
}

extern ag_mblock_t *ag_mblock_new_aligned(size_t align, size_t sz)
{
    ag_assert (align && align <= ALIGNMENT_MAX && !(align & (align - 1)));
    ag_assert (sz && !(sz % align));
    ag_mblock_t *bfr = ag_mempool_new_aligned(align, sz + METADATA_OFFSET);

    alignment_set(bfr, align);
    return bfr;
}

extern void ag_mblock_resize(ag_mblock_t **bfr, size_t sz)
{
    ag_assert (bfr && *bfr);
    size_t align = ((const char *) *bfr)[METADATA_INDEX]; 

    align ? ag_mempool_resize(bfr, sz) : ag_mempool_resize_aligned(bfr, sz, 
            align);
}

extern void ag_mblock_free(ag_mblock_t **bfr)
{
    ag_mempool_free(bfr);
}

extern size_t ag_mblock_alignment(const ag_mblock_t *bfr)
{
    ag_assert (bfr);
    return ((const char *) bfr)[METADATA_INDEX];
}

