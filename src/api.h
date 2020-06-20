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
#include <stddef.h>
#include <stdint.h>
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
    AG_ERNO_MEMBLOCK_NEW,
    AG_ERNO_MEMBLOCK_RESIZE,
    AG_ERNO_TEST,
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
 *                                 MEMORY BLOCK
 */

                                            /* block of heap memory [AgDM:??] */
typedef void ag_memblock_t;

                                             /* allocate heap block [AgDM:??] */
extern ag_memblock_t *ag_memblock_new(size_t sz);

                                               /* resize heap block [AgDM:??] */
extern void ag_memblock_resize(ag_memblock_t **bfr, size_t sz);

                                       /* free allocated heap block [AgDM:??] */
extern void ag_memblock_free(ag_memblock_t **bfr);



/*******************************************************************************
 *                                OBJECT MODEL
 */


                                        /* base object for all ADTs [AgDM:??] */
typedef struct ag_object_t ag_object_t;


                                      /* smart version of ag_object [AgDM:??] */
#if (defined __GNUC__ || defined __clang__)
#   define ag_object_smart_t \
            __attribute__((cleanup(ag_object_dispose))) ag_object_t
#else
#   define ag_object_smart_t ag_object_t
#   warning "[!] ag_object_smart_t leaks memory on current compiler"
#endif


                                           /* reserved object types [AgDM:??] */
#define AG_OBJECT_TYPE_OBJECT ((size_t) 0x0)
#define AG_OBJECT_TYPE_LIST ((size_t) 0x1)


                        /* tristate result of comparing two objects [AgDM:??] */
enum ag_object_cmp {
    AG_OBJECT_CMP_LT = -1,
    AG_OBJECT_CMP_EQ = 0,
    AG_OBJECT_CMP_GT = 1
};


                                       /* v-table of object methods [AgDM:??] */
struct ag_object_vtable {
    ag_memblock_t *(*copy)(const ag_memblock_t *payload);
    void (*dispose)(ag_memblock_t *payload);
    size_t (*sz)(const ag_memblock_t *payload);
    size_t (*len)(const ag_memblock_t *payload);
    size_t (*hash)(const ag_object_t *payload);
    enum ag_object_cmp (*cmp)(const ag_object_t *lhs, const ag_object_t *rhs);
    const char *(*str)(const ag_object_t *obj);
};


                                       /* initialises object system [AgDM:??] */
extern void ag_object_init(size_t len);


                                        /* shuts down object system [AgDM:??] */
extern void ag_object_exit(void);


                                                /* registers object [AgDM:??] */
extern void ag_object_register(size_t type, const struct ag_object_vtable *vt);


                                              /* creates new object [AgDM:??] */
extern ag_hot ag_object_t *ag_object_new(size_t type, size_t id, 
        ag_memblock_t *payload);


                                   /* creates new object without ID [AgDM:??] */
extern ag_hot ag_object_t *ag_object_new_noid(size_t type, 
        ag_memblock_t *payload);


                                                  /* copies object [AgDM:??] */
extern ag_hot ag_object_t *ag_object_copy(const ag_object_t *ctx);


                                                 /* disposes object [AgDM:??] */
extern ag_hot void ag_object_dispose(ag_object_t **ctx);


                                                /* gets object type [AgDM:??] */
extern ag_pure size_t ag_object_type(const ag_object_t *ctx);


                                                  /* gets object ID [AgDM:??] */
extern ag_pure size_t ag_object_id(const ag_object_t *ctx);


                                                  /* sets object ID [AgDM:??] */
extern ag_cold void ag_object_id_set(ag_object_t **ctx, size_t id);


                                             /* gets hash of object [AgDM:??] */
extern ag_pure size_t ag_object_hash(const ag_object_t *ctx);


                                                /* gets object size [AgDM:??] */
extern ag_pure size_t ag_object_sz(const ag_object_t *ctx);


                                              /* gets object length [AgDM:??] */
extern ag_pure size_t ag_object_len(const ag_object_t *ctx);


                                       /* checks if object is empty [AgDM:??] */
inline bool ag_object_empty(const ag_object_t *ctx)
{
    return !ag_object_len(ctx);
}


                                            /* compares two objects [AgDM:??] */
extern ag_pure enum ag_object_cmp ag_object_cmp(const ag_object_t *ctx, 
        const ag_object_t *cmp);


                           /* checks if object is less than another [AgDM:??] */
inline bool ag_object_lt(const ag_object_t *ctx, const ag_object_t *cmp)
{
    return ag_object_cmp(ctx, cmp) == AG_OBJECT_CMP_LT;
}


                       /* checks if object is equivalent to another [AgDM:??] */
inline bool ag_object_eq(const ag_object_t *ctx, const ag_object_t *cmp)
{
    return ag_object_cmp(ctx, cmp) == AG_OBJECT_CMP_EQ;
}


                        /* checks if object is greater than another [AgDM:??] */
inline bool ag_object_gt(const ag_object_t *ctx, const ag_object_t *cmp)
{
    return ag_object_cmp(ctx, cmp) == AG_OBJECT_CMP_GT;
}


                         /* gets read-only handle to object payload [AgDM:??] */
extern ag_hot ag_pure const ag_memblock_t *ag_object_payload(
        const ag_object_t *ctx);


                        /* gets read-write handle to object payload [AgDM:??] */
extern ag_hot ag_memblock_t *ag_object_payload_mutable(ag_object_t **ctx);


                            /* gets string representation of object [AgDM:??] */
extern ag_pure const char *ag_object_str(const ag_object_t *ctx);




/*******************************************************************************
 *                              SINGLY LINKED LIST
 */


                                   /* singly linked list of objects [AgDM:??] */
typedef ag_object_t ag_list_t;


                                 /* smart version of ag_list [AgDM:??] */
#define ag_list_smart_t ag_object_smart_t


                                                  /* registers list [AgDM:??] */
extern void ag_list_register(void);


                                                /* creates new list [AgDM:??] */
extern ag_list_t *ag_list_new(void);


                                                     /* copies list [AgDM:??] */
inline ag_list_t *ag_list_copy(const ag_list_t *ctx)
{
    return ag_object_copy(ctx);
}


                                                   /* disposes list [AgDM:??] */
inline void ag_list_dispose(ag_list_t **ctx)
{
    ag_object_dispose(ctx);
}


                                        /* gets object type of list [AgDM:??] */
inline size_t ag_list_type(const ag_list_t *ctx)
{
    return ag_object_type(ctx);
}


                                          /* gets object ID of list [AgDM:??] */
inline size_t ag_list_id(const ag_list_t *ctx)
{
    return ag_object_id(ctx);
}


                                          /* sets object ID of list [AgDM:??] */
inline void ag_list_id_set(ag_list_t **ctx, size_t id)
{
    ag_object_id_set(ctx, id);
}


                                               /* gets hash of list [AgDM:??] */
inline size_t ag_list_hash(const ag_list_t *ctx)
{
    return ag_object_hash(ctx);
}


                            /* gets sum of all object sizes in list [AgDM:??] */
inline size_t ag_list_sz(const ag_list_t *ctx)
{
    return ag_object_sz(ctx);
}


                                            /* gets number of objects in list */
inline size_t ag_list_len(const ag_list_t *ctx)
{
    return ag_object_len(ctx);
}


                                         /* checks if list is empty [AgDM:??] */
inline bool ag_list_empty(const ag_list_t *ctx)
{
    return ag_object_empty(ctx);
}


                                              /* compares two lists [AgDM:??] */
inline enum ag_object_cmp ag_list_cmp(const ag_list_t *ctx, 
        const ag_list_t *cmp)
{
    return ag_object_cmp(ctx, cmp);
}


                             /* checks if list is less than another [AgDM:??] */
inline bool ag_list_lt(const ag_list_t *ctx, const ag_list_t*cmp)
{
    return ag_object_lt(ctx, cmp);
}


                         /* checks if list is equivalent to another [AgDM:??] */
inline bool ag_list_eq(const ag_list_t *ctx, const ag_list_t *cmp)
{
    return ag_object_eq(ctx, cmp);
}


                          /* checks if list is greater than another [AgDM:??] */
inline bool ag_list_gt(const ag_list_t *ctx, const ag_list_t *cmp)
{
    return ag_object_gt(ctx, cmp);
}


                              /* gets string representation of list [AgDM:??] */
inline const char *ag_list_str(const ag_object_t *ctx)
{
    return ag_object_str(ctx);
}

                                /* starts internal iterator of list [AgDM:??] */
extern void ag_list_start(ag_list_t **ctx);


                                /* moves list iterator to next node [AgDM:??] */
extern bool ag_list_next(ag_list_t **ctx);


                  /* gets object at currently iterated node of list [AgDM:??] */
extern ag_object_t *ag_list_get(const ag_list_t *ctx);


                            /* gets object at 1-based index of list [AgDM:??] */
extern ag_object_t *ag_list_get_at(const ag_list_t *ctx, size_t idx);


                  /* sets object at currently iterated node of list [AgDM:??] */
extern void ag_list_set(ag_list_t **ctx, const ag_object_t *val);


                            /* sets object at 1-based index of list [AgDM:??] */
extern void ag_list_set_at(ag_list_t **ctx, size_t idx, const ag_object_t *val);


                                    /* pushes object to end of list [AgDM:??] */
extern void ag_list_push(ag_list_t **ctx, const ag_object_t *val);


                                  /* iterates through nodes of list [AgDM:??] */
extern void ag_list_map(const ag_list_t *ctx, void (*cbk)(
            const ag_object_t *node, void *opt), void *opt);


                          /* iterates mutably through nodes of list [AgDM:??] */
extern void ag_list_map_mutable(ag_list_t **ctx, void (*cbk)(ag_object_t **node,
        void *opt), void *opt);

