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
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>




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


enum ag_tristate {
    AG_TRISTATE_LO = -1,
    AG_TRISTATE_GND,
    AG_TRISTATE_HI
};


/*******************************************************************************
 *                                   LOGGING
 */

                                       /* logs an emergency message [AgDM:??] */
extern void ag_log_emerg(const char *fmt, ...);

                                           /* logs an alert message [AgDM:??] */
extern void ag_log_alert(const char *fmt, ...);

                                         /* logs a critical message [AgDM:??] */
extern void ag_log_crit(const char *fmt, ...);

                                           /* logs an error message [AgDM:??] */
extern void ag_log_err(const char *fmt, ...);

                                          /* logs a warning message [AgDM:??] */
extern void ag_log_warning(const char *fmt, ...);

                                           /* logs a notice message [AgDM:??] */
extern void ag_log_notice(const char *fmt, ...);

                                      /* logs an information message [AgDM:??] */
extern void ag_log_info(const char *fmt, ...);

                                            /* logs a debug message [AgDM:??] */
extern void ag_log_debug(const char *fmt, ...);


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
    AG_ERNO_HTTP_INIT,
    AG_ERNO_HTTP_PARAM,
    AG_ERNO_HTTP_FILE,
    AG_ERNO_HTTP_METHOD,
    AG_ERNO_HTTP_TYPE,
    AG_ERNO_LEN,
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
            ag_log_err("assertion failed: %s [%s(), %s:%d]\n", \
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




/*******************************************************************************
 *                                   STRINGS
 */

                                                    /* UTF-8 string [AgDM:??] */
typedef char ag_string_t;

                                    /* smart version of ag_string_t [AgDM:??] */
#if (defined __GNUC__ || defined __clang__)
#   define ag_string_smart_t \
            __attribute__((cleanup(ag_string_dispose))) ag_string_t
#else
#   define ag_string_smart_t ag_string_t
#   warning "[!] ag_string_smart_t leaks memory on current compiler"
#endif

                                               /* create new string [AgDM:??] */
extern ag_string_t *ag_string_new(const char *cstr);

                                         /* create new empty string [AgDM:??] */
inline ag_string_t *ag_string_new_empty(void)
{
    return ag_string_new("");
}

                                     /* create new formatted string [AgDM:??] */
extern ag_string_t *ag_string_new_fmt(const char *fmt, ...);

                                            /* copy existing string [AgDM:??] */
extern ag_string_t *ag_string_copy(const ag_string_t *ctx);

                                         /* dispose existing string [AgDM:??] */
extern void ag_string_dispose(ag_string_t **ctx);

                              /* get lexicographic length of string [AgDM:??] */
extern size_t ag_string_len(const ag_string_t *ctx);

                                     /* get size in bytes of string [AgDM:??] */
extern size_t ag_string_sz(const ag_string_t *ctx);

                                    /* compare two string instances [AgDM:??] */
extern enum ag_tristate ag_string_cmp(const ag_string_t *lhs, const char *rhs);

                            /* check if string is less than another [AgDM:??] */
inline bool ag_string_lt(const ag_string_t *lhs, const char *rhs)
{
    return ag_string_cmp(lhs, rhs) == AG_TRISTATE_LO;
}

                             /* check if string is equal to another [AgDM:??] */
inline bool ag_string_eq(const ag_string_t *lhs, const char *rhs)
{
    return ag_string_cmp(lhs, rhs) == AG_TRISTATE_GND;
}

                         /* check if string is greater than another [AgDM:??] */
inline bool ag_string_gt(const ag_string_t *lhs, const char *rhs)
{
    return ag_string_cmp(lhs, rhs) == AG_TRISTATE_HI;
}

                                           /* add string to another [AgDM:??] */
extern void ag_string_add(ag_string_t **ctx, const ag_string_t *cat);

                                 /* add C-string to string instance [AgDM:??] */
inline void ag_string_add_cstr(ag_string_t **ctx, const char *cat)
{
    ag_string_smart_t *s = ag_string_new(cat);
    ag_string_add(ctx, s);
}

extern void ag_string_lower(ag_string_t **ctx);

extern void ag_string_upper(ag_string_t **ctx);



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


                                       /* v-table of object methods [AgDM:??] */
struct ag_object_vtable {
    ag_memblock_t *(*copy)(const ag_memblock_t *payload);
    void (*dispose)(ag_memblock_t *payload);
    size_t (*id)(const ag_object_t *obj);
    size_t (*sz)(const ag_object_t *obj);
    size_t (*len)(const ag_object_t *obj);
    size_t (*hash)(const ag_object_t *obj);
    enum ag_tristate (*cmp)(const ag_object_t *lhs, const ag_object_t *rhs);
    ag_string_t *(*str)(const ag_object_t *obj);
};


                                       /* initialises object system [AgDM:??] */
extern void ag_object_init(size_t len);


                                        /* shuts down object system [AgDM:??] */
extern void ag_object_exit(void);


                                                /* registers object [AgDM:??] */
extern void ag_object_register(size_t type, const struct ag_object_vtable *vt);


                                              /* creates new object [AgDM:??] */
extern ag_hot ag_object_t *ag_object_new(size_t type, ag_memblock_t *payload);


                                                  /* copies object [AgDM:??] */
extern ag_hot ag_object_t *ag_object_copy(const ag_object_t *ctx);


                                                 /* disposes object [AgDM:??] */
extern ag_hot void ag_object_dispose(ag_object_t **ctx);


                                                /* gets object type [AgDM:??] */
extern ag_pure size_t ag_object_type(const ag_object_t *ctx);


                                     /* gets object reference count [AgDM:??] */
extern size_t ag_object_refc(const ag_object_t *ctx);


                                                  /* gets object ID [AgDM:??] */
extern ag_pure size_t ag_object_id(const ag_object_t *ctx);


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
extern ag_pure enum ag_tristate ag_object_cmp(const ag_object_t *ctx, 
        const ag_object_t *cmp);


                           /* checks if object is less than another [AgDM:??] */
inline bool ag_object_lt(const ag_object_t *ctx, const ag_object_t *cmp)
{
    return ag_object_cmp(ctx, cmp) == AG_TRISTATE_LO;
}


                       /* checks if object is equivalent to another [AgDM:??] */
inline bool ag_object_eq(const ag_object_t *ctx, const ag_object_t *cmp)
{
    return ag_object_cmp(ctx, cmp) == AG_TRISTATE_GND;
}


                        /* checks if object is greater than another [AgDM:??] */
inline bool ag_object_gt(const ag_object_t *ctx, const ag_object_t *cmp)
{
    return ag_object_cmp(ctx, cmp) == AG_TRISTATE_HI;
}


                         /* gets read-only handle to object payload [AgDM:??] */
extern ag_hot ag_pure const ag_memblock_t *ag_object_payload(
        const ag_object_t *ctx);


                        /* gets read-write handle to object payload [AgDM:??] */
extern ag_hot ag_memblock_t *ag_object_payload_mutable(ag_object_t **ctx);


                            /* gets string representation of object [AgDM:??] */
extern ag_string_t *ag_object_str(const ag_object_t *ctx);


/*******************************************************************************
 *                             DYNAMIC TYPE SYSTEM
 */

typedef intptr_t ag_int;
typedef uintptr_t ag_uint;
typedef double ag_float;

#define AG_UINT_MIN ((ag_uint)0)
#define AG_UINT_MAX UINTPTR_MAX

#define AG_INT_MIN ((ag_int)(-(AG_UINT_MAX / (ag_uint)2)))
#define AG_INT_MAX ((ag_int)((AG_UINT_MAX / (ag_uint)2) - (ag_uint)1))

#define AG_FLOAT_MIN DBL_MIN
#define AG_FLOAT_MAX DBL_MAX


enum ag_value_type {
    AG_VALUE_TYPE_OBJECT = 0, /* 0b000 */
    AG_VALUE_TYPE_UINT = 1, /* 0b001 */
    AG_VALUE_TYPE_STRING = 2, /* 0b010 */
    AG_VALUE_TYPE_FLOAT = 4, /* 0b100 */
    AG_VALUE_TYPE_INT = 6 /* 0b110 */
};

typedef void ag_value_t;

#if (defined __GNUC__ || defined __clang__)
#   define ag_value_smart_t \
            __attribute__((cleanup(ag_value_dispose))) ag_value_t
#else
#   define ag_value_smart_t ag_value_t
#   warning "[!] ag_value_smart_t leaks memory on current compiler"
#endif

inline bool ag_int_lt(ag_int ctx, ag_int cmp)
{
    return ctx < cmp;
}

inline bool ag_int_eq(ag_int ctx, ag_int cmp)
{
    return ctx == cmp;
}

inline bool ag_int_gt(ag_int ctx, ag_int cmp)
{
    return ctx > cmp;
}

inline enum ag_tristate ag_int_cmp(ag_int ctx, ag_int cmp)
{
    if (ctx == cmp)
        return AG_TRISTATE_GND;

    return ctx < cmp ? AG_TRISTATE_LO: AG_TRISTATE_HI;
}

inline bool ag_uint_lt(ag_uint ctx, ag_uint cmp)
{
    return ctx < cmp;
}

inline bool ag_uint_eq(ag_uint ctx, ag_uint cmp)
{
    return ctx == cmp;
}

inline bool ag_uint_gt(ag_uint ctx, ag_uint cmp)
{
    return ctx > cmp;
}

inline enum ag_tristate ag_uint_cmp(ag_uint ctx, ag_uint cmp)
{
    if (ctx == cmp)
        return AG_TRISTATE_GND;

    return ctx < cmp ? AG_TRISTATE_LO: AG_TRISTATE_HI;
}

inline bool ag_float_lt(ag_float ctx, ag_float cmp)
{
    /* https://stackoverflow.com/questions/17333 */
    return (cmp - ctx) > ((fabs(ctx) < fabs(cmp) 
        ? fabs(cmp) : fabs(ctx)) * DBL_EPSILON);
}

inline bool ag_float_eq(ag_float ctx, ag_float cmp)
{
    /* https://stackoverflow.com/questions/17333 */
    return fabs(ctx - cmp) <= ((fabs(ctx) > fabs(cmp) 
        ? fabs(cmp) : fabs(ctx)) * DBL_EPSILON);
}

inline bool ag_float_gt(ag_float ctx, ag_float cmp)
{
    /* https://stackoverflow.com/questions/17333 */
    return (ctx - cmp) > ( (fabs(ctx) < fabs(cmp) 
        ? fabs(cmp) : fabs(ctx)) * DBL_EPSILON);
}

inline enum ag_tristate ag_float_cmp(ag_float ctx, ag_float cmp)
{
    if (ag_float_eq(ctx, cmp))
        return AG_TRISTATE_GND;

    return ag_float_lt(ctx, cmp) ? AG_TRISTATE_LO: AG_TRISTATE_HI;
}

extern ag_value_t *ag_value_new_int(ag_int val);

extern ag_value_t *ag_value_new_uint(ag_uint val);

extern ag_value_t *ag_value_new_float(ag_float val);

extern ag_value_t *ag_value_new_string(const ag_string_t *val);

extern ag_value_t *ag_value_new_object(const ag_object_t *val);

extern ag_value_t *ag_value_copy(const ag_value_t *ctx);

extern void ag_value_dispose(ag_value_t **ctx);

extern enum ag_tristate ag_value_cmp(const ag_value_t *ctx, 
    const ag_value_t *cmp);

inline bool ag_value_lt(const ag_value_t *ctx, const ag_value_t *cmp)
{
    return ag_value_cmp(ctx, cmp) == AG_TRISTATE_LO;
}

inline bool ag_value_eq(const ag_value_t *ctx, const ag_value_t *cmp)
{
    return ag_value_cmp(ctx, cmp) == AG_TRISTATE_GND;
}

inline bool ag_value_gt(const ag_value_t *ctx, const ag_value_t *cmp)
{
    return ag_value_cmp(ctx, cmp) == AG_TRISTATE_HI;
}

extern enum ag_value_type ag_value_type(const ag_value_t *ctx);

inline bool ag_value_is_int(const ag_value_t *ctx)
{
    return ag_value_type(ctx) == AG_VALUE_TYPE_INT;
}

inline bool ag_value_is_uint(const ag_value_t *ctx)
{
    return ag_value_type(ctx) == AG_VALUE_TYPE_UINT;
}

inline bool ag_value_is_float(const ag_value_t *ctx)
{
    return ag_value_type(ctx) == AG_VALUE_TYPE_FLOAT;
}

inline bool ag_value_is_string(const ag_value_t *ctx)
{
    return ag_value_type(ctx) == AG_VALUE_TYPE_STRING;
}

inline bool ag_value_is_object(const ag_value_t *ctx)
{
    return ag_value_type(ctx) == AG_VALUE_TYPE_OBJECT;
}

extern ag_int ag_value_int(const ag_value_t *ctx);

extern ag_uint ag_value_uint(const ag_value_t *ctx);

extern ag_float ag_value_float(const ag_value_t *ctx);

extern ag_string_t *ag_value_string(const ag_value_t *ctx);

extern ag_object_t *ag_value_object(const ag_value_t *ctx);


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
inline enum ag_tristate ag_list_cmp(const ag_list_t *ctx, 
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


/*******************************************************************************
 *                                   FAST CGI
 */
typedef void (ag_http_handler)(void);

enum ag_http_method {
    AG_HTTP_METHOD_GET,
    AG_HTTP_METHOD_POST,
    AG_HTTP_METHOD_PUT,
    AG_HTTP_METHOD_PATCH,
    AG_HTTP_METHOD_DELETE,
    __AG_HTTP_METHOD_LEN,
};


// https://restfulapi.net/http-status-codes/
enum ag_http_status {
    AG_HTTP_STATUS_200_OK,
    AG_HTTP_STATUS_201_CREATED,
    AG_HTTP_STATUS_202_ACCEPTED,
    AG_HTTP_STATUS_204_NO_CONTENT,
    AG_HTTP_STATUS_301_MOVED_PERMANENTLY,
    AG_HTTP_STATUS_302_FOUND,
    AG_HTTP_STATUS_303_SEE_OTHER,
    AG_HTTP_STATUS_304_NOT_MODIFIED,
    AG_HTTP_STATUS_307_TEMPORARY_REDIRECT,
    AG_HTTP_STATUS_400_BAD_REQUEST,
    AG_HTTP_STATUS_401_UNAUTHORIZED,
    AG_HTTP_STATUS_403_FORBIDDEN,
    AG_HTTP_STATUS_404_NOT_FOUND,
    AG_HTTP_STATUS_405_METHOD_NOT_ALLOWED,
    AG_HTTP_STATUS_406_NOT_ACCEPTABLE,
    AG_HTTP_STATUS_412_PRECONDITION_FAILED,
    AG_HTTP_STATUS_415_UNSUPPORTED_MEDIA_TYPE,
    AG_HTTP_STATUS_500_INTERNAL_SERVER_ERROR,
    AG_HTTP_STATUS_501_NOT_IMPLEMENTED,
    __AG_HTTP_STATUS_LEN,
};


// https://github.com/cujojs/rest/blob/master/docs/mime.md
// https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
enum ag_http_mime {
    AG_HTTP_MIME_APPLICATION_FORM,
    AG_HTTP_MIME_APPLICATION_JSON,
    AG_HTTP_MIME_APPLICATION_OCTET,
    AG_HTTP_MIME_APPLICATION_XML,
    AG_HTTP_MIME_MULTIPART_FORM,
    AG_HTTP_MIME_TEXT_CSS,
    AG_HTTP_MIME_TEXT_CSV,
    AG_HTTP_MIME_TEXT_HTML,
    AG_HTTP_MIME_TEXT_JS,
    AG_HTTP_MIME_TEXT_PLAIN,
    AG_HTTP_MIME_TEXT_XML,
    __AG_HTTP_MIME_LEN,
};


extern void ag_http_init(void);
extern void ag_http_exit(void);
extern void ag_http_register(ag_http_handler *req);
extern void ag_http_run(void);
extern ag_string_t *ag_http_param(const char *key);
extern void ag_http_respond(enum ag_http_mime type, enum ag_http_status code,
        const char *fmt, ...);
extern void ag_http_respond_file(enum ag_http_mime type,
        enum ag_http_status code, const char *fpath);


extern enum ag_http_method ag_http_request_method(void);
extern enum ag_http_mime ag_http_request_type(void);
extern ag_string_t *ag_http_request_referer(void);

extern ag_string_t *ag_http_request_user_agent(void);
extern ag_string_t *ag_http_request_user_ip(void);
extern ag_string_t *ag_http_request_user_host(void);
extern ag_string_t *ag_http_request_user_port(void);

extern bool ag_http_request_url_secure(void);
extern ag_string_t *ag_http_request_url_host(void);
extern ag_string_t *ag_http_request_url_port(void);
extern ag_string_t *ag_http_request_url_path(void);

extern void ag_http_response_string(enum ag_http_mime type, 
        enum ag_http_status code, const ag_string_t *str);

