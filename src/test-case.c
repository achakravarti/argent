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


/*
 * File: argent/src/test-case.c
 *
 * This file contains the implementation of the test case interface of the
 * Testing Module of the Argent Library. See argent/include/test.h for an
 * overview description of this interface.
 */


#include "../include/argent.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * struct ag_test_case: internal structure of `ag_test_case` instance.
 *
 * @test: unit test callback.
 * @desc: unit test description.
 * @stat: unit test execution status.
 */
struct ag_test_case {
        ag_test *test;
        char *desc;
        enum ag_test_status stat;
};


/*
 * str_new(): create new dynamic string.
 *
 * @src: static source string.
 *
 * Return: new dynamic string.
 */
static inline char *str_new(const char *src)
{
        size_t sz = strlen(src);
        char *ret = malloc(sz + 1);
        if (!ret)
                exit(1);

        strncpy(ret, src, sz);
        return ret;
}


/*
 * str_new_fmt(): create new dynamic formatted string.
 *
 * @fmt: formatted static source string.
 * @...: format arguments.
 *
 * Return: new dynamic formatted string.
 */
static char *str_new_fmt(const char *fmt, ...)
{
        va_list args;

        va_start(args, fmt);
        char *bfr = malloc(vsnprintf(NULL, 0, fmt, args) + 1);
        va_end(args);

        va_start(args, fmt);
        (void) vsprintf(bfr, fmt, args);
        va_end(args);

        return bfr;
}


/*
 * str_dispose(): dispose dynamic string.
 *
 * @ctx: contextual string.
 */
static inline void str_dispose(char *ctx)
{
        if (ctx)
                free(ctx);
}


/*
 * ag_test_case_new(): create new test case.
 *
 * @desc:
 * @test:
 *
 * Return:
 */
extern ag_test_case *ag_test_case_new(ag_test *test)
{
        ag_test_case *ret = malloc(sizeof *ret);
        if (!ret)
                exit(1);

        ret->test = test;
        ret->desc = str_new("waiting to execute test case");
        ret->stat = AG_TEST_STATUS_WAIT;

        return ret;
}


/*
 * ag_test_case_copy(): create deep copy of test case.
 *
 * @ctx: contextual test case.
 *
 * Return:
 */
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


/*
 * ag_test_case_dispose(): dispose test case.
 *
 * @ctx: contextual test case.
 */
extern void ag_test_case_dispose(ag_test_case **ctx)
{
        ag_test_case *hnd;

        if (ctx && (hnd = *ctx)) {
                str_dispose(hnd->desc);
                free(hnd);
                *ctx = NULL;
        }
}


/*
 * ag_test_case_status(): get execution status of test case.
 *
 * @ctx: contextual test case.
 *
 * Return: AG_TEST_STATUS_OK      - @ctx executed successfully,
 *         AG_TEST_STATUS_WAIT    - @ctx not yet executed,
 *         AG_TEST_STATUS_SKIP    - @ctx execution skipped,
 *         AG_TEST_STATUS_FAIL    - @ctx execution failed,
 *         AG_TEST_STATUS_SIGABRT - @ctx execution failed with SIGABRT,
 *         AG_TEST_STATUS_SIGSEGV - @ctx execution failed with SIGSEGV.
 */
extern enum ag_test_status ag_test_case_status(const ag_test_case *ctx)
{
        return ctx->stat;
}


/*
 * ag_test_case_desc(): get description of test case.
 *
 * @ctx: contextual test case.
 *
 * Return: description of @ctx.
 */
extern char *ag_test_case_desc(const ag_test_case *ctx)
{
        return str_new(ctx->desc);
}


/*
 * ag_test_case_msg(): get execution result message of test case.
 *
 * @ctx: contextual test case.
 *
 * Return: execution result message of @ctx.
 */
extern char *ag_test_case_msg(const ag_test_case *ctx)
{
        switch (ctx->stat) {
                case AG_TEST_STATUS_OK:
                        return str_new_fmt("[OK]   %s", ctx->desc);

                case AG_TEST_STATUS_WAIT:
                        return str_new_fmt("[WAIT] %s", ctx->desc);

                case AG_TEST_STATUS_SKIP:
                        return str_new_fmt("[SKIP] %s", ctx->desc);

                case AG_TEST_STATUS_SIGABRT:
                        return str_new_fmt("[FAIL] %s (SIGABRT)", ctx->desc);

                case AG_TEST_STATUS_SIGSEGV:
                        return str_new_fmt("[FAIL] %s (SIGSEGV)", ctx->desc);

                default:
                        return str_new_fmt("[FAIL] %s", ctx->desc);
        }
}


/*
 * ag_test_case_desc_set(): set description of test case.
 *
 * @ctx : contextual test case.
 * @desc: test case description.
 */
extern void ag_test_case_desc_set(ag_test_case *ctx, const char *desc)
{
        str_dispose(ctx->desc);
        ctx->desc = str_new(desc);
}


/*
 * ag_test_case_exec(): execute test case.
 *
 * @ctx: contextual test case.
 */
extern void ag_test_case_exec(ag_test_case *ctx)
{
        ctx->stat = ctx->test(ctx);
}

