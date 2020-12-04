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

/*-
 * File: argent/src/test-harness.c
 *
 * This file contains the implementation of the test harness interface of the
 * Testing Module of the Argent Library. See argent/include/test.h for an
 * overview description of this interface.
 */


#include "../include/argent.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


/*
 * struct node: node in list of test suites.
 *
 * @ts : test suite.
 * @nxt: next node in list.
 */
struct node {
        ag_test_suite *ts;
        struct node *nxt;
};


/*
 * node_new(): create new test suite list node.
 *
 * @ts: test suite.
 *
 * Return: new test suite list node.
 */
static inline struct node *node_new(const ag_test_suite *ts)
{
        struct node *n = malloc(sizeof *n);
        n->ts = ag_test_suite_copy(ts);
        n->nxt = NULL;

        return n;
}


/*
 * node_copy(): create deep copy of test suite list node.
 *
 * @ctx: contextual test suite list node.
 *
 * Return: copy of @ctx.
 */
static inline struct node *node_copy(const struct node *ctx)
{
        struct node *n = malloc(sizeof *n);
        n->ts = ag_test_suite_copy(ctx->ts);
        n->nxt = ctx->nxt;

        return n;
}


/*
 * node_dispose(): dispose test suite list node.
 *
 * @ctx: contextual test suite list node.
 */
static inline void node_dispose(struct node *ctx)
{
        ag_test_suite_dispose(&ctx->ts);
        free(ctx);
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
 * struct ag_test_harness: internal structure of `ag_test_harness`.
 *
 * @head: head of test suite list.
 */
struct ag_test_harness {
        struct node *head;
};


/*
 * ag_test_harness_new(): create new test harness.
 *
 * Return: new test harness.
 */
extern ag_test_harness *ag_test_harness_new(void)
{
        ag_test_harness *ctx = malloc(sizeof *ctx);
        ctx->head = NULL;

        return ctx;
}


/*
 * ag_test_harness_copy(): create deep copy of test harness.
 *
 * @ctx: contextual test harness.
 *
 * Return: copy of @ctx.
 */
extern ag_test_harness *ag_test_harness_copy(const ag_test_harness *ctx)
{
        ag_test_harness *cp = ag_test_harness_new();

        struct node *n = ctx->head;
        while (n) {
                ag_test_harness_push(cp, n->ts);
                n = n->nxt;
        }

        return cp;
}


/*
 * ag_test_suite_dispose(): dispose test suite.
 *
 * @ctx: contextual test suite.
 */
extern void ag_test_harness_dispose(ag_test_harness **ctx)
{
        ag_test_harness *hnd;

        if (ctx && (hnd = *ctx)) {
                struct node *n1 = hnd->head, *n2;
                while (n1) {
                        n2 = n1->nxt;
                        node_dispose(n1);
                        n1 = n2;
                }

                free(hnd);
                *ctx = NULL;
        }
}


/*
 * ag_test_harness_len(): get number of test suites in test harness.
 *
 * @ctx: contextual test harness.
 *
 * Return: number of test suites in @ctx.
 */
extern int ag_test_harness_len(const ag_test_harness *ctx)
{
        register size_t len = 0;

        struct node *n = ctx->head;
        while (n) {
                len++;
                n = n->nxt;
        }

        return len;
}


/*
 * ag_test_harness_pass(): get number of passed test suites in test harness.
 *
 * @ctx: contextual test harness.
 *
 * Return: number of passed test suites in @ctx.
 */
extern int ag_test_harness_pass(const ag_test_harness *ctx)
{
        register int pass = 0;

        struct node *n = ctx->head;
        while (n) {
                pass += ag_test_suite_pass(n->ts);
                n = n->nxt;
        }

        return pass;
}


/*
 * ag_test_harness_skip(): get number of skipped test suites in test harness.
 *
 * @ctx: contextual test harness.
 *
 * Return: number of skipped test suites in @ctx.
 */
extern int ag_test_harness_skip(const ag_test_harness *ctx)
{
        register int skip = 0;

        struct node *n = ctx->head;
        while (n) {
                skip += ag_test_suite_skip(n->ts);
                n = n->nxt;
        }

        return skip;
}


/*
 * ag_test_harness_fail(): get number of failed test suites in test harness.
 *
 * @ctx: contextual test harness.
 *
 * Return: number of failed test suites in @ctx.
 */
extern int ag_test_harness_fail(const ag_test_harness *ctx)
{
        register int fail = 0;

        struct node *n = ctx->head;
        while (n) {
                fail += ag_test_suite_fail(n->ts);
                n = n->nxt;
        }

        return fail;
}


/*
 * ag_test_suite_push(): push new test suite into test harness.
 *
 * @ctx: contextual test harness.
 * @tc : test suite to push into @ctx.
 */
extern void ag_test_harness_push(ag_test_harness *ctx, const ag_test_suite *ts)
{
        struct node *push = node_new(ts);

        if (ctx->head) {
                struct node *n = ctx->head;
                while (n->nxt)
                        n = n->nxt;

                n->nxt = push;
        } else
                ctx->head = push;
}


/*
 * ag_test_harness_exec(): execute test suites in test harness.
 *
 * @ctx: contextual test harness.
 */
extern void ag_test_harness_exec(ag_test_harness *ctx)
{
        struct node *n = ctx->head;

        while (n) {
                ag_test_suite_exec(n->ts); 
                n = n->nxt;
        }
}


/*
 * ag_test_harness_exec_file(): execute test suites in test harness and log to
 *                              file.
 *
 * @ctx: contextual test harness.
 * @log: log file.
 */
extern void ag_test_harness_exec_file(ag_test_harness *ctx, const char *file)
{
        struct node *n = ctx->head;

        while (n) {
                ag_test_suite_exec_file(n->ts, file); 
                n = n->nxt;
        }
}

