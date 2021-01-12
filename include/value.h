#ifndef __ARGENT_VALUE_H__
#define __ARGENT_VALUE_H__

#ifdef __cplusplus
extern "C" {
#endif 


#include "./object.h"


enum ag_value_type {
    AG_VALUE_TYPE_OBJECT = 0, /* 0b000 */
    AG_VALUE_TYPE_UINT = 1, /* 0b001 */
    AG_VALUE_TYPE_STRING = 2, /* 0b010 */
    AG_VALUE_TYPE_FLOAT = 4, /* 0b100 */
    AG_VALUE_TYPE_INT = 6 /* 0b110 */
};


typedef void ag_value;



extern ag_value         *ag_value_new_int(ag_int);
extern ag_value         *ag_value_new_uint(ag_uint);
extern ag_value         *ag_value_new_float(ag_float);
extern ag_value         *ag_value_new_string(const ag_string *);
extern ag_value         *ag_value_new_object(const ag_object *);
extern ag_value         *ag_value_copy(const ag_value *);
extern void              ag_value_release(ag_value **);



extern enum ag_cmp ag_value_cmp(const ag_value *, const ag_value *);

inline bool ag_value_lt(const ag_value *ctx, const ag_value *cmp)
{
        return ag_value_cmp(ctx, cmp) == AG_CMP_LT;
}

inline bool ag_value_eq(const ag_value *ctx, const ag_value *cmp)
{
        return ag_value_cmp(ctx, cmp) == AG_CMP_EQ;
}

inline bool ag_value_gt(const ag_value *ctx, const ag_value *cmp)
{
        return ag_value_cmp(ctx, cmp) == AG_CMP_GT;
}


extern enum ag_value_type        ag_value_type(const ag_value *);
extern bool                      ag_value_valid(const ag_value *);
extern ag_hash                   ag_value_hash(const ag_value *);
extern size_t                    ag_value_sz(const ag_value *);
extern size_t                    ag_value_len(const ag_value *);
extern ag_string                *ag_value_str(const ag_value *);
extern ag_int                    ag_value_int(const ag_value *);
extern ag_uint                   ag_value_uint(const ag_value *);
extern ag_float                  ag_value_float(const ag_value *);
extern const ag_string          *ag_value_string(const ag_value *);
extern const ag_object          *ag_value_object(const ag_value *);

inline bool ag_value_type_int(const ag_value *ctx)
{
        return ag_value_type(ctx) == AG_VALUE_TYPE_INT;
}

inline bool ag_value_type_uint(const ag_value *ctx)
{
        return ag_value_type(ctx) == AG_VALUE_TYPE_UINT;
}

inline bool ag_value_type_float(const ag_value *ctx)
{
        return ag_value_type(ctx) == AG_VALUE_TYPE_FLOAT;
}

inline bool ag_value_type_string(const ag_value *ctx)
{
        return ag_value_type(ctx) == AG_VALUE_TYPE_STRING;
}

inline bool ag_value_type_object(const ag_value *ctx)
{
        return ag_value_type(ctx) == AG_VALUE_TYPE_OBJECT;
}


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_VALUE_H__ */

