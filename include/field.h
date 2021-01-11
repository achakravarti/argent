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


#include "./exception.h"
#include "./value.h"


typedef ag_object       ag_field;

extern void      __ag_field_register__(void);
extern ag_field *ag_field_new(const ag_value *, const ag_value *);

inline ag_field *
ag_field_copy(const ag_field *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        return ag_object_copy(ctx);
}

inline ag_field *
ag_field_clone(const ag_field *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        return ag_object_clone(ctx);
}


inline void
ag_field_release(ag_field **ctx)
{
        ag_object_release(ctx);
}

        
inline enum ag_cmp
ag_field_cmp(const ag_field *ctx, const ag_field *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_FIELD);

        return ag_object_cmp(ctx, cmp);
}


inline bool
ag_field_lt(const ag_field *ctx, const ag_field *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_FIELD);

        return ag_object_lt(ctx, cmp);
}


inline bool
ag_field_eq(const ag_field *ctx, const ag_field *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_FIELD);

        return ag_object_eq(ctx, cmp);
}


inline bool
ag_field_gt(const ag_field *ctx, const ag_field *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        return ag_object_gt(ctx, cmp);
}

        
inline bool
ag_field_empty(const ag_field *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        return ag_object_empty(ctx);
}


inline ag_typeid
ag_field_typeid(const ag_field *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        return ag_object_typeid(ctx);
}


inline ag_uuid *
ag_field_uuid(const ag_field *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        return ag_object_uuid(ctx);
}


inline bool
ag_field_valid(const ag_field *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        return ag_object_valid(ctx);
}


inline size_t
ag_field_sz(const ag_field *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        return ag_object_sz(ctx);
}


inline size_t
ag_field_refc(const ag_field *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        return ag_object_refc(ctx);
}


inline size_t
ag_field_len(const ag_field *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        return ag_object_len(ctx);
}


inline ag_hash
ag_field_hash(const ag_field *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        return ag_object_hash(ctx);
}


inline ag_string *
ag_field_str(const ag_field *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        return ag_object_str(ctx);
}

extern ag_value *ag_field_key(const ag_field *);
extern ag_value *ag_field_val(const ag_field *);

extern void ag_field_key_set(ag_field **, const ag_value *);
extern void ag_field_val_set(ag_field **, const ag_value *);

