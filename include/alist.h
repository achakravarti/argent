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
#include "./field.h"
#include "./value.h"

        
AG_OBJECT_DECLARE(ag_alist, AG_TYPEID_ALIST);
typedef bool    (ag_alist_iterator)(const ag_field *, void *, void *);
typedef bool    (ag_alist_iterator_mutable)(ag_field **, void *, void *);


extern ag_alist *ag_alist_new(const ag_field *);
extern ag_alist *ag_alist_new_array(const ag_field **, size_t);
extern ag_alist *ag_alist_new_empty(void);


extern bool      ag_alist_has(const ag_alist *, const ag_field *);
extern bool      ag_alist_has_key(const ag_alist *, const ag_value *);
extern bool      ag_alist_has_val(const ag_alist *, const ag_value *);
extern ag_field *ag_alist_get(const ag_alist *);
extern ag_field *ag_alist_get_at(const ag_alist *, size_t);
extern ag_value *ag_alist_val(const ag_alist *, const ag_value *);
extern void      ag_alist_map(const ag_alist *, ag_alist_iterator *, void *,
                    void *);


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

#endif /* !__ARGENT_ALIST_H__ */

