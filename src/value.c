#include "./api.h"

#define MASK_TAG ((uintptr_t) (8 - 1))
#define MASK_PTR (~MASK_TAG)

#define TAG_OBJECT ((uintptr_t) 0) // 0b000
#define TAG_INT ((uintptr_t) 1) // 0b001
#define TAG_STRING ((uintptr_t) 2) // 0b010
#define TAG_FLOAT ((uintptr_t) 4) // 0b100
#define TAG_UINT ((uintptr_t) 6) // 0b110

#define SHIFT_INT ((uintptr_t) 1)
#define SHIFT_UINT ((uintptr_t) 3)


extern ag_value_t ag_value_new_int(int val)
{
    return ((uintptr_t) val << SHIFT_INT) | TAG_INT;
}


extern ag_value_t ag_value_new_uint(unsigned int val)
{
    return ((uintptr_t) val << SHIFT_UINT) | TAG_UINT;
}


extern ag_value_t ag_value_new_float(double val)
{
    double *d = ag_memblock_new(sizeof *d);
    *d = val;

    ag_value_t v = (uintptr_t) d;
    return v | TAG_FLOAT;
}


extern ag_value_t ag_value_new_string(const ag_string_t *val)
{
    ag_value_t v = (uintptr_t) ag_string_copy(val);
    return v | TAG_STRING;
}


extern ag_value_t ag_value_new_object(const ag_object_t *val)
{
    ag_value_t v = (uintptr_t) ag_object_copy(val);
    return v | TAG_OBJECT;
}


extern ag_value_t ag_value_copy(ag_value_t ctx)
{
    ag_assert (ctx);

    if (ag_value_is_float(ctx))
        return ag_value_new_float(ag_value_float(ctx));

    if (ag_value_is_string(ctx))
        return ag_value_new_string(ag_value_string(ctx));

    if (ag_value_is_object(ctx))
        return ag_value_new_object(ag_value_object(ctx));

    return ctx;
}


extern void ag_value_dispose(ag_value_t *ctx)
{
    ag_assert (ctx);
    ag_value_t v = *ctx;

    if (ag_likely (v)) {
        if (ag_value_is_float(v))
            ag_memblock_free((void **) ctx);

        if (ag_value_is_string(v)) {
            ag_string_t *s = ag_value_string(v);
            ag_string_dispose(&s);
        }

        if (ag_value_is_object(v)) {
            ag_object_t *o = ag_value_object(v);
            ag_object_dispose(&o);
        }
    }
}


extern bool ag_value_is_int(ag_value_t ctx)
{
    ag_assert (ctx);
    return (ctx & TAG_INT) == TAG_INT;
}


extern bool ag_value_is_uint(ag_value_t ctx)
{
    ag_assert (ctx);
    return (ctx & TAG_UINT) == TAG_UINT;
}


extern bool ag_value_is_float(ag_value_t ctx)
{
    ag_assert (ctx);
    return (ctx & TAG_FLOAT) == TAG_FLOAT;
}


extern bool ag_value_is_string(ag_value_t ctx)
{
    ag_assert (ctx);
    return (ctx & TAG_STRING) == TAG_STRING;
}


extern bool ag_value_is_object(ag_value_t ctx)
{
    ag_assert (ctx);
    return (ctx & TAG_OBJECT) == TAG_OBJECT;
}


extern int ag_value_int(ag_value_t ctx)
{
    ag_assert (ctx && ag_value_is_int(ctx));
    return ctx >> SHIFT_INT;
}


extern unsigned int ag_value_uint(ag_value_t ctx)
{
    ag_assert (ctx && ag_value_is_uint(ctx));
    return ctx >> SHIFT_UINT;
}


extern double ag_value_float(ag_value_t ctx)
{
    ag_assert (ctx && ag_value_is_float(ctx));
    return *((double *) (ctx & MASK_PTR));
}


extern ag_string_t *ag_value_string(ag_value_t ctx)
{
    ag_assert (ctx && ag_value_is_string(ctx));
    return (ag_string_t *) (ctx & MASK_PTR);
}


extern ag_object_t *ag_value_object(ag_value_t ctx)
{
    ag_assert (ctx && ag_value_is_object(ctx));
    return (ag_object_t *) (ctx & MASK_PTR);
}

