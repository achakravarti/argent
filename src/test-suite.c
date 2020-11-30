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
 * File: argent/src/test-suite.c
 *
 * This file contains the implementation of the test suite interface of the
 * Testing Module of the Argent Library. See argent/include/test.h for an
 * overview description of this interface.
 */


#include "../include/argent.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


/*
 * struct node: node in list of test cases.
 *
 * @tc : test case.
 * @nxt: next node in list.
 */
struct node {
        ag_test_case *tc;
        struct node *nxt;
};


/*
 * node_new(): create new test case list node.
 *
 * @tc: test case.
 *
 * Return: new test case list node.
 */
static inline struct node *node_new(const ag_test_case *tc)
{
        struct node *n = malloc(sizeof *n);
        n->tc = ag_test_case_copy(tc);
        n->nxt = NULL;

        return n;
}


/*
 * node_copy(): create deep copy of test case list node.
 *
 * @ctx: contextual test case list node.
 *
 * Return: copy of @ctx.
 */
static inline struct node *node_copy(const struct node *ctx)
{
        struct node *n = malloc(sizeof *n);
        n->tc = ag_test_case_copy(ctx->tc);
        n->nxt = ctx->nxt;

        return n;
}


/*
 * node_dispose(): dispose test case list node.
 *
 * @ctx: contextual test case list node.
 */
static inline void node_dispose(struct node *ctx)
{
        ag_test_case_dispose(&ctx->tc);
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
 * struct ag_test_suite: internal structure of `ag_test_suite`.
 *
 * @desc: test suite description.
 * @head: head of test case list.
 */
struct ag_test_suite {
        char *desc;
        struct node *head;
};


/*
 * log_header(): write test suite header to log file.
 *
 * @ctx: contextual test suite.
 * @log: log file.
 */
static inline void log_header(const ag_test_suite *ctx, FILE *log)
{
        fprintf(log, "\nTest Suite: %s\n", ctx->desc);

        for (register size_t i = 0; i < strlen(ctx->desc) + 12; i++)
                fputs("=", log);
}


/*
 * log_footer(): write test suite footer to log file.
 *
 * @ctx: contextual test suite.
 * @log: log file.
 */
static inline void log_footer(const ag_test_suite *ctx, FILE *log)
{
        char *s = str_new_fmt("%d test(s), %d passed, %d skipped, %d failed.",
                        ag_test_suite_len(ctx),
                        ag_test_suite_poll(ctx, AG_TEST_STATUS_OK),
                        ag_test_suite_poll(ctx, AG_TEST_STATUS_SKIP),
                        ag_test_suite_poll(ctx, AG_TEST_STATUS_FAIL));
        fprintf(log, "\n\n%s\n", s);
        str_dispose(s);
}


/*
 * log_body(): write test suite details to log file.
 *
 * @ctx: contextual test suite.
 * @log: log file.
 */
static inline void log_body(const ag_test_suite *ctx, FILE *log)
{
        register size_t i = 0;

        struct node *n = ctx->head;
        while (n) {
                fprintf(log, "\n%.2lu. ", ++i);
                ag_test_case_log(n->tc, log);
                n = n->nxt;
        }
}


/*
 * ag_test_suite_new(): create new test suite.
 *
 * @desc: description of test suite.
 *
 * Return: new test suite.
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
 * ag_test_suite_copy(): create deep copy of test suite.
 *
 * @ctx: contextual test suite.
 *
 * Return: copy of @ctx.
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
 * ag_test_suite_dispose(): dispose test suite.
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
 * ag_test_suite_len(): get number of test cases in test suite.
 *
 * @ctx: contextual test suite.
 *
 * Return: number of test cases in @ctx.
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
 * ag_test_suite_poll(): poll execution statistics of test suite.
 *
 * @ctx   : contextual test suite.
 * @status: execution status to poll.
 *
 * Return: number of test cases in @ctx with @status.
 */
extern size_t ag_test_suite_poll(const ag_test_suite *ctx,
                enum ag_test_status status)
{
        register size_t tot = 0;
        
        struct node *n = ctx->head;
        while (n) {
                if (ag_test_case_status(n->tc) == status)
                        tot++;
                n = n->nxt;
        }

        return tot;
}


/*
 * ag_test_suite_push(): push new test case into test suite.
 *
 * @ctx: contextual test suite.
 * @tc : test case to push into @ctx.
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
 * ag_test_suite_exec(): execute test cases in test suite.
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


/*
 * ag_test_suite_log(): log test cases in test suite.
 *
 * @ctx: contextual test suite.
 * @log: log file.
 */
extern void ag_test_suite_log(const ag_test_suite *ctx, FILE *log)
{
        if (log) {
                log_header(ctx, log);
                log_body(ctx, log);
                log_footer(ctx, log);
        }
}

