/*******************************************************************************
 *                        __   ____   ___  ____  __ _  ____ 
 *                       / _\ (  _ \ / __)(  __)(  ( \(_  _)
 *                      /    \ )   /( (_ \ ) _) /    /  )(  
 *                      \_/\_/(__\_) \___/(____)\_)__) (__)                     
 *
 * Argent Library
 * Copyright (c) 2020 Abhishek Chakravarti <abhishek@taranjali.org>
 *
 * This file is part of the Argent Library. It implements the memory pool
 * interface of the Argent Library.
 *
 * The contents of this file are released under the GPLv3 License. See the
 * accompanying LICENSE file or the generated Developer Manual (section I:?) for 
 * complete licensing details.
 *
 * BY CONTINUING TO USE AND/OR DISTRIBUTE THIS FILE, YOU ACKNOWLEDGE THAT YOU
 * HAVE UNDERSTOOD THESE LICENSE TERMS AND ACCEPT TO BE LEGALLY BOUND BY THEM.
 ******************************************************************************/


#include <string.h>
#include "./api.h"




/*******************************************************************************
 *                          INTERFACE IMPLEMENTATION
 */


/*
 *      Implementation of the ag_mempool_new() interface function [DM:??].
 */
extern void *ag_mempool_new(size_t sz)
{
    ag_assert (sz);
    void *bfr = malloc(sz);
    ag_require (bfr, AG_ERNO_MEMPOOL_NEW, NULL);

    memset(bfr, 0, sz);
    return bfr;
}


/*
 *      Implementation of the ag_mempool_resize() interface function [DM:??].
 */
extern void ag_mempool_resize(void **bfr, size_t sz)
{
    ag_assert (bfr && *bfr && sz);
    *bfr = realloc(*bfr, sz);
    ag_require (*bfr, AG_ERNO_MEMPOOL_RESIZE, NULL);
}


/*
 *      Implementaiton of the ag_mempool_free() interface function [DM:??].
 */
extern void ag_mempool_free(void **bfr)
{
    if (ag_likely (bfr && *bfr)) {
        free(*bfr);
        *bfr = NULL;
    }
}

