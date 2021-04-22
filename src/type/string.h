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


#ifndef __ARGENT_INCLUDE_STRING_H__
#define __ARGENT_INCLUDE_STRING_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "../base/base.h"
#include "./primitives.h"


/*
 * Declare the string type. The string type of the Argent Library is an alias
 * for the standard char type in order to allow it to be passed to functions
 * that expect standard C-style strings, such as printf(). 
 *
 * In the Argent Library we maintain the semantic difference that ag_string
 * instances are dynamically allocated on the heap whereas the standard C-style
 * strings are statically allocated. So please keep in mind that whenever we
 * refer to string instances we mean the dynamically allocated form.
 *
 * Another point to note is that string instances are immutable; this is by
 * design in order to gain the benefit of immutable types.
 */


typedef char    ag_string;


/*
 * Declare the prototypes of the string manager interface. This string manager
 * interface consists of four functions to manage the creation and destruction
 * of string instances. 
 *
 * ag_string_new() creates a new string instance from a statically allocated
 * C-style string, and. ag_string_new_fmt() creates a new string instances from
 * formatted string. ag_string_copy() creates a shallow copy of a string, and
 * ag_string_clone() creates a deep copy. String instances are released through
 * ag_string_release().
 */


extern ag_string        *ag_string_new(const char *);
extern ag_string        *ag_string_new_fmt(const char *, ...);
extern ag_string        *ag_string_copy(const ag_string *);
extern ag_string        *ag_string_clone(const ag_string *);
extern void              ag_string_release(ag_string **);


inline ag_string *
ag_string_new_empty(void)
{
    return ag_string_new("");
}


/*
 * Declare the prototypes for the string comparison interface. The most
 * important comparison function is ag_string_cmp(), the other three simply
 * being convenience wrappers around the former.
 *
 * All four comparison functions compare a dynamic string against another string
 * which may either be dynamic or static. ag_string_cmp() returns an ag_cmp enum
 * value indicating the result of the comparison; ag_string_lt(), ag_string_eq()
 * and ag_string_gt() return a Boolean value indicating whether or not the
 * contextual string is, respectively, less than, equal to, or greater than the
 * comparison string.
 */

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


/*
 * Declare the prototypes for the string accessor functions. ag_string_len()
 * determines the lexicograpchical length of a string, ag_string_sz() gets the
 * size in bytes of a string, ag_string_refc() returns the current reference
 * count of a string, ag_string_has() checks whether a string contains a given
 * substring, and ag_string_match() checks whether a string matches a given
 * POSIX-style regular expression. ag_string_empty() checks whether a string is
 * empty, i.e., whether its length is zero.
 */


extern size_t   ag_string_len(const ag_string *);
extern size_t   ag_string_sz(const ag_string *);
extern size_t   ag_string_refc(const ag_string *);
extern bool     ag_string_has(const ag_string *, const char *);
extern bool     ag_string_match(const ag_string *, const char *);
extern bool     ag_string_url_encoded(const ag_string *);


inline bool
ag_string_empty(const ag_string *ctx)
{
        return ag_string_sz(ctx) == 1;
}


/*
 * Declare the prototypes for the string mutator functionsx. ag_string_lower(),
 * ag_string_upper() and ag_string_proper() return, respectively, the lowercase,
 * uppercase and proper case forms of a given string. ag_string_split() and
 * ag_string_split_right() split a string around a given pivot and return,
 * respectively, the left hand and right sides.
 */


extern ag_string        *ag_string_lower(const ag_string *);
extern ag_string        *ag_string_upper(const ag_string *);
extern ag_string        *ag_string_proper(const ag_string *);
extern ag_string        *ag_string_split(const ag_string *, const char *);
extern ag_string        *ag_string_split_right(const ag_string *, const char *);
extern ag_string        *ag_string_url_encode(const ag_string *);
extern ag_string        *ag_string_url_decode(const ag_string *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_INCLUDE_STRING_H__ */

