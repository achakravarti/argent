/*******************************************************************************
 *                        __   ____   ___  ____  __ _  ____ 
 *                       / _\ (  _ \ / __)(  __)(  ( \(_  _)
 *                      /    \ )   /( (_ \ ) _) /    /  )(  
 *                      \_/\_/(__\_) \___/(____)\_)__) (__)                     
 *
 * Argent Library
 * Copyright (c) 2020 Abhishek Chakravarti <abhishek@taranjali.org>
 *
 * This file is part of the Argent Library. It declares application programming
 * interface of the Argent Library.
 *
 * The contents of this file are released under the GPLv3 License. See the
 * accompanying LICENSE file or the generated Developer Manual (section I:?) for 
 * complete licensing details.
 *
 * BY CONTINUING TO USE AND/OR DISTRIBUTE THIS FILE, YOU ACKNOWLEDGE THAT YOU
 * HAVE UNDERSTOOD THESE LICENSE TERMS AND ACCEPT TO BE LEGALLY BOUND BY THEM.
 ******************************************************************************/


#include <stdio.h>
#include <stdlib.h>




/*******************************************************************************
 *                             COMPILER EXTENSIONS
 */


/*
 *      The ag_pure macro hints that a given function is pure, having no side
 *      effects [DM:2.1]
 */
#if (defined __GNUC__ || defined __clang__)
#   define ag_pure __attribute__((pure))
#else
#   define ag_pure
#   warning "[!] ag_pure not supported by current compiler"
#endif


/*
 *      The ag_hot macro hints that a given function is called frequently
 *      [DM:2.2].
 */
#if (defined __GNUC__ || defined __clang__)
#   define ag_hot __attribute__((hot))
#else
#   define ag_hot
#   warning "[!] ag_hot not supported by current compiler"
#endif


/*
 *      The ag_cold macro hints that a given function is called infrequently
 *      [DM:2.3].
 */
#if (defined __GNUC__ || defined __clang__)
#   define ag_cold __attribute__((cold))
#else
#   define ag_cold
#   warning "[!] ag_cold not supported by current compiler"
#endif


/*
 *      The ag_likely() macro provides a branch prediction hint that a predicate
 *      is likely to be true [DM:2.4]
 */
#if (defined __GNUC__ || defined __clang__)
#   define ag_likely(p) (__builtin_expect(!!(p), 1))
#else
#   define ag_likely(p) (p)
#   warning "[!] ag_likely() not supported by current compiler"
#endif


/*
 *      The ag_unlikely() macro provides a branch prediction hint that a 
 *      predicate is likely to be false [DM:2.5]
 */
#if (defined __GNUC__ || defined __clang__)
#   define ag_unlikely(p) (__builtin_expect(!!(p), 0))
#else
#   define ag_unlikely(p) (p)
#   warning "[!] ag_unlikely not supported by current compiler"
#endif


/*
 *      The ag_threadlocal macro hints that a given variable has thread local
 *      storage [DM:2.6].
 */
#if (defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L)
#   include <threads.h>
#   define ag_threadlocal thread_local
#elif (defined __GNUC__ || defined __clang__)
#   define ag_threadlocal __thread
#else
#    define ag_threadlocal
#    warning "[!] ag_threadlocal not supported by current compiler"
#endif




/*******************************************************************************
 *                             EXCEPTION HANDLING
 */


/*
 *      The ag_erno enumeration holds the error codes of the runtime exceptions 
 *      raised by the Argent Library [DM:??].
 */
enum ag_erno {
    AG_ERNO_NULL = 0x0,
    AG_ERNO_MEMPOOL_NEW,
    AG_ERNO_MEMPOOL_RESIZE,
    AG_ERNO_LEN
};


/*
 *      The ag_exception struct represents the details of a runtime exception
 *      raised by the Argent Library [DM:??].
 */
struct ag_exception {
    enum ag_erno erno;
    const char *func;
    const char *file;
    size_t line;
};


/*
 *      The ag_exception_handler callback function allows client code to handle
 *      runtime exceptions [DM:??].
 */
typedef void (ag_exception_handler)(const struct ag_exception *ex, void *opt);


/*
 *      The ag_exception_message() interface function gets the human-readable
 *      error message corresponding to an error code [DM:??].
 */
extern const char *ag_exception_message(enum ag_erno erno);


/*
 *      The ag_exception_handler_get() interface function gets the thread-local
 *      exception handler [DM:??].
 */
extern ag_exception_handler *ag_exception_handler_get(void);


/*
 *      The ag_exception_handler_set() interface function sets the thread-local
 *      exception handler [DM:??].
 */
extern void ag_exception_handler_set(ag_exception_handler *eh);


/*
 *      The ag_assert() macro performs a runtime assertion that a given 
 *      predicate is true [DM:??].
 */
#if (!defined NDEBUG)
#   define ag_assert(p) do {                                   \
        if (ag_unlikely (!(p))) {                              \
            printf("[!] assertion failed: %s [%s(), %s:%d]\n", \
                    #p, __func__, __FILE__, __LINE__);         \
            abort();                                           \
        }                                                      \
    } while (0)
#else
#   define ag_assert(p)
#endif


/*
 *      The ag_require() macro ensures that a given predicate is true, raising
 *      an exception otherwise [DM:??].
 */
#define ag_require(p, e, opt) do {            \
    if (ag_unlikely (!(p))) {                 \
        struct ag_exception ex = {            \
            .erno = e,                        \
            .func = __func__,                 \
            .file = __FILE__,                 \
            .line = __LINE__                  \
        };                                    \
        ag_exception_handler_get()(&ex, opt); \
    }                                         \
} while (0)




/*******************************************************************************
 *                                MEMORY POOL
 */


/*
 *      The ag_mempool_new() interface function allocates a new block of heap
 *      memory of a given size [DM:??].
 */
extern void *ag_mempool_new(size_t sz);


/*
 *      The ag_mempool_resize() interface function resizes an existing block of
 *      heap memory [DM:??].
 */
extern void ag_mempool_resize(void **bfr, size_t sz);


/*
 *      The ag_mempool_free() interface function releases a previously allocated
 *      block of heap memory [DM:??].
 */
extern void ag_mempool_free(void **bfr);


