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


#ifndef __ARGENT_INCLUDE_ALIST_H__
#define __ARGENT_INCLUDE_ALIST_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "./exception.h"
#include "./field.h"
#include "./value.h"


/*
 * Declare the types relevant to the association list interface. ag_alist is an
 * object with a type ID of AG_TYPEID_ALIST, and reifies an association list,
 * which is a simple singly linked list of key-value pairs (represented through
 * the ag_field type). It is declared through the AG_OBJECT_DECLARE() macro,
 * which metaprogrammatically declares the following inherited functions:
 *   - ag_alist_copy()
 *   - ag_alist_clone()
 *   - ag_alist_release()
 *   - ag_alist_cmp()
 *   - ag_alist_lt()
 *   - ag_alist_eq()
 *   - ag_alist_gt()
 *   - ag_alist_empty()
 *   - ag_alist_typeid()
 *   - ag_alist_uuid()
 *   - ag_alist_valid()
 *   - ag_alist_sz()
 *   - ag_alist_refc()
 *   - ag_alist_len()
 *   - ag_alist_hash()
 *   - ag_alist_str()
 *
 * Supporting the ag_alist type are two iterator function callbacks which are
 * used by the mapping functions of the ag_alist interface. Both of these
 * iterator functions have the same signature, differing only in the first
 * parameter. ag_alist_iterator iterates through an immutable association list
 * without modifying any of its nodes, whereas ag_alist_iterator_mutable
 * iterates through an association list, and optionally modifies its nodes.
 *
 * Both of these functions return a Boolean flag indicating whether or not the
 * iteration is to continue. The first parameter is a handle to the contextual
 * association list; the handle signature depends on whether contextual
 * assocation list is immutable or mutable. The second and third parameters are
 * the optional data items to be sent to and retrieved from the iterator.
 */
AG_OBJECT_DECLARE(ag_alist, AG_TYPEID_ALIST);
typedef bool    (ag_alist_iterator)(const ag_field *, void *, void *);
typedef bool    (ag_alist_iterator_mutable)(ag_field **, void *, void *);


/*
 * Declare the non-inherited manager interface for the ag_alist object. The
 * manager interface consists of three related fucntions, each of which helps
 * create a new association list instance.
 *
 * ag_alist_new() creates an association list with a single field, whereas
 * ag_alist_new_array() creates an association list with an array of fields.
 * ag_alist_new_empty() creates an empty association list.
 */
extern ag_alist *ag_alist_new(const ag_field *);
extern ag_alist *ag_alist_new_array(const ag_field **, size_t);
extern ag_alist *ag_alist_new_empty(void);


/*
 * Declare the prototypes of the functions that make up non-inherited accessor 
 * interface for the ag_alist object. Each of these functions takes a pointer to
 * the contextual association list as its first parameter, and with the
 * exception of ag_alist_map(), directly returns either a Boolean value or a
 * field contained within the association list.
 *
 * ag_alist_has(), ag_alist_has_key() and ag_alist_has_val() check whether an
 * association list contains, respectively, a given field, ag given key, and a
 * given value. ag_alist_get() gets the field at the currently iterated node,
 * and ag_alist_get_at() gets the field at a given index. ag_alist_val() gets
 * the value corresponding to a key, and ag_alist_map() runs an immutable
 * iterator across all fields of an association list.
 */
extern bool      ag_alist_has(const ag_alist *, const ag_field *);
extern bool      ag_alist_has_key(const ag_alist *, const ag_value *);
extern bool      ag_alist_has_val(const ag_alist *, const ag_value *);
extern ag_field *ag_alist_get(const ag_alist *);
extern ag_field *ag_alist_get_at(const ag_alist *, size_t);
extern ag_value *ag_alist_val(const ag_alist *, const ag_value *);
extern void      ag_alist_map(const ag_alist *, ag_alist_iterator *, void *,
                    void *);


/*
 * Declare the prototypes for the non-inheritied mutator functions of the
 * ag_alist interface. Each of these functions takes a pointer to an association
 * list pointer as the argument to the first parameter, and return nothing,
 * excepting ag_alist_next().
 *
 * ag_alist_set() sets the field at the currently iterated node of the
 * association list, and ag_alist_set_at() sets the field at a given index of
 * the association list. ag_alist_val_set() sets the value for a given key in
 * the association list. ag_alist_start() starts the internal iterator of an
 * association list, and ag_alist_next() moves the iterator to the next node in
 * the association list. ag_alist_push() pushes a new field to the end of an
 * association list.
 */
extern void     ag_alist_set(ag_alist **, const ag_field *);
extern void     ag_alist_set_at(ag_alist **, const ag_field *, size_t);
extern void     ag_alist_val_set(ag_alist **, const ag_value *,
                    const ag_value *);
extern void     ag_alist_map_mutable(ag_alist **, ag_alist_iterator_mutable *,
                    void *, void *);
extern void     ag_alist_start(ag_alist **);
extern bool     ag_alist_next(ag_alist **);
extern void     ag_alist_push(ag_alist **, const ag_field *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_INCLUDE_ALIST_H__ */

