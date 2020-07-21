#include "./api.h"


/*******************************************************************************
 *                               VALUE INTERNALS
 */


#define MASK_TAG ((uintptr_t)(8 - 1))
#define MASK_PTR (~MASK_TAG)

#define SHIFT_INT ((uintptr_t)1)
#define SHIFT_UINT ((uintptr_t)3)


static inline bool tag_check(const ag_value_t *ctx, uintptr_t tag)
{
    return ((uintptr_t)ctx & tag) == tag;
}


/*******************************************************************************
 *                               VALUE EXTERNALS
 */

extern inline bool ag_value_is_int(const ag_value_t *ctx);

extern inline bool ag_value_is_uint(const ag_value_t *ctx);

extern inline bool ag_value_is_float(const ag_value_t *ctx);

extern inline bool ag_value_is_string(const ag_value_t *ctx);

extern inline bool ag_value_is_object(const ag_value_t *ctx);

extern ag_value_t *ag_value_new_int(int64_t val)
{
    intptr_t bits = ((intptr_t)val << SHIFT_INT) | AG_VALUE_TYPE_INT;
    return (ag_value_t *)bits;
}

extern ag_value_t *ag_value_new_uint(unsigned int val)
{
    uintptr_t bits = ((uintptr_t)val << SHIFT_UINT) | AG_VALUE_TYPE_UINT;
    return (ag_value_t *)bits;
}

extern ag_value_t *ag_value_new_float(double val)
{
    double *v = ag_memblock_new(sizeof *v);
    *v = val;

    uintptr_t bits = (uintptr_t)v | AG_VALUE_TYPE_FLOAT;
    return (ag_value_t *)bits;
}

extern ag_value_t *ag_value_new_string(const ag_string_t *val)
{
    ag_value_t *v = ag_string_copy(val);
    uintptr_t bits = (uintptr_t)v | AG_VALUE_TYPE_STRING;
    return (ag_value_t *)bits;    
}

extern ag_value_t *ag_value_new_object(const ag_object_t *val)
{
    ag_value_t *v = ag_object_copy(val);
    uintptr_t bits = (uintptr_t)v | AG_VALUE_TYPE_OBJECT;
    return (ag_value_t *)bits;
}

extern ag_value_t *ag_value_copy(const ag_value_t *ctx)
{
    ag_assert (ctx);

    if (ag_value_is_object(ctx))
        return ag_value_new_object(ag_value_object(ctx));
    
    if (ag_value_is_string(ctx))
        return ag_value_new_string(ag_value_string(ctx));
    
    if (ag_value_is_float(ctx))
        return ag_value_new_float(ag_value_float(ctx));

    return (ag_value_t *)ctx;
}

extern void ag_value_dispose(ag_value_t **ctx)
{
    ag_assert (ctx);
    ag_value_t *v = *ctx;

    if (ag_likely (v)) {
        if (ag_value_is_object(v)) {
            ag_object_t *o = ag_value_object(v);
            ag_object_dispose(&o);
        }

        if (ag_value_is_string(v)) {
            ag_string_t *s = ag_value_string(v);
            ag_string_dispose(&s);
        }

        if (ag_value_is_float(v)) {
            void *ptr = (void *)((uintptr_t)v & MASK_PTR);
            ag_memblock_free((void **) &ptr);
        }
    }

    *ctx = NULL;
}

extern enum ag_tristate ag_value_cmp(const ag_value_t *ctx, 
    const ag_value_t *cmp)
{
    ag_assert (ctx && cmp && ag_value_type(ctx) == ag_value_type(cmp));

    switch (ag_value_type(ctx)) {
        case AG_VALUE_TYPE_OBJECT:
            return ag_object_cmp(ag_value_object(ctx), ag_value_object(cmp));
            break;

        case AG_VALUE_TYPE_STRING:
            return ag_string_cmp(ag_value_string(ctx), ag_value_string(cmp));
            break;

        case AG_VALUE_TYPE_FLOAT: {
            double lhs = ag_value_float(ctx);
            double rhs = ag_value_float(cmp);
            
            if (lhs == rhs)
                return AG_TRISTATE_GND;

            return lhs < rhs ? AG_TRISTATE_LO : AG_TRISTATE_HI;
            break;
        }

        case AG_VALUE_TYPE_UINT: {
            uint64_t lhs = ag_value_uint(ctx);
            uint64_t rhs = ag_value_uint(cmp);
            
            if (lhs == rhs)
                return AG_TRISTATE_GND;

            return lhs < rhs ? AG_TRISTATE_LO : AG_TRISTATE_HI;
            break;
        }

        default: {
            int64_t lhs = ag_value_int(ctx);
            int64_t rhs = ag_value_int(cmp);
            
            if (lhs == rhs)
                return AG_TRISTATE_GND;

            return lhs < rhs ? AG_TRISTATE_LO : AG_TRISTATE_HI;
            break;
        }
    };
}

extern enum ag_value_type ag_value_type(const ag_value_t *ctx)
{
    ag_assert (ctx);
    uintptr_t bits = (uintptr_t)ctx;

    if ((bits & AG_VALUE_TYPE_INT) == AG_VALUE_TYPE_INT)
        return AG_VALUE_TYPE_INT;

    return bits & MASK_TAG;
}

extern int64_t ag_value_int(const ag_value_t *ctx)
{
    ag_assert (ctx && ag_value_is_int(ctx));
    return (intptr_t)ctx >> SHIFT_INT;
}

extern uint64_t ag_value_uint(const ag_value_t *ctx)
{
    ag_assert (ctx && ag_value_is_uint(ctx));
    return (uintptr_t)ctx >> SHIFT_UINT;
}

extern double ag_value_float(const ag_value_t *ctx)
{
    ag_assert (ctx && ag_value_is_float(ctx));
    return *((double *)((uintptr_t)ctx & MASK_PTR));
}

extern ag_string_t *ag_value_string(const ag_value_t *ctx)
{
    ag_assert (ctx && ag_value_is_string(ctx));
    return (ag_string_t *)((uintptr_t)ctx & MASK_PTR);
}

extern ag_object_t *ag_value_object(const ag_value_t *ctx)
{
    ag_assert (ctx && ag_value_is_object(ctx));
    return (ag_object_t *)((uintptr_t)ctx & MASK_PTR);
}

