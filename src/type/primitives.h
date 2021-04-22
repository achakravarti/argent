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


#ifndef __ARGENT_INCLUDE_PRIMITIVES_H__
#define __ARGENT_INCLUDE_PRIMITIVES_H__


#ifdef __cplusplus
        extern "C" {
#endif

#include "../base/base.h"
#include <float.h>
#include <stdint.h>

typedef intptr_t ag_int;
typedef uintptr_t ag_uint;
typedef double ag_float;


#define AG_UINT_MIN ((ag_uint)0)
#define AG_UINT_MAX UINTPTR_MAX

#define AG_INT_MIN ((ag_int)(-(AG_UINT_MAX / (ag_uint)2)))
#define AG_INT_MAX ((ag_int)((AG_UINT_MAX / (ag_uint)2) - (ag_uint)1))

#define AG_FLOAT_MIN DBL_MIN
#define AG_FLOAT_MAX DBL_MAX

 ag_int ag_int_parse(const char *);

extern ag_uint ag_uint_parse(const char *);

inline bool ag_int_lt(ag_int ctx, ag_int cmp)
{
    return ctx < cmp;
}

inline bool ag_int_eq(ag_int ctx, ag_int cmp)
{
    return ctx == cmp;
}

inline bool ag_int_gt(ag_int ctx, ag_int cmp)
{
    return ctx > cmp;
}

inline enum ag_cmp ag_int_cmp(ag_int ctx, ag_int cmp)
{
    if (ctx == cmp)
        return AG_CMP_EQ;

    return ctx < cmp ? AG_CMP_LT: AG_CMP_GT;
}

inline bool ag_uint_lt(ag_uint ctx, ag_uint cmp)
{
    return ctx < cmp;
}

inline bool ag_uint_eq(ag_uint ctx, ag_uint cmp)
{
    return ctx == cmp;
}

inline bool ag_uint_gt(ag_uint ctx, ag_uint cmp)
{
    return ctx > cmp;
}

inline enum ag_cmp ag_uint_cmp(ag_uint ctx, ag_uint cmp)
{
    if (ctx == cmp)
        return AG_CMP_EQ;

    return ctx < cmp ? AG_CMP_LT: AG_CMP_GT;
}

extern bool ag_float_lt(ag_float, ag_float);
extern bool ag_float_eq(ag_float, ag_float);
extern bool ag_float_gt(ag_float, ag_float);

inline enum ag_cmp ag_float_cmp(ag_float ctx, ag_float cmp)
{
    if (ag_float_eq(ctx, cmp))
        return AG_CMP_EQ;

    return ag_float_lt(ctx, cmp) ? AG_CMP_LT: AG_CMP_GT;
}


#ifdef __cplusplus
        }
#endif

#endif /* !__ARGENT_INCLUDE_PRIMITIVE_H__ */

