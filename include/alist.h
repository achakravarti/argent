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


#ifndef __ARGENT_ALIST_H__
#define __ARGENT_ALIST_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "./exception.h"
#include "./value.h"

        
typedef ag_object       ag_alist;
typedef void            (ag_alist_iterator)(const ag_value *, const ag_value *,
                            void *);
typedef void            (ag_alist_iterator_mutable)(ag_value **, ag_value **,
                            void *);


extern void      __ag_alist_register__(void);
extern ag_alist  *ag_alist_new(void);


inline ag_alist *
ag_alist_copy(const ag_alist *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_copy(ctx);
}


inline ag_alist *
ag_alist_clone(const ag_alist *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_clone(ctx);
}


inline void
ag_alist_release(ag_alist **ctx)
{
        ag_object_release(ctx);
}

        
inline enum ag_cmp
ag_alist_cmp(const ag_alist *ctx, const ag_alist *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_LIST);

        return ag_object_cmp(ctx, cmp);
}


inline bool
ag_alist_lt(const ag_alist *ctx, const ag_alist *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_LIST);

        return ag_object_lt(ctx, cmp);
}


inline bool
ag_alist_eq(const ag_alist *ctx, const ag_alist *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_LIST);

        return ag_object_eq(ctx, cmp);
}


inline bool
ag_alist_gt(const ag_alist *ctx, const ag_alist *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_gt(ctx, cmp);
}

        
inline bool
ag_alist_empty(const ag_alist *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_empty(ctx);
}


inline ag_typeid
ag_alist_typeid(const ag_alist *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_typeid(ctx);
}


inline ag_uuid *
ag_alist_uuid(const ag_alist *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_uuid(ctx);
}


inline bool
ag_alist_valid(const ag_alist *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_valid(ctx);
}


inline size_t
ag_alist_sz(const ag_alist *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_sz(ctx);
}


inline size_t
ag_alist_refc(const ag_alist *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_refc(ctx);
}


inline size_t
ag_alist_len(const ag_alist *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_len(ctx);
}


inline ag_hash
ag_alist_hash(const ag_alist *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_hash(ctx);
}


inline ag_string *
ag_alist_str(const ag_alist *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_str(ctx);
}


extern ag_value *ag_alist_get(const ag_alist *);
extern ag_value *ag_alist_get_at(const ag_alist *, size_t);
extern void      ag_alist_map(const ag_alist *, ag_alist_iterator *, void *);


extern void     ag_alist_set(ag_alist **, const ag_value *);
extern void     ag_alist_set_at(ag_alist **, const ag_value *, size_t);
extern void     ag_alist_map_mutable(ag_alist **, ag_alist_iterator_mutable *,
                    void *);
extern void     ag_alist_start(ag_alist **);
extern bool     ag_alist_next(ag_alist **);
extern void     ag_alist_push(ag_alist **, const ag_value *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_ALIST_H__ */

