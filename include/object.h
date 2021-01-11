/*-
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Argent - infrastructure for building web services
 * Copyright (C) 2020 Abhishek Chakravarti
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact Abhishek Chakravarti at <abhishek@taranjali.org>.
 */


#ifndef __ARGENT_OBJECT_H__
#define __ARGENT_OBJECT_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "./memblock.h"
#include "./hash.h"
#include "./typeid.h"
#include "./uuid.h"


typedef struct ag_object ag_object;

typedef ag_memblock *(ag_object_clone_virt)(const ag_memblock *);
typedef void         (ag_object_release_virt)(ag_memblock *);
typedef enum ag_cmp  (ag_object_cmp_virt)(const ag_object *, const ag_object *);
typedef bool         (ag_object_valid_virt)(const ag_object *);
typedef size_t       (ag_object_sz_virt)(const ag_object *);
typedef size_t       (ag_object_len_virt)(const ag_object *);
typedef ag_hash      (ag_object_hash_virt)(const ag_object *);
typedef ag_string   *(ag_object_str_virt)(const ag_object *);


struct ag_object_vtable {
        ag_object_clone_virt   *clone;   /* Clone callback              */
        ag_object_release_virt *release; /* Release callback            */
        ag_object_cmp_virt     *cmp;     /* Comparison callback         */
        ag_object_valid_virt   *valid;   /* Validation callback         */
        ag_object_sz_virt      *sz;      /* Size computation callback   */
        ag_object_len_virt     *len;     /* Length computation callback */
        ag_object_hash_virt    *hash;    /* Hash computation callback   */
        ag_object_str_virt     *str;     /* String generation callback  */
};


extern ag_object *ag_object_new(ag_typeid, ag_memblock *);
extern ag_object *ag_object_copy(const ag_object *);
extern ag_object *ag_object_clone(const ag_object *);
extern void ag_object_release(ag_object **);


extern enum ag_cmp ag_object_cmp(const ag_object *, const ag_object *);

inline bool ag_object_lt(const ag_object *ctx, const ag_object *cmp)
{
        return ag_object_cmp(ctx, cmp) == AG_CMP_LT;
}

inline bool ag_object_eq(const ag_object *ctx, const ag_object *cmp)
{
        return ag_object_cmp(ctx, cmp) == AG_CMP_EQ;
}

inline bool ag_object_gt(const ag_object *ctx, const ag_object *cmp)
{
        return ag_object_cmp(ctx, cmp) == AG_CMP_GT;
}


extern ag_typeid ag_object_typeid(const ag_object *);
extern ag_uuid *ag_object_uuid(const ag_object *);
extern bool ag_object_valid(const ag_object *);
extern size_t ag_object_sz(const ag_object *);
extern size_t ag_object_refc(const ag_object *);
extern size_t ag_object_len(const ag_object *);
extern ag_hash ag_object_hash(const ag_object *);
extern ag_string *ag_object_str(const ag_object *);
extern const ag_memblock *ag_object_payload(const ag_object *);
extern ag_memblock *ag_object_payload_mutable(ag_object **);

inline bool ag_object_empty(const ag_object *ctx)
{
        return !ag_object_len(ctx);
}


#define AG_OBJECT_DEFINE(name)                                                 \
        extern inline name *name ## _copy(const name *);                       \
        extern inline name *name ## _clone(const name *);                      \
        extern inline void name ## _release(name **);                          \
        extern inline enum ag_cmp name ## _cmp(const name *, const name *);    \
        extern inline bool name ## _lt(const name *, const name *);            \
        extern inline bool name ## _eq(const name *, const name *);            \
        extern inline bool name ## _gt(const name *, const name *);            \
        extern inline bool name ## _empty(const name *);                       \
        extern inline ag_typeid name ## _typeid(const name *);                 \
        extern inline ag_typeid name ## _typeid(const name *);                 \
        extern inline ag_uuid *name ## _uuid(const name *);                    \
        extern inline bool name ## _valid(const name *);                       \
        extern inline size_t name ## _sz(const name *);                        \
        extern inline size_t name ## _refc(const name *);                      \
        extern inline size_t name ## _len(const name *);                       \
        extern inline ag_hash name ## _hash(const name *);                     \
        extern inline ag_string *name ## _str(const name *);                   \
        extern void __ ## name ## _register__(void)


#define AG_OBJECT_REGISTER(name) __ ## name ##_register__()


extern void ag_object_registry_init(void);
extern void ag_object_registry_exit(void);

extern const struct ag_object_vtable *ag_object_registry_get(ag_typeid);
extern void ag_object_registry_set(ag_typeid, const struct ag_object_vtable *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_OBJECT_H__ */

