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


#ifndef __ARGENT_ERNO_H__
#define __ARGENT_ERNO_H__


#ifdef __cplusplus
        extern "C" {
#endif

typedef int ag_erno;

#define AG_ERNO_MSG(p) #p ": " p##_MSG

#define AG_ERNO_NULL ((ag_erno) 0)
#define AG_ERNO_MBLOCK ((ag_erno) -1)

#define AG_ERNO_MBLOCK_MSG "failed to allocate memory block"



#ifdef __cplusplus
        }
#endif

#endif /* !__ARGENT_ERNO_H__ */
