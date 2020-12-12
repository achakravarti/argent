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

#if 0
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#endif

typedef int ag_erno;

typedef struct ag_exception ag_exception;

typedef void (ag_exception_handler)(const ag_exception *, void *);

#if 0
typedef void (ag_exception_handler)(ag_erno, const char *, const char *, int,
                void *);

extern void ag_exception_init(void);
extern void ag_exception_exit(void);

extern const char *ag_exception_msg(ag_erno);
extern ag_exception_handler *ag_exception_hnd(ag_erno);

extern void ag_exception_register(ag_erno, const char *,
                ag_exception_handler *);
#endif



extern ag_exception *ag_exception_new(ag_erno, const char *, const char *, int);
extern ag_exception *ag_exception_copy(const ag_exception *);
extern void ag_exception_dispose(ag_exception **);

extern ag_erno ag_exception_erno(const ag_exception *);
extern const char *ag_exception_msg(const ag_exception *);
extern const char *ag_exception_func(const ag_exception *);
extern const char *ag_exception_file(const ag_exception *);
extern int ag_exception_line(const ag_exception *);
extern ag_exception_handler *ag_exception_hnd(const ag_exception *);



extern void ag_exception_registry_init(void);
extern void ag_exception_registry_exit(void);

extern const char *ag_exception_registry_msg(ag_erno);
extern ag_exception_handler *ag_exception_registry_hnd(ag_erno);

extern void ag_exception_registry_set(ag_erno, const char *,
                ag_exception_handler *);








#ifndef NDEBUG
#       define AG_ASSERT(p) do { \
                if (AG_UNLIKELY (!(p))) { \
                        printf("[!] assertion failed: %s [%s(), %s:%d]\n",   \
                                        #p, __func__, __FILE__, __LINE__);   \
                        ag_log_debug("assertion failed: %s [%s(), %s:%d]\n", \
                                        #p, __func__, __FILE__, __LINE__);   \
                        abort();                                             \
                }                                                            \
        } while (0)
#else
#       define AG_ASSERT(p)
#endif



#define AG_REQUIRE(p, e) do {                                                \
        if (AG_UNLIKELY (!(p)))                                              \
                ag_exception *_ex_ = ag_exception_new(e, __func__, __FILE__, \
                                __LINE__);                                   \
                ag_exception_hnd(_ex_)(_ex_, NULL);                          \
} while (0)


#ifdef __cplusplus
        extern "C" {
#endif

#ifdef __cplusplus
        }
#endif

#endif /* !__ARGENT_EXCEPTION_H__ */

