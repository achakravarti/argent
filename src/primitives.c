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


#include "../include/argent.h"

#include <errno.h>
#include <inttypes.h>
#include <math.h>


extern inline bool              ag_int_lt(ag_int, ag_int);
extern inline bool              ag_int_eq(ag_int, ag_int);
extern inline bool              ag_int_gt(ag_int, ag_int);
extern inline enum ag_cmp       ag_int_cmp(ag_int, ag_int);

extern inline bool              ag_uint_lt(ag_uint ctx, ag_uint cmp);
extern inline bool              ag_uint_eq(ag_uint ctx, ag_uint cmp);
extern inline bool              ag_uint_gt(ag_uint ctx, ag_uint cmp);
extern inline enum ag_cmp       ag_uint_cmp(ag_uint ctx, ag_uint cmp);

extern inline enum ag_cmp       ag_float_cmp(ag_float, ag_float);


extern ag_int
ag_int_parse(const char *str)
{
        AG_ASSERT_STR (str);

        ag_int num = strtoimax(str, NULL, 10);

        struct ag_exception_parse x = {.str = str, .ctx = "ag_int"};
        AG_REQUIRE_OPT (!(num == INTMAX_MAX && errno == ERANGE), AG_ERNO_PARSE,
            &x);

        return num;
}


extern ag_uint
ag_uint_parse(const char *str)
{
        AG_ASSERT_STR (str);

        ag_uint num = strtoumax(str, NULL, 10);
        
        struct ag_exception_parse x = {.str = str, .ctx = "ag_uint"};
        AG_REQUIRE_OPT (!(num == UINTMAX_MAX && errno == ERANGE), AG_ERNO_PARSE,
            &x);

        return num;
}


extern bool
ag_float_lt(ag_float ctx, ag_float cmp)
{
        /* https://stackoverflow.com/questions/17333 */
        return ((cmp - ctx) > ((fabs(ctx) < fabs(cmp) 
            ? fabs(cmp) : fabs(ctx)) * DBL_EPSILON));
}


extern bool
ag_float_eq(ag_float ctx, ag_float cmp)
{
        /* https://stackoverflow.com/questions/17333 */
        return (fabs(ctx - cmp) <= ((fabs(ctx) > fabs(cmp) 
            ? fabs(cmp) : fabs(ctx)) * DBL_EPSILON));
}


extern bool
ag_float_gt(ag_float ctx, ag_float cmp)
{
        /* https://stackoverflow.com/questions/17333 */
        return ((ctx - cmp) > ( (fabs(ctx) < fabs(cmp) 
            ? fabs(cmp) : fabs(ctx)) * DBL_EPSILON));
}

