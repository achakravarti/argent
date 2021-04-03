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


#ifndef __ARGENT_INCLUDE_UUID_H__
#define __ARGENT_INCLUDE_UUID_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "../type/primitives.h"
#include "./hash.h"
#include "./string.h"


typedef struct ag_uuid ag_uuid;


extern ag_uuid *ag_uuid_new(void);
extern ag_uuid *ag_uuid_new_empty(void);
extern ag_uuid *ag_uuid_parse(const char *);
ag_uuid *ag_uuid_copy(const ag_uuid *);
extern ag_uuid *ag_uuid_clone(const ag_uuid *);
extern void ag_uuid_release(ag_uuid **);


extern enum ag_cmp ag_uuid_cmp(const ag_uuid *, const ag_uuid *);

inline bool ag_uuid_lt(const ag_uuid *ctx, const ag_uuid *cmp)
{
        return ag_uuid_cmp(ctx, cmp) == AG_CMP_LT;
}

inline bool ag_uuid_eq(const ag_uuid *ctx, const ag_uuid *cmp)
{
        return ag_uuid_cmp(ctx, cmp) == AG_CMP_EQ;
}

inline bool ag_uuid_gt(const ag_uuid *ctx, const ag_uuid *cmp)
{
        return ag_uuid_cmp(ctx, cmp) == AG_CMP_GT;
}


extern bool ag_uuid_empty(const ag_uuid *);
extern ag_hash ag_uuid_hash(const ag_uuid *);
extern ag_string *ag_uuid_str(const ag_uuid *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_INCLUDE_UUID_H__ */
