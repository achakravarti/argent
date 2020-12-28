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


#ifndef __ARGENT_STRING_H__
#define __ARGENT_STRING_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "./argent.h"


typedef char ag_str;


extern ag_str *ag_str_new(const char *);
extern ag_str *ag_str_new_fmt(const char *, ...);
extern ag_str *ag_str_copy(const ag_str *);
extern void ag_str_release(ag_str **);

inline ag_str *ag_str_new_empty(void)
{
    return ag_str_new("");
}


extern enum ag_cmp ag_str_cmp(const ag_str *,  const char *);

inline bool ag_str_lt(const ag_str *ctx, const char *cmp)
{
    return ag_str_cmp(ctx, cmp) == AG_CMP_LT;
}

inline bool ag_str_eq(const ag_str *ctx, const char *cmp)
{
    return ag_str_cmp(ctx, cmp) == AG_CMP_EQ;
}

inline bool ag_str_gt(const ag_str *ctx, const char *cmp)
{
    return ag_str_cmp(ctx, cmp) == AG_CMP_GT;
}


extern size_t ag_str_len(const ag_str *);
extern size_t ag_str_sz(const ag_str *);
extern size_t ag_str_refc(const ag_str *);
extern bool ag_str_has(const ag_str *, const char *);

inline bool ag_str_empty(const ag_str *ctx)
{
        return ag_str_sz(ctx) == 1;
}



extern ag_str *ag_str_lower(const ag_str *);
extern ag_str *ag_str_upper(const ag_str *);
extern ag_str *ag_str_proper(const ag_str *);
extern ag_str *ag_str_split(const ag_str *, const char *);
extern ag_str *ag_str_split_right(const ag_str *, const char *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_STRING_H__ */

