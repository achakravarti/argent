#if (!defined __ARGENT_VALUE_H__)
#define __ARGENT_VALUE_H__


#include <stdbool.h>
#include "./primitives.h"
#include "./string.h"
#include "./object.h"


/*******************************************************************************
 *                             DYNAMIC TYPE SYSTEM
 */



enum ag_value_type {
    AG_VALUE_TYPE_OBJECT = 0, /* 0b000 */
    AG_VALUE_TYPE_UINT = 1, /* 0b001 */
    AG_VALUE_TYPE_STRING = 2, /* 0b010 */
    AG_VALUE_TYPE_FLOAT = 4, /* 0b100 */
    AG_VALUE_TYPE_INT = 6 /* 0b110 */
};

typedef void ag_value_t;

#if (defined __GNUC__ || defined __clang__)
#   define ag_value_smart_t \
            __attribute__((cleanup(ag_value_dispose))) ag_value_t
#else
#   define ag_value_smart_t ag_value_t
#   warning "[!] ag_value_smart_t leaks memory on current compiler"
#endif


extern ag_value_t *ag_value_new_int(ag_int val);

extern ag_value_t *ag_value_new_uint(ag_uint val);

extern ag_value_t *ag_value_new_float(ag_float val);

extern ag_value_t *ag_value_new_string(const ag_string_t *val);

extern ag_value_t *ag_value_new_object(const ag_object_t *val);

extern ag_value_t *ag_value_copy(const ag_value_t *ctx);

extern void ag_value_dispose(ag_value_t **ctx);

extern enum ag_tristate ag_value_cmp(const ag_value_t *ctx, 
    const ag_value_t *cmp);

inline bool ag_value_lt(const ag_value_t *ctx, const ag_value_t *cmp)
{
    return ag_value_cmp(ctx, cmp) == AG_TRISTATE_LO;
}

inline bool ag_value_eq(const ag_value_t *ctx, const ag_value_t *cmp)
{
    return ag_value_cmp(ctx, cmp) == AG_TRISTATE_GND;
}

inline bool ag_value_gt(const ag_value_t *ctx, const ag_value_t *cmp)
{
    return ag_value_cmp(ctx, cmp) == AG_TRISTATE_HI;
}

extern enum ag_value_type ag_value_type(const ag_value_t *ctx);

inline bool ag_value_is_int(const ag_value_t *ctx)
{
    return ag_value_type(ctx) == AG_VALUE_TYPE_INT;
}

inline bool ag_value_is_uint(const ag_value_t *ctx)
{
    return ag_value_type(ctx) == AG_VALUE_TYPE_UINT;
}

inline bool ag_value_is_float(const ag_value_t *ctx)
{
    return ag_value_type(ctx) == AG_VALUE_TYPE_FLOAT;
}

inline bool ag_value_is_string(const ag_value_t *ctx)
{
    return ag_value_type(ctx) == AG_VALUE_TYPE_STRING;
}

inline bool ag_value_is_object(const ag_value_t *ctx)
{
    return ag_value_type(ctx) == AG_VALUE_TYPE_OBJECT;
}

extern ag_int ag_value_int(const ag_value_t *ctx);

extern ag_uint ag_value_uint(const ag_value_t *ctx);

extern ag_float ag_value_float(const ag_value_t *ctx);

extern ag_string_t *ag_value_string(const ag_value_t *ctx);

extern ag_object_t *ag_value_object(const ag_value_t *ctx);


#endif /* !defined __ARGENT_VALUE_H__ */

