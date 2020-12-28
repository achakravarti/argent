#include "../include/argent.h"

#include <math.h>


extern inline bool              ag_int_lt(ag_int, ag_int);
extern inline bool              ag_int_eq(ag_int, ag_int);
extern inline bool              ag_int_gt(ag_int, ag_int);
extern inline enum ag_cmp       ag_int_cmp(ag_int, ag_int);

extern inline bool              ag_uint_lt(ag_uint ctx, ag_uint cmp);
extern inline bool              ag_uint_eq(ag_uint ctx, ag_uint cmp);
extern inline bool              ag_uint_gt(ag_uint ctx, ag_uint cmp);
extern inline enum ag_cmp       ag_uint_cmp(ag_uint ctx, ag_uint cmp);

extern inline enum ag_cmp       ag_float_cmp(ag_float, ag_float);


extern bool
ag_float_lt(ag_float ctx, ag_float cmp)
{
        /* https://stackoverflow.com/questions/17333 */
        return ((cmp - ctx) > ((fabs(ctx) < fabs(cmp) 
            ? fabs(cmp) : fabs(ctx)) * DBL_EPSILON));
}


extern bool
ag_float_eq(ag_float ctx, ag_float cmp)
{
        /* https://stackoverflow.com/questions/17333 */
        return (fabs(ctx - cmp) <= ((fabs(ctx) > fabs(cmp) 
            ? fabs(cmp) : fabs(ctx)) * DBL_EPSILON));
}


extern bool
ag_float_gt(ag_float ctx, ag_float cmp)
{
        /* https://stackoverflow.com/questions/17333 */
        return ((ctx - cmp) > ( (fabs(ctx) < fabs(cmp) 
            ? fabs(cmp) : fabs(ctx)) * DBL_EPSILON));
}

