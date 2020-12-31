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



#include "../include/argent.h"


/*
 * Declare the prototypes for the inline functions of the ag_list interface.
 * These inline functions are the aliases of their object counterparts.
 */


extern inline ag_list           *ag_list_copy(const ag_list *);
extern inline ag_list           *ag_list_clone(const ag_list *);
extern inline void               ag_list_release(ag_list **);
extern inline enum ag_cmp        ag_list_cmp(const ag_list *, const ag_list *);
extern inline bool               ag_list_lt(const ag_list *, const ag_list *);
extern inline bool               ag_list_eq(const ag_list *, const ag_list *);
extern inline bool               ag_list_gt(const ag_list *, const ag_list *);
extern inline bool               ag_list_empty(const ag_list *);
extern inline ag_typeid          ag_list_typeid(const ag_list *);
extern inline ag_uuid           *ag_list_uuid(const ag_list *);
extern inline bool               ag_list_valid(const ag_list *);
extern inline size_t             ag_list_sz(const ag_list *);
extern inline size_t             ag_list_refc(const ag_list *);
extern inline size_t             ag_list_len(const ag_list *);
extern inline ag_hash            ag_list_hash(const ag_list *);
extern inline ag_string         *ag_list_str(const ag_list *);
