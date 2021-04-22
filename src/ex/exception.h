/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Argent---infrastructure for building web services
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
 ******************************************************************************/


#ifndef __ARGENT_INCLUDE_EXCEPTION_H__
#define __ARGENT_INCLUDE_EXCEPTION_H__

#ifdef __cplusplus
        extern "C" {
#endif


#include "./erno.h"
#include "../util/util.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


/*******************************************************************************
 * When an exception occurs, we need a way to pass around metadata regarding the
 * error. We encapsulate this metadata in the `ag_exception` struct. We are
 * deliberately choosing a struct instead of an abstract data type in order to
 * avoid relying on the heap.
 *
 * In addition to `ag_exception`, we have structs holding the details for the
 * specific exceptions handled by the Argent Libray:
 *   - `ag_exception_memblock` for `AG_ERNO_MEMBLOCK`,
 *   - `ag_exception_regex` for `AG_ERNO_REGEX`, and
 *   - `ag_exception_parse` for `AG_ERNO_PARSE`.
 */

struct ag_exception {
        ag_erno          erno;  /* Error code.                       */
        const char      *func;  /* Function where error occurred.    */
        const char      *file;  /* File where error occured.         */
        size_t           line;  /* Line number where error occurred. */
};

struct ag_exception_memblock {
        size_t  sz;     /* Memory block size      */
        size_t  align;  /* Memory block alignment */
};

struct ag_exception_regex {
        const char      *str;           /* String on which regex is applied */
        const char      *regex;         /* Regex to apply                   */
        int              ecode;         /* Regex internal error code        */
};

struct ag_exception_parse {
        const char *str;        /* String being parsed       */
        const char *ctx;        /* Parsing context of string */
};


/*******************************************************************************
 * The `ag_exception_handler` callback allows us to handle specific error
 * conditions. Metadata about the the exception is passed through the first
 * parameter, and optional exception data through the second parameter.
 *
 * The exception handlers used to handle the exceptions raised by the Argent
 * Library are:
 *   - `ag_exception_hnd_memblock()` for `AG_ERNO_MEMBLOCK`,
 *   - `ag_exception_hnd_regex()` for `AG_ERNO_REGEX`,
 *   - `ag_exception_hnd_parse()` for `AG_ERNO_PARSE`.
 *
 * See src/exception-handler.c for more details.
 */

typedef void (ag_exception_hnd)(const struct ag_exception *, void *);

extern void     ag_exception_hnd_memblock(const struct ag_exception *, void *);
extern void     ag_exception_hnd_regex(const struct ag_exception *, void *);
extern void     ag_exception_hnd_parse(const struct ag_exception *, void *);


/*******************************************************************************
 * The exception registry maintains a list of exception messages and handlers
 * associated with each error code of both the Argent Library and client code.
 * The exception registry is implemented as a type-less interface similar to a
 * singleton.
 *
 * See src/exception-registry.c for more details.
 */

extern void              ag_exception_registry_init(void);
extern void              ag_exception_registry_exit(void);
extern const char       *ag_exception_registry_msg(ag_erno);
extern ag_exception_hnd *ag_exception_registry_hnd(ag_erno);
extern void              ag_exception_registry_set(ag_erno, const char *, 
                            ag_exception_hnd *);


/*******************************************************************************
 * The `AG_ASSERT()` macro asserts whether a given predicate is true. This macro
 * is avaiable only in debug builds, and provides a way for both the Argent
 * Library and client code to assert conditions that should *never* be false.
 *
 * `AG_ASSERT_TAG()` is essentially the same as `AG_ASSERT()`, the only
 * difference begin that the former allows a a tag to be specified along with
 * the predicate in order to provide a more meaningful failure message.
 *
 * `AG_ASSERT_PTR()` and `AG_ASSERT_STR()` are specialisations of
 * `AG_ASSERT_TAG()`, checking, respectively, whether a given pointer and string
 * is valid. A pointer is considered to be valid if it is not NULL, and a string
 * is considered to be valid if it is not null and not empty.
 */

#ifndef NDEBUG
#       define AG_ASSERT(p) do {                                             \
                if (AG_UNLIKELY (!(p))) {                                    \
                        printf("[!] assertion failed: %s [%s(), %s:%d]\n",   \
                            #p, __func__, __FILE__, __LINE__);               \
                        ag_log_debug("assertion failed: %s [%s(), %s:%d]\n", \
                            #p, __func__, __FILE__, __LINE__);               \
                        exit(EXIT_FAILURE);                                  \
                }                                                            \
        } while (0)

#       define AG_ASSERT_TAG(t, p) do {                                 \
                if (AG_UNLIKELY (!(p))) {                               \
                        printf("[!] assertion failed: "                 \
                            "%s (%s) [%s(), %s:%d]\n",                  \
                            t, #p, __func__, __FILE__, __LINE__);       \
                        ag_log_debug("assertion failed: "               \
                            "%s (%s) [%s(), %s:%d]\n",                  \
                            t, #p, __func__, __FILE__, __LINE__);       \
                        exit(EXIT_FAILURE);                             \
                }                                                       \
        } while (0)

#       define AG_ASSERT_PTR(p) AG_ASSERT_TAG("PTR_VALID", p)
#       define AG_ASSERT_STR(s) AG_ASSERT_TAG("STR_VALID", s && *s)
#else
#       define AG_ASSERT(p)
#       define AG_ASSERT_TAG(p)
#       define AG_ASSERT_PTR(p)
#       define AG_ASSERT_STR(s)
#endif


/*******************************************************************************
 * The `AG_REQUIRE()` and `AG_REQUIRE_OPT()` macros are used to check whether a
 * given predicate is true, and if not, signal an appropriate error code.
 *
 * The exception handler associated with the error code is automatically invoked
 * and passed the exception metadata. Before the exception handler is called, we
 * print and log the exception location metadata.
 *
 * `AG_REQUIRE_OPT()` behaves identically to AG_REQUIRE(), except that it allows
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

#endif /* !__ARGENT_INCLUDE_EXCEPTION_H__ */

