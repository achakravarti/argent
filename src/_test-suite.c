#include "../include/argent.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


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
 * str_free(): release dynamic string.
 *
 * @ctx: contextual string.
 */
static inline void str_free(char *ctx)
{
        if (ctx)
                free(ctx);
}


struct node {
        _ag_test *test;
        char *desc;
        enum ag_test_status status;
        struct node *next;
};


static struct node *node_new(_ag_test *test, const char *desc)
{
        struct node *n = malloc(sizeof *n);
        n->test = test;
        n->desc = str_new_fmt("%s", desc);
        n->status = AG_TEST_STATUS_WAIT;
        n->next = NULL;

        return n;
}


static inline struct node *node_free(struct node *ctx)
{
        struct node *next = ctx->next;

        str_free(ctx->desc);
        free(ctx);

        return next;
}


static void node_log(const struct node *ctx, FILE *log)
{
        switch (ctx->status) {
                case AG_TEST_STATUS_OK:
                        fprintf(log, "[OK]   %s", ctx->desc);
                        break;

                case AG_TEST_STATUS_WAIT:
                        fprintf(log, "[WAIT] %s", ctx->desc);
                        break;

                case AG_TEST_STATUS_SKIP:
                        fprintf(log, "[SKIP] %s", ctx->desc);
                        break;

                case AG_TEST_STATUS_SIGABRT:
                        fprintf(log, "[FAIL] %s (SIGABRT)", ctx->desc);
                        break;

                case AG_TEST_STATUS_SIGSEGV:
                        fprintf(log, "[FAIL] %s (SIGSEGV)", ctx->desc);
                        break;

                default:
                        fprintf(log, "[FAIL] %s", ctx->desc);
        }
}


struct _ag_test_suite {
        char *desc;
        struct node *head;
};




static void log_header(const _ag_test_suite *ctx, FILE *log)
{
        fprintf(log, "\nTest Suite: %s\n", ctx->desc);

        for (register size_t i = 0; i < strlen(ctx->desc) + 12; i++)
                fputs("=", log);
}


static inline void log_footer(const _ag_test_suite *ctx, FILE *log)
{
        fprintf(log, "\n\n%lu test(s), %lu passed, %lu skipped, %lu failed.\n",
                        _ag_test_suite_len(ctx),
                        _ag_test_suite_poll(ctx, AG_TEST_STATUS_OK),
                        _ag_test_suite_poll(ctx, AG_TEST_STATUS_SKIP),
                        _ag_test_suite_poll(ctx, AG_TEST_STATUS_FAIL));
}


static void log_body(const _ag_test_suite *ctx, FILE *log)
{
        register size_t i = 0;

        struct node *n = ctx->head;
        while (n) {
                fprintf(log, "\n%.2lu. ", ++i);
                node_log(n, log);
                n = n->next;
        }
}


extern _ag_test_suite *_ag_test_suite_new(const char *desc)
{
        _ag_test_suite *ctx = malloc(sizeof *ctx);
        ctx->desc = str_new_fmt("%s", desc);
        ctx->head = NULL;


        return ctx;
}


extern _ag_test_suite *_ag_test_suite_copy(const _ag_test_suite *ctx)
{
        _ag_test_suite *cp = _ag_test_suite_new(ctx->desc);

        struct node *n = ctx->head;
        while (n) {
                _ag_test_suite_push(cp, n->test, n->desc);
                n = n->next;
        }

        return cp;
}


extern void _ag_test_suite_free(_ag_test_suite **ctx)
{
        _ag_test_suite *hnd;

        if (ctx && (hnd = *ctx)) {
                str_free(hnd->desc);

                struct node *n = hnd->head;
                while (n) 
                        n = node_free(n);

                free(hnd);
                *ctx = NULL;
        }
}


extern size_t _ag_test_suite_len(const _ag_test_suite *ctx)
{
        register size_t len = 0;

        struct node *n = ctx->head;
        while (n) {
                len++;
                n = n->next;
        }

        return len;
}


extern size_t _ag_test_suite_poll(const _ag_test_suite *ctx,
                enum ag_test_status status)
{
        register size_t tot = 0;
        
        struct node *n = ctx->head;
        while (n) {
                if ((n->status) == status)
                        tot++;
                n = n->next;
        }

        return tot;
}


extern void _ag_test_suite_push(_ag_test_suite *ctx, _ag_test *test,
                const char *desc)
{
        struct node *push = node_new(test, desc);

        if (ctx->head) {
                struct node *n = ctx->head;
                while (n->next)
                        n = n->next;

                n->next = push;
        } else
                ctx->head = push;
}


extern void _ag_test_suite_push_array(_ag_test_suite *ctx, _ag_test *test[],
                const char *desc[], size_t len)
{
        for (register size_t i = 0; i < len; i++)
                _ag_test_suite_push(ctx, test[i], desc[i]);
}


extern void _ag_test_suite_exec(_ag_test_suite *ctx)
{
        struct node *n = ctx->head;
        while (n) {
                n->status = n->test();
                if (n->status != AG_TEST_STATUS_OK)
                        node_log(n, stdout);
                n = n->next;
        }
}


extern void _ag_test_suite_log(_ag_test_suite *ctx, FILE *log)
{
        if (log) {
                log_header(ctx, log);
                log_body(ctx, log);
                log_footer(ctx, log);
        }
}

