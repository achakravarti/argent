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


struct ag_exception {
        ag_erno erno;
        char *func;
        char *file;
        int line;
};


static inline void *mem_new(size_t);
static inline char *str_new(const char *);
static inline void str_dispose(char *);


extern ag_exception *ag_exception_new(ag_erno erno, const char *func,
                const char *file, int line)
{
        AG_ASSERT (func && *func);
        AG_ASSERT (file && *file);
        AG_ASSERT (line);

        ag_exception *ctx = mem_new(sizeof *ctx);

        ctx->func = str_new(func);
        ctx->file = str_new(file);
        ctx->line = line;

        return ctx;
}


extern ag_exception *ag_exception_copy(const ag_exception *ctx)
{
        AG_ASSERT (ctx);

        return ag_exception_new(ctx->erno, ctx->func, ctx->file, ctx->line);
}


extern void ag_exception_dispose(ag_exception **ctx)
{
        ag_exception *hnd;
        
        if (AG_LIKELY (ctx && (hnd = *ctx))) {
                str_dispose(hnd->func);
                str_dispose(hnd->file);

                free(hnd);
                *ctx = NULL;
        }
}


extern ag_erno ag_exception_erno(const ag_exception *ctx)
{
        AG_ASSERT (ctx);

        return ctx->erno;
}


extern const char *ag_exception_msg(const ag_exception *ctx)
{
        AG_ASSERT (ctx);

        return ag_exception_registry_msg(ctx->erno);
}


extern const char *ag_exception_func(const ag_exception *ctx)
{
        AG_ASSERT (ctx);

        return ctx->func;
}


extern const char *ag_exception_file(const ag_exception *ctx)
{
        AG_ASSERT (ctx);

        return ctx->file;
}


extern int ag_exception_line(const ag_exception *ctx)
{
        AG_ASSERT (ctx);

        return ctx->line;
}


extern ag_exception_handler *ag_exception_hnd(const ag_exception *ctx)
{
        AG_ASSERT (ctx);

        return ag_exception_registry_hnd(ctx->erno);
}


static inline void *mem_new(size_t sz)
{
        void *ctx = malloc(sz);

        if (AG_UNLIKELY (!ctx)) {
                printf("[!] failed to malloc for exception, aborting...\n");
                abort();
        }

        return ctx;
}

static inline char *str_new(const char *src)
{
        size_t sz = strlen(src);

        char *ctx = mem_new(sz + 1);
        strncpy(ctx, src, sz);
        ctx[sz] = '\0';
        
        return ctx;
}


inline void str_dispose(char *ctx)
{
        if (AG_LIKELY (ctx))
                free(ctx);
}


#if 0
struct node {
        ag_erno err;
        char *msg;
        ag_exception_handler *eh;
        struct node *nxt;
};


static struct node *g_ex = NULL;

static char *str_new(const char *);


static struct node *node_new(ag_erno, const char *, ag_exception_handler *);
static struct node *node_free(struct node *);

#if 0
static inline void eh_default(ag_erno, const char *, const char *, int, void *);
#endif



extern void ag_exception_init(void)
{
        if (AG_UNLIKELY (g_ex)) {
                printf("exception list already initialised, aborting...\n");
                abort();
        }
}


extern void ag_exception_exit(void)
{
        struct node *n = g_ex;
        while (AG_LIKELY (n))
                n = node_free(n);
}


extern void ag_exception_register(ag_erno err, const char *msg,
                ag_exception_handler *eh)
{
        struct node *ctx = node_new(err, msg, eh);

        if (AG_LIKELY (g_ex)) {
                struct node *n = g_ex;
                while (AG_LIKELY (n && n->nxt))
                        n = n->nxt;

                n->nxt = ctx;
        } else
                g_ex = ctx;
}


extern const char *ag_exception_msg(ag_erno err)
{
        struct node *n = g_ex;
        while (AG_LIKELY (n)) {
                if (n->err == err)
                        return n->msg;

                n = n->nxt;
        }

        return "undefined exception";
}


extern ag_exception_handler *ag_exception_hnd(ag_erno err)
{
        struct node *n = g_ex;
        while (AG_LIKELY (n)) {
                if (n->err == err)
                        return n->eh;

                n = n->nxt;
        }

        return NULL;
}


static char *str_new(const char *src)
{
        size_t sz = strlen(src);
        char *ctx = malloc(sz + 1);
        if (AG_UNLIKELY (!ctx)) {
                printf("failed to malloc exception message, aborting...\n");
                abort();
        }

        memset(ctx, '\0', sz);
        strncpy(ctx, src, sz);

        return ctx;
}

static inline void str_free(char *ctx)
{
        if (ctx)
                free(ctx);
}

static struct node *node_new(ag_erno err, const char *msg,
                ag_exception_handler *eh)
{
        struct node *ctx = malloc(sizeof *ctx);
        if (AG_UNLIKELY (!ctx)) {
                printf("failed to malloc exception list node, aborting...\n");
                abort();
        }

        ctx->err = err;
        ctx->msg = str_new(msg);
        ctx->eh = eh;
        ctx->nxt = NULL;

        return ctx;
}


static struct node *node_free(struct node *ctx)
{
        if (AG_LIKELY (ctx)) {
                struct node *nxt = ctx->nxt;

                str_free(ctx->msg);
                free(ctx);

                return nxt;
        }

        return NULL;
}
#endif

#if 0
static inline void eh_default(ag_erno, const char *, const char *, int, void *)
{
}
#endif

