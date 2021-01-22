/*-
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
 */


#ifndef __ARGENT_INCLUDE_MEMBLOCK_H__
#define __ARGENT_INCLUDE_MEMBLOCK_H__

#ifdef __cplusplus
        extern "C" {
#endif


#include "./string.h"


typedef void ag_memblock;

extern ag_memblock *ag_memblock_new(size_t);
extern ag_memblock *ag_memblock_new_align(size_t, size_t);
extern ag_memblock *ag_memblock_copy(const ag_memblock *);
extern ag_memblock *ag_memblock_clone(const ag_memblock *);
extern ag_memblock *ag_memblock_clone_align(const ag_memblock *, size_t);
extern void ag_memblock_release(ag_memblock **);

// warning: don't use with structs containing non-scalar members
// if not equal, comparison based on first differing byte
extern enum ag_cmp ag_memblock_cmp(const ag_memblock *, const ag_memblock *cmp);

inline bool ag_memblock_lt(const ag_memblock *ctx, const ag_memblock *cmp)
{
        return ag_memblock_cmp(ctx, cmp) == AG_CMP_LT;
}

inline bool ag_memblock_eq(const ag_memblock *ctx, const ag_memblock *cmp)
{
        return ag_memblock_cmp(ctx, cmp) == AG_CMP_EQ;
}

inline bool ag_memblock_gt(const ag_memblock *ctx, const ag_memblock *cmp)
{
        return ag_memblock_cmp(ctx, cmp) == AG_CMP_GT;
}

extern size_t ag_memblock_sz(const ag_memblock *);
extern size_t ag_memblock_sz_total(const ag_memblock *);
extern size_t ag_memblock_refc(const ag_memblock *);
extern bool ag_memblock_aligned(const ag_memblock *, size_t);

extern void ag_memblock_resize(ag_memblock **, size_t);
extern void ag_memblock_resize_align(ag_memblock **, size_t, size_t);
extern ag_string *ag_memblock_str(const ag_memblock *);


#ifdef __cplusplus
        }
#endif

#endif /* !__ARGENT_INCLUDE_MEMBLOCK_H__ */

