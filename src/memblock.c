#include <string.h>
#include "./api.h"


extern ag_memblock_t *ag_memblock_new(size_t sz)
{
    ag_assert (sz);
    void *bfr = malloc(sz);
    ag_require (bfr, AG_ERNO_MEMBLOCK_NEW, NULL);

    memset(bfr, 0, sz);
    return bfr;
}


extern void ag_memblock_resize(ag_memblock_t **bfr, size_t sz)
{
    ag_assert (bfr && *bfr && sz);
    *bfr = realloc(*bfr, sz);
    ag_require (*bfr, AG_ERNO_MEMBLOCK_RESIZE, NULL);
}


extern void ag_memblock_free(ag_memblock_t **bfr)
{
    if (ag_likely (bfr && *bfr)) {
        free(*bfr);
        *bfr = NULL;
    }
}

