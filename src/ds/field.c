/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Argent---infrastructure for building web services
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
 ******************************************************************************/


#include "../../include/argent.h"

struct payload {
        ag_value        *key;
        ag_value        *val;
};

static struct payload   *payload_new(const ag_value *, const ag_value *);


AG_OBJECT_DEFINE_CLONE(ag_field,
        const struct payload *p = _p_;
        return payload_new(p->key, p->val);
);

AG_OBJECT_DEFINE_RELEASE(ag_field,
        struct payload *p = _p_;
        ag_value_release(&p->key);
        ag_value_release(&p->val);
);

AG_OBJECT_DEFINE_CMP(ag_field,
        const struct payload *p1 = ag_object_payload(_o1_);
        const struct payload *p2 = ag_object_payload(_o2_);

        if (ag_value_eq(p1->key, p2->key))
                return ag_value_cmp(p1->val, p2->val);

        return ag_value_lt(p1->key, p2->key) ? AG_CMP_LT : AG_CMP_GT;
);

AG_OBJECT_DEFINE_VALID(ag_field,
        const struct payload *p = ag_object_payload(_o_);
        return ag_value_valid(p->key) && ag_value_valid(p->val);
);

AG_OBJECT_DEFINE_SZ(ag_field,
        const struct payload *p = ag_object_payload(_o_);
        return ag_value_sz(p->key) + ag_value_sz(p->val);
);

AG_OBJECT_DEFINE_LEN(ag_field,
        const struct payload *p = ag_object_payload(_o_);
        return ag_value_len(p->val);
);

AG_OBJECT_DEFINE_HASH(ag_field,
        const struct payload *p = ag_object_payload(_o_);
        return ag_value_hash(p->key);
);

AG_OBJECT_DEFINE_STR(ag_field,
        const struct payload *p = ag_object_payload(_o_);
        AG_AUTO(ag_string) *key = ag_value_str(p->key);
        AG_AUTO(ag_string) *val = ag_value_str(p->val);

        return ag_string_new_fmt("%s:%s", key, val);
);

AG_OBJECT_DEFINE(ag_field, AG_TYPEID_FIELD);


/*
 * Define the ag_field_new() interface function. This function is responsible
 * for creating a new field with a given key and value, specified through its
 * parameters. Both key and value can be of any acceptable value type, and don't
 * need to be of the same type.
 */
extern ag_field *
ag_field_new(const ag_value *key, const ag_value *val)
{
        AG_ASSERT_PTR (key);
        AG_ASSERT_PTR (val);

        return ag_object_new(AG_TYPEID_FIELD, payload_new(key, val));
}


/*
 * Define the ag_field_parse() interface function. This function creates a new
 * field object instance by parsing a given string with a specific separator.
 * The string to parse is passed through the first parameter, and the separator
 * is specified by the second parameter. The arguments to both parameters need
 * to be valid non-empty strings, and the separator needs to exist in the source
 * string.
 *
 * As of now, parsing a string results in a field whose key and value are both
 * string types, but this may change later to allow greater flexibility, if
 * required.
 */
extern ag_field *
ag_field_parse(const char *src, const char *sep)
{
        AG_ASSERT_STR (src);
        AG_ASSERT_STR (sep);

        AG_AUTO(ag_string) *s = ag_string_new(src);
        AG_AUTO(ag_string) *k;
        AG_AUTO(ag_string) *v;

        
        if (ag_string_has(s, sep)) {
                k = ag_string_split(s, sep);
                v = ag_string_split_right(s, sep);
        } else {
                k = ag_string_new(s);
                v = ag_string_new_empty();
        }
                
        AG_AUTO(ag_value) *kv = ag_value_new_string(k);
        AG_AUTO(ag_value) *vv = ag_value_new_string(v);

        return ag_object_new(AG_TYPEID_FIELD, payload_new(kv, vv));
}


extern ag_value *ag_field_key(const ag_field *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        const struct payload *p = ag_object_payload(ctx);
        return ag_value_copy(p->key);
}


extern ag_value *ag_field_val(const ag_field *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_FIELD);

        const struct payload *p = ag_object_payload(ctx);
        return ag_value_copy(p->val);
}


extern void ag_field_key_set(ag_field **ctx, const ag_value *key)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT (ag_object_typeid(*ctx) == AG_TYPEID_FIELD);

        struct payload *p = ag_object_payload_mutable(ctx);
        ag_value_release(&p->key);
        p->key = ag_value_copy(key);
}


extern void ag_field_val_set(ag_field **ctx, const ag_value *val)
{
        AG_ASSERT_PTR (ctx && *ctx);
        AG_ASSERT (ag_object_typeid(*ctx) == AG_TYPEID_FIELD);

        struct payload *p = ag_object_payload_mutable(ctx);
        ag_value_release(&p->val);
        p->val = ag_value_copy(val);
}


static struct payload *
payload_new(const ag_value *key, const ag_value *val)
{
        AG_ASSERT_PTR (key);
        AG_ASSERT_PTR (val);

        struct payload *p = ag_memblock_new(sizeof *p);
        p->key = ag_value_copy(key);
        p->val = ag_value_copy(val);

        return p;
}

