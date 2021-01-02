
#include "../include/argent.h"



/*******************************************************************************
 *                               VALUE INTERNALS
 */


#define MASK_TAG ((uintptr_t)(8 - 1))
#define MASK_PTR (~MASK_TAG)

#define SHIFT_UINT ((uintptr_t)1)
#define SHIFT_INT ((uintptr_t)3)


static inline bool tag_check(const ag_value *ctx, uintptr_t tag)
{
    return ((uintptr_t)ctx & tag) == tag;
}


extern inline bool      ag_value_lt(const ag_value *, const ag_value *);
extern inline bool      ag_value_eq(const ag_value *, const ag_value *);
extern inline bool      ag_value_gt(const ag_value *, const ag_value *);
extern inline bool      ag_value_type_int(const ag_value *);
extern inline bool      ag_value_type_uint(const ag_value *);
extern inline bool      ag_value_type_float(const ag_value *);
extern inline bool      ag_value_type_string(const ag_value *);
extern inline bool      ag_value_type_object(const ag_value *);



extern ag_value *
ag_value_new_int(ag_int val)
{
        intptr_t bits = ((intptr_t)val << SHIFT_INT) | AG_VALUE_TYPE_INT;
        return ((ag_value *)bits);
}


extern ag_value *
ag_value_new_uint(ag_uint val)
{
        uintptr_t bits = ((uintptr_t)val << SHIFT_UINT) | AG_VALUE_TYPE_UINT;
        return ((ag_value *)bits);
}


extern ag_value *
ag_value_new_float(ag_float val)
{
        double *v = ag_memblock_new(sizeof *v);
        *v = val;

        uintptr_t bits = (uintptr_t)v | AG_VALUE_TYPE_FLOAT;
        return ((ag_value *)bits);
}


extern ag_value *
ag_value_new_string(const ag_string *val)
{
        AG_ASSERT_PTR (val);

        ag_value *v = ag_string_copy(val);
        uintptr_t bits = (uintptr_t)v | AG_VALUE_TYPE_STRING;
        return ((ag_value *) bits);
}


extern ag_value *
ag_value_new_object(const ag_object *val)
{
        AG_ASSERT_PTR (val);

        ag_value *v = ag_object_copy(val);
        uintptr_t bits = (uintptr_t)v | AG_VALUE_TYPE_OBJECT;
        return ((ag_value *)bits);
}


extern ag_value *
ag_value_copy(const ag_value *ctx)
{
        AG_ASSERT_PTR (ctx);

        if (ag_value_type_object(ctx))
                return ag_value_new_object(ag_value_object(ctx));
    
        if (ag_value_type_string(ctx))
                return ag_value_new_string(ag_value_string(ctx));
    
        if (ag_value_type_float(ctx))
                return ag_value_new_float(ag_value_float(ctx));

        return ((ag_value *)ctx);
}


extern void
ag_value_release(ag_value **ctx)
{
        AG_ASSERT_PTR (ctx);

        ag_value *v = *ctx;

        if (AG_LIKELY (v)) {
                if (ag_value_type_object(v)) {
                        ag_object *o = ag_value_object(v);
                        ag_object_release(&o);
                }

                if (ag_value_type_string(v)) {
                        ag_string *s = ag_value_string(v);
                        ag_string_release(&s);
                }

                if (ag_value_type_float(v)) {
                        void *ptr = (void *)((uintptr_t)v & MASK_PTR);
                        ag_memblock_release(&ptr);
                }
        }

    *ctx = NULL;
}


extern enum ag_cmp
ag_value_cmp(const ag_value *ctx, const ag_value *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_value_type(ctx) == ag_value_type(cmp));

        switch (ag_value_type(ctx)) {
        case AG_VALUE_TYPE_OBJECT:
                return (ag_object_cmp(ag_value_object(ctx),
                    ag_value_object(cmp)));
                break;
        case AG_VALUE_TYPE_STRING:
                return (ag_string_cmp(ag_value_string(ctx),
                    ag_value_string(cmp)));
                break;
        case AG_VALUE_TYPE_FLOAT:
                return (ag_float_cmp(ag_value_float(ctx), ag_value_float(cmp)));
                break;
        case AG_VALUE_TYPE_UINT:
                return (ag_uint_cmp(ag_value_uint(ctx), ag_value_uint(cmp)));
                break;
        default:
                return (ag_int_cmp(ag_value_int(ctx), ag_value_int(cmp)));
        };
}


extern enum ag_value_type
ag_value_type(const ag_value *ctx)
{
        AG_ASSERT_PTR (ctx);

        uintptr_t bits = (uintptr_t)ctx;

        if ((bits & AG_VALUE_TYPE_UINT) == AG_VALUE_TYPE_UINT)
                return (AG_VALUE_TYPE_UINT);

        return (bits & MASK_TAG);
}


extern bool
ag_value_valid(const ag_value *ctx)
{
        AG_ASSERT_PTR (ctx);

        switch (ag_value_type(ctx)) {
        case AG_VALUE_TYPE_STRING:
                return !ag_string_empty(ag_value_string(ctx));
                break;
        case AG_VALUE_TYPE_OBJECT:
                return ag_object_valid(ag_value_object(ctx));
                break;
        default:
                return true;
        };
}


extern ag_int
ag_value_int(const ag_value *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_value_type_int(ctx));

        return ((intptr_t)ctx >> SHIFT_INT);
}


extern ag_uint
ag_value_uint(const ag_value *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_value_type_uint(ctx));

        return ((uintptr_t)ctx >> SHIFT_UINT);
}


extern ag_float
ag_value_float(const ag_value *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_value_type_float(ctx));

        return (*((double *)((uintptr_t)ctx & MASK_PTR)));
}


extern ag_string *
ag_value_string(const ag_value *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_value_type_string(ctx));

        return ((ag_string *)((uintptr_t)ctx & MASK_PTR));
}


extern ag_object *
ag_value_object(const ag_value *ctx)
{
        AG_ASSERT_PTR (ctx); 
        AG_ASSERT (ag_value_type_object(ctx));

        return ((ag_object *)((uintptr_t)ctx & MASK_PTR));
}

