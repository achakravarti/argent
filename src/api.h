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


#include <stdbool.h>
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




/*******************************************************************************
 *                                OBJECT MODEL
 */


/*
 *      The ag_object abstract data type represents a reference-counted
 *      polymorphic object that is the base for all other ADTs of the Argent 
 *      Library [DM:??].
 */
typedef struct ag_object ag_object;


/*
 *      The ag_object_smart macro represents a smart version of an ag_object ADT
 *      that is able to automatically release the heap memory allocated to it
 *      once it goes out of scope [DM:??].
 */
#if (defined __GNUC__ || defined __clang__)
#   define ag_object_smart __attribute__((cleanup(ag_object_free))) ag_object
#else
#   define ag_object_smart ag_object
#   warning "[!] ag_object_smart leaks memory on current compiler"
#endif


/*
 *      The ag_object_cmp enumeration is a tristate representation of the result
 *      of comparing to object instances [DM:??].
 */
enum ag_object_cmp {
    AG_OBJECT_CMP_LT = -1,
    AG_OBJECT_CMP_EQ = 0,
    AG_OBJECT_CMP_GT = 1
};


/*
 *      The ag_object_payload type represents the payload of an object [DM:??].
 */
typedef void ag_object_payload;


/*
 *      The ag_object_vtable_copy type is the callback used to make a deep copy
 *      of the payload of an object [DM:??].
 */
typedef ag_object_payload *(ag_object_method_copy)(
        const ag_object_payload *ctx);


/*
 *      The ag_object_vtable_free type is the callback used to release the
 *      resources allocated to the payload of an object [DM:??].
 */
typedef void (ag_object_method_free)(ag_object_payload *ctx);


/*
 *      The ag_object_vtable_len type is the callback used to determine the
 *      length of the payload of an object [DM:??].
 */
typedef size_t (ag_object_method_len)(const ag_object_payload *ctx);


/*
 *      The ag_object_vtable_cmp type is the callback used to compare two object
 *      instances [DM:??].
 */
typedef enum ag_object_cmp (ag_object_method_cmp)(const ag_object *ctx,
        const ag_object *cmp);


/*
 *      The ag_object_vtable_str type is the callback used to generate the
 *      string representation of an object [DM:??].
 */
typedef const char *(ag_object_method_str)(const ag_object *ctx);


/*
 *      The ag_object_vtable structure is the virtual table of polymorphic
 *      methods of an object [DM:??].
 */
struct ag_object_method {
    ag_object_method_copy *copy;
    ag_object_method_free *free;
    ag_object_method_len *len;
    ag_object_method_cmp *cmp;
    ag_object_method_str *str;
};


/*
 *      The ag_object_new() interface function creates a new instance of an
 *      object [DM:??].
 */
extern ag_hot ag_object *ag_object_new(unsigned id, ag_object_payload *ld,
        const struct ag_object_method *vt);


/*
 *      The ag_object_new_noid() interface function creates a new instance of an
 *      object without an object ID [DM:??].
 */
extern ag_hot ag_object *ag_object_new_noid(ag_object_payload *ld,
        const struct ag_object_method *vt);


/*
 *      The ag_object_copy() interface function creates a lazy copy of an object
 *      [DM:??].
 */
extern ag_hot ag_object *ag_object_copy(const ag_object *ctx);


/*
 *      The ag_object_free() interface function releases the resources allocated
 *      to an object [DM:??].
 */
extern ag_hot void ag_object_free(ag_object **ctx);


/*
 *      The ag_object_id() interface function gets the ID of an object [DM:??].
 */
extern ag_pure unsigned ag_object_id(const ag_object *ctx);


/*
 *      The ag_object_id_set() interface function sets the ID of an object
 *      [DM:??].
 */
extern ag_cold void ag_object_id_set(ag_object **ctx, unsigned id);


/*
 *      The ag_object_hash() interface function gets the hash of an object
 *      [DM:??].
 */
extern ag_pure unsigned ag_object_hash(const ag_object *ctx, size_t len);


/*
 *      The ag_object_len() interface function gets the length of an object
 *      [DM:??].
 */
extern ag_pure size_t ag_object_len(const ag_object *ctx);


/*
 *      The ag_object_cmp() interface function compares one object against
 *      another [DM:??].
 */
extern ag_pure enum ag_object_cmp ag_object_cmp(const ag_object *ctx, 
        const ag_object *cmp);


/*
 *      The ag_object_lt() interface function checks if an object is less than
 *      another [DM:??].
 */
inline ag_pure bool ag_object_lt(const ag_object *ctx, const ag_object *cmp)
{
    ag_assert (ctx && cmp);
    return ag_object_cmp(ctx, cmp) == AG_OBJECT_CMP_LT;
}


/*
 *      The ag_object_eq() interface function checks if an object is equivalent
 *      to another [DM:??].
 */
inline ag_pure bool ag_object_eq(const ag_object *ctx, const ag_object *cmp)
{
    ag_assert (ctx && cmp);
    return ag_object_cmp(ctx, cmp) == AG_OBJECT_CMP_EQ;
}


/*
 *      The ag_object_gt() interface function check is an object is greater than
 *      another [DM:??].
 */
inline ag_pure bool ag_object_gt(const ag_object *ctx, const ag_object *cmp)
{
    ag_assert (ctx && cmp);
    return ag_object_cmp(ctx, cmp) == AG_OBJECT_CMP_GT;
}


/*
 *      The ag_object_payload_hnd() interface function gets a read-only handle
 *      to the payload of an object [DM:??].
 */
extern ag_hot ag_pure const ag_object_payload *ag_object_payload_hnd(
        const ag_object *ctx);


/*
 *      The ag_object_payload_hnd_mutable() interface function gets a read-write
 *      handle to the payload of an object [DM:??].
 */
extern ag_hot ag_object_payload *ag_object_payload_hnd_mutable(ag_object **ctx);


/*
 *      The ag_object_str() interface function generates the string
 *      representation of an object [DM:??].
 */
extern ag_pure const char *ag_object_str(const ag_object *ctx);

