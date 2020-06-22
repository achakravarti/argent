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


                             /* implementation of ag_memblock_new() [AgDM:??] */
extern ag_memblock_t *ag_memblock_new(size_t sz)
{
    ag_assert (sz);
    void *bfr = malloc(sz);
    ag_require (bfr, AG_ERNO_MEMBLOCK_NEW, NULL);

    memset(bfr, 0, sz);
    return bfr;
}


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

