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


#ifndef __ARGENT_INCLUDE_TYPEID_H__
#define __ARGENT_INCLUDE_TYPEID_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef int ag_typeid;


#define AG_TYPEID_OBJECT        ((ag_typeid)  0)
#define AG_TYPEID_FIELD         ((ag_typeid) -1)
#define AG_TYPEID_LIST          ((ag_typeid) -2)
#define AG_TYPEID_ALIST         ((ag_typeid) -3)
#define AG_TYPEID_HTTP_URL      ((ag_typeid) -4)
#define AG_TYPEID_HTTP_CLIENT   ((ag_typeid) -5)
#define AG_TYPEID_HTTP_REQUEST  ((ag_typeid) -6)
#define AG_TYPEID_HTTP_RESPONSE ((ag_typeid) -7)
#define AG_TYPEID_PLUGIN        ((ag_typeid) -8)


#ifdef __cplusplus
}
#endif

#endif /* __ARGENT_INCLUDE_TYPEID_H__ */

