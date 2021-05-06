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

#if (!defined __GNUC__ && !defined __clang__)
#       error "[!] unsupported compiler; use gcc or clang"
#endif

#define AG_AUTO(T)      __attribute__((cleanup(T##_release))) T
#define AG_COLD         __attribute__((cold))
#define AG_HOT          __attribute__((hot))
#define AG_LIKELY(P)    (__builtin_expect(!!(P), 1))
#define AG_NONULL       __attribute__((nonnull))
#define AG_PURE         __attribute__((pure))
#define AG_THREADLOCAL  __thread
#define AG_UNLIKELY(P)  (__builtin_expect(!!(P), 0))


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

extern AG_NONULL void   ag_log_init(const char *);
extern void             ag_log_exit(void);
extern AG_NONULL void   __ag_log_write__(enum ag_log_level, const char *, ...);
extern AG_NONULL void   __ag_log_write_meta__(const char *, const char *, int,
                            enum ag_log_level, const char *, ...);

#define ag_log_emerg(MSG, ...)                                  \
        __ag_log_write_meta__(__func__, __FILE__, __LINE__,     \
            AG_LOG_LEVEL_EMERG, MSG, ##__VA_ARGS__)

#define ag_log_alert(MSG, ...)                                  \
        __ag_log_write_meta__(__func__, __FILE__, __LINE__,     \
            AG_LOG_LEVEL_ALERT, MSG, ##__VA_ARGS__)

#define ag_log_crit(MSG, ...)   \
        __ag_log_write__(AG_LOG_LEVEL_CRIT, MSG, ##__VA_ARGS__)

#define ag_log_err(MSG, ...)                                    \
        __ag_log_write_meta__(__func__, __FILE__, __LINE__,     \
            AG_LOG_LEVEL_ERR, MSG, ##__VA_ARGS__)

#define ag_log_warning(MSG, ...)   \
        __ag_log_write__(AG_LOG_LEVEL_WARNING, MSG, ##__VA_ARGS__)

#define ag_log_notice(MSG, ...)   \
        __ag_log_write__(AG_LOG_LEVEL_NOTICE, MSG, ##__VA_ARGS__)

#define ag_log_info(MSG, ...)   \
        __ag_log_write__(AG_LOG_LEVEL_INFO, MSG, ##__VA_ARGS__)

#ifdef NDEBUG
#       define ag_log_debug(MSG, ...)
#else
#       define ag_log_debug(MSG, ...)                                   \
                __ag_log_write_meta__(__func__, __FILE__, __LINE__,     \
                    AG_LOG_LEVEL_DEBUG, MSG, ##__VA_ARGS__)
#endif


/*******************************************************************************
 *
 */

typedef void    ag_memblock;
typedef char    ag_string;      // forward-declared

extern ag_memblock              *ag_memblock_new(size_t);
extern ag_memblock              *ag_memblock_new_align(size_t, size_t);
extern AG_NONULL ag_memblock    *ag_memblock_copy(const ag_memblock *);
extern AG_NONULL ag_memblock    *ag_memblock_clone(const ag_memblock *);
extern AG_NONULL ag_memblock    *ag_memblock_clone_align(const ag_memblock *, 
                                    size_t);
extern void                      ag_memblock_release(ag_memblock **);
extern AG_NONULL enum ag_cmp     ag_memblock_cmp(const ag_memblock *, 
                                    const ag_memblock *cmp); // 1.
extern AG_NONULL size_t          ag_memblock_sz(const ag_memblock *);
extern AG_NONULL size_t          ag_memblock_sz_total(const ag_memblock *);
extern AG_NONULL size_t          ag_memblock_refc(const ag_memblock *);
extern AG_NONULL bool            ag_memblock_aligned(const ag_memblock *,
                                    size_t);
extern AG_NONULL void            ag_memblock_resize(ag_memblock **, size_t);
extern AG_NONULL void            ag_memblock_resize_align(ag_memblock **,
                                    size_t, size_t);
extern AG_NONULL ag_string      *ag_memblock_str(const ag_memblock *);

AG_NONULL inline bool
ag_memblock_lt(const ag_memblock *ctx, const ag_memblock *cmp)
{
        return ag_memblock_cmp(ctx, cmp) == AG_CMP_LT;
}

AG_NONULL inline bool
ag_memblock_eq(const ag_memblock *ctx, const ag_memblock *cmp)
{
        return ag_memblock_cmp(ctx, cmp) == AG_CMP_EQ;
}

AG_NONULL inline bool
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

