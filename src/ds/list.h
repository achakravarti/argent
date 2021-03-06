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


#ifndef __ARGENT_INCLUDE_LIST_H__
#define __ARGENT_INCLUDE_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "../ex/exception.h"
#include "../type/value.h"


/*
 * Declare the types associated with the list interface. ag_list is an opaque
 * object representing a list that can hold any value type. For convenience, we
 * have also typedef'd two callback functions that serve as iterators to a list.
 * ag_list_iterator is a callback that iterates through an immutable list, and
 * ag_list_iterator_mutable is a callback that iterates through a mutable list.
 */
AG_OBJECT_DECLARE(ag_list, AG_TYPEID_LIST);
typedef bool    (ag_list_iterator)(const ag_value *, void *, void *);
typedef bool    (ag_list_iterator_mutable)(ag_value **, void *, void *);


/*
 * Declare the manager interface for ag_list. ag_list_new() creates a new empty
 * list instance. The remaining manager functions, which are aliases of their
 * object counterparts, are automatically generated by AG_OBJECT_DECLARE().
 */
extern ag_list  *ag_list_new(void);


/*
 * Declare the accessor interface for ag_list. The ag_list_get() and
 * ag_list_get_at() functions are used to retrieve a value from a list, and
 * ag_list_map() is used to run an iterator across an immutable list instance..
 */
extern ag_value *ag_list_get(const ag_list *);
extern ag_value *ag_list_get_at(const ag_list *, size_t);
extern void      ag_list_map(const ag_list *, ag_list_iterator *, void *,
                    void *);


/*
 * Declare the mutator interface for ag_list. ag_list_set() and ag_list_set_at()
 * are used to set a value in a list, ag_list_push() is used to push a value to
 * the end of a list, ag_list_map_mutable() is used to iterate across a mutable
 * list, ag_list_start() and ag_list_next() are used to traverse across a list.
 */
extern void     ag_list_set(ag_list **, const ag_value *);
extern void     ag_list_set_at(ag_list **, const ag_value *, size_t);
extern void     ag_list_map_mutable(ag_list **, ag_list_iterator_mutable *,
                    void *, void *);
extern void     ag_list_start(ag_list **);
extern bool     ag_list_next(ag_list **);
extern void     ag_list_push(ag_list **, const ag_value *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_INCLUDE_LIST_H__ */

