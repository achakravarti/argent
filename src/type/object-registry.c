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


#include "../../include/argent.h"


/*******************************************************************************
 * Although the object registry presents a unified interface, it is actually
 * implemented as two separate internal registries, just like the exception
 * registry. The `g_argent` registry is responsible for holding the objects
 * specific to the Argent Library, and the `g_client` registry holds the objects
 * of client code.
 *
 * The objects specific to the Argent Library are distinguished by having a
 * negative type ID whereas those of the client code are required to be
 * positive. By having two separate registries, hash collisions between Argent
 * Library and client code objects is prevented (since the hash of the type ID
 * is used as the key, and the hash is always determined on the absolute value).
 *
 * Supporting these two internal registries is the `reg_dispose()` helper
 * function, which is invoked during the disposal of the registries.
 */

static ag_registry *g_argent;
static ag_registry *g_client;

static void     reg_dispose(void *);


/*******************************************************************************
 * The object v-table methods are set to default callbacks in case they are not
 * provied by client code. By providing default callbacks we not only gain in
 * convenience but also improve performance since we avoid the need to check
 * whether or not a callback has been assigned when an object method is invoked.
 */

static ag_memblock      *def_clone(const ag_memblock *);
static void              def_release(ag_memblock *);
static enum ag_cmp       def_cmp(const ag_object *, const ag_object *);
static bool              def_valid(const ag_object *);
static size_t            def_sz(const ag_object *);
static size_t            def_len(const ag_object *);
static size_t            def_hash(const ag_object *);
static ag_string        *def_str(const ag_object *);
static ag_string        *def_json(const ag_object *);


/*******************************************************************************
 * The `ag_object_registry_init()` interface function initialises the object
 * registry by creating new instances of the internal registries. Both of the
 * internal registries are passed the `reg_dispose()` helper function as the
 * disposal callback.
 */

extern void
ag_object_registry_init(void)
{
        g_argent = ag_registry_new(reg_dispose);
        g_client = ag_registry_new(reg_dispose);
}


/*******************************************************************************
 * The `ag_object_registry_exit()` interface function is the converse of the
 * `ag_object_registry_init()` function, and is responsible for releasing the
 * heap memory resources used by the object registry.
 */

extern void
ag_object_registry_exit(void)
{
        ag_registry_release(&g_argent);
        ag_registry_release(&g_client);
}


/*******************************************************************************
 * The `ag_object_registry_get()` interface function gets the the object v-table
 * associated with an object with a given type ID (passed through the first
 * parameter). The retrieval from the correct internal registry is determined
 * based on whether or not the type ID is positive.
 */

extern const struct ag_object_vtable *
ag_object_registry_get(ag_typeid typeid)
{
        ag_registry *r = typeid < 0 ? g_argent : g_client;
        ag_hash h = ag_hash_new(typeid);

        struct ag_object_vtable *v = ag_registry_get(r, h);
        return v;
}


/*******************************************************************************
 * The `ag_object_registry_push()` interface function pushes the object v-table
 * (second parameter) associated with a given object type ID (first parameter).
 * If any of the callbacks in the v-table are not provided (indicated by `NULL`)
 * then they are set to their corresponding default callback.
 */

extern void
ag_object_registry_push(ag_typeid typeid, const struct ag_object_vtable *vt)
{
        struct ag_object_vtable *v = ag_memblock_new(sizeof *v);
        v->clone = vt->clone ? vt->clone : def_clone;
        v->release = vt->release ? vt->release : def_release;
        v->cmp = vt->cmp ? vt->cmp : def_cmp;
        v->valid = vt->valid ? vt->valid : def_valid;
        v->sz = vt->sz ? vt->sz : def_sz;
        v->len = vt->len ? vt->len : def_len;
        v->hash = vt->hash ? vt->hash : def_hash;
        v->str = vt->str ? vt->str : def_str;
        v->json = vt->json ? vt->json : def_json;

        ag_registry *r = typeid < 0 ? g_argent : g_client;
        ag_hash h = ag_hash_new(typeid);
        ag_registry_push(r, h, v);
}


/*******************************************************************************
 * The `reg_dispose()` helper function is the callback function invoked by
 * `ag_registry_release()` to dispose of each object v-table instance contained
 * within the internal registries during the release cycle.
 */

static void
reg_dispose(void *hnd)
{
        ag_memblock_release(&hnd);
}


/*******************************************************************************
 * The `def_clone()` helper function is the default callback function of the
 * `ag_object_clone()` method. The handle to the object payload is passed
 * through the only parameter, and we simply return a clone of it.
 */

static ag_memblock *
def_clone(const ag_memblock *hnd)
{
        return ag_memblock_clone(hnd);
}


/*******************************************************************************
 * The `def_release()` helper function is the default callback function of the
 * `ag_object_release()` method. The handle to the payload is passed as the only
 * parameter. We don't do anything because `ag_object_release()` takes care of
 * releasing the memory allocated to the payload.
 */

static void
def_release(ag_memblock *hnd)
{
        (void)hnd;
}


/*******************************************************************************
 * The `def_cmp()` helper function is the default callback function of the
 * `ag_object_cmp()` method. The handles to the left-hand and right-hand objects
 * are respectively passed as parameters, and we return the result of the memory
 * comparison of both.
 */

static enum ag_cmp
def_cmp(const ag_object *lhs, const ag_object *rhs)
{
        return ag_memblock_cmp(lhs, rhs);
}


/*******************************************************************************
 * The `def_valid()` helper function is the default callback function of the
 * `ag_object_valid()` method. The handle to the contextual object is passed as
 * the only parameter. By default, an object is considered to be valid if its
 * handle is not `NULL`.
 */

static bool
def_valid(const ag_object *hnd)
{
        return hnd;
}


/*******************************************************************************
 * The `def_sz()` helper function is the default callback function for the
 * `ag_object_sz()` method. The handle to the contextual object is passed as the
 * only parameter. We return the size of object along with the size of its
 * payload.
 */

static size_t
def_sz(const ag_object *hnd)
{
        return ag_memblock_sz(hnd) + ag_memblock_sz(ag_object_payload(hnd));
}


/*******************************************************************************
 * The `def_len()` helper function is the default callback function for the
 * `ag_object_len()` method. The handle to the contextual object is passed as
 * the only parameter, but is unused as we return 1 by default.
 */

static size_t
def_len(const ag_object *hnd)
{
        (void)hnd;
        return 1;
}


/*******************************************************************************
 * The `def_hash()` helper function is the default callback function for the
 * `ag_object_hash()` method. The handle to the contextual object is passed as
 * the only parameter, and we return the hash of its UUID.
 */

static ag_hash
def_hash(const ag_object *hnd)
{
        AG_AUTO(ag_uuid) *u = ag_object_uuid(hnd);
        return ag_uuid_hash(u);
}


/*******************************************************************************
 * The `def_str()` helper function is the default callback function for the
 * `ag_object_str()` method. The handle to the contextual object is passed as
 * the only parameter. By default, we return a string specifying the type ID,
 * the UUID, and the address in memory of the object.
 */

static ag_string *
def_str(const ag_object *hnd)
{
        AG_AUTO(ag_uuid) *u = ag_object_uuid(hnd);
        AG_AUTO(ag_string)  *ustr = ag_uuid_str(u);
        AG_AUTO(ag_string)  *mstr = ag_memblock_str(hnd);

        return ag_string_new_fmt("typeid = %d, uuid = %s, address = %s",
            ag_object_typeid(hnd), ustr, mstr);
}


static ag_string *
def_json(const ag_object *hnd)
{
        AG_AUTO(ag_uuid) *u = ag_object_uuid(hnd);
        AG_AUTO(ag_string) *ustr = ag_uuid_str(u);
        AG_AUTO(ag_string) *mstr = ag_memblock_str(hnd);

        return ag_string_new_fmt(
            "{\"object\":"
            "{\"typeid\":\"%d\",\"uuid\":\"%s\",\"address\":\"%s\"}}",
            ag_object_typeid(hnd), ustr, mstr);
}

