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
        struct node *n = ag_memblock_new(sizeof *n);
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
        struct node *n = ag_memblock_new(sizeof *n);
        n->ts = ag_test_suite_copy(ctx->ts);
        n->nxt = ctx->nxt;

        return n;
}


/*
 * node_free(): free test suite list node.
 *
 * @ctx: contextual test suite list node.
 *
 * Return: node next to @ctx.
 */
static inline struct node *node_free(struct node *ctx)
{
        struct node *nxt = ctx->nxt;

        ag_test_suite_release(&ctx->ts);
        ag_memblock_release((ag_memblock **)&ctx);

        return nxt;
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
        ag_test_harness *ctx = ag_memblock_new(sizeof *ctx);
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
        AG_ASSERT (ctx);

        return ag_memblock_copy(ctx);
}


/*
 * ag_test_harness_free(): release test harness.
 *
 * @ctx: contextual test suite.
 */
extern void ag_test_harness_release(ag_test_harness **ctx)
{
        ag_test_harness *hnd;

        if (AG_LIKELY (ctx && (hnd = *ctx))) {
                if (ag_memblock_refc(hnd) == 1) {
                        struct node *n = hnd->head;
                        while (n)
                                n = node_free(n);
                }

                ag_memblock_release((ag_memblock **)ctx);
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
        AG_ASSERT (ctx);

        register size_t len = 0;

        struct node *n = ctx->head;
        while (AG_LIKELY (n)) {
                len++;
                n = n->nxt;
        }

        return len;
}


/*
 * ag_test_harness_poll(): poll execution statistics of test harness.
 *
 * @ctx   : contextual test harness.
 * @status: execution status to poll.
 *
 * Return: number of test cases in @ctx with @status.
 */
extern size_t ag_test_harness_poll(const ag_test_harness *ctx,
                enum ag_test_status status)
{
        AG_ASSERT (ctx);

        register size_t tot = 0;

        struct node *n = ctx->head;
        while (AG_LIKELY (n)) {
                tot += ag_test_suite_poll(n->ts, status);
                n = n->nxt;
        }

        return tot;
}


/*
 * ag_test_harness_push(): push new test suite into test harness.
 *
 * @ctx: contextual test harness.
 * @ts : test suite to push into @ctx.
 */
extern void ag_test_harness_push(ag_test_harness *ctx, const ag_test_suite *ts)
{
        AG_ASSERT (ctx);
        AG_ASSERT (ts);

        struct node *push = node_new(ts);

        if (AG_LIKELY (ctx->head)) {
                struct node *n = ctx->head;
                while (AG_LIKELY (n->nxt))
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

        while (AG_LIKELY (n)) {
                ag_test_suite_exec(n->ts); 
                n = n->nxt;
        }
}


extern void ag_test_harness_log(const ag_test_harness *ctx, FILE *log)
{
        AG_ASSERT (ctx);
        AG_ASSERT (log);

        register struct node *n = ctx->head;

        while (AG_LIKELY (n)) {
                ag_test_suite_log(n->ts, log);
                n = n->nxt;
        }

        size_t pass = ag_test_harness_poll(ctx, AG_TEST_STATUS_OK);
        size_t skip = ag_test_harness_poll(ctx, AG_TEST_STATUS_SKIP)
                        + ag_test_harness_poll(ctx, AG_TEST_STATUS_WAIT);
        size_t fail = ag_test_harness_poll(ctx, AG_TEST_STATUS_FAIL);

        AG_AUTO(ag_string) *s = ag_string_new_fmt("%d test suite(s), %d test(s),"
                        " %d passed, %d skipped, %d failed.",
                        ag_test_harness_len(ctx), pass + skip + fail, 
                        pass, skip, fail);
        fprintf(log, "\n%s\n", s);
}

