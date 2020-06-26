#include <string.h>
#include "./api.h"



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
    size_t len = strlen(cstr);
    char *ctx = ag_memblock_new(len + 1);

    strncpy(ctx, cstr, len);
    ctx[len] = '\0';

    return ctx;
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
    ag_memblock_free((ag_memblock_t **) ctx);
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
    return strlen(ctx);
}


                               /* implementation of ag_string_cmp() [AgDM:??] */
extern int ag_string_cmp(const ag_string_t *lhs, const ag_string_t *rhs)
{
    ag_assert (lhs && rhs);
    return strcmp(lhs, rhs);
}


                               /* implementation of ag_string_add() [AgDM:??] */
extern void ag_string_add(ag_string_t **ctx, const ag_string_t *cat)
{
    ag_assert (ctx && *ctx && cat);
    ag_string_t *oldstr = *ctx;

    size_t oldlen = strlen(oldstr);
    size_t addlen = strlen(cat);
    size_t newlen = oldlen + addlen;

    ag_string_t *newstr = ag_memblock_new(newlen + 1);
    strncpy(newstr, oldstr, oldlen);
    strncpy(newstr + oldlen, cat, addlen);
    newstr[newlen] = '\0';

    ag_memblock_free((ag_memblock_t **) ctx);
    *ctx = newstr;
}

