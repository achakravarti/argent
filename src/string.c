#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "../include/argent.h"


/*******************************************************************************
 *                               STRING INTERNALS
 */

                     /* index offset from string to reference count [AgDM:??] */
#define STRING_OFFSET_REFC -2

                                /* index offset from string to size [AgDM:??] */
#define STRING_OFFSET_SZ -1

                      /* size offset for terminating null character [AgDM:??] */
#define STRING_OFFSET_NULL 1

                                        /* index of reference count [AgDM:??] */
#define STRING_INDEX_REFC 0

                                                   /* index of size [AgDM:??[ */
#define STRING_INDEX_SZ 1

                                          /* index of string buffer [AgDM:??] */
#define STRING_INDEX_STR 2

                                       /* creates new padded string [AgDM:??] */
static inline char *string_new(size_t sz)
{
    size_t *p = ag_memblock_new(sizeof (size_t) * 2 + sz + STRING_OFFSET_NULL);
    p[STRING_INDEX_REFC] = 1;
    p[STRING_INDEX_SZ] = sz;
    return (char *) &p[STRING_INDEX_STR];
}

                            /* gets starting point of padded string [AgDM:??] */
static inline size_t *string_head(ag_string_t *ctx)
{
    return &((size_t *) ctx)[STRING_OFFSET_REFC];
}

                                  /* gets reference count of string [AgDM:??] */
static inline size_t string_sz(const ag_string_t *ctx)
{
    return ((size_t *) ctx)[STRING_OFFSET_SZ];
}

                                             /* gets size of string [AgDM:??] */
static inline size_t string_refc(const ag_string_t *ctx)
{
    return ((size_t *) ctx)[STRING_OFFSET_REFC];
}

                             /* increases reference count of string [AgDM:??] */
static inline void string_refc_inc(ag_string_t *ctx)
{
    ((size_t *) ctx)[STRING_OFFSET_REFC]++;
}

                             /* decreases reference count of string [AgDM:??] */
static inline void string_refc_dec(ag_string_t *ctx)
{
    ((size_t *) ctx)[STRING_OFFSET_REFC]--;
}


/*******************************************************************************
 *                               STRING EXTERNALS
 */


                            /* declaration of ag_string_new_empty() [AgDM:??] */
extern inline ag_string_t *ag_string_new_empty(void);

                                   /* declaration of ag_string_lt() [AgDM:??] */
extern inline bool ag_string_lt(const ag_string_t *lhs, const char *rhs);

                                   /* declaration of ag_string_eq() [AgDM:??] */
extern inline bool ag_string_eq(const ag_string_t *lhs, const char *rhs);

                                   /* declaration of ag_string_gt() [AgDM:??] */
extern inline bool ag_string_gt(const ag_string_t *lhs, const char *rhs);

                             /* declaration of ag_string_add_cstr() [AgDM:??] */
extern inline void ag_string_add_cstr(ag_string_t **ctx, const char *cat);

                               /* implementation of ag_string_new() [AgDM:??] */
extern ag_string_t *ag_string_new(const char *cstr)
{
    ag_assert (cstr);
    size_t sz = strlen(cstr);
    char *s = string_new(sz);

    strncpy(s, cstr, sz);
    s[sz] = '\0';
    return s;
}

                           /* implementation of ag_string_new_fmt() [AgDM:??] */
extern ag_string_t *ag_string_new_fmt(const char *fmt, ...)
{
    va_list args;

    ag_assert (fmt && *fmt);
    va_start(args, fmt);
    char *bfr = ag_memblock_new(vsnprintf(NULL, 0, fmt, args) + 1);
    va_end(args);

    va_start(args, fmt);
    (void) vsprintf(bfr, fmt, args);
    va_end(args);

    ag_string_t *s = ag_string_new(bfr);
    ag_memblock_free((ag_memblock_t **) &bfr);
    return s;
}


                              /* implementation of ag_string_copy() [AgDM:??] */
extern ag_string_t *ag_string_copy(const ag_string_t *ctx)
{
    ag_assert (ctx);
    ag_string_t *cp = (ag_string_t *) ctx;

    string_refc_inc(cp);
    return cp;
}

                           /* implementation of ag_string_dispose() [AgDM:??] */
extern void ag_string_dispose(ag_string_t **ctx)
{
    ag_string_t *hnd;

    if (ag_likely (ctx && (hnd = *ctx))) {
        string_refc_dec(hnd);

        if (!string_refc(hnd)) {
            size_t *h = string_head(hnd);
            ag_memblock_free((ag_memblock_t **) &h);
        }
    }
}

                               /* implementation of ag_string_len() [AgDM:??] */
extern size_t ag_string_len(const ag_string_t *ctx)
{
    register size_t i = 0, len = 0;

    while (ctx[i]) {
        if ((ctx[i] & 0xC0) != 0x80)
            len++;
        i++;
    }

    return len;
}

                                /* implementation of ag_string_sz() [AgDM:??] */
extern size_t ag_string_sz(const ag_string_t *ctx)
{
    ag_assert (ctx);
    return string_sz(ctx);
}

                               /* implementation of ag_string_cmp() [AgDM:??] */
extern enum ag_tristate ag_string_cmp(const ag_string_t *lhs, const char *rhs)
{
    ag_assert (lhs && rhs);
    int cmp = strcmp(lhs, rhs);

    if (!cmp)
        return AG_TRISTATE_GND;

    return cmp > 0 ? AG_TRISTATE_HI : AG_TRISTATE_LO;
}

                               /* implementation of ag_string_add() [AgDM:??] */
extern void ag_string_add(ag_string_t **ctx, const ag_string_t *cat)
{
    ag_assert (cat);
    if (ag_unlikely (!*cat))
        return;

    ag_assert (ctx && *ctx);
    ag_string_t *oldstr = *ctx;
    size_t oldsz = string_sz(oldstr);

    size_t addsz = string_sz(cat);
    size_t newsz = oldsz + addsz;

    char *s = string_new(newsz);
    strncpy(s, oldstr, oldsz);
    strncpy(s + oldsz, cat, addsz);
    s[newsz] = '\0';

    ag_string_dispose(ctx);
    *ctx = s;
}


extern void ag_string_lower(ag_string_t **ctx)
{
    ag_assert (ctx && *ctx);
    char *c = *ctx;

    while (*c) {
        *c = tolower(*c);
        c++;
    }
}


extern void ag_string_upper(ag_string_t **ctx)
{
    ag_assert (ctx && *ctx);
    char *c = *ctx;

    while (*c) {
        *c = toupper(*c);
        c++;
    }
}


extern void ag_string_proper(ag_string_t **ctx)
{
    ag_assert (ctx && *ctx);
    char *hnd = *ctx;

    register size_t sz = string_sz(hnd);
    for (register size_t i = 0; i < sz; i++)
        hnd[i] = (!i || hnd[i - 1] == ' ')  ? toupper(hnd[i]) : tolower(hnd[i]);
}


extern inline ag_hash_t ag_string_hash(const ag_string_t *ctx);



// https://stackoverflow.com/questions/29414709
extern ag_string_t *ag_string_url_encode(const ag_string_t *ctx)
{
    ag_assert (ctx);
    
    const char *hnd = ctx;
    if (!*hnd)
        return ag_string_new_empty();

    register size_t sz = (string_sz(hnd) * 3) + 1; /* 1 to suppress valgrind */
    char *bfr = ag_memblock_new(sz);
    
    register size_t n = 0;
    register int c;
    while ((c = *hnd)) {
        if (c < 33 || c > 126 || strchr("!\"*%'();:@&=+$,/?#[]", *hnd)) {
            snprintf(bfr + n, sz, "%%%02X", c & 0xff);
            n += 3;
        } else
            bfr[n++] = c;

        hnd++;
    }

    bfr[n] = '\0';
    ag_string_t *ret = ag_string_new(bfr);
    ag_memblock_free((void **) &bfr);
    return ret;
}


static inline bool url_encoded(const char *str)
{
    return *str == '%' && isxdigit(str[1]) && isxdigit(str[2]);
}


extern bool ag_string_url_encoded(const ag_string_t *ctx)
{
    ag_assert (ctx);

    register size_t sz = string_sz(ctx) - STRING_OFFSET_NULL;
    if (sz < 3)
        return false;

    sz =- 2;
    for (register size_t i = 0; i < sz; i++) {
        if (url_encoded(ctx + i))
            return true;
    }

    return false;
}


static inline char url_decode(char c)
{
    if (c >= 'a')
        return c - ('a' - 'A');
    else if (c >= 'A')
        return c - ('A' - 10);
    else
        return c - '0';
}


extern ag_string_t *ag_string_url_decode(const ag_string_t *ctx)
{
    ag_assert (ctx);
    const char *hnd = ctx;

    if (!*hnd)
        return ag_string_new_empty();

    size_t sz = string_sz(hnd) + 1; /* 1 to suppress valgrind error */
    char *bfr = ag_memblock_new(sz);

    char *c = bfr;
    while (*hnd) {
        if (url_encoded(hnd)) {
            *c++ = (16 * url_decode(hnd[1])) + url_decode(hnd[2]);
            hnd += 3;
        } else if (*c == '+') {
            *c++ = ' ';
            hnd++;
        } else
            *c++ = *hnd++;
    }

    *c = '\0';
    ag_string_t *ret = ag_string_new(bfr);
    ag_memblock_free((void **) &bfr);
    return ret;
}


extern bool ag_string_has(const ag_string_t *ctx, const char *needle)
{
    ag_assert (ctx && needle);

    return (bool) strstr(ctx, needle);
}


extern ag_string_t *ag_string_split_left(const ag_string_t *ctx,
        const char *pivot)
{
    ag_assert (ctx && pivot && *pivot);

    if (ag_unlikely (!*ctx || !strstr(ctx, pivot)))
        return ag_string_new_empty();

    char *save;
    ag_string_smart_t *cp = ag_string_new(ctx);
    return ag_string_new(strtok_r(cp, pivot, &save));
}


extern ag_string_t *ag_string_split_right(const ag_string_t *ctx,
        const char *pivot)
{
    ag_assert (ctx && pivot && *pivot);
    
    if (ag_unlikely (!*ctx || !strstr(ctx, pivot)))
        return ag_string_new_empty();

    char *save;
    ag_string_smart_t *cp = ag_string_new(ctx);
    (void) strtok_r(cp, pivot, &save);
    return ag_string_new(save);
}


