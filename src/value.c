#include "./api.h"

#define MASK_TAG ((uintptr_t)(8 - 1))
#define MASK_PTR (~MASK_TAG)

/*#define TAG_OBJECT ((uintptr_t)0) // 0b000
#define TAG_INT ((uintptr_t)1) // 0b001
#define TAG_STRING ((uintptr_t)2) // 0b010
#define TAG_FLOAT ((uintptr_t)4) // 0b100
#define TAG_UINT ((uintptr_t)6) // 0b110*/

#define SHIFT_INT ((uintptr_t)1)
#define SHIFT_UINT ((uintptr_t)3)


static inline bool tag_check(const ag_value_t *ctx, uintptr_t tag)
{
    return ((uintptr_t)ctx & tag) == tag;
}


extern ag_value_t *ag_value_new_int(int64_t val)
{
    intptr_t bits = ((intptr_t) val << SHIFT_INT) | AG_VALUE_TYPE_INT;
    return (ag_value_t *)bits;
}


extern ag_value_t *ag_value_new_uint(unsigned int val)
{
    uintptr_t bits = ((uintptr_t) val << SHIFT_UINT) | AG_VALUE_TYPE_UINT;
    return (ag_value_t *)bits;
}


extern ag_value_t *ag_value_new_float(double val)
{
    double *d = ag_memblock_new(sizeof *d);
    *d = val;

    uintptr_t bits = (uintptr_t) d | AG_VALUE_TYPE_FLOAT;
    return (ag_value_t *)bits;
}


extern ag_value_t *ag_value_new_string(const ag_string_t *val)
{
    //ag_value_t v = (uintptr_t) ag_string_copy(val);
    //return v | TAG_STRING;
    
    ag_value_t *v = ag_string_copy(val);
    uintptr_t bits = (uintptr_t) v | AG_VALUE_TYPE_STRING;
    return (ag_value_t *)bits;    
}


extern ag_value_t *ag_value_new_object(const ag_object_t *val)
{
    //ag_value_t v = (uintptr_t) ag_object_copy(val);
    //return v | TAG_OBJECT;

    ag_value_t *v = ag_object_copy(val);
    uintptr_t bits = (uintptr_t) v | AG_VALUE_TYPE_OBJECT;
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


extern enum ag_value_type ag_value_type(const ag_value_t *ctx)
{
    ag_assert (ctx);
    uintptr_t bits = (uintptr_t)ctx;

    if ((bits & AG_VALUE_TYPE_INT) == AG_VALUE_TYPE_INT)
        return AG_VALUE_TYPE_INT;

    return bits & MASK_TAG;
}


extern inline bool ag_value_is_int(const ag_value_t *ctx);


extern inline bool ag_value_is_uint(const ag_value_t *ctx);


extern inline bool ag_value_is_float(const ag_value_t *ctx);


extern inline bool ag_value_is_string(const ag_value_t *ctx);


extern inline bool ag_value_is_object(const ag_value_t *ctx);


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

