#ifndef __ARGENT_HASH_H__
#define __ARGENT_HASH_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "./argent.h"


typedef size_t ag_hash;

extern ag_hash ag_hash_new(size_t);
extern ag_hash ag_hash_new_str(const char *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_HASH_H__ */

