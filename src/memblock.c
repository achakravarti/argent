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

#if 0
#include <malloc.h>
#include <string.h>
#include "./api.h"




/*******************************************************************************
 *                           INTERFACE IMPLEMENTATION
 */


/*
 * Function  : `ag_memblock_new()`
 * Synopsis  : Allocates block of heap memory.
 * Parameters: `sz` - size in bytes to allocate.
 * Return    : Allocated block of heap memory.
 *
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
 * Function  :  `ag_memblock_copy()`
 * Synopsis  : Copies existing block of heap memory.
 * Parameters: `bfr` - heap block to copy.
 * Return    : Copied block of heap memory.
 *
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
 * Function  : `ag_memblock_sz()`
 * Synopsis  : Gets size of block of heap memory.
 * Parameters: `bfr` - heap block to query.
 * Return    : size in bytes of heap block.
 *
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


/* 
 * Function  : `ag_memblock_resize()`
 * Synopsis  : Resizes allocated block of heap memory.
 * Parameters: `bfr` - heap block to resize,
 *             `sz` - new size in bytes.
 *
 * The `ag_memblock_resize()` function is responsible for resizing the amount of
 * memory allocated to an existing block of heap memory. Although this function
 * is not used commonly within the Argent Library itself, it may prove to be
 * useful for client code, especially for the resizing of dynamic data
 * structures that can grow and shrink, such as vectors.
 *
 * `ag_memblock_resize()` takes two arguments, the first being the handle to the
 * heap buffer `bfr` that is to be allocated, and the second being the new size
 * `sz` value. `bfr` is expected to be a valid pointer to a pointer to a memory
 * block, and `sz` is expected to be greater than zero; these two conditions are
 * asserted in debug builds.
 *
 * If successful, the heap memory block pointed to by `bfr` will be resized to
 * the value specified by \texttt{sz}. Note that, as in the case of
 * `ag_memblock_new()`, the actual size of the heap buffer may be larger than
 * that requested in order to account for alignment and minimum size
 * constraints. In case the resizing operation failed, the `AG_MEMBLOCK_RESIZE`
 * exception is raised. Since ag_memblock_resize() is guaranteed to either
 * succeed, or to raise an exception, there is no need to test whether the heap
 * buffer pointed to by `bfr` is valid after the operation.
 */
extern void ag_memblock_resize(ag_memblock_t **bfr, size_t sz)
{
    ag_assert (bfr && *bfr && sz);
    *bfr = realloc(*bfr, sz);
    ag_require (*bfr, AG_ERNO_MEMBLOCK_RESIZE, NULL);
}


/*
 * Function  : `ag_memblock_free()`
 * Synopsis  : Releases allocated block of heap memory.
 * Parameters: `bfr` - heap block to release.
 *
 * After `ag_memblock_new()`, the `ag_memblock_free() function is perhaps the
 * most important one in the memory module of the Argent Library.
 * `ag_memblock_free()` is responsible for releasing the heap memory that has
 * been allocated through either `ag_memblock_new()` or `ag_memblock_copy()`.
 * 
 * `ag_memblock_free()` accepts only one argument `bfr`, which is a pointer to
 * the heap memory block that has been allocated earlier. If both `bfr` and the
 * heap memory block pointed by it are valid (i.e.  non-`NULL`, then the latter
 * is released and the former is set to `NULL`.  This ensures that `bfr` is not
 * a dangling pointer after the memory block it points to has been released.
 * 
 * `ag_memblock_free()` is designed to be robust, and performs a safe no-op if
 * either `bfr` or the heap memory block pointed to by it are invalid (i.e.
 * `NULL`. This allows `ag_memblock_free()` to be safely called even in
 * exception conditions. However, it is important to remember that passing a
 * `bfr` to `ag_memblock_free()` that has not been created by either
 * `ag_memblock_new()` or ag_memblock_copy()` will result in undefined
 * behaviour.
 */
extern void ag_memblock_free(ag_memblock_t **bfr)
{
    if (ag_likely (bfr && *bfr)) {
        free(*bfr);
        *bfr = NULL;
    }
}
#endif
