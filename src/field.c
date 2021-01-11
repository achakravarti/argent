#include "../include/argent.h"


extern inline ag_field          *ag_field_copy(const ag_field *);
extern inline ag_field          *ag_field_clone(const ag_field *);
extern inline void               ag_field_release(ag_field **);
extern inline enum ag_cmp        ag_field_cmp(const ag_field *,
                                    const ag_field *);
extern inline bool               ag_field_lt(const ag_field *, const ag_field *);
extern inline bool               ag_field_eq(const ag_field *, const ag_field *);
extern inline bool               ag_field_gt(const ag_field *, const ag_field *);
extern inline bool               ag_field_empty(const ag_field *);
extern inline ag_typeid          ag_field_typeid(const ag_field *);
extern inline ag_uuid           *ag_field_uuid(const ag_field *);
extern inline bool               ag_field_valid(const ag_field *);
extern inline size_t             ag_field_sz(const ag_field *);
extern inline size_t             ag_field_refc(const ag_field *);
extern inline size_t             ag_field_len(const ag_field *);
extern inline ag_hash            ag_field_hash(const ag_field *);
extern inline ag_string         *ag_field_str(const ag_field *);
                        

extern void      
__ag_field_register__(void)
{
}


extern ag_field *
ag_field_new(const ag_value *, const ag_value *)
{
}



extern ag_value *ag_field_key(const ag_field *)
{
}


extern ag_value *ag_field_val(const ag_field *)
{
}


extern void ag_field_key_set(ag_field **, const ag_value *)
{
}


extern void ag_field_val_set(ag_field **, const ag_value *)
{
}

