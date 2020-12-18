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


#include "../include/argent.h"

#include <ctype.h>
#include <string.h>
#include <stdarg.h>


/*
 * The following macros are helpers for the assertion checks. We use these
 * macros to improve readability when debugging. is_string_valid() checks
 * whether a given string is not a null pointer, and is_string_not_empty()
 * checks whether a string is not null and not empty.
 */
#ifndef NDEBUG
#       define is_string_valid(s) (s)
#       define is_string_not_empty(s) (s && *s)
#endif


/* Declare the public inline functions of the string interface. */
extern inline ag_str *ag_str_new_empty(void);
extern inline bool ag_str_lt(const char *, const char *);
extern inline bool ag_str_eq(const char *, const char *);
extern inline bool ag_str_gt(const char *, const char *);
extern inline bool ag_str_empty(const ag_str *);


/* 
 * ag_str_new() creates a new instance of a dynamic string from a statically
 * allocated string.
 */
extern ag_str *ag_str_new(const char *src)
{
        AG_ASSERT (is_string_valid(src));

        size_t sz = strlen(src);
        char *s = ag_mblock_new(sz + 1);

        strncpy(s, src, sz);
        s[sz] = '\0';
        return s;
}


/*
 * ag_str_new_fmt() creates a new instance of a dynamic string from a statically
 * allocated format string with variable arguments a la printf().
 */
extern ag_str *ag_str_new_fmt(const char *fmt, ...)
{
        AG_ASSERT (is_string_not_empty(fmt));

        va_list args;
        va_start(args, fmt);
        char *bfr = ag_mblock_new(vsnprintf(NULL, 0, fmt, args));
        va_end(args);

        va_start(args, fmt);
        (void)vsprintf(bfr, fmt, args);
        va_end(args);

        char *s = ag_str_new(bfr);
        ag_mblock_release((ag_mblock **)&bfr);
        return s;
}


/* ag_str_copy() creates a shallow copy of a dynamic string. */
extern ag_str *ag_str_copy(const ag_str *ctx)
{
        AG_ASSERT (is_string_valid(ctx));

        ag_str *cp = (ag_str *)ctx;
        ag_mblock_retain(cp);
        return cp;
}


/* ag_str_release() releases a dynamic string. */
extern void ag_str_release(ag_str **ctx)
{
        ag_mblock_release((ag_mblock **)ctx);
}


/* ag_str_cmp() compares two strings lexicographically. */
extern enum ag_cmp ag_str_cmp(const char *ctx,  const char *cmp)
{
        AG_ASSERT (is_string_valid(ctx));
        AG_ASSERT (is_string_valid(cmp));

        return strcmp(ctx, cmp);
}


/* ag_str_has() checks whether a string contains a particular substring. */
extern bool ag_str_has(const ag_str *ctx, const char *tgt)
{
        AG_ASSERT (is_string_valid(ctx));
        AG_ASSERT (is_string_not_empty(tgt));

        return strstr(ctx, tgt);
}


/* 
 * ag_str_len() determines the lexicographcical length of a string, taking into
 * consideration that the string may contain non-ASCII UTF-8 characters.
 */
extern size_t ag_str_len(const ag_str *ctx)
{
        AG_ASSERT (is_string_valid(ctx));

        register size_t i = 0, len = 0;

        while (ctx[i]) {
                if ((ctx[i] & 0xC0) != 0x80)
                        len++;
                
                i++;
        }

        return len;
}


/*
 * ag_str_sz() gets the size in bytes of a dynamic string. Since dynamic strings
 * are allocated through memory blocks, we can retrieve their size by querying
 * ag_mblock_sz().
 */
extern size_t ag_str_sz(const ag_str *ctx)
{
        AG_ASSERT (is_string_valid(ctx));

        return ag_mblock_sz(ctx);
}


/*
 * ag_str_refc() gets the reference count of a dynamic string. Again, as in the
 * case of ag_str_sz(), we use the memory block interface to do so.
 */
extern size_t ag_str_refc(const ag_str *ctx)
{
        AG_ASSERT (is_string_valid(ctx));

        return ag_mblock_refc(ctx);
}


/*
 * ag_str_lower() transforms a string to lowercase. Since we have chosen to keep
 * strings as immutable, we return a new string instance after processing.
 */
extern ag_str *ag_str_lower(const char *ctx)
{
        AG_ASSERT (is_string_valid(ctx));

        ag_str *s = ag_str_new(ctx);
        register char *c = s;

        while (*c) {
                *c = tolower(*c);
                c++;
        }

        return s;
}


/*
 * ag_str_upper() transforms a string to uppercase. As in the case of
 * ag_str_lower(), we choose to return a new instance instead of modifying the
 * original string.
 */
extern ag_str *ag_str_upper(const char *ctx)
{
        AG_ASSERT (is_string_valid(ctx));
        
        ag_str *s = ag_str_new(ctx);
        register char *c = s;

        while (*c) {
                *c = toupper(*c);
                c++;
        }

        return s;
}


/* 
 * ag_str_proper() transforms a string to proper case. In proper case, we
 * capitalise a character if:
 *   - it is the first character,
 *   - it is preceded by a space, or
 *   - it is preceded by a period.
 *
 * As in the case of ag_str_lower() and ag_str_upper(), we choose to return a
 * new string instance after processing.
 */
extern ag_str *ag_str_proper(const char *ctx)
{
        AG_ASSERT (is_string_valid(ctx));
        
        ag_str *s = ag_str_new(ctx);
        register size_t sz = ag_mblock_sz(s);

        for (register size_t i = 0; i < sz; i++) {
                s[i] = (!i || s[i - 1] == ' ' || s[i - 1] == '.')
                        ? toupper(s[i]) : tolower(s[i]);
        }

        return s;
}


/*
 * ag_str_split() splits a string around a pivot, returning the left side of the
 * pivot. In case the pivot isn't found, then an empty string is returned. We
 * use the reentrant version of strtok() in order to be thread-safe.
 */
extern ag_str *ag_str_split(const char *ctx, const char *pvt)
{
        AG_ASSERT (is_string_valid(ctx));
        AG_ASSERT (is_string_not_empty(pvt));

        if (AG_UNLIKELY (!(*ctx && strstr(ctx, pvt))))
                return ag_str_new_empty();

        char *save;
        ag_str_auto *s = ag_str_new(ctx);
        return ag_str_new(strtok_r(s, pvt, &save));
}


/*
 * ag_str_split_right() splits a string around a pivot and returns the substring
 * on the right side of the pivot. As in the case of ag_str_split(), in case the
 * pivot doesn't exist then an empty string is returned.
 */
extern ag_str *ag_str_split_right(const char *ctx, const char *pvt)
{
        AG_ASSERT (is_string_valid(ctx));
        AG_ASSERT (is_string_not_empty(pvt));

        if (AG_UNLIKELY (!(*ctx && strstr(ctx, pvt))))
                return ag_str_new_empty();

        char *save;
        ag_str_auto *s = ag_str_new(ctx);
        (void)strtok_r(s, pvt, &save);
        return ag_str_new(save);
}

