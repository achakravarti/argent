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
extern ag_uuid *ag_uuid_copy(const ag_uuid *);
extern ag_uuid *ag_uuid_clone(const ag_uuid *);
extern void ag_uuid_release(ag_uuid **);


extern enum ag_cmp ag_uuid_cmp(const ag_uuid *, const ag_uuid *);
extern bool ag_uuid_lt(const ag_uuid *, const ag_uuid *);
extern bool ag_uuid_eq(const ag_uuid *, const ag_uuid *);
extern bool ag_uuid_gt(const ag_uuid *, const ag_uuid *);


extern bool ag_uuid_empty(const ag_uuid *);
extern ag_str *ag_uuid_str(const ag_uuid *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_UUID_H__ */
