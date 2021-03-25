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


struct data {
        char                    *msg;
        ag_exception_hnd        *hnd;
};

static void     data_release(void *);


/*
 * Although the exception registry presents itself through its interface as a
 * single list, it is implemented as two separate vectors. One vector maintains
 * the exception messages and handlers for the Argent Library error codes
 * (negative `ag_erno` values), and once for client code (positive `ag_erno`
 * values).
 */
static ag_registry      *g_argent = NULL;
static ag_registry      *g_client = NULL;


/* 
 * Prototypes for assertion helpers; we use these functions instead of the
 * predicates they wrap to improve readability. These helper functions are
 * available only in debug builds.
 */
#ifndef NDEBUG
static inline bool      is_exception_registry_initialised(void);
static inline bool      is_erno_valid(ag_erno);
static inline bool      is_msg_valid(const char *);
#endif


/* Prototypes for helper functions for managing dynamic memory and strings. */
static inline void      *mem_new(size_t);
static inline void       mem_release(void *);
static inline char      *str_new(const char *);
static inline void       str_release(char *);


/* Prototype for the default exception handler */
static void     hnd_default(const struct ag_exception *, void *);


/* 
 * ag_exception_registry_init() initialises the exception registry vectors with
 * a starting capacity.
 */
extern void
ag_exception_registry_init(void)
{
        AG_ASSERT (!is_exception_registry_initialised());

        g_argent = ag_registry_new(data_release);
        g_client = ag_registry_new(data_release);
}


/*
 * ag_exception_registry() shuts down the exception registry, releaseing the
 * heap memory resources used by it.
 */
extern void
ag_exception_registry_exit(void)
{
        ag_registry_release(&g_argent);
        ag_registry_release(&g_client);
}


/*
 * ag_exception_registry_msg() gets the exception message associated with a
 * given error code.
 */
extern const char *
ag_exception_registry_msg(ag_erno erno)
{
        AG_ASSERT (is_exception_registry_initialised());
        AG_ASSERT (is_erno_valid(erno));

        ag_registry *r = erno < 0 ? g_argent : g_client;
        ag_hash h = ag_hash_new(erno);

        struct data *d = ag_registry_get(r, h);
        return d->msg;
}


/*
 * ag_exception_registry_hnd() gets the exception handler associated with a
 * given error code.
 */
extern ag_exception_hnd *
ag_exception_registry_hnd(ag_erno erno)
{
        AG_ASSERT (is_exception_registry_initialised());
        AG_ASSERT (is_erno_valid(erno));

        ag_registry *r = erno < 0 ? g_argent : g_client;
        ag_hash h = ag_hash_new(erno);

        struct data *d = ag_registry_get(r, h);
        return d->hnd;
}


/*
 * ag_exception_registry_set() sets the exception message and handler associated
 * with a given error code. In case NULL is passed for hnd, then the default
 * exception handler is associated with the error code.
 */
/*
 * The `ag_exception_registry_set()` function is the only mutator for the
 * exception registry, and sets the exception message and handler associated
 * with a given error code. Subsequent calls to this function override the
 * previous values, and passing a NULL pointer for the handler causes the
 * default unhandled exception handler to be set.
 */
extern void
ag_exception_registry_set(ag_erno erno, const char *msg, ag_exception_hnd *hnd)
{
        AG_ASSERT (is_exception_registry_initialised());
        AG_ASSERT (is_erno_valid(erno));
        AG_ASSERT (is_msg_valid(msg));

        struct data *d = mem_new(sizeof *d);
        d->msg = str_new(msg);
        d->hnd = hnd ? hnd : hnd_default;

        ag_registry *r = erno < 0 ? g_argent : g_client;
        ag_hash h = ag_hash_new(erno);
        ag_registry_push(r, h, d);
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


#ifndef NDEBUG
static inline bool
is_erno_valid(ag_erno erno)
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
                printf("[!] failed to allocate memory for exception "
                    "registry\n");
                exit(EXIT_FAILURE);
        }

        memset(ctx, 0, sz);

        return ctx;
}


static inline void
mem_release(void *hnd)
{
        if (AG_LIKELY (hnd))
                free(hnd);
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
str_release(char *ctx)
{
        mem_release(ctx);
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


static void
data_release(void *hnd)
{
        str_release(((struct data *)hnd)->msg);
        mem_release(hnd);
}

