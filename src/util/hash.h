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


#ifndef __ARGENT_INCLUDE_HASH_H__
#define __ARGENT_INCLUDE_HASH_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>


typedef size_t ag_hash;

extern ag_hash ag_hash_new(size_t);
extern ag_hash ag_hash_new_str(const char *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_INCLUDE_HASH_H__ */

