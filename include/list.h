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


#ifndef __ARGENT_LIST_H__
#define __ARGENT_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "./argent.h"


typedef ag_object       ag_list;
typedef void            (ag_list_iterator)(const ag_value *, void *);
typedef void            (ag_list_iterator_mutable)(ag_value **, void *);


extern ag_list *ag_list_new(void);

inline ag_list *ag_list_copy(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_copy(ctx);
}

inline ag_list *ag_list_clone(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_clone(ctx);
}

inline void ag_list_release(ag_list **ctx)
{
        ag_object_release(ctx);
}


inline enum ag_cmp ag_list_cmp(const ag_list *ctx, const ag_list *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_LIST);

        return ag_object_cmp(ctx, cmp);
}

inline bool ag_list_lt(const ag_list *ctx, const ag_list *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_LIST);

        return ag_object_lt(ctx, cmp);
}

inline bool ag_list_eq(const ag_list *ctx, const ag_list *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_LIST);

        return ag_object_eq(ctx, cmp);
}

inline bool ag_list_gt(const ag_list *ctx, const ag_list *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_gt(ctx, cmp);
}

inline bool ag_list_empty(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_empty(ctx);
}

inline ag_typeid ag_list_typeid(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_typeid(ctx);
}

inline ag_uuid *ag_list_uuid(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_uuid(ctx);
}

inline bool ag_list_valid(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_valid(ctx);
}

inline size_t ag_list_sz(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_sz(ctx);
}

inline size_t ag_list_refc(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_refc(ctx);
}

inline size_t ag_list_len(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_len(ctx);
}

inline ag_hash ag_list_hash(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_hash(ctx);
}

inline ag_string *ag_list_str(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_str(ctx);
}

extern ag_value *ag_list_get(const ag_list *);
extern ag_value *ag_list_get_at(const ag_list *, size_t);
extern void ag_list_map(const ag_list *, ag_list_iterator *, void *);

extern void ag_list_set(ag_list **, const ag_value *);
extern void ag_list_set_at(ag_list **, size_t, const ag_value *);
extern void ag_list_map_mutable(ag_list **, ag_list_iterator_mutable *, void *);
extern void ag_list_start(ag_list **);
extern bool ag_list_next(ag_list **);
extern void ag_list_push(ag_list **, const ag_value *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_LIST_H__ */

