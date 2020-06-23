#include <string.h>
#include <stdarg.h>
#include "./api.h"


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
extern inline bool ag_string_lt(const ag_string_t *lhs, const ag_string_t *rhs);

                                   /* declaration of ag_string_eq() [AgDM:??] */
extern inline bool ag_string_eq(const ag_string_t *lhs, const ag_string_t *rhs);

                                   /* declaration of ag_string_gt() [AgDM:??] */
extern inline bool ag_string_gt(const ag_string_t *lhs, const ag_string_t *rhs);

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
extern enum ag_tristate ag_string_cmp(const ag_string_t *lhs, 
        const ag_string_t *rhs)
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

