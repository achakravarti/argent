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

typedef int ag_erno;

typedef void (ag_exception_handler)(ag_erno, const char *, const char *, size_t,
                void *);

extern void ag_exception_init(void);
extern void ag_exception_exit(void);
extern void ag_exception_register(ag_erno, const char *,
                ag_exception_handler *);

extern const char *ag_exception_msg(ag_erno);
extern ag_exception_handler *ag_exception_hnd(ag_erno);

#ifndef NDEBUG
#       define AG_ASSERT(p) do { \
                if (AG_UNLIKELY (!(p))) { \
                        printf("[!] assertion failed: %s [%s(), %s:%d]\n, "  \
                                        #p, __func__, __FILE__, __LINE__);   \
                        ag_log_debug("assertion failed: %s [%s(), %s:%d]\n", \
                                        #p, __func__, __FILE__, __LINE__);   \
                        abort();                                             \
                }                                                            \
        } while (0)
#else
#       define AG_ASSERT(p)
#endif

#ifdef __cplusplus
        extern "C" {
#endif

#ifdef __cplusplus
        }
#endif

#endif /* !__ARGENT_EXCEPTION_H__ */

