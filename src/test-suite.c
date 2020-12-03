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

#include <stdlib.h>
#include <string.h>


struct node {
        ag_test_case *tc;
        struct node *nxt;
};


static inline struct node *node_new(const ag_test_case *tc)
{
        struct node *n = malloc(sizeof *n);
        n->tc = ag_test_case_copy(tc);
        n->nxt = NULL;

        return n;
}


static inline struct node *node_copy(const struct node *ctx)
{
        struct node *n = malloc(sizeof *n);
        n->tc = ag_test_case_copy(ctx->tc);
        n->nxt = ctx->nxt;

        return n;
}


static inline void node_dispose(struct node *ctx)
{
        ag_test_case_dispose(&ctx->tc);
        free(ctx);
}


struct ag_test_suite {
        struct node *head;
        char *desc;
};


/*
 * ag_test_suite_new():
 *
 * @desc:
 */
extern ag_test_suite *ag_test_suite_new(const char *desc)
{
        ag_test_suite *ctx = malloc(sizeof *ctx);
        ctx->head = NULL;

        size_t len = strlen(desc);
        ctx->desc = malloc(len + 1);
        strncpy(ctx->desc, desc, len);

        return ctx;
}


/*
 * ag_test_suite_copy():
 *
 * @ctx: contextual test suite.
 */
extern ag_test_suite *ag_test_suite_copy(const ag_test_suite *ctx)
{
        ag_test_suite *cp = ag_test_suite_new(ctx->desc);

        struct node *n = ctx->head;
        while (n) {
                ag_test_suite_push(cp, n->tc);
                n = n->nxt;
        }

        return cp;
}


/*
 * ag_test_suite_dispose():
 *
 * @ctx: contextual test suite.
 */
extern void ag_test_suite_dispose(ag_test_suite **ctx)
{
        ag_test_suite *hnd;

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
 * ag_test_suite_len():
 *
 * @ctx: contextual test suite.
 */
extern size_t ag_test_suite_len(const  ag_test_suite *ctx)
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
 * ag_test_suite_pass():
 *
 * @ctx: contextual test suite.
 */
extern int ag_test_suite_pass(const ag_test_suite *ctx)
{
        register int pass = 0;

        struct node *n = ctx->head;
        while (n) {
                if (ag_test_case_status(n->tc) == AG_TEST_STATUS_OK)
                        pass++;
        }

        return pass;
}


/*
 * ag_test_suite_skip():
 *
 * @ctx: contextual test suite.
 */
extern int ag_test_suite_skip(const ag_test_suite *ctx)
{
        register int skip = 0;

        struct node *n = ctx->head;
        while (n) {
                if (ag_test_case_status(n->tc) == AG_TEST_STATUS_SKIP)
                        skip++;
        }

        return skip;
}


/*
 * ag_test_suite_fail():
 *
 * @ctx: contextual test suite.
 */
extern int ag_test_suite_fail(const ag_test_suite *ctx)
{
        register int fail = 0;

        struct node *n = ctx->head;
        while (n) {
                if (ag_test_case_status(n->tc) == AG_TEST_STATUS_FAIL)
                        fail++;
        }

        return fail;
}


/*
 * ag_test_suite_push():
 *
 * @ctx: contextual test suite.
 * @tc :
 */
extern void ag_test_suite_push(ag_test_suite *ctx, const ag_test_case *tc)
{
        struct node *push = node_new(tc);

        if (ctx->head) {
                struct node *n = ctx->head;
                while (n->nxt)
                        n = n->nxt;

                n->nxt = push;
        } else
                ctx->head = push;
}


/*
 * ag_test_suite_exec():
 *
 * @ctx: contextual test suite.
 */
extern void ag_test_suite_exec(ag_test_suite *ctx)
{
        struct node *n = ctx->head;
        while (n) {
                ag_test_case_exec(n->tc); 
                n = n->nxt;
        }
}

