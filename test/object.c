#include "./test.h"

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

        ag_object_registry_set(TYPEID_BASE, &vt);
}


static void
register_derived(void)
{
        struct ag_object_vtable vt = {
                .clone = virt_clone, .release = virt_release, .cmp = virt_cmp,
                .valid = virt_valid, .sz      = virt_sz,      .len = virt_len,
                .hash  = virt_hash,  .str     = virt_str,
        };

        ag_object_registry_set(TYPEID_DERIVED, &vt);
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


AG_TEST_CASE("ag_object_copy() makes a shallow copy of a base object")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (o == o2);
}


AG_TEST_CASE("ag_object_copy() makes a shallow copy of a derived"
                      " object")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (o == o2);
}


AG_TEST_CASE(
    "ag_object_copy() updates the reference count of a shallow copy of a base"
    " object")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (ag_object_refc(o) == 2);
}


AG_TEST_CASE(
    "ag_object_copy() updates the reference count of a shallow copy of a"
    " derived object")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (ag_object_refc(o) == 2);
}


AG_TEST_CASE(
    "ag_object_copy() preserves the data of the shallow copy of a base object")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        const struct payload_base *p = ag_object_payload(o);
        const struct payload_base *p2 = ag_object_payload(o2);

        AG_TEST (p->x == p2->x && p->y == p2->y);
}


AG_TEST_CASE(
    "ag_object_copy() preserves the data of the shallow copy of a derived"
    " object")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        const struct payload_derived *p = ag_object_payload(o);
        const struct payload_derived *p2 = ag_object_payload(o2);

        AG_TEST (*p->x == *p2->x && *p->y == *p2->y);
}


AG_TEST_CASE("ag_object_clone() makes a deep copy of a base object")
{
        AG_AUTO(ag_object) *o  = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_clone(o);
        AG_TEST (o != o2);
}


AG_TEST_CASE(
    "ag_object_clone() makes a deep copy of a derived object")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_clone(o);

        AG_TEST (o != o2);
}


AG_TEST_CASE(
    "ag_object_clone() does not affect the reference count of the original base"
    " object")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_clone(o);

        AG_TEST (ag_object_refc(o) == 1);
}


AG_TEST_CASE(
    "ag_object_clone() does not affect the reference count of the original"
    " derived object")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_clone(o);

        AG_TEST (ag_object_refc(o) == 1);
}


AG_TEST_CASE(
    "ag_object_clone() preserves the data of the deep copy of a base object")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_clone(o);

        const struct payload_base *p = ag_object_payload(o);
        const struct payload_base *p2 = ag_object_payload(o2);

        AG_TEST (p->x == p2->x && p->y == p2->y);
}


AG_TEST_CASE(
    "ag_object_copy() preserves the data of the deep copy of a a derived"
    " object")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_clone(o);

        const struct payload_derived *p = ag_object_payload(o);
        const struct payload_derived *p2 = ag_object_payload(o2);

        AG_TEST (*p->x == *p2->x && *p->y == *p2->y);
}


AG_TEST_CASE("ag_object_release() performs a no-op if passed NULL")
{
        ag_object_release(NULL);
        AG_TEST (true);
}


AG_TEST_CASE(
    "ag_object_release() performs a no-op if passed a handle to a null pointer")
{
        ag_object *o = NULL;
        ag_object_release(&o);

        AG_TEST (true);
}


AG_TEST_CASE("ag_object_release() releases a base object")
{
        ag_object *o = sample_base();
        ag_object_release(&o);

        AG_TEST (!o);
}

AG_TEST_CASE("ag_object_release() releases a derived object")
{
        ag_object *o = sample_derived();
        ag_object_release(&o);

        AG_TEST (!o);
}


AG_TEST_CASE(
    "ag_object_release() reduces the reference count by 1 for a base object")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        ag_object *o3 = ag_object_copy(o);
        ag_object_release(&o3);

        AG_TEST (ag_object_refc(o) == 2);
}


AG_TEST_CASE(
"ag_object_release() reduces the reference count by 1 for a derived object")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        ag_object *o3 = ag_object_copy(o);
        ag_object_release(&o3);

        AG_TEST (ag_object_refc(o) == 2);
}


AG_TEST_CASE(
    "ag_object_cmp() returns AG_CMP_EQ when comparing the same base objects")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (ag_object_cmp(o, o2) == AG_CMP_EQ);
}


AG_TEST_CASE(
    "ag_object_cmp() return AG_CMP_EQ when comparing the same derived objects")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (ag_object_cmp(o, o2) == AG_CMP_EQ);
}


AG_TEST_CASE(
    "ag_object_lt() returns false when comparing the same base objects")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (!ag_object_lt(o, o2));
}


AG_TEST_CASE(
    "ag_object_lt() returns false when comparing the same derived objects")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (!ag_object_lt(o, o2));
}


AG_TEST_CASE(
    "ag_object_gt() returns false when comparing the same base objects")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (!ag_object_gt(o, o2));
}


AG_TEST_CASE(
    "ag_object_gt() returns false when comparing the same derived objects")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (!ag_object_gt(o, o2));
}


AG_TEST_CASE(
    "ag_object_typeid() returns the type ID of a base object")
{
        AG_AUTO(ag_object) *o = sample_base();

        AG_TEST (ag_object_typeid(o) == TYPEID_BASE);
}


AG_TEST_CASE(
    "ag_object_typeid() returns the type ID of a derived object")
{
        AG_AUTO(ag_object) *o = sample_derived();

        AG_TEST (ag_object_typeid(o) == TYPEID_DERIVED);
}


AG_TEST_CASE("ag_object_uuid() returns the UUID of a base object")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_uuid) *u = ag_object_uuid(o);

        AG_TEST (!ag_uuid_empty(u));
}


AG_TEST_CASE("ag_object_uuid() returns the UUID of a derived object")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_uuid) *u = ag_object_uuid(o);

        AG_TEST (!ag_uuid_empty(u));
}


AG_TEST_CASE("ag_object_sz() returns the size of a base object")
{
        AG_AUTO(ag_object) *o = sample_base();

        AG_TEST (ag_object_sz(o));
}


AG_TEST_CASE("ag_object_sz() returns the size of a derived object")
{
        AG_AUTO(ag_object) *o = sample_derived();

        AG_TEST (ag_object_sz(o));
}


AG_TEST_CASE(
    "ag_object_sz() returns a greater size for a derived object than that of a"
    " base object")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = sample_derived();

        AG_TEST (ag_object_sz(o2) > ag_object_sz(o));
}


AG_TEST_CASE(
    "ag_object_refc() returns the reference count of a base object")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (ag_object_refc(o) == 2);
}


AG_TEST_CASE(
    "ag_object_refc() returns the reference count of a derived object")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        AG_TEST (ag_object_refc(o) == 2);
}


AG_TEST_CASE("ag_object_len() returns 1 for a base object")
{
        AG_AUTO(ag_object) *o = sample_base();

        AG_TEST (ag_object_len(o) == 1);
}


AG_TEST_CASE("ag_object_len() returns 0 for a derived object")
{
        AG_AUTO(ag_object) *o = sample_derived();

        AG_TEST (ag_object_len(o) == 0);
}


AG_TEST_CASE(
    "ag_object_valid() executes its default callback if not overridden")
{
        AG_AUTO(ag_object) *o = sample_base();

        AG_TEST (ag_object_valid(o));
}


AG_TEST_CASE(
    "ag_object_valid() executes its provided callback if overridden")
{
        AG_AUTO(ag_object) *o = sample_derived();

        AG_TEST (ag_object_valid(o));
}


AG_TEST_CASE(
    "ag_object_hash() executes its default callback if not overridden")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_uuid) *u = ag_object_uuid(o);

        AG_TEST (ag_object_hash(o) == ag_uuid_hash(u));
}


AG_TEST_CASE(
    "ag_object_hash() executes its provided callback if overridden")
{
        AG_AUTO(ag_object) *o = sample_derived();
        const struct payload_derived *p = ag_object_payload(o);

        AG_TEST (ag_object_hash(o) == ag_hash_new(*p->x));
}


AG_TEST_CASE(
    "ag_object_str() executes its default callback if not overridden")
{
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_string) *s = ag_object_str(o);

        AG_TEST (ag_string_has(s, "uuid"));
}


AG_TEST_CASE(
    "ag_object_str() executes its provided callback if overridden")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_string) *s = ag_object_str(o);

        AG_TEST (ag_string_eq(s, "This is a sample derived object"));
}


AG_TEST_CASE(
    "ag_object_empty() returns false if the object length is greater than zero")
{
        AG_AUTO(ag_object) *o = sample_base();

        AG_TEST (!ag_object_empty(o));
}


AG_TEST_CASE(
    "ag_object_empty() returns true if the object length is zero")
{
        AG_AUTO(ag_object) *o = sample_derived();

        AG_TEST (ag_object_empty(o));
}


AG_TEST_CASE(
    "ag_object_payload() gets a handle to the object payload")
{
        AG_AUTO(ag_object) *o = sample_base();
        const struct payload_base *p = ag_object_payload(o);

        AG_TEST (p->x == 555 && p->y == -666);
}


AG_TEST_CASE(
    "ag_object_payload_mutable() gets a handle to the object payload")
{
        AG_AUTO(ag_object) *o = sample_derived();
        struct payload_derived *p = ag_object_payload_mutable(&o);

        AG_TEST (*p->x == 555 && *p->y == -666);
}


AG_TEST_CASE(
    "ag_object_payload_mutable() does not create a clone of the object if its"
    " refc is 1")
{
        AG_AUTO(ag_object) *o = sample_derived();
        ag_object *o2 = o;
        (void)ag_object_payload_mutable(&o);

        AG_TEST (o == o2);
}


AG_TEST_CASE(
    "ag_object_payload_mutable() creates a clone of  the object if its"
    " refc > 1")
{
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        (void)ag_object_payload_mutable(&o);

        AG_TEST (o != o2);
}


AG_TEST_CASE(
    "ag_object_payload_mutable() reduces the refc of the original object by 1"
    " if it creates a clone")
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

