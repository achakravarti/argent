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

#include "./test.h"
#include "./object.h"


#define __AG_TEST_SUITE_ID__ 3


#define TYPEID_BASE    ((ag_typeid) 1)
#define TYPEID_DERIVED ((ag_typeid) 2)


struct payload_base {
        int     x;
        int     y;
};


struct payload_derived {
        int     *x;
        int     *y;
};


static ag_object *
sample_base(void)
{
        struct payload_base *p = ag_memblock_new(sizeof *p);
        p->x = 555;
        p->y = -666;

        return ag_object_new(TYPEID_BASE, p);
}


static ag_object *
sample_derived(void)
{
        struct payload_derived *p = ag_memblock_new(sizeof *p);

        p->x = ag_memblock_new(sizeof *p->x);
        p->y = ag_memblock_new(sizeof *p->y);

        *p->x = 555;
        *p->y = -666;

        return ag_object_new(TYPEID_DERIVED, p);
}

        
static size_t
sample_base_sz(void)
{
        AG_AUTO(ag_object) *o = sample_base();
        return ag_memblock_sz(o) + sizeof(struct payload_base);
}

        
static ag_memblock *
virt_clone(const ag_memblock *payload)
{
        const struct payload_derived *p = payload;
        struct payload_derived *cp = ag_memblock_new(sizeof *p);

        cp->x = ag_memblock_new(sizeof *cp->x);
        cp->y = ag_memblock_new(sizeof *cp->y);

        *cp->x = *p->x;
        *cp->y = *p->y;

        return cp;
}


static void
virt_release(ag_memblock *payload)
{
        struct payload_derived *p = (struct payload_derived *)payload;
        ag_memblock_release((void **) &p->x);
        ag_memblock_release((void **) &p->y);
}


static enum ag_cmp
virt_cmp(const ag_object *ctx, const ag_object *cmp)
{
        AG_AUTO(ag_uuid) *u1 = ag_object_uuid(ctx);
        AG_AUTO(ag_uuid) *u2 = ag_object_uuid(cmp);

        return ag_uuid_cmp(u1, u2);
}


static bool
virt_valid(const ag_object *ctx)
{
        const struct payload_derived *p = ag_object_payload(ctx);
        return *p->x == 555 && *p->y == -666;
}


static size_t
virt_sz(const ag_object *ctx)
{
        const struct payload_derived *p = ag_object_payload(ctx);

        return ag_memblock_sz(ctx) + ag_memblock_sz(p) + ag_memblock_sz(p->x)
               + ag_memblock_sz(p->y);
}

        
static size_t
sample_derived_sz(void)
{
        AG_AUTO(ag_object) *o = sample_derived();
        return virt_sz(o);
}


static size_t
virt_len(const ag_object *ctx)
{
        (void)ctx;
        return 0;
}


static ag_hash
virt_hash(const ag_object *ctx)
{
        const struct payload_derived *p = ag_object_payload(ctx);
        return ag_hash_new(*p->x);
}


static ag_string *
virt_str(const ag_object *ctx)
{
        (void)ctx;
        return ag_string_new("This is a sample derived object");
}


static void
register_base(void)
{
        struct ag_object_vtable vt = {
                .clone = NULL, .release = NULL, .cmp = NULL,
                .valid = NULL, .sz      = NULL, .len = NULL,
                .hash = NULL,  .str     = NULL,
        };

        ag_object_registry_push(TYPEID_BASE, "BASE", &vt);
}


static void
register_derived(void)
{
        struct ag_object_vtable vt = {
                .clone = virt_clone, .release = virt_release, .cmp = virt_cmp,
                .valid = virt_valid, .sz      = virt_sz,      .len = virt_len,
                .hash  = virt_hash,  .str     = virt_str,
        };

        ag_object_registry_push(TYPEID_DERIVED, "DERIVED", &vt);
}


AG_TEST_CASE("ag_object_new() creates a new base object")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_TEST (o);
}


AG_TEST_CASE("ag_object_new() creates a new derived object")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_TEST (o);
}


AG_METATEST_OBJECT_COPY(ag_object, sample_base());
AG_METATEST_OBJECT_COPY(ag_object, sample_derived());


//AG_METATEST_OBJECT_CLONE(ag_object, sample_base());
//AG_METATEST_OBJECT_CLONE(ag_object, sample_derived());


AG_METATEST_OBJECT_RELEASE(ag_object, sample_base());
AG_METATEST_OBJECT_RELEASE(ag_object, sample_derived());


AG_TEST_CASE("ag_object_cmp() returns AG_CMP_EQ when comparing the same base"
    " objects")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (ag_object_cmp(o, o2) == AG_CMP_EQ);
}


AG_TEST_CASE("ag_object_cmp() return AG_CMP_EQ when comparing the same derived"
    " objects")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (ag_object_cmp(o, o2) == AG_CMP_EQ);
}


AG_TEST_CASE("ag_object_lt() returns false when comparing the same base"
    " objects")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (!ag_object_lt(o, o2));
}


AG_TEST_CASE("ag_object_lt() returns false when comparing the same derived"
    " objects")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (!ag_object_lt(o, o2));
}


AG_TEST_CASE("ag_object_gt() returns false when comparing the same base"
    " objects")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (!ag_object_gt(o, o2));
}


AG_TEST_CASE("ag_object_gt() returns false when comparing the same derived"
    " objects")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (!ag_object_gt(o, o2));
}


AG_METATEST_OBJECT_TYPEID(ag_object, sample_base(), TYPEID_BASE);
AG_METATEST_OBJECT_TYPEID(ag_object, sample_derived(), TYPEID_DERIVED);


AG_METATEST_OBJECT_UUID(ag_object, sample_base());
AG_METATEST_OBJECT_UUID(ag_object, sample_derived());


AG_METATEST_OBJECT_SZ(ag_object, sample_base(), sample_base_sz());
AG_METATEST_OBJECT_SZ(ag_object, sample_derived(), sample_derived_sz());


AG_METATEST_OBJECT_REFC(ag_object, sample_base());
AG_METATEST_OBJECT_REFC(ag_object, sample_derived());


AG_METATEST_OBJECT_LEN(ag_object, sample_base(), 1);
AG_METATEST_OBJECT_LEN(ag_object, sample_derived(), 0);


AG_METATEST_OBJECT_VALID(ag_object, sample_base());
AG_METATEST_OBJECT_VALID(ag_object, sample_derived());


AG_METATEST_OBJECT_HASH(ag_object, sample_derived(), ag_hash_new(555));


AG_TEST_CASE("ag_object_hash() executes its default callback if not overridden")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_uuid) *u = ag_object_uuid(o);

        AG_TEST (ag_object_hash(o) == ag_uuid_hash(u));
}


AG_METATEST_OBJECT_STR_HAS(ag_object, sample_base(), "uuid");
AG_METATEST_OBJECT_STR(ag_object, sample_derived(),
    "This is a sample derived object");


AG_METATEST_OBJECT_EMPTY(ag_object, sample_derived());
AG_METATEST_OBJECT_EMPTY_NOT(ag_object, sample_base())


AG_TEST_CASE("ag_object_payload() gets a handle to the object payload")
{
        AG_AUTO(ag_object) *o = sample_base();
        const struct payload_base *p = ag_object_payload(o);

        AG_TEST (p->x == 555 && p->y == -666);
}


AG_TEST_CASE("ag_object_payload_mutable() gets a handle to the object payload")
{
        AG_AUTO(ag_object) *o = sample_derived();
        struct payload_derived *p = ag_object_payload_mutable(&o);

        AG_TEST (*p->x == 555 && *p->y == -666);
}


AG_TEST_CASE("ag_object_payload_mutable() does not create a clone of the object"
    " if its refc is 1")
{
        AG_AUTO(ag_object) *o = sample_derived();
        ag_object *o2 = o;
        (void)ag_object_payload_mutable(&o);

        AG_TEST (o == o2);
}


AG_TEST_CASE("ag_object_payload_mutable() creates a clone of  the object if its"
    " refc > 1")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        (void)ag_object_payload_mutable(&o);

        AG_TEST (o != o2);
}


AG_TEST_CASE("ag_object_payload_mutable() reduces the refc of the original"
    " object by 1 if it creates a clone")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_AUTO(ag_object) *o3 = ag_object_copy(o);
        (void)ag_object_payload_mutable(&o);

        AG_TEST (ag_object_refc(o3) == 2);
}


extern ag_test_suite *test_suite_object(void)
{
        register_base();
        register_derived();

        return AG_TEST_SUITE_GENERATE("ag_object interface");
}

