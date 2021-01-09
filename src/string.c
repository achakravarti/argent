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
#include <regex.h>
#include <string.h>
#include <stdarg.h>



/*
 * Declare the public inline functions of the string interface.
 */


extern inline ag_string *ag_string_new_empty(void);
extern inline bool       ag_string_lt(const ag_string *, const char *);
extern inline bool       ag_string_eq(const ag_string *, const char *);
extern inline bool       ag_string_gt(const ag_string *, const char *);
extern inline bool       ag_string_empty(const ag_string *);


/*
 * Define the ag_string_new() interface function. This function creates a new
 * instance of a dynamic string from a statically allocated string.
 */


extern ag_string *
ag_string_new(const char *src)
{
        AG_ASSERT_PTR (src);

        size_t sz = strlen(src);
        char *s = ag_memblock_new(sz + 1);

        strncpy(s, src, sz);
        s[sz] = '\0';
        return (s);
}


/*
 * Define the ag_string_new_fmt() interface function. This function creates a
 * new instance of a dynamic string from a statically allocated format string
 * with variable arguments a la printf(). By passing NULL and 0 as the first two
 * arguments to vsnprintf() we determine the size of the formatted string
 * (excluding the terminating null character).
 */


extern ag_string *
ag_string_new_fmt(const char *fmt, ...)
{
        AG_ASSERT_STR (fmt);

        va_list args;
        va_start(args, fmt);
        char *bfr = ag_memblock_new(vsnprintf(NULL, 0, fmt, args) + 1);
        va_end(args);

        va_start(args, fmt);
        (void)vsprintf(bfr, fmt, args);
        va_end(args);

        char *s = ag_string_new(bfr);
        ag_memblock_release((ag_memblock **)&bfr);
        return (s);
}


/*
 * Define the ag_string_copy() interface function. This function creates a
 * shallow copy of a dynamic string.
 */


extern ag_string *
ag_string_copy(const ag_string *ctx)
{
        AG_ASSERT_PTR (ctx);

        return (ag_memblock_copy(ctx));
}


/*
 * Define the ag_string_clone() interface function. This function creates a deep
 * copy of a string instance.
 */


extern ag_string *
ag_string_clone(const ag_string *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_memblock_clone(ctx);
}


/*
 * Define the ag_string_release() interface function. This function releases a
 * dynamic string. We don't cast ctx to (void **) when calling
 * ag_memblock_release() in order to avoid potential portability issues in case
 * the size of pointers differ. See the C-FAQ List question 4.9 at
 * http://c-faq.com/ptrs/genericpp.html.
 */


extern void
ag_string_release(ag_string **ctx)
{
        if (AG_LIKELY (ctx && *ctx)) {
                ag_string *hnd = *ctx;
                void *ptr = hnd;
                ag_memblock_release(&ptr);
                *ctx = ptr;
        }
}


/*
 * Define the ag_string_cmp() interface function. This function compares two
 * strings lexicographically. We have adapted the code from the uf8cmp()
 * function in Sheredom's UTF-8 header only library (see
 * https://github.com/sheredom/utf8.h/blob/master/utf8.h). We could have simply
 * used strcmp(), but there may be edge cases in Unicode strings which strcmp()
 * doesn't handle.
 */


extern enum ag_cmp
ag_string_cmp(const ag_string *ctx,  const char *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);

        if (!*ctx && *cmp)
                return (AG_CMP_LT);

        if (*ctx && !*cmp)
                return (AG_CMP_GT);

        register const unsigned char *s1 = (const unsigned char *)ctx;
        register const unsigned char *s2 = (const unsigned char *)cmp;

        while (*s1 || *s2) {
                if (*s1 < *s2)
                        return (AG_CMP_LT);
                else if (*s1 > *s2)
                        return (AG_CMP_GT);

                s1++;
                s2++;
        }

        return (AG_CMP_EQ);
}


/*
 * Define the ag_string_has() interface function. This function checks whether a
 * string contains a particular substring.
 */


extern bool
ag_string_has(const ag_string *ctx, const char *tgt)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (tgt);

        if (AG_UNLIKELY (!*tgt && *ctx))
                return false;

        return strstr(ctx, tgt);
}


extern bool
ag_string_match(const ag_string *ctx, const char *regex)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (regex);

        if (AG_UNLIKELY (!(*ctx && *regex)))
                return false;

        regex_t r;
        int rc = regcomp(&r, regex, 0);

        struct ag_exception_regex x = {.str = ctx, .regex = regex};
        AG_REQUIRE_OPT (!rc, AG_ERNO_REGEX, &x);

        rc = regexec(&r, ctx, 0, NULL, 0);
        AG_REQUIRE_OPT (!rc || rc == REG_NOMATCH, AG_ERNO_REGEX, &x);

        regfree(&r);
        return !rc;
}


/*
 * Define the ag_string_len() interface function. This function determines the
 * lexicographcical length of a string, taking into consideration that the
 * string may contain non-ASCII UTF-8 characters.
 */


extern size_t
ag_string_len(const ag_string *ctx)
{
        AG_ASSERT_PTR (ctx);

        register size_t i = 0, len = 0;

        while (ctx[i]) {
                if ((ctx[i] & 0xC0) != 0x80)
                        len++;

                i++;
        }

        return len;
}


/*
 * Define the ag_string_sz() interface function. This function gets the size in
 * bytes of a dynamic string. Since dynamic strings are allocated through memory
 * blocks, we can retrieve their size by querying ag_memblock_sz().
 */


extern size_t
ag_string_sz(const ag_string *ctx)
{
        AG_ASSERT_PTR (ctx);

        return (ag_memblock_sz(ctx));
}


/*
 * Define the ag_string_refc() interface function. This function gets the
 * reference count of a dynamic string. Again, as in the case of ag_string_sz(),
 * we use the memory block interface to do so.
 */


extern size_t
ag_string_refc(const ag_string *ctx)
{
        AG_ASSERT_PTR (ctx);

        return (ag_memblock_refc(ctx));
}


/*
 * Define the ag_string_lower() interface function. This function transforms a
 * string to lowercase. Since we have chosen to keep strings as immutable, we
 * return a new string instance after processing.  Since we rely on tolower(),
 * this function isn't guaranteed to work correctly with Unicode strings.
 *
 * TODO: make ag_string_lower() Unicode-safe.
 */


extern ag_string *
ag_string_lower(const ag_string *ctx)
{
        AG_ASSERT_PTR (ctx);

        ag_string *s = ag_memblock_clone(ctx);
        register size_t sz = ag_memblock_sz(ctx);

        for (register size_t i = 0; i < sz; i++)
                s[i] = tolower(s[i]);

        return (s);
}


/*
 * Define the ag_string_upper() interface function. This function transforms a
 * string to uppercase. As in the case of ag_string_lower(), we choose to return
 * a new instance instead of modifying the original string. Again, as in the
 * case of ag_string_lower(), this function isn't guaranteed to be Unicode-safe.
 *
 * TODO: make ag_string_upper() Unicode-safe.
 */


extern ag_string *
ag_string_upper(const ag_string *ctx)
{
        AG_ASSERT_PTR (ctx);

        ag_string *s = ag_memblock_clone(ctx);
        register size_t sz = ag_memblock_sz(ctx);

        for (register size_t i = 0; i < sz; i++)
                s[i] = toupper(s[i]);

        return (s);
}


/*
 * Define the ag_string_proper() interface function. This function transforms a
 * string to proper case. In proper case, we capitalise a character if:
 *   - it is the first character,
 *   - it is preceded by a space, or
 *   - it is preceded by a period.
 *
 * As in the case of ag_string_lower() and ag_string_upper(), we choose to
 * return a new string instance after processing. And like both these functions,
 * this one isn't Unicode-safe.
 *
 * TODO: make ag_string_proper() Unicode-safe.
 */


extern ag_string *
ag_string_proper(const ag_string *ctx)
{
        AG_ASSERT_PTR (ctx);

        ag_string *s = ag_memblock_clone(ctx);
        register size_t sz = ag_memblock_sz(s);

        for (register size_t i = 0; i < sz; i++) {
                s[i] = (!i || s[i - 1] == ' ' || s[i - 1] == '.')
                        ? toupper(s[i]) : tolower(s[i]);
        }

        return (s);
}


/*
 * Define the ag_string_split() interface function. This function splits a
 * string around a pivot, returning the left side of the pivot. In case the
 * pivot isn't found, then an empty string is returned.  In the unlikely case
 * that an empty string is provided for a pivot, then we return a copy of the
 * original string.
 */


extern ag_string *
ag_string_split(const ag_string *ctx, const char *pvt)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (pvt);

        if (AG_UNLIKELY (!*pvt))
                return ag_string_copy(ctx);

        char *find;

        if (AG_UNLIKELY (!(*ctx && (find = strstr(ctx, pvt)))))
                return ag_string_new_empty();

        size_t sz = find - ctx;
        ag_string *lhs = ag_memblock_new(sz + 1);
        strncpy(lhs, ctx, sz);
        lhs[sz] = '\0';

        return (lhs);
}


/*
 * Define the ag_string_split_right() interface function. This function splits a
 * string around a pivot and returns the substring on the right side of the
 * pivot. As in the case of ag_string_split(), in case the pivot doesn't exist
 * then an empty string is returned, and if the pivot is an empty string then a
 * copy of the original string is returned.
 */


extern ag_string *
ag_string_split_right(const ag_string *ctx, const char *pvt)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (pvt);

        if (AG_UNLIKELY (!*pvt))
                return ag_string_copy(ctx);

        char *find;

        if (AG_UNLIKELY (!(*ctx && (find = strstr(ctx, pvt)))))
                return ag_string_new_empty();

        size_t off = strlen(pvt);
        size_t sz = strlen(find) - off;

        ag_string *rhs = ag_memblock_new(sz + 1);
        strncpy(rhs, find + strlen(pvt), sz);
        rhs[sz] = '\0';

        return (rhs);
}

