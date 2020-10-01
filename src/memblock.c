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


/*
 * `ag_memblock_copy()` makes a deep copy of an existing block of heap memory,
 * passed as a handle to its only parameter `bfr`. `bfr` is required to be
 * valid; this is asserted in debug guilds. Additionally, it is required for
 * `bfr` to have been allocated in the heap by an earlier call to
 * `ag_memblock_new()` (or `ag_memblock_copy()` itself); passing a handle to a
 * block of memory on the stack is an error, leading to undefined behaviour 
 * (most likely a segmentation fault).
 *
 * On successful completion, `ag_memblock_copy()` returns a handle to the newly
 * allocated copy of `bfr`. In case of failure to do so, the
 * `AG_ERNO_MEMBLOCK_NEW` exception is raised, and control is passed to the
 * client-provided exception handler. Since in the normal flow of control the
 * newly allocated copy is guaranteed to be valid, there is no need for client
 * code to make a further check on the return value.
 *
 * Since `ag_memblock_copy()` relies on a non-standard `libc` call, it is
 * important to remember the portability restrictions this entails; see
 * `ag_memblock_sz()` for more details.
 */
extern ag_memblock_t *ag_memblock_copy(const ag_memblock_t *bfr)
{
    ag_assert (bfr);
    size_t sz = ag_memblock_sz(bfr);

    ag_memblock_t *cp = ag_memblock_new(sz);
    memcpy(cp, bfr, sz);

    return cp;
}


/*
 * Sometimes it is useful to be able to determine the size of a block of heap
 * memory that was allocated earlier without having to resort to storing the
 * size in a separate variable. The interface function `ag_memblock_sz()`
 * provides a way to do os, allowing client code to query the size of a block of
 * heap memory `bfr` that was allocated by either `ag_memblock_new()` or
 * `ag_memblock_copy()`.
 *
 * `ag_memblock_sz()` requires that `bfr` be a valid handle to a block of heap
 * memory; this function asserts in debug builds that the handle is not `NULL`.
 * Passing a pointer to the stack memory is an error, and will result in
 * undefined behavour, most likely leading to a segmentation fault.
 *
 * As in the case of `ag_memblock_new()`, it is important to remember that the
 * size returned by `ag_memblock_sz()` may be greater than that which had been
 * originally requested at the time of allocation, since the underlying
 * `malloc()` call needs to take into account alignment and size restrictions.
 *
 * There is an important caveat regarding portability. The C standard does not
 * define a way to query the size of the heap block returned by `malloc()`, and
 * so we need to rely on the functionality provided by the underlying `libc`
 * implementation. As of the current version, `ag_memblock_sz()` is guaranteed
 * to work on GNU/Linux, Cygwin and the various BSDs.
 */
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

