#include <inttypes.h>
#include "../include/argent.h"

extern inline bool ag_int_lt(ag_int ctx, ag_int cmp);

extern inline bool ag_int_eq(ag_int ctx, ag_int cmp);

extern inline bool ag_int_gt(ag_int ctx, ag_int cmp);

extern inline enum ag_tristate ag_int_cmp(ag_int ctx, ag_int cmp);

extern inline bool ag_uint_lt(ag_uint ctx, ag_uint cmp);

extern inline bool ag_uint_eq(ag_uint ctx, ag_uint cmp);

extern inline bool ag_uint_gt(ag_uint ctx, ag_uint cmp);

extern inline enum ag_tristate ag_uint_cmp(ag_uint ctx, ag_uint cmp);

extern inline bool ag_float_lt(ag_float ctx, ag_float cmp);

extern inline bool ag_float_eq(ag_float ctx, ag_float cmp);

extern inline bool ag_float_gt(ag_float ctx, ag_float cmp);

extern inline enum ag_tristate ag_float_cmp(ag_float ctx, ag_float cmp);


extern ag_int ag_int_parse(const char *str)
{
    ag_assert (str && *str);
    
    char *endptr;
    return strtoimax(str, &endptr, 10);
}


extern ag_uint ag_uint_parse(const char *str)
{
    ag_assert (str && *str);

    char *endptr;
    return strtoumax(str, &endptr, 10);
}
