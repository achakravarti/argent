#if (!defined __ARGENT_PRIMITIVES_H__)
#define __ARGENT_PRIMITIVES_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <float.h>
#include <math.h>


enum ag_tristate {
    AG_TRISTATE_LO = -1,
    AG_TRISTATE_GND,
    AG_TRISTATE_HI
};

typedef intptr_t ag_int;
typedef uintptr_t ag_uint;
typedef double ag_float;

#define AG_UINT_MIN ((ag_uint)0)
#define AG_UINT_MAX UINTPTR_MAX

#define AG_INT_MIN ((ag_int)(-(AG_UINT_MAX / (ag_uint)2)))
#define AG_INT_MAX ((ag_int)((AG_UINT_MAX / (ag_uint)2) - (ag_uint)1))

#define AG_FLOAT_MIN DBL_MIN
#define AG_FLOAT_MAX DBL_MAX


inline bool ag_int_lt(ag_int ctx, ag_int cmp)
{
    return ctx < cmp;
}

inline bool ag_int_eq(ag_int ctx, ag_int cmp)
{
    return ctx == cmp;
}

inline bool ag_int_gt(ag_int ctx, ag_int cmp)
{
    return ctx > cmp;
}

inline enum ag_tristate ag_int_cmp(ag_int ctx, ag_int cmp)
{
    if (ctx == cmp)
        return AG_TRISTATE_GND;

    return ctx < cmp ? AG_TRISTATE_LO: AG_TRISTATE_HI;
}

extern ag_int ag_int_parse(const char *str);

inline bool ag_uint_lt(ag_uint ctx, ag_uint cmp)
{
    return ctx < cmp;
}

inline bool ag_uint_eq(ag_uint ctx, ag_uint cmp)
{
    return ctx == cmp;
}

inline bool ag_uint_gt(ag_uint ctx, ag_uint cmp)
{
    return ctx > cmp;
}

inline enum ag_tristate ag_uint_cmp(ag_uint ctx, ag_uint cmp)
{
    if (ctx == cmp)
        return AG_TRISTATE_GND;

    return ctx < cmp ? AG_TRISTATE_LO: AG_TRISTATE_HI;
}

extern ag_uint ag_uint_parse(const char *str);

inline bool ag_float_lt(ag_float ctx, ag_float cmp)
{
    /* https://stackoverflow.com/questions/17333 */
    return (cmp - ctx) > ((fabs(ctx) < fabs(cmp) 
        ? fabs(cmp) : fabs(ctx)) * DBL_EPSILON);
}

inline bool ag_float_eq(ag_float ctx, ag_float cmp)
{
    /* https://stackoverflow.com/questions/17333 */
    return fabs(ctx - cmp) <= ((fabs(ctx) > fabs(cmp) 
        ? fabs(cmp) : fabs(ctx)) * DBL_EPSILON);
}

inline bool ag_float_gt(ag_float ctx, ag_float cmp)
{
    /* https://stackoverflow.com/questions/17333 */
    return (ctx - cmp) > ( (fabs(ctx) < fabs(cmp) 
        ? fabs(cmp) : fabs(ctx)) * DBL_EPSILON);
}

inline enum ag_tristate ag_float_cmp(ag_float ctx, ag_float cmp)
{
    if (ag_float_eq(ctx, cmp))
        return AG_TRISTATE_GND;

    return ag_float_lt(ctx, cmp) ? AG_TRISTATE_LO: AG_TRISTATE_HI;
}

/*******************************************************************************
 *                                     HASH
 */


typedef size_t ag_hash_t;


extern ag_hash_t ag_hash_new(size_t sz);


extern ag_hash_t ag_hash_new_str(const char *str);


#endif /* !defined __ARGENT_PRIMITIVES_H__ */

