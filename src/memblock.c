/*******************************************************************************
 *                        __   ____   ___  ____  __ _  ____ 
 *                       / _\ (  _ \ / __)(  __)(  ( \(_  _)
 *                      /    \ )   /( (_ \ ) _) /    /  )(  
 *                      \_/\_/(__\_) \___/(____)\_)__) (__)                     
 *
 * Argent Library
 * Copyright (c) 2020 Abhishek Chakravarti <abhishek@taranjali.org>
 *
 * This file is part of the Argent Library. It implements the memory block
 * interface of the Argent Library.
 *
 * The contents of this file are released under the GPLv3 License. See the
 * accompanying LICENSE file or the generated Developer Manual (section I:?) for 
 * complete licensing details.
 *
 * BY CONTINUING TO USE AND/OR DISTRIBUTE THIS FILE, YOU ACKNOWLEDGE THAT YOU
 * HAVE UNDERSTOOD THESE LICENSE TERMS AND ACCEPT TO BE LEGALLY BOUND BY THEM.
 ******************************************************************************/


#include <malloc.h>
#include <string.h>
#include "./api.h"




/*******************************************************************************
 *                           INTERFACE IMPLEMENTATION
 */


/*
 * `ag_memblock_new()` is responsible for allocating a new block of heap memory.
 * The size in bytes to allocate is passed through the `sz` parameter. The value
 * of `sz` is required to be greater than zero; this condition is asserted in
 * debug builds.
 *
 * If successful, a handle to the new block of heap memory (initialised to zero)
 * is returned. In case of failure to allocate heap memory, `ag_memblock_new()` 
 * raises an exception with the `AG_ERNO_MEMBLOCK_NEW` error code, which is left
 * for the exception handler callback function (defined by client code) to 
 * process. Hence, there is no need to check for the returned handle being 
 * non-`NULL` in the normal flow of code.
 *
 * It is important to note that the actual size of the allocated block may well
 * be greater than that specified by `sz`. This is because the underlying
 * `malloc()` function may allocate a larger block in order to account for
 * alignment and minimum size constraints.
 *
 * The two most likely reasons for failure are the value of `sz` being larger
 * than the amount of heap memory physically available, and excess fragmentation
 * of the heap. Neither of these faiulre conditions have been tested, though.
 */
extern ag_memblock_t *ag_memblock_new(size_t sz)
{
    ag_assert (sz);
    void *bfr = malloc(sz);
    ag_require (bfr, AG_ERNO_MEMBLOCK_NEW, NULL);

    memset(bfr, 0, sz);
    return bfr;
}


                         /* implementation of ag_memblock_copy() [AgDM:??] */
extern ag_memblock_t *ag_memblock_copy(const ag_memblock_t *bfr)
{
    ag_assert (bfr);
    size_t sz = ag_memblock_sz(bfr);

    ag_memblock_t *cp = ag_memblock_new(sz);
    memcpy(cp, bfr, sz);

    return cp;
}


                              /* implementation of ag_memblock_sz() [AgDM:??] */
extern size_t ag_memblock_sz(const ag_memblock_t *bfr)
{
    ag_assert (bfr);
    return malloc_usable_size((void *) bfr);
}


                          /* implementation of ag_memblock_resize() [AgDM:??] */
extern void ag_memblock_resize(ag_memblock_t **bfr, size_t sz)
{
    ag_assert (bfr && *bfr && sz);
    *bfr = realloc(*bfr, sz);
    ag_require (*bfr, AG_ERNO_MEMBLOCK_RESIZE, NULL);
}


                            /* implementation of ag_memblock_free() [AgDM:??] */
extern void ag_memblock_free(ag_memblock_t **bfr)
{
    if (ag_likely (bfr && *bfr)) {
        free(*bfr);
        *bfr = NULL;
    }
}

