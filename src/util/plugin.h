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


#ifndef __ARGENT_INCLUDE_PLUGIN_H__
#define __ARGENT_INCLUDE_PLUGIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../type/object.h"


AG_OBJECT_DECLARE(ag_plugin, AG_TYPEID_PLUGIN);

extern ag_plugin        *ag_plugin_new(const char *, const char *);
extern ag_plugin        *ag_plugin_new_local(const char *);

extern ag_string        *ag_plugin_dso(const ag_plugin *);
extern ag_string        *ag_plugin_sym(const ag_plugin *);
extern void             *ag_plugin_hnd(const ag_plugin *);

#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_INCLUDE_PLUGIN_H__ */

