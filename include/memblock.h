#if (!defined __ARGENT_MEMBLOCK_H__)
#define __ARGENT_MEMBLOCK_H__


#include <stddef.h>

/*******************************************************************************
 *                                 MEMORY BLOCK
 *
 * The Argent Library makes heavy use of the heap memory, and so an interface
 * for managing heap memory is essential. We had initially considered using the
 * Boehm Garbage Collector, but decided against it, favouring reference counting
 * instead; see the documentation for the Object System for further details.
 *
 * The traditional approach to memory management in C code is to use `malloc()`
 * and friends. However, there are some limitations to this approach for the
 * Argent Library, since in some situations we require alignment guarantees (as
 * in the case of the Value System). In order to do so, the Memory Management
 * interface relies on non-standard extensions supported by the GNU and BSD
 * `libc`-s.
 *
 * Although the Memory Management interface does not support garbage collection,
 * semi-automatic cleanup of dynamically allocated heap memory is often
 * desirable. To achieve this, the interfaces of the Argent Library often define
 * a "smart" type that is a macro around the GCC (and Clang) built-in extension
 * `__attribute__((cleanup))`.
 */


/*
 * Type    : `ag_memblock_t`
 * Synopsis: block of heap memory.
 *
 * The `ag_memblock_t` type represents a block of memory allocated on the heap.
 * This type has been introduced to distinguish it from other pointers, and
 * thereby improve code readability.
 *
 * The conventional approach is to typedef `void *`, but we have deliberately
 * typedef'd to `void` instead so that it is necessary to declare an
 * `ag_memblock_t` type with a `*`; again, this has been done to improve code
 * readability.
 *
 * It is important to remember that `ag_memblock_t` values are not released
 * automatically; it is up to the client code to ensure that they are released
 * by passing them to `ag_memblock_free()`. If semi-automatic release is
 * required, consider declaring this type with the GCC built-in extension
 * `__attribute__((cleanup))`; for an example, see how the `ag_string_smart_t`
 * type has been declared below.
 */
typedef void ag_memblock_t;


/*
 * Function: `ag_memblock_new()`
 * See     : argent/src/memblock.c
 */
extern ag_memblock_t *ag_memblock_new(size_t sz);

   
/*
 * Function: `ag_memblock_copy()`
 * See     : argent/src/memblock.c
 */
extern ag_memblock_t *ag_memblock_copy(const ag_memblock_t *bfr);


/*
 * Function: `ag_memblock_sz()`
 * See     : argent/src/memblock.c
 */
extern size_t ag_memblock_sz(const ag_memblock_t *bfr);


/*
 * Function: `ag_memblock_resize()`
 * See     : argent/src/memblock.c
 */
extern void ag_memblock_resize(ag_memblock_t **bfr, size_t sz);


/*
 * Function: `ag_memblock_free()`
 * See     : argent/src/memblock.c
 */
extern void ag_memblock_free(ag_memblock_t **bfr);

#endif /* !defined __ARGENT_MEMBLOCK_H__ */

