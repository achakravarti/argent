#include "../include/argent.h"

#include <stdlib.h>
#include <string.h>


struct vector {
        char **msg;
        ag_exception_handler **hnd;
        size_t cap;
};

#ifndef NDEBUG
static inline bool is_exception_registry_initialised(void);
static inline bool is_erno_valid_for_get(ag_erno);
static inline bool is_erno_valid_for_set(ag_erno);
static inline bool is_msg_valid(const char *);
#endif

static struct vector *g_argent = NULL;
static struct vector *g_client = NULL;

static inline void *mem_new(size_t);

static inline char *str_new(const char *);
static inline void str_dispose(char *);

static inline struct vector *erno_vector(ag_erno);
static inline size_t erno_index(ag_erno);

static inline struct vector *vector_new(size_t);
static inline void vector_dispose(struct vector *);

static inline const char *vector_msg(const struct vector *, size_t);
static inline ag_exception_handler *vector_hnd(const struct vector *, size_t);

static void vector_set(struct vector *, size_t, const char *,
                ag_exception_handler *);
static inline void vector_resize(struct vector *, size_t);

static void hnd_default(const struct ag_exception *, void *);



extern void ag_exception_registry_init(size_t cap)
{
        AG_ASSERT (!is_exception_registry_initialised());

        g_argent = vector_new(cap);
        g_client = vector_new(cap);
}


extern void ag_exception_registry_exit(void)
{
        vector_dispose(g_argent);
        vector_dispose(g_client);
        g_argent = g_client = NULL;
}


extern const char *ag_exception_registry_msg(ag_erno erno)
{
        AG_ASSERT (is_exception_registry_initialised());
        AG_ASSERT (is_erno_valid_for_get(erno));

        return vector_msg(erno_vector(erno), erno_index(erno));
}


extern ag_exception_handler *ag_exception_registry_hnd(ag_erno erno)
{
        AG_ASSERT (is_exception_registry_initialised());
        AG_ASSERT (is_erno_valid_for_get(erno));

        return vector_hnd(erno_vector(erno), erno_index(erno));
}


extern void ag_exception_registry_set(ag_erno erno, const char *msg,
                ag_exception_handler *hnd)
{
        AG_ASSERT (is_exception_registry_initialised());
        AG_ASSERT (is_erno_valid_for_set(erno));
        AG_ASSERT (is_msg_valid(msg));

        vector_set(erno_vector(erno), erno_index(erno), msg, hnd);
}

#ifndef NDEBUG

static inline bool is_exception_registry_initialised(void)
{
        return g_argent && g_client;
}

static inline bool is_erno_valid_for_get(ag_erno erno)
{
        if (!erno)
                return false;

        if (erno < AG_ERNO_NULL)
                return (size_t)(AG_ERNO_NULL - erno) <= g_argent->cap;
        else
                return (size_t)erno <= g_client->cap;
}

static inline bool is_erno_valid_for_set(ag_erno erno)
{
        return erno;
}

static inline bool is_msg_valid(const char *msg)
{
        return msg && *msg;
}

#endif



static inline void *mem_new(size_t sz)
{
        void *ctx = malloc(sz);

        if (AG_UNLIKELY (!ctx)) {
                printf("[!] failed to allocate memory for exception registry,"
                                " aborting...\n");
                abort();
        }

        memset(ctx, 0, sz);

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


static inline void str_dispose(char *ctx)
{
        if (AG_LIKELY (ctx))
                free(ctx);
}


static inline struct vector *erno_vector(ag_erno erno)
{
        return erno < AG_ERNO_NULL ? g_argent : g_client;
}


static inline size_t erno_index(ag_erno erno)
{
        return erno < AG_ERNO_NULL ? AG_ERNO_NULL - erno : erno;
}


static inline struct vector *vector_new(size_t cap)
{
        struct vector *ctx = mem_new(sizeof *ctx);

        ctx->msg = mem_new(sizeof *ctx->msg * cap);
        ctx->hnd = mem_new(sizeof *ctx->hnd * cap);

        return ctx;
}


static inline void vector_dispose(struct vector *ctx)
{
        if (AG_LIKELY (ctx)) {
                for (register size_t i = 0; i < ctx->cap; i++)
                        str_dispose(ctx->msg[i]);

                free(ctx->msg);
                free(ctx->hnd);
                free(ctx);
        }
}


static inline const char *vector_msg(const struct vector *ctx, size_t idx)
{
        return ctx->msg[idx];
}


static inline ag_exception_handler *vector_hnd(const struct vector *ctx,
                size_t idx)
{
        return ctx->hnd[idx];
}

static void vector_set(struct vector *ctx, size_t idx, const char *msg,
                ag_exception_handler *hnd)
{
        size_t cap = ctx->cap;
        while (cap < idx)
                cap *= 2;

        vector_resize(ctx, cap);

        str_dispose(ctx->msg[idx]);
        ctx->msg[idx] = str_new(msg);
        ctx->hnd[idx] = hnd ? hnd : &hnd_default;
}


static inline void vector_resize(struct vector *ctx, size_t cap)
{
        ctx->msg = realloc(ctx->msg, sizeof *ctx->msg * cap);
        ctx->hnd = realloc(ctx->hnd, sizeof *ctx->hnd * cap);
        ctx->cap = cap;

        if (AG_UNLIKELY (!(ctx->msg && ctx->hnd))) {
                printf("[!] failed to resize memory for exception registry,"
                                " aborting...\n");
                abort();
        }
}


static void hnd_default(const struct ag_exception *ex, void *opt)
{
        (void)opt;
    
        printf("[!] unhandled exception: %d [%s(), %s:%lu]\n%s\n", ex->erno, 
            ex->func, ex->file, ex->line, ag_exception_registry_msg(ex->erno));

        ag_log_err("unhandled exception: %d [%s(), %s:%lu]\n%s\n", ex->erno, 
            ex->func, ex->file, ex->line, ag_exception_registry_msg(ex->erno));
}

