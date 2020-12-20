#ifndef __ARGENT_UUID_H__
#define __ARGENT_UUID_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "./argent.h"


typedef struct ag_uuid ag_uuid;


extern ag_uuid *ag_uuid_new(void);
extern ag_uuid *ag_uuid_new_empty(void);
extern ag_uuid *ag_uuid_parse(const char *);
ag_uuid *ag_uuid_copy(const ag_uuid *);
extern ag_uuid *ag_uuid_clone(const ag_uuid *);
extern void ag_uuid_release(ag_uuid **);


extern enum ag_cmp ag_uuid_cmp(const ag_uuid *, const ag_uuid *);

inline bool ag_uuid_lt(const ag_uuid *ctx, const ag_uuid *cmp)
{
        return ag_uuid_cmp(ctx, cmp) == AG_CMP_LT;
}

inline bool ag_uuid_eq(const ag_uuid *ctx, const ag_uuid *cmp)
{
        return ag_uuid_cmp(ctx, cmp) == AG_CMP_EQ;
}

inline bool ag_uuid_gt(const ag_uuid *ctx, const ag_uuid *cmp)
{
        return ag_uuid_cmp(ctx, cmp) == AG_CMP_GT;
}


extern bool ag_uuid_empty(const ag_uuid *);
extern ag_hash ag_uuid_hash(const ag_uuid *);
extern ag_str *ag_uuid_str(const ag_uuid *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_UUID_H__ */
