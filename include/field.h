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


AG_OBJECT_DECLARE(ag_field, AG_TYPEID_FIELD);

        
extern ag_field *ag_field_new(const ag_value *, const ag_value *);
                

extern ag_value *ag_field_key(const ag_field *);
extern ag_value *ag_field_val(const ag_field *);
                

extern void     ag_field_key_set(ag_field **, const ag_value *);
extern void     ag_field_val_set(ag_field **, const ag_value *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_FIELD_H__ */

