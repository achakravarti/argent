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


#ifndef __ARGENT_FIELD_H__
#define __ARGENT_FIELD_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "./exception.h"
#include "./value.h"


/*
 * Declare the ag_field type as an object with a type ID of AG_TYPEID_FIELD.
 * This object reifies a key-value pair (also referred to as a field). Since
 * both the key and value of a field object are ag_value instances, the field
 * object allows flexible combinations of keys and values.
 *
 * In order to distinguish between a value type and the value component of a
 * field, the latter is often abbreviated to 'val' in the relevant contexts.
 *
 * We use the AG_OBJECT_DECLARE() macro to declare the field type as an object,
 * and to metaprogrammatically declare its inherited object functions, which
 * are:
 *   - ag_fieldcopy()
 *   - ag_field_clone()
 *   - ag_field_release()
 *   - ag_field_cmp()
 *   - ag_field_lt()
 *   - ag_field_eq()
 *   - ag_field_gt()
 *   - ag_field_empty()
 *   - ag_field_typeid()
 *   - ag_field_uuid()
 *   - ag_field_valid()
 *   - ag_field_sz()
 *   - ag_field_refc()
 *   - ag_field_len()
 *   - ag_field_hash()
 *   - ag_field_str()
 */


AG_OBJECT_DECLARE(ag_field, AG_TYPEID_FIELD);


/*
 * Declare the non-inherited manager interface for the ag_field object. There is
 * only one such function, ag_field_new(), which is responsible for creating a
 * field object with a given key and value.
 */


extern ag_field *ag_field_new(const ag_value *, const ag_value *);
                

/*
 * Declare the non-inherited accessor functions of the ag_field object. There
 * are only two accessor functions, ag_field_key() and ag_field_val(), returning
 * the key and value of a field object respectively.
 */


extern ag_value *ag_field_key(const ag_field *);
extern ag_value *ag_field_val(const ag_field *);
                

/*
 * Declare the non-inherited mutator functions of the ag_field object. Similar
 * to the accessor functions, ag_field_key_set() and ag_field_val_set() set the
 * key and value of a field object respectively.
 */


extern void     ag_field_key_set(ag_field **, const ag_value *);
extern void     ag_field_val_set(ag_field **, const ag_value *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_FIELD_H__ */

