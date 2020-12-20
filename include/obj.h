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


#ifndef __ARGENT_OBJ_H__
#define __ARGENT_OBJ_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "./argent.h"


typedef struct ag_obj ag_obj;

typedef ag_mblock *(ag_obj_clone_virt)(const ag_mblock *);
typedef void (ag_obj_release_virt)(ag_mblock *);
typedef enum ag_cmp (ag_obj_cmp_virt)(const ag_obj *, const ag_obj *);
typedef bool (ag_obj_valid_virt)(const ag_obj *);
typedef size_t (ag_obj_len_virt)(const ag_obj *);
typedef size_t (ag_obj_hash_virt)(const ag_obj *);
typedef ag_str *(ag_obj_str_virt)(const ag_obj *);


struct ag_obj_vtable {
        ag_mblock       *(*clone)(const ag_mblock *);
        void             (*dispose)(ag_mblock *);
        enum ag_cmp      (*cmp)(const ag_obj *, const ag_obj *);
        bool             (*valid)(const ag_obj *);
        size_t           (*len)(const ag_obj *);
        size_t           (*hash)(const ag_obj *);
        ag_str          *(*str)(const ag_obj *);
};


extern ag_obj *ag_obj_new(size_t, ag_mblock *);
extern ag_obj *ag_obj_copy(const ag_obj *);
extern ag_obj *ag_obj_clone(const ag_obj *);
extern void ag_obj_release(ag_obj **);


extern enum ag_cmp ag_obj_cmp(const ag_obj *, const ag_obj *);

inline bool ag_obj_lt(const ag_obj *ctx, const ag_obj *cmp)
{
        return ag_obj_cmp(ctx, cmp) == AG_CMP_LT;
}

inline bool ag_obj_eq(const ag_obj *ctx, const ag_obj *cmp)
{
        return ag_obj_cmp(ctx, cmp) == AG_CMP_EQ;
}

inline bool ag_obj_gt(const ag_obj *ctx, const ag_obj *cmp)
{
        return ag_obj_cmp(ctx, cmp) == AG_CMP_GT;
}


extern bool ag_obj_valid(const ag_obj *);
extern size_t ag_obj_sz(const ag_obj *);
extern size_t ag_obj_refc(const ag_obj *);
extern size_t ag_obj_len(const ag_obj *);
extern size_t ag_obj_hash(const ag_obj *);
extern ag_str *ag_obj_str(const ag_obj *);
extern const ag_mblock *ag_obj_payload(const ag_obj *);
extern ag_mblock *ag_obj_payload_mutable(ag_obj **);

inline bool ag_obj_empty(const ag_obj *ctx)
{
        return !ag_obj_len(ctx);
}


extern void ag_obj_registry_init(size_t);
extern void ag_obj_registry_exit(void);

extern const struct ag_obj_vtable *ag_obj_registry_get(size_t);

extern void ag_obj_registry_set(size_t, const struct ag_obj_vtable *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_OBJ_H__ */

