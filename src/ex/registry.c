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

#include <stdlib.h>
#include <string.h>


/*******************************************************************************
 * For every exception that we register, we need to keep track of its exception
 * message and its exception handler. The `ex` struct encapsulates this data,
 * and is supported by three related functions:
 *   - `ex_new(), 
 *   - `ex_release()`, and
 *   - `ex_default()`.
 */

struct ex {
        char                    *msg;
        ag_exception_hnd        *hnd;
};

static struct ex        *ex_new(const char *, ag_exception_hnd *);
static void              ex_release(void *);
static void              ex_default(const struct ag_exception *, void *);


/*******************************************************************************
 * Although the exception registry presents itself through its interface as a
 * single registry, it is implemented as two separate units. One unit maintains
 * the exception messages and handlers for the Argent Library error codes
 * (negative `ag_erno` values), and one for client code (positive `ag_erno`
 * values).
 */

static ag_registry      *g_argent = NULL;
static ag_registry      *g_client = NULL;


/*******************************************************************************
 * `ag_exception_registry_init()` initialises the exception registry vectors 
 * with a starting capacity.
 */

extern void
ag_exception_registry_init(void)
{
        AG_ASSERT_TAG ("EXCEPTION_REGISTRY_NOT_INIT", !(g_argent && g_client));

        g_argent = ag_registry_new(ex_release);
        g_client = ag_registry_new(ex_release);
}


/*******************************************************************************
 * `ag_exception_registry()` shuts down the exception registry, releasing the
 * heap memory resources used by it.
 */

extern void
ag_exception_registry_exit(void)
{
        ag_registry_release(&g_argent);
        ag_registry_release(&g_client);
}


/*******************************************************************************
 * `ag_exception_registry_msg()` gets the exception message associated with a
 * given error code.
 */

extern const char *
ag_exception_registry_msg(ag_erno erno)
{
        AG_ASSERT_TAG ("EXCEPTION_REGISTRY_INIT", g_argent && g_client);
        AG_ASSERT_TAG ("ERNO_VALID", erno);

        ag_registry *r = erno < 0 ? g_argent : g_client;
        ag_hash h = ag_hash_new(erno);

        struct ex *x = ag_registry_get(r, h);
        return x->msg;
}


/*******************************************************************************
 * `ag_exception_registry_hnd()` gets the exception handler associated with a
 * given error code.
 */

extern ag_exception_hnd *
ag_exception_registry_hnd(ag_erno erno)
{
        AG_ASSERT_TAG ("EXCEPTION_REGISTRY_INIT", g_argent && g_client);
        AG_ASSERT_TAG ("ERNO_VALID", erno);

        ag_registry *r = erno < 0 ? g_argent : g_client;
        ag_hash h = ag_hash_new(erno);

        struct ex *x = ag_registry_get(r, h);
        return x->hnd;
}


/*******************************************************************************
 * The `ag_exception_registry_set()` function is the only mutator for the
 * exception registry, and sets the exception message and handler associated
 * with a given error code. Subsequent calls to this function override the
 * previous values, and passing a NULL pointer for the handler causes the
 * default unhandled exception handler to be set.
 */

extern void
ag_exception_registry_set(ag_erno erno, const char *msg, ag_exception_hnd *hnd)
{
        AG_ASSERT_TAG ("EXCEPTION_REGISTRY_INIT", g_argent && g_client);
        AG_ASSERT_TAG ("ERNO_VALID", erno);
        AG_ASSERT_STR (msg);

        struct ex *x = ex_new(msg, hnd);

        ag_registry *r = erno < 0 ? g_argent : g_client;
        ag_hash h = ag_hash_new(erno);
        ag_registry_push(r, h, x);
}


/*******************************************************************************
 * `ex_new()` creates a new instance of an `ex` struct with a given exception
 * message and handler. In case a null pointer is passed for the second
 * parameter, then the default exception handler will be used. We avoid the use
 * of the `ag_memblock` and `ag_string` interfaces in order to prevent potential
 * circular dependencies.
 */

static struct ex *
ex_new(const char *msg, ag_exception_hnd *hnd)
{
        AG_ASSERT_STR (msg);

        struct ex *x = malloc(sizeof *x);

        if (AG_UNLIKELY (!x))
                goto error;

        size_t sz = strlen(msg);
        x->msg = malloc(sz + 1);

        if (AG_UNLIKELY (!x->msg))
                goto error;

        strcpy(x->msg, msg);
        x->msg[sz] = '\0';

        x->hnd = hnd ? hnd : ex_default;
        return x;

error:
        printf("[!] failed to allocate memory for exception registry\n");
        exit(EXIT_FAILURE);
}

       
/*******************************************************************************
 * `ex_release()` frees up the heap memory resources used by an `ex` struct
 * instance.
 */

static void
ex_release(void *hnd)
{
        if (AG_LIKELY (hnd)) {
                free(((struct ex *)hnd)->msg);
                free(hnd);
        }
}


/*******************************************************************************
 * `ex_default()` is the default exception handler used in case client code
 * passes a NULL exception handler through `ag_exception_reistry_set()`. This
 * handler simply terminates the application after printing and logging the
 * exception metadata.
 */

static void
ex_default(const struct ag_exception *ex, void *opt)
{
        (void) opt;
    
        printf("[!] (unhandled) %d [%s(), %s:%lu]: %s\n", ex->erno, ex->func,
            ex->file, ex->line, ag_exception_registry_msg(ex->erno));

        ag_log_err("(unhandled) %d [%s(), %s:%lu]: %s", ex->erno, ex->func,
            ex->file, ex->line, ag_exception_registry_msg(ex->erno));

        ag_exit(EXIT_FAILURE);
}

