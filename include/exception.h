/*-
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Argent - infrastructure for building web services
 * Copyright (C) 2020 Abhishek Chakravarti
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact Abhishek Chakravarti at <abhishek@taranjali.org>.
 */


#ifndef __ARGENT_EXCEPTION_H__
#define __ARGENT_EXCEPTION_H__

#ifdef __cplusplus
        extern "C" {
#endif


#include "./erno.h"
#include "./log.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


/*
 * When an exception occurs, we need a way to pass around metadata regarding the
 * error. We encapsulate this metadata in the `ag_exception` struct. We are
 * deliberately choosing a struct instead of an abstract data type in order to
 * avoid relying on the heap.
 */
struct ag_exception {
        ag_erno          erno;  /* Error code.                       */
        const char      *func;  /* Function where error occurred.    */
        const char      *file;  /* File where error occured.         */
        size_t           line;  /* Line number where error occurred. */
};


/*
 * The `ag_exception_handler` callback allows us to handle specific error
 * conditions. Metadata about the the exception is passed through the first
 * parameter, and optional exception data through the second parameter.
 */
typedef void (ag_exception_handler)(const struct ag_exception *, void *);


/*
 * The following structures are used to pass exception-related data to their
 * corresponding exception handlers. The ag_exception_memblock struct holds the
 * exception data related to AG_ERNO_MEMBLOCK, and the ag_exception_regex struct
 * holds exception data related to AG_ERNO_REGEX.
 */


struct ag_exception_memblock {
        size_t  sz;     /* Memory block size      */
        size_t  align;  /* Memory block alignment */
};


struct ag_exception_regex {
        const char      *str;           /* String on which regex is applied */
        const char      *regex;         /* Regex to apply                   */
        int              ecode;         /* Regex interal error code         */
};


/*
 * Define the exception handlers used to handle the errors raised by the Argent
 * Library. ag_exception_memblock_hnd() handles the AG_ERNO_MEMBLOCK exception,
 * and ag_exception_regex_hnd() handles the AG_ERNO_REGEX exception.
 */


extern void     ag_exception_memblock_hnd(const struct ag_exception *, void *);
extern void     ag_exception_regex_hnd(const struct ag_exception *, void *);


/*
 * The exception registry maintains a list of exception messages and handlers
 * associated with each error code of both the Argent Library and client code.
 * The following functions manage the instantiation and destruction of the
 * exception registry, and *must* be called at program startup and termination.
 */


extern void     ag_exception_registry_init(size_t);
extern void     ag_exception_registry_exit(void);

/*
 * The following functions are the accessors for the exception registry,
 * returning the exception message and handler associated with a given error
 * code.
 */


extern const char               *ag_exception_registry_msg(ag_erno);
extern ag_exception_handler     *ag_exception_registry_hnd(ag_erno);


/*
 * The `ag_exception_registry_set()` function is the only mutator for the
 * exception registry, and sets the exception message and handler associated
 * with a given error code. Subsequent calls to this function override the
 * previous values, and passing a NULL pointer for the handler causes the
 * default unhandled exception handler to be set.
 */


extern void     ag_exception_registry_set(ag_erno, const char *,
                    ag_exception_handler *);


/*
 * The AG_ASSERT() macro asserts whether a given predicate is true. This macro
 * is avaiable only in debug builds, and provides a way for both the Argent
 * Library and client code to assert conditions that should *never* be false.
 *
 * AG_ASSERT_PTR() is similar to AG_ASSERT(), except that it is specifically
 * used to assert whether a pointer is valid. AG_ASSERT_PTR() provides a more
 * focused failure message as compared to the generic one given by AG_ASSERT().
 *
 * AG_ASSERT_STR() asserts that a string is valid, i.e., it is a valid pointer
 * and it is not an empty string.
 */


#ifndef NDEBUG
#       define AG_ASSERT(p) do {                                             \
                if (AG_UNLIKELY (!(p))) {                                    \
                        printf("[!] assertion failed: %s [%s(), %s:%d]\n",   \
                            #p, __func__, __FILE__, __LINE__);               \
                        ag_log_debug("assertion failed: %s [%s(), %s:%d]\n", \
                            #p, __func__, __FILE__, __LINE__);               \
                        abort();                                             \
                }                                                            \
        } while (0)


        #define AG_ASSERT_PTR(p) do {                                        \
                if (AG_UNLIKELY (!(p))) {                                    \
                        printf("[!] assertion failed: %s must not be null"   \
                            " [%s(), %s:%d]\n", #p,                          \
                            __func__, __FILE__, __LINE__);                   \
                        ag_log_debug("assertion failed: %s must not be null" \
                            " [%s(), %s:%d]\n", #p,                          \
                             __func__, __FILE__, __LINE__);                  \
                        abort();                                             \
                }                                                            \
        } while (0)


        #define AG_ASSERT_STR(str) do {                                      \
                if (AG_UNLIKELY (!(str && *str))) {                          \
                        printf("[!] assertion failed: string %s must not be" \
                            " null or empty [%s(), %s:%d]\n",                \
                            #str, __func__, __FILE__,   __LINE__);           \
                        ag_log_debug("assertion failed: string %s must not"  \
                            " be null or empty [%s(), %s:%d]\n",             \
                            #str, __func__, __FILE__, __LINE__);             \
                        abort();                                             \
                }                                                            \
        } while (0)
#else
#       define AG_ASSERT(p)
#       define AG_ASSERT_PTR(p)
#       define AG_ASSERT_STR(s)
#endif


/*
 * The AG_REQUIRE() and AG_REQUIRE_OPT() macros are used to check whether a
 * given predicate is true, and if not, signal an appropriate error code.
 *
 * The exception handler associated with the error code is automatically invoked
 * and passed the exception metadata. Before the exception handler is called, we
 * print and log the exception location metadata.
 *
 * AG_REQUIRE_OPT() behaves identically to AG_REQUIRE(), except that it allows
 * optional exception data to be passed along to the exception handler along
 * with the exception metadata.
 */


#define AG_REQUIRE(p, e) do {                                   \
        if (AG_UNLIKELY (!(p))) {                               \
                printf("[!] %d [%s(), %s:%d]: %s\n",            \
                    (e), __func__, __FILE__, __LINE__,          \
                    ag_exception_registry_msg(e));              \
                ag_log_err("%d [%s(), %s:%d]: %s",              \
                    (e), __func__, __FILE__, __LINE__,          \
                    ag_exception_registry_msg(e));              \
                struct ag_exception _x_ = {                     \
                        .erno = (e),                            \
                        .func = __func__,                       \
                        .file = __FILE__,                       \
                        .line = __LINE__                        \
                };                                              \
                ag_exception_registry_hnd((e))(&_x_, NULL);     \
        }                                                       \
} while (0)


#define AG_REQUIRE_OPT(p, e, o) do {                            \
        if (AG_UNLIKELY (!(p))) {                               \
                printf("[!] %d [%s(), %s:%d]: %s\n",            \
                    (e), __func__, __FILE__, __LINE__,          \
                    ag_exception_registry_msg(e));              \
                ag_log_err("%d [%s(), %s:%d]: %s",              \
                    (e), __func__, __FILE__, __LINE__,          \
                    ag_exception_registry_msg(e));              \
                struct ag_exception _x_ = {                     \
                        .erno = (e),                            \
                        .func = __func__,                       \
                        .file = __FILE__,                       \
                        .line = __LINE__                        \
                };                                              \
                ag_exception_registry_hnd((e))(&_x_, (o));      \
        }                                                       \
} while (0)


#ifdef __cplusplus
        }
#endif

#endif /* !__ARGENT_EXCEPTION_H__ */

