#if (!defined __ARGENT_STRING_H__)
#define __ARGENT_STRING_H__


#include <stddef.h>
#include <stdbool.h>
#include "./primitives.h"

/*******************************************************************************
 *                                   STRINGS
 */

                                                    /* UTF-8 string [AgDM:??] */
typedef char ag_string_t;

                                    /* smart version of ag_string_t [AgDM:??] */
#if (defined __GNUC__ || defined __clang__)
#   define ag_string_smart_t \
            __attribute__((cleanup(ag_string_dispose))) ag_string_t
#else
#   define ag_string_smart_t ag_string_t
#   warning "[!] ag_string_smart_t leaks memory on current compiler"
#endif

                                               /* create new string [AgDM:??] */
extern ag_string_t *ag_string_new(const char *cstr);

                                         /* create new empty string [AgDM:??] */
inline ag_string_t *ag_string_new_empty(void)
{
    return ag_string_new("");
}

                                     /* create new formatted string [AgDM:??] */
extern ag_string_t *ag_string_new_fmt(const char *fmt, ...);

                                            /* copy existing string [AgDM:??] */
extern ag_string_t *ag_string_copy(const ag_string_t *ctx);

                                         /* dispose existing string [AgDM:??] */
extern void ag_string_dispose(ag_string_t **ctx);

                              /* get lexicographic length of string [AgDM:??] */
extern size_t ag_string_len(const ag_string_t *ctx);

                                     /* get size in bytes of string [AgDM:??] */
extern size_t ag_string_sz(const ag_string_t *ctx);

                                    /* compare two string instances [AgDM:??] */
extern enum ag_tristate ag_string_cmp(const ag_string_t *lhs, const char *rhs);

                            /* check if string is less than another [AgDM:??] */
inline bool ag_string_lt(const ag_string_t *lhs, const char *rhs)
{
    return ag_string_cmp(lhs, rhs) == AG_TRISTATE_LO;
}

                             /* check if string is equal to another [AgDM:??] */
inline bool ag_string_eq(const ag_string_t *lhs, const char *rhs)
{
    return ag_string_cmp(lhs, rhs) == AG_TRISTATE_GND;
}

                         /* check if string is greater than another [AgDM:??] */
inline bool ag_string_gt(const ag_string_t *lhs, const char *rhs)
{
    return ag_string_cmp(lhs, rhs) == AG_TRISTATE_HI;
}

                                           /* add string to another [AgDM:??] */
extern void ag_string_add(ag_string_t **ctx, const ag_string_t *cat);

                                 /* add C-string to string instance [AgDM:??] */
inline void ag_string_add_cstr(ag_string_t **ctx, const char *cat)
{
    ag_string_smart_t *s = ag_string_new(cat);
    ag_string_add(ctx, s);
}

extern void ag_string_lower(ag_string_t **ctx);

extern void ag_string_upper(ag_string_t **ctx);

extern void ag_string_proper(ag_string_t **ctx);

inline ag_hash_t ag_string_hash(const ag_string_t *ctx)
{
    return ag_hash_new_str(ctx);
}

extern void ag_string_url_encode(ag_string_t **ctx);

extern void ag_string_url_decode(ag_string_t **ctx);

extern bool ag_string_has(const ag_string_t *ctx, const char *needle);

extern ag_string_t *ag_string_split_left(const ag_string_t *ctx,
        const char *pivot);

extern ag_string_t *ag_string_split_right(const ag_string_t *ctx,
        const char *pivot);

#endif /* !defined __ARGENT_STRING_H__ */

