#include <string.h>
#include "./api.h"


/*******************************************************************************
 *                               STRING INTERNALS
 */

                                       /* creates new padded string [AgDM:??] */
static inline char *string_new(size_t sz)
{
    size_t *p = ag_memblock_new(sizeof (size_t) + sz + 1);
    p[0] = sz;
    return (char *) &p[1];
}

                                    /* gets size metadata of string [AgDM:??] */
static inline size_t string_sz(const ag_string_t *ctx)
{
    return ((size_t *) ctx)[-1];
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

                              /* implementation of ag_string_copy() [AgDM:??] */
extern ag_string_t *ag_string_copy(const ag_string_t *ctx)
{
    ag_assert (ctx);
    return ag_string_new(ctx);
}

                           /* implementation of ag_string_dispose() [AgDM:??] */
extern void ag_string_dispose(ag_string_t **ctx)
{
    ag_string_t *hnd;

    if (ag_likely (ctx && (hnd = *ctx))) {
        size_t *p = &((size_t *) hnd)[-1];
        ag_memblock_free((ag_memblock_t **) &p);
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
    ag_assert (ctx && *ctx);
    ag_string_t *oldstr = *ctx;
    size_t oldsz = string_sz(oldstr);

    ag_assert (cat);
    size_t addsz = string_sz(cat);
    size_t newsz = oldsz + addsz;

    char *s = string_new(newsz);
    strncpy(s, oldstr, oldsz);
    strncpy(s + oldsz, cat, addsz);
    s[newsz] = '\0';

    ag_string_dispose(ctx);
    *ctx = s;
}

