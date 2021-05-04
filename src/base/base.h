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


#ifndef __ARGENT_BASE_H_INCLUDED__
#define __ARGENT_BASE_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>


/*******************************************************************************
 *
 */

#if (defined __GNUC__ || defined __clang__)
#       define AG_AUTO(t) __attribute__((cleanup(t##_release))) t
#       define AG_COLD __attribute__((cold))
#       define AG_HOT __attribute__((hot))
#       define AG_LIKELY(p) (__builtin_expect(!!(p), 1))
#       define AG_PURE __attribute__((pure))
#       define AG_THREADLOCAL __thread
#       define AG_UNLIKELY(p) (__builtin_expect(!!(p), 0))
#elif (defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L \
    && !defined __STDC_NO_TRHEADS__)
#       include <threads.h>
#       define AG_THREADLOCAL thread_local
#else
#       define AG_AUTO(t) t
#       define AG_COLD
#       define AG_HOT
#       define AG_LIKELY(p) (p)
#       define AG_PURE
#       define AG_THREADLOCAL
#       define AG_UNLIKELY(p) (p)
#       warning "[!] AG_AUTO() not supported on current compiler"
#       warning "[!] AG_COLD not supported by current compiler"
#       warning "[!] AG_HOT not supported by current compiler"
#       warning "[!] AG_LIKELY() not supported by current compiler"
#       warning "[!] AG_PURE not supported by current compiler"
#       warning "[!] AG_THREADLOCAL not supported by current compiler"
#       warning "[!] AG_UNLIKELY() not supported by current compiler"
#endif


/*******************************************************************************
 * We often need to compare two entities, and the conventional approach is that
 * of `strcmp()` and `memcmp()`. We maintain this approach, but opt to use
 * enumerators instead for better readability.
 */

enum ag_cmp {
        AG_CMP_LT = -1, /* lhs < rhs  */
        AG_CMP_EQ,      /* lhs == rhs */
        AG_CMP_GT       /* lhs > rhs  */
};


/*******************************************************************************
 *
 */

enum ag_log_level {
        AG_LOG_LEVEL_EMERG = 0,
        AG_LOG_LEVEL_ALERT,
        AG_LOG_LEVEL_CRIT,
        AG_LOG_LEVEL_ERR,
        AG_LOG_LEVEL_WARNING,
        AG_LOG_LEVEL_NOTICE,
        AG_LOG_LEVEL_INFO,
        AG_LOG_LEVEL_DEBUG
};

extern void     ag_log_init(const char *);
extern void     ag_log_exit(void);
extern void     ag_log_write(enum ag_log_level, const char *, ...);
extern void     ag_log_emerg(const char *, ...);
extern void     ag_log_alert(const char *, ...);
extern void     ag_log_crit(const char *, ...);
extern void     ag_log_err(const char *, ...);
extern void     ag_log_warning(const char *, ...);
extern void     ag_log_notice(const char *, ...);
extern void     ag_log_info(const char *, ...);

#ifdef NDEBUG
#       define ag_log_debug(MSG, ...)
#else
        extern void     __ag_log_debug__(const char *, const char *, int,
                            const char *, ...);
#       define ag_log_debug(MSG, ...)   \
                __ag_log_debug__(__func__, __FILE__, __LINE__, MSG,     \
                    ##__VA_ARGS__)
#endif


/*******************************************************************************
 *
 */

typedef void    ag_memblock;
typedef char    ag_string;      // forward-declared

extern ag_memblock      *ag_memblock_new(size_t);
extern ag_memblock      *ag_memblock_new_align(size_t, size_t);
extern ag_memblock      *ag_memblock_copy(const ag_memblock *);
extern ag_memblock      *ag_memblock_clone(const ag_memblock *);
extern ag_memblock      *ag_memblock_clone_align(const ag_memblock *, size_t);
extern void              ag_memblock_release(ag_memblock **);
extern enum ag_cmp       ag_memblock_cmp(const ag_memblock *, 
                            const ag_memblock *cmp); // 1.
extern size_t            ag_memblock_sz(const ag_memblock *);
extern size_t            ag_memblock_sz_total(const ag_memblock *);
extern size_t            ag_memblock_refc(const ag_memblock *);
extern bool              ag_memblock_aligned(const ag_memblock *, size_t);
extern void              ag_memblock_resize(ag_memblock **, size_t);
extern void              ag_memblock_resize_align(ag_memblock **, size_t, size_t);
extern ag_string        *ag_memblock_str(const ag_memblock *);

inline bool
ag_memblock_lt(const ag_memblock *ctx, const ag_memblock *cmp)
{
        return ag_memblock_cmp(ctx, cmp) == AG_CMP_LT;
}

inline bool
ag_memblock_eq(const ag_memblock *ctx, const ag_memblock *cmp)
{
        return ag_memblock_cmp(ctx, cmp) == AG_CMP_EQ;
}

inline bool
ag_memblock_gt(const ag_memblock *ctx, const ag_memblock *cmp)
{
        return ag_memblock_cmp(ctx, cmp) == AG_CMP_GT;
}

//1. warning: don't use with structs containing non-scalar members
// if not equal, comparison based on first differing byte


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_BASE_H_INCLUDED__ */

