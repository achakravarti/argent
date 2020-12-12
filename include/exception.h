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

#include <stddef.h>

#ifdef __cplusplus
        extern "C" {
#endif


typedef int ag_erno;
#define AG_ERNO_NULL ((ag_erno) 0)

struct ag_exception {
        ag_erno erno;
        const char *func;
        const char *file;
        size_t line;
};

typedef void (ag_exception_handler)(const struct ag_exception *, void *);


extern void ag_exception_registry_init(size_t);
extern void ag_exception_registry_exit(void);

extern const char *ag_exception_registry_msg(ag_erno);
extern ag_exception_handler *ag_exception_registry_hnd(ag_erno);

extern void ag_exception_registry_set(ag_erno, const char *,
                ag_exception_handler *);


#ifndef NDEBUG
#       define AG_ASSERT(p) do {                                             \
                if (AG_UNLIKELY (!(p))) {                                    \
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



#define AG_REQUIRE(p, e) do {                                   \
        if (AG_UNLIKELY (!(p))) {                               \
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

