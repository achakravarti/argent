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


/*
 * Declare the string type. The string type of the Argent Library is an alias
 * for the standard char type in order to allow it to be passed to functions
 * that expect standard C-style strings, such as printf(). In the Argent Library
 * we maintain the semantic difference that ag_string instances are dynamically
 * allocated on the heap whereas the standard C-style strings are statically
 * allocated.
 */


typedef char    ag_string;


extern ag_string        *ag_string_new(const char *);
extern ag_string        *ag_string_new_fmt(const char *, ...);
extern ag_string        *ag_string_copy(const ag_string *);
extern void              ag_string_release(ag_string **);


inline ag_string *
ag_string_new_empty(void)
{
    return ag_string_new("");
}


extern enum ag_cmp      ag_string_cmp(const ag_string *,  const char *);


inline bool
ag_string_lt(const ag_string *ctx, const char *cmp)
{
    return ag_string_cmp(ctx, cmp) == AG_CMP_LT;
}


inline bool
ag_string_eq(const ag_string *ctx, const char *cmp)
{
    return ag_string_cmp(ctx, cmp) == AG_CMP_EQ;
}


inline bool
ag_string_gt(const ag_string *ctx, const char *cmp)
{
    return ag_string_cmp(ctx, cmp) == AG_CMP_GT;
}


extern size_t   ag_string_len(const ag_string *);
extern size_t   ag_string_sz(const ag_string *);
extern size_t   ag_string_refc(const ag_string *);
extern bool     ag_string_has(const ag_string *, const char *);



inline bool
ag_string_empty(const ag_string *ctx)
{
        return ag_string_sz(ctx) == 1;
}


extern ag_string        *ag_string_lower(const ag_string *);
extern ag_string        *ag_string_upper(const ag_string *);
extern ag_string        *ag_string_proper(const ag_string *);
extern ag_string        *ag_string_split(const ag_string *, const char *);
extern ag_string        *ag_string_split_right(const ag_string *, const char *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_STRING_H__ */

