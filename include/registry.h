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


#ifndef __ARGENT_INCLUDE_REGISTRY_H__
#define __ARGENT_INCLUDE_REGISTRY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "./primitives.h"


typedef struct ag_registry      ag_registry;
typedef void                    (ag_registry_release_cbk)(void *);

extern ag_registry      *ag_registry_new(size_t, ag_registry_release_cbk *);
extern void              ag_registry_release(ag_registry **);

extern void     *ag_registry_get(size_t);

extern void     ag_registry_push(size_t, void *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_INCLUDE_REGISTRY_H__ */

