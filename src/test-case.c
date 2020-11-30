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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ag_test_case {
        ag_test *test;
        char *desc;
        enum ag_test_status stat;
};


static inline char *str_new(const char *src)
{
        size_t sz = strlen(src);
        char *ret = malloc(sz + 1);
        if (!ret)
                exit(1);

        strncpy(ret, src, sz);
        return ret;
}


static inline void str_dispose(char *ctx)
{
        if (ctx)
                free(ctx);
}


extern ag_test_case *ag_test_case_new(const char *desc, ag_test *test)
{
        ag_test_case *ret = malloc(sizeof *ret);
        if (!ret)
                exit(1);

        ret->test = test;
        ret->desc = str_new(desc);
        ret->stat = AG_TEST_STATUS_WAIT;

        return ret;
}

extern ag_test_case *ag_test_case_copy(const ag_test_case *ctx)
{
        ag_test_case *ret = malloc(sizeof *ret);
        if (!ret)
                exit(1);

        ret->test = ctx->test;
        ret->desc = str_new(ctx->desc);
        ret->stat = ctx->stat;

        return ret;
}

extern void ag_test_case_dispose(ag_test_case **ctx)
{
        ag_test_case *hnd;

        if (ctx && (hnd = *ctx)) {
                str_dispose(hnd->desc);
                free(hnd);
                *ctx = NULL;
        }
}

extern enum ag_test_status ag_test_case_status(const ag_test_case *ctx)
{
        return ctx->stat;
}

extern char *ag_test_case_msg(const ag_test_case *ctx)
{
        return str_new("WAIT");
}

extern void ag_test_case_exec(ag_test_case *ctx)
{
        ctx->stat = ctx->test();
}

