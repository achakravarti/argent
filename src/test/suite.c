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


#include "../argent.h"

#include <dlfcn.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


struct node {
        ag_test         *test;
        ag_string       *desc;
        enum            ag_test_status status;
        struct          node *next;
};


static struct node *
node_new(ag_test *test, const char *desc)
{
        struct node *n = ag_memblock_new(sizeof *n);
        n->test = test;
        n->desc = ag_string_new_fmt("%s", desc);
        n->status = AG_TEST_STATUS_WAIT;
        n->next = NULL;

        return (n);
}


static inline struct node *
node_free(struct node *ctx)
{
        struct node *next = ctx->next;

        ag_string_release(&ctx->desc);
        ag_memblock_release((ag_memblock **)&ctx);

        return (next);
}


static void
node_log(const struct node *ctx, FILE *log)
{
        switch (ctx->status) {
                case AG_TEST_STATUS_OK:
                        fprintf(log, "[OK]   %s\n", ctx->desc);
                        break;

                case AG_TEST_STATUS_WAIT:
                        fprintf(log, "[WAIT] %s\n", ctx->desc);
                        break;

                case AG_TEST_STATUS_SKIP:
                        fprintf(log, "[SKIP] %s\n", ctx->desc);
                        break;

                case AG_TEST_STATUS_SIGABRT:
                        fprintf(log, "[FAIL] %s (SIGABRT)\n", ctx->desc);
                        break;

                case AG_TEST_STATUS_SIGSEGV:
                        fprintf(log, "[FAIL] %s (SIGSEGV)\n", ctx->desc);
                        break;

                default:
                        fprintf(log, "[FAIL] %s\n", ctx->desc);
        }
}


struct ag_test_suite {
        ag_string       *desc;
        struct          node *head;
};




static void
log_header(const ag_test_suite *ctx, FILE *log)
{
        fprintf(log, "\nTest Suite: %s\n", ctx->desc);

        for (register size_t i = 0; i < strlen(ctx->desc) + 12; i++)
                fputs("=", log);

        fputs("\n", log);
}


static inline void
log_footer(const ag_test_suite *ctx, FILE *log)
{
        fprintf(log, "\n\n%lu test(s), %lu passed, %lu skipped, %lu failed.\n",
            ag_test_suite_len(ctx), ag_test_suite_poll(ctx, AG_TEST_STATUS_OK),
            ag_test_suite_poll(ctx, AG_TEST_STATUS_SKIP) +
                ag_test_suite_poll(ctx, AG_TEST_STATUS_WAIT),
            ag_test_suite_poll(ctx, AG_TEST_STATUS_FAIL));
}


static void
log_body(const ag_test_suite *ctx, FILE *log)
{
        register size_t i = 0;

        struct node *n = ctx->head;
        while (AG_LIKELY (n)) {
                fprintf(log, "%.2lu. ", ++i);
                node_log(n, log);
                n = n->next;
        }
}


extern ag_test_suite *
ag_test_suite_new(const char *desc)
{
        AG_ASSERT_STR (desc && *desc);

        ag_test_suite *ctx = ag_memblock_new(sizeof *ctx);
        ctx->desc = ag_string_new_fmt("%s", desc);
        ctx->head = NULL;

        return (ctx);
}


extern ag_test_suite *
__ag_test_suite_generate__(const char *desc, int id, int counter)
{
        AG_ASSERT_STR (desc);

        char *err;
        enum ag_test_status (*testf)(void);
        const char *testd;
        ag_string *tsym;
        ag_string *dsym;

        dlerror();
        void *hnd = dlopen(NULL, RTLD_LAZY);

        if (AG_UNLIKELY (!hnd)) {
                char *err = dlerror();
                fprintf(stderr, "failed to open DSO for tests: %s\n", err);
                exit(EXIT_FAILURE);
        }

        ag_test_suite *ts = ag_test_suite_new(desc);

        for (register int i = 0; i < counter; i++) {
                tsym = ag_string_new_fmt("__ag_test_%d_%d", id, i);
                dsym = ag_string_new_fmt("__ag_desc_%d_%d", id, i);

                dlerror();
                testf = dlsym(hnd, tsym);

                if (AG_UNLIKELY (err = dlerror())) {
                        fprintf(stderr, "failed to load test: %s\n", err);
                        exit(EXIT_FAILURE);
                }

                testd = dlsym(hnd, dsym);
                if (AG_UNLIKELY (err = dlerror())) {
                        fprintf(stderr, "failed to load test desc: %s\n", err);
                        exit(1);
                }

                ag_test_suite_push(ts, testf, testd);

                ag_string_release(&tsym);
                ag_string_release(&dsym);
        }

        return ts;
}


extern ag_test_suite *
ag_test_suite_copy(const ag_test_suite *ctx)
{
        AG_ASSERT (ctx);

        return (ag_memblock_copy(ctx));
}


extern void
ag_test_suite_release(ag_test_suite **ctx)
{
        ag_test_suite *hnd;

        if (AG_LIKELY (ctx && (hnd = *ctx))) {
                if (ag_memblock_refc(hnd) == 1) {
                        ag_string_release(&hnd->desc);

                        struct node *n = hnd->head;
                        while (n)
                                n = node_free(n);
                }

                ag_memblock_release((ag_memblock **)ctx);
        }
}


extern size_t
ag_test_suite_len(const ag_test_suite *ctx)
{
        AG_ASSERT (ctx);

        register size_t len = 0;

        struct node *n = ctx->head;
        while (AG_LIKELY (n)) {
                len++;
                n = n->next;
        }

        return (len);
}


extern size_t
ag_test_suite_poll(const ag_test_suite *ctx, enum ag_test_status status)
{
        AG_ASSERT (ctx);

        register size_t tot = 0;
        register struct node *n = ctx->head;

        while (AG_LIKELY (n)) {
                if ((n->status) == status)
                        tot++;
                n = n->next;
        }

        return (tot);
}


extern void
ag_test_suite_push(ag_test_suite *ctx, ag_test *test, const char *desc)
{
        AG_ASSERT (ctx);
        AG_ASSERT (test);
        AG_ASSERT (desc && *desc);

        struct node *push = node_new(test, desc);

        if (AG_LIKELY (ctx->head)) {
                struct node *n = ctx->head;
                while (AG_LIKELY (n->next))
                        n = n->next;

                n->next = push;
        } else
                ctx->head = push;
}


extern void
ag_test_suite_push_array(ag_test_suite *ctx, ag_test *test[],
    const char *desc[], size_t len)
{
        AG_ASSERT (ctx);
        AG_ASSERT (test);
        AG_ASSERT (desc);
        AG_ASSERT (len);

        for (register size_t i = 0; i < len; i++)
                ag_test_suite_push(ctx, test[i], desc[i]);
}


extern void
ag_test_suite_exec(ag_test_suite *ctx)
{
        AG_ASSERT (ctx);

        struct node *n = ctx->head;
        while (AG_LIKELY (n)) {
                n->status = n->test();

                if (AG_UNLIKELY (n->status != AG_TEST_STATUS_OK))
                        node_log(n, stdout);

                n = n->next;
        }
}


extern void
ag_test_suite_log(const ag_test_suite *ctx, FILE *log)
{
        AG_ASSERT (ctx);
        AG_ASSERT (log);

        log_header(ctx, log);
        log_body(ctx, log);
        log_footer(ctx, log);
}

