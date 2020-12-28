#include "./test.h"


#define TYPEID_BASE    ((ag_typeid) 1)
#define TYPEID_DERIVED ((ag_typeid) 2)


struct payload_base {
        int x;
        int y;
};


struct payload_derived {
        int *x;
        int *y;
};


static ag_object *sample_base(void)
{
        struct payload_base *p = ag_mblock_new(sizeof *p);
        p->x = 555;
        p->y = -666;

        return ag_object_new(TYPEID_BASE, p);
}


static ag_object *sample_derived(void)
{
        struct payload_derived *p = ag_mblock_new(sizeof *p);

        p->x = ag_mblock_new(sizeof *p->x);
        p->y = ag_mblock_new(sizeof *p->y);

        *p->x = 555;
        *p->y = -666;

        return ag_object_new(TYPEID_DERIVED, p);
}


static ag_mblock *virt_clone(const ag_mblock *payload)
{
    const struct payload_derived *p = (const struct payload_derived *)payload;
    struct payload_derived *cp = ag_mblock_new(sizeof *p);

    cp->x = ag_mblock_new(sizeof *cp->x);
    cp->y = ag_mblock_new(sizeof *cp->y);

    *cp->x = *p->x;
    *cp->y = *p->y;

    return cp;
}


static void virt_release(ag_mblock *payload)
{
    struct payload_derived *p = (struct payload_derived *)payload;
    ag_mblock_release((void **) &p->x);
    ag_mblock_release((void **) &p->y);
}


static enum ag_cmp virt_cmp(const ag_object *ctx, const ag_object *cmp)
{
        AG_AUTO(ag_uuid) *u1 = ag_object_uuid(ctx);
        AG_AUTO(ag_uuid) *u2 = ag_object_uuid(cmp);

        return ag_uuid_cmp(u1, u2);
}


static bool virt_valid(const ag_object *ctx)
{
        const struct payload_derived *p = ag_object_payload(ctx);
        return *p->x == 555 && *p->y == -666;
}


static size_t virt_sz(const ag_object *ctx)
{
        const struct payload_derived *p = ag_object_payload(ctx);

        return ag_mblock_sz(ctx) + ag_mblock_sz(p) + ag_mblock_sz(p->x)
               + ag_mblock_sz(p->y);
}


static size_t virt_len(const ag_object *ctx)
{
        (void)ctx;
        return 0;
}


static ag_hash virt_hash(const ag_object *ctx)
{
        const struct payload_derived *p = ag_object_payload(ctx);
        return ag_hash_new(*p->x);
}


static ag_str *virt_str(const ag_object *ctx)
{
        (void)ctx;
        return ag_str_new("This is a sample derived object");
}


static void register_base(void)
{
        struct ag_object_vtable vt = {
                .clone = NULL, .release = NULL, .cmp = NULL,
                .valid = NULL, .sz      = NULL, .len = NULL,
                .hash = NULL,  .str     = NULL,
        };

        ag_object_registry_set(TYPEID_BASE, &vt);
}


static void register_derived(void)
{
        struct ag_object_vtable vt = {
                .clone = virt_clone, .release = virt_release, .cmp = virt_cmp,
                .valid = virt_valid, .sz      = virt_sz,      .len = virt_len,
                .hash  = virt_hash,  .str     = virt_str,
        };

        ag_object_registry_set(TYPEID_DERIVED, &vt);
}


AG_TEST_INIT(new_01, "ag_object_new() creates a new base object") {
        AG_AUTO(ag_object) *o = sample_base();
        AG_TEST_ASSERT (o);
} AG_TEST_EXIT();


AG_TEST_INIT(new_02, "ag_object_new() creates a new derived object") {
        AG_AUTO(ag_object) *o = sample_derived();
        AG_TEST_ASSERT (o);
} AG_TEST_EXIT();


AG_TEST_INIT(copy_01, "ag_object_copy() makes a shallow copy of a base object") {
        AG_AUTO(ag_object) *o  = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_TEST_ASSERT (o == o2);
} AG_TEST_EXIT();


AG_TEST_INIT(copy_02, "ag_object_copy() makes a shallow copy of a derived"
                      " object") {
        AG_AUTO(ag_object) *o  = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_TEST_ASSERT (o == o2);
} AG_TEST_EXIT();


AG_TEST_INIT(copy_03, "ag_object_copy() updates the reference count of a shallow"
                      " copy of a base object") {
        AG_AUTO(ag_object) *o  = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_TEST_ASSERT (ag_object_refc(o) == 2);
} AG_TEST_EXIT();


AG_TEST_INIT(copy_04, "ag_object_copy() updates the reference count of a shallow"
                      " copy of a derived object") {
        AG_AUTO(ag_object) *o  = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_TEST_ASSERT (ag_object_refc(o) == 2);
} AG_TEST_EXIT();


AG_TEST_INIT(copy_05, "ag_object_copy() preserves the data of the shallow copy of"
                      " a base object") {
        AG_AUTO(ag_object) *o  = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        const struct payload_base *p  = ag_object_payload(o);
        const struct payload_base *p2 = ag_object_payload(o2);

        AG_TEST_ASSERT (p->x == p2->x && p->y == p2->y);
} AG_TEST_EXIT();


AG_TEST_INIT(copy_06, "ag_object_copy() preserves the data of the shallow copy of"
                      " a derived object") {
        AG_AUTO(ag_object) *o  = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);

        const struct payload_derived *p  = ag_object_payload(o);
        const struct payload_derived *p2 = ag_object_payload(o2);

        AG_TEST_ASSERT (*p->x == *p2->x && *p->y == *p2->y);
} AG_TEST_EXIT();


AG_TEST_INIT(clone_01, "ag_object_clone() makes a deep copy of a base object") {
        AG_AUTO(ag_object) *o  = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_clone(o);
        AG_TEST_ASSERT (o != o2);
} AG_TEST_EXIT();


AG_TEST_INIT(clone_02, "ag_object_clone() makes a deep copy of a derived object") {
        AG_AUTO(ag_object) *o  = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_clone(o);
        AG_TEST_ASSERT (o != o2);
} AG_TEST_EXIT();


AG_TEST_INIT(clone_03, "ag_object_clone() does not affect the reference count of"
                       " the original base object") {
        AG_AUTO(ag_object) *o  = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_clone(o);
        AG_TEST_ASSERT (ag_object_refc(o) == 1);
} AG_TEST_EXIT();


AG_TEST_INIT(clone_04, "ag_object_clone() does not affect the reference count of"
                       " the original derived object") {
        AG_AUTO(ag_object) *o  = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_clone(o);
        AG_TEST_ASSERT (ag_object_refc(o) == 1);
} AG_TEST_EXIT();


AG_TEST_INIT(clone_05, "ag_object_clone() preserves the data of the deep copy of a"
                       " base object") {
        AG_AUTO(ag_object) *o  = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_clone(o);

        const struct payload_base *p  = ag_object_payload(o);
        const struct payload_base *p2 = ag_object_payload(o2);

        AG_TEST_ASSERT (p->x == p2->x && p->y == p2->y);
} AG_TEST_EXIT();


AG_TEST_INIT(clone_06, "ag_object_copy() preserves the data of the deep copy of a"
                       " a derived object") {
        AG_AUTO(ag_object) *o  = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_clone(o);

        const struct payload_derived *p  = ag_object_payload(o);
        const struct payload_derived *p2 = ag_object_payload(o2);

        AG_TEST_ASSERT (*p->x == *p2->x && *p->y == *p2->y);
} AG_TEST_EXIT();


AG_TEST_INIT(release_01, "ag_object_release() performs a no-op if passed NULL") {
        ag_object_release(NULL);
        AG_TEST_ASSERT (true);
} AG_TEST_EXIT();


AG_TEST_INIT(release_02, "ag_object_release() performs a no-op if passed a handle"
                         " to a null pointer") {
        ag_object *o = NULL;
        ag_object_release(&o);
        AG_TEST_ASSERT (true);
} AG_TEST_EXIT();


AG_TEST_INIT(release_03, "ag_object_release() releases a base object") {
        ag_object *o = sample_base();
        ag_object_release(&o);
        AG_TEST_ASSERT (!o);
} AG_TEST_EXIT();

AG_TEST_INIT(release_04, "ag_object_release() releases a derived object") {
        ag_object *o = sample_derived();
        ag_object_release(&o);
        AG_TEST_ASSERT (!o);
} AG_TEST_EXIT();


AG_TEST_INIT(release_05, "ag_object_release() reduces the reference count by 1 for"
                         " a base object") {
        AG_AUTO(ag_object) *o  = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        ag_object          *o3 = ag_object_copy(o);

        ag_object_release(&o3);
        AG_TEST_ASSERT (ag_object_refc(o) == 2);
} AG_TEST_EXIT();


AG_TEST_INIT(release_06, "ag_object_release() reduces the reference count by 1 for"
                         " a derived object") {
        AG_AUTO(ag_object) *o  = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        ag_object          *o3 = ag_object_copy(o);

        ag_object_release(&o3);
        AG_TEST_ASSERT (ag_object_refc(o) == 2);
} AG_TEST_EXIT();


AG_TEST_INIT(cmp_01, "ag_object_cmp() returns AG_CMP_EQ when comparing the same"
                     " base objects") {
        AG_AUTO(ag_object) *o  = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_TEST_ASSERT (ag_object_cmp(o, o2) == AG_CMP_EQ);
} AG_TEST_EXIT();


AG_TEST_INIT(cmp_02, "ag_object_cmp() return AG_CMP_EQ when comparing the same"
                     " derived objects") {
        AG_AUTO(ag_object) *o  = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_TEST_ASSERT (ag_object_cmp(o, o2) == AG_CMP_EQ);
} AG_TEST_EXIT();


AG_TEST_INIT(lt_01, "ag_object_lt() returns false when comparing the same base"
                    " objects") {
        AG_AUTO(ag_object) *o  = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_TEST_ASSERT (!ag_object_lt(o, o2));
} AG_TEST_EXIT();


AG_TEST_INIT(lt_02, "ag_object_lt() returns false when comparing the same derived"
                    " objects") {
        AG_AUTO(ag_object) *o  = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_TEST_ASSERT (!ag_object_lt(o, o2));
} AG_TEST_EXIT();


AG_TEST_INIT(gt_01, "ag_object_gt() returns false when comparing the same base"
                    " objects") {
        AG_AUTO(ag_object) *o  = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_TEST_ASSERT (!ag_object_gt(o, o2));
} AG_TEST_EXIT();


AG_TEST_INIT(gt_02, "ag_object_gt() returns false when comparing the same derived"
                    " objects") {
        AG_AUTO(ag_object) *o  = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_TEST_ASSERT (!ag_object_gt(o, o2));
} AG_TEST_EXIT();


AG_TEST_INIT(typeid_01, "ag_object_typeid() returns the type ID of a base"
                        " object") {
        AG_AUTO(ag_object) *o  = sample_base();
        AG_TEST_ASSERT (ag_object_typeid(o) == TYPEID_BASE);
} AG_TEST_EXIT();


AG_TEST_INIT(typeid_02, "ag_object_typeid() returns the type ID of a derived"
                        " object") {
        AG_AUTO(ag_object) *o  = sample_derived();
        AG_TEST_ASSERT (ag_object_typeid(o) == TYPEID_DERIVED);
} AG_TEST_EXIT();


AG_TEST_INIT(uuid_01, "ag_object_uuid() returns the UUID of a base object") {
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_uuid) *u = ag_object_uuid(o);
        AG_TEST_ASSERT (!ag_uuid_empty(u));
} AG_TEST_EXIT();


AG_TEST_INIT(uuid_02, "ag_object_uuid() returns the UUID of a derived object") {
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_uuid) *u = ag_object_uuid(o);
        AG_TEST_ASSERT (!ag_uuid_empty(u));
} AG_TEST_EXIT();


AG_TEST_INIT(sz_01, "ag_object_sz() returns the size of a base object") {
        AG_AUTO(ag_object) *o = sample_base();
        AG_TEST_ASSERT (ag_object_sz(o));
} AG_TEST_EXIT();


AG_TEST_INIT(sz_02, "ag_object_sz() returns the size of a derived object") {
        AG_AUTO(ag_object) *o = sample_derived();
        AG_TEST_ASSERT (ag_object_sz(o));
} AG_TEST_EXIT();


AG_TEST_INIT(sz_03, "ag_object_sz() returns a greater size for a derived object"
                    " than that of a base object") {
        AG_AUTO(ag_object) *o  = sample_base();
        AG_AUTO(ag_object) *o2 = sample_derived();
        AG_TEST_ASSERT (ag_object_sz(o2) > ag_object_sz(o));
} AG_TEST_EXIT();


AG_TEST_INIT(refc_01, "ag_object_refc() returns the reference count of a base"
                      " object") {
        AG_AUTO(ag_object) *o  = sample_base();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_TEST_ASSERT (ag_object_refc(o) == 2);
} AG_TEST_EXIT();


AG_TEST_INIT(refc_02, "ag_object_refc() returns the reference count of a derived"
                      " object") {
        AG_AUTO(ag_object) *o  = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_TEST_ASSERT (ag_object_refc(o) == 2);
} AG_TEST_EXIT();


AG_TEST_INIT(len_01, "ag_object_len() returns 1 for a base object") {
        AG_AUTO(ag_object) *o = sample_base();
        AG_TEST_ASSERT (ag_object_len(o) == 1);
} AG_TEST_EXIT();


AG_TEST_INIT(len_02, "ag_object_len() returns 0 for a derived object") {
        AG_AUTO(ag_object) *o = sample_derived();
        AG_TEST_ASSERT (ag_object_len(o) == 0);
} AG_TEST_EXIT();


AG_TEST_INIT(valid_01, "ag_object_valid() executes its default callback if not"
                       " overridden") {
        AG_AUTO(ag_object) *o = sample_base();
        AG_TEST_ASSERT (ag_object_valid(o));
} AG_TEST_EXIT();


AG_TEST_INIT(valid_02, "ag_object_valid() executes its provided callback if"
                       " overridden") {
        AG_AUTO(ag_object) *o = sample_derived();
        AG_TEST_ASSERT (ag_object_valid(o));
} AG_TEST_EXIT();


AG_TEST_INIT(hash_01, "ag_object_hash() executes its default callback if not"
                      " overridden") {
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_uuid) *u = ag_object_uuid(o);
        AG_TEST_ASSERT (ag_object_hash(o) == ag_uuid_hash(u));
} AG_TEST_EXIT();


AG_TEST_INIT(hash_02, "ag_object_hash() executes its provided callback if"
                      " overridden") {
        AG_AUTO(ag_object) *o = sample_derived();
        const struct payload_derived *p = ag_object_payload(o);
        AG_TEST_ASSERT (ag_object_hash(o) == ag_hash_new(*p->x));
} AG_TEST_EXIT();


AG_TEST_INIT(str_01, "ag_object_str() executes its default callback if not"
                     " overridden") {
        AG_AUTO(ag_object) *o = sample_base();
        AG_AUTO(ag_str) *s = ag_object_str(o);
        AG_TEST_ASSERT (ag_str_has(s, "uuid"));
} AG_TEST_EXIT();


AG_TEST_INIT(str_02, "ag_object_str() executes its provided callback if"
                     " overridden") {
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_str) *s = ag_object_str(o);
        AG_TEST_ASSERT (ag_str_eq(s, "This is a sample derived object"));
} AG_TEST_EXIT();


AG_TEST_INIT(empty_01, "ag_object_empty() returns false if the object length"
                       " is greater than zero") {
        AG_AUTO(ag_object) *o = sample_base();
        AG_TEST_ASSERT (!ag_object_empty(o));
} AG_TEST_EXIT();


AG_TEST_INIT(empty_02, "ag_object_empty() returns true if the object length is"
                       " zero") {
        AG_AUTO(ag_object) *o = sample_derived();
        AG_TEST_ASSERT (ag_object_empty(o));
} AG_TEST_EXIT();


AG_TEST_INIT(payload_01, "ag_object_payload() gets a handle to the object"
                         " payload") {
        AG_AUTO(ag_object) *o = sample_base();
        const struct payload_base *p = ag_object_payload(o);
        AG_TEST_ASSERT (p->x == 555 && p->y == -666);
} AG_TEST_EXIT();


AG_TEST_INIT(payload_mutable_01, "ag_object_payload_mutable() gets a handle to"
                                 " the object payload") {
        AG_AUTO(ag_object) *o = sample_derived();
        struct payload_derived *p = ag_object_payload_mutable(&o);
        AG_TEST_ASSERT (*p->x == 555 && *p->y == -666);
} AG_TEST_EXIT();


AG_TEST_INIT(payload_mutable_02, "ag_object_payload_mutable() does not create a"
                                 " clone of the object if its refc is 1") {
        AG_AUTO(ag_object) *o  = sample_derived();
        ag_object          *o2 = o;
        (void)ag_object_payload_mutable(&o);
        AG_TEST_ASSERT (o == o2);
} AG_TEST_EXIT();


AG_TEST_INIT(payload_mutable_03, "ag_object_payload_mutable() creates a clone of"
                                 " the object if its refc > 1") {
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        (void)ag_object_payload_mutable(&o);
        AG_TEST_ASSERT (o != o2);
} AG_TEST_EXIT();


AG_TEST_INIT(payload_mutable_04, "ag_object_payload_mutable() reduces the refc of"
                                 " the original object by 1 if it creates a"
                                 " clone") {
        AG_AUTO(ag_object) *o = sample_derived();
        AG_AUTO(ag_object) *o2 = ag_object_copy(o);
        AG_AUTO(ag_object) *o3 = ag_object_copy(o);
        (void)ag_object_payload_mutable(&o);
        AG_TEST_ASSERT (ag_object_refc(o3) == 2);
} AG_TEST_EXIT();


extern ag_test_suite *test_suite_obj(void)
{
        register_base();
        register_derived();

        ag_test *test[] = {
                new_01,             new_02,             copy_01,
                copy_02,            copy_03,            copy_04,
                copy_05,            copy_06,            clone_01,
                clone_02,           clone_03,           clone_04,
                clone_05,           clone_06,           release_01,
                release_02,         release_03,         release_04,
                release_05,         release_06,         cmp_01,
                cmp_02,             lt_01,              lt_02,
                gt_01,              gt_02,              typeid_01,
                typeid_02,          uuid_01,            uuid_02,
                sz_01,              sz_02,              sz_03,
                refc_01,            refc_02,            len_01,
                len_02,             valid_01,           valid_02,
                hash_01,            hash_02,            str_01,
                str_02,             empty_01,           empty_02,
                payload_01,         payload_mutable_01, payload_mutable_02,
                payload_mutable_03, payload_mutable_04,
        };

        const char *desc[] = {
                new_01_desc,             new_02_desc,     
                copy_01_desc,            copy_02_desc,    
                copy_03_desc,            copy_04_desc,
                copy_05_desc,            copy_06_desc,    
                clone_01_desc,           clone_02_desc,   
                clone_03_desc,           clone_04_desc,
                clone_05_desc,           clone_06_desc,   
                release_01_desc,         release_02_desc, 
                release_03_desc,         release_04_desc,
                release_05_desc,         release_06_desc, 
                cmp_01_desc,             cmp_02_desc,     
                lt_01_desc,              lt_02_desc,
                gt_01_desc,              gt_02_desc,      
                typeid_01_desc,          typeid_02_desc,  
                uuid_01_desc,            uuid_02_desc,
                sz_01_desc,              sz_02_desc,      
                sz_03_desc,              refc_01_desc,    
                refc_02_desc,            len_01_desc,
                len_02_desc,             valid_01_desc,   
                valid_02_desc,           hash_01_desc,    
                hash_02_desc,            str_01_desc,
                str_02_desc,             empty_01_desc,   
                empty_02_desc,           payload_01_desc, 
                payload_mutable_01_desc, payload_mutable_02_desc,
                payload_mutable_03_desc, payload_mutable_04_desc,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_object interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

