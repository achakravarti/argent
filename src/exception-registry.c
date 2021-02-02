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


#include "../include/argent.h"

#include <stdlib.h>
#include <string.h>


/*
 * The exception registry uses a vector of dynamically sized arrays to hold the
 * exception messages and handlers associated with error codes. The error codes
 * are used as indices for the arrays.
 */
struct vector {
        char                    **msg; /* Exception message. */
        ag_exception_handler    **hnd; /* Exception handler.  */
        size_t                    cap; /* Capacity of vector. */
};


/*
 * Although the exception registry presents itself through its interface as a
 * single list, it is implemented as two separate vectors. One vector maintains
 * the exception messages and handlers for the Argent Library error codes
 * (negative `ag_erno` values), and once for client code (positive `ag_erno`
 * values).
 */
static struct vector    *g_argent = NULL;
static struct vector    *g_client = NULL;


/* 
 * Prototypes for assertion helpers; we use these functions instead of the
 * predicates they wrap to improve readability. These helper functions are
 * available only in debug builds.
 */
#ifndef NDEBUG
static inline bool      is_exception_registry_initialised(void);
static inline bool      is_erno_valid_for_get(ag_erno);
static inline bool      is_erno_valid_for_set(ag_erno);
static inline bool      is_msg_valid(const char *);
#endif


/* Prototypes for helper functions for managing dynamic memory and strings. */
static inline void      *mem_new(size_t);
static inline char      *str_new(const char *);
static inline void       str_dispose(char *);


/*
 * Prototypes for the helper functions for selecting the appropriate vector and
 * determining the vector index according to error codes.
 */
static inline struct vector     *erno_vector(ag_erno);
static inline size_t             erno_index(ag_erno);


/* Prototypes to manage the creation and disoosal of vectors. */
static inline struct vector     *vector_new(size_t);
static inline void               vector_dispose(struct vector *);


/* Prototypes to access exception messages and handlers from a vector */
static inline const char                *vector_msg(const struct vector *,
                                            size_t);
static inline ag_exception_handler      *vector_hnd(const struct vector *,
                                            size_t);


/* Prototypes to help mutate vectors */
static void             vector_set(struct vector *, size_t, const char *,
                            ag_exception_handler *);
static inline void      vector_resize(struct vector *, size_t);


/* Prototype for the default exception handler */
static void     hnd_default(const struct ag_exception *, void *);


/* 
 * ag_exception_registry_init() initialises the exception registry vectors with
 * a starting capacity.
 */
extern void
ag_exception_registry_init(size_t cap)
{
        AG_ASSERT (!is_exception_registry_initialised());

        g_argent = vector_new(cap);
        g_client = vector_new(cap);
}


/*
 * ag_exception_registry() shuts down the exception registry, releaseing the
 * heap memory resources used by it.
 */
extern void
ag_exception_registry_exit(void)
{
        vector_dispose(g_argent);
        vector_dispose(g_client);
        g_argent = g_client = NULL;
}


/*
 * ag_exception_registry_msg() gets the exception message associated with a
 * given error code.
 */
extern const char *
ag_exception_registry_msg(ag_erno erno)
{
        AG_ASSERT (is_exception_registry_initialised());
        AG_ASSERT (is_erno_valid_for_get(erno));

        return vector_msg(erno_vector(erno), erno_index(erno));
}


/*
 * ag_exception_registry_hnd() gets the exception handler associated with a
 * given error code.
 */
extern ag_exception_handler *
ag_exception_registry_hnd(ag_erno erno)
{
        AG_ASSERT (is_exception_registry_initialised());
        AG_ASSERT (is_erno_valid_for_get(erno));

        return vector_hnd(erno_vector(erno), erno_index(erno));
}


/*
 * ag_exception_registry_set() sets the exception message and handler associated
 * with a given error code. In case NULL is passed for hnd, then the default
 * exception handler is associated with the error code.
 */
extern void
ag_exception_registry_set(ag_erno erno, const char *msg,
    ag_exception_handler *hnd)
{
        AG_ASSERT (is_exception_registry_initialised());
        AG_ASSERT (is_erno_valid_for_set(erno));
        AG_ASSERT (is_msg_valid(msg));

        vector_set(erno_vector(erno), erno_index(erno), msg, hnd);
}


/* 
 * is_exception_registry_initialised() checks if the vectors for both the Argent
 * Library and client code have been initialised. This function is required only
 * for debug builds.
 */
#ifndef NDEBUG
static inline bool
is_exception_registry_initialised(void)
{
        return g_argent && g_client;
}
#endif


/*
 * is_erno_valid_for_get() checks whether a given error code can be used as a
 * valid index in its corresponding vector for an access operation. This
 * function is required only for debug builds.
 */
#ifndef NDEBUG
static inline bool
is_erno_valid_for_get(ag_erno erno)
{
        if (!erno)
                return false;

        if (erno < AG_ERNO_NULL)
                return (size_t)(AG_ERNO_NULL - erno) <= g_argent->cap;
        else
                return (size_t)erno <= g_client->cap;
}
#endif


/*
 * is_erno_valid_for_set() checks whether a given error code can be used as a
 * valid index in its corresponding vector a mutation operation. This function
 * is required only for debug builds.
 */
#ifndef NDEBUG
static inline bool
is_erno_valid_for_set(ag_erno erno)
{
        return erno;
}
#endif


/*
 * is_msg_valid() checks whether a given string is valid for use as an exception
 * message, i.e. it is not an empty string.
 */
#ifndef NDEBUG
static inline bool
is_msg_valid(const char *msg)
{
        return msg && *msg;
}
#endif


/* 
 * mem_new() allocates a new block of heap memory of a given size, and fills the
 * block with 0.
 */
static inline void *
mem_new(size_t sz)
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


/* str_new() creates a new dynamic string from a given static string. */
static inline char *
str_new(const char *src)
{
        size_t sz = strlen(src);

        char *ctx = mem_new(sz + 1);
        strncpy(ctx, src, sz);
        ctx[sz] = '\0';
        
        return ctx;
}


/* str_dispose() release a dynamic string created with str_new(). */
static inline void
str_dispose(char *ctx)
{
        if (AG_LIKELY (ctx))
                free(ctx);
}


/*
 * erno_vector() determines which of the vectors in the exception registry needs
 * to be processed depending on an error code. Negative error codes indicate
 * that the Argent Library vector needs to processed, wheereas positive error
 * codes indicate that the client code vector needs to be used.
 */
static inline struct vector *
erno_vector(ag_erno erno)
{
        return erno < AG_ERNO_NULL ? g_argent : g_client;
}


/*
 * erno_index() transforms a given error code into a vector index. It works by
 * simply returning the absolute value of the error code.
 */
static inline size_t
erno_index(ag_erno erno)
{
        return erno < AG_ERNO_NULL ? AG_ERNO_NULL - erno : erno;
}


/*
 * vector_new() creates a new instance of an exception registry vector, sizing
 * it to a given capacity.
 */
static inline struct vector *
vector_new(size_t cap)
{
        struct vector *ctx = mem_new(sizeof *ctx);

        ctx->msg = mem_new(sizeof *ctx->msg * cap);
        ctx->hnd = mem_new(sizeof *ctx->hnd * cap);
        ctx->cap = cap;

        return ctx;
}


/*
 * vector_dispose() releases all the heap memory associated with a given vector.
 * It takes care to iterate through the list of dynamically created exception
 * messages that it holds, releasing each one it passes by.
 */
static inline void
vector_dispose(struct vector *ctx)
{
        if (AG_LIKELY (ctx)) {
                for (register size_t i = 0; i < ctx->cap; i++)
                        str_dispose(ctx->msg[i]);

                free(ctx->msg);
                free(ctx->hnd);
                free(ctx);
        }
}


/*
 * vector_msg() gets the exception message at a given index in the array of
 * exception messages of a vector.
 */
static inline const char *
vector_msg(const struct vector *ctx, size_t idx)
{
        return ctx->msg[idx];
}


/*
 * vector_hnd() gets the exception handler at a given index in the array of
 * exception handlers of a vector.
 */
static inline ag_exception_handler *
vector_hnd(const struct vector *ctx, size_t idx)
{
        return ctx->hnd[idx];
}


/*
 * vector_set() sets the exception message and handler at a given index. If hnd
 * is passed NULL, then the default exception handler is set. This function
 * takes care to resize the vector as required, using a standard expansion
 * factor of 2.
 */
static void 
vector_set(struct vector *ctx, size_t idx, const char *msg,
    ag_exception_handler *hnd)
{
        register size_t cap = ctx->cap;
       
        if (cap < idx) { 
                while (cap < idx)
                        cap *= 2;

                vector_resize(ctx, cap);
        }

        str_dispose(ctx->msg[idx]);
        ctx->msg[idx] = str_new(msg);
        ctx->hnd[idx] = hnd ? hnd : &hnd_default;
}


/*
 * vector_resize() resizes the capacity of a vector to a new given capacity
 * while preserving the existing. Note that this function *doesn't* check
 * whether the new capacity is greater than the existing capacity; it's up to
 * the calling function to do do so.
 */
static inline void
vector_resize(struct vector *ctx, size_t cap)
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


/*
 * hnd_default() is the default exception handler used in case client code
 * passes a NULL exception handler through ag_exception_reistry_set(). This
 * handler simply terminates the application after printing and logging the
 * exception metadata.
 */
static void
hnd_default(const struct ag_exception *ex, void *opt)
{
        (void) opt;
    
        printf("[!] (unhandled) %d [%s(), %s:%lu]: %s\n", ex->erno, ex->func,
            ex->file, ex->line, ag_exception_registry_msg(ex->erno));

        ag_log_err("(unhandled) %d [%s(), %s:%lu]: %s", ex->erno, ex->func,
            ex->file, ex->line, ag_exception_registry_msg(ex->erno));

        ag_exit(EXIT_FAILURE);
}

