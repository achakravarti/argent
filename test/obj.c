#include "./test.h"


#define TYPEID_BASE    ((ag_typeid) 101)
#define TYPEID_DERIVED ((ag_typeid) 102)


struct payload_base {
        int x;
        int y;
};


struct payload_derived {
        int *x;
        int *y;
};


static ag_obj *sample_base(void)
{
        struct payload_base *p = ag_mblock_new(sizeof *p);
        p->x = 555;
        p->y = -666;

        return ag_obj_new(TYPEID_BASE, p);
}


static ag_obj *sample_derived(void)
{
        struct payload_derived *p = ag_mblock_new(sizeof *p);

        p->x = ag_mblock_new(sizeof *p->x);
        p->y = ag_mblock_new(sizeof *p->y);

        *p->x = 555;
        *p->y = -666;

        return ag_obj_new(TYPEID_DERIVED, p);
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


static enum ag_cmp virt_cmp(const ag_obj *ctx, const ag_obj *cmp)
{
        AG_AUTO(ag_uuid) *u1 = ag_obj_uuid(ctx);
        AG_AUTO(ag_uuid) *u2 = ag_obj_uuid(cmp);

        return ag_uuid_cmp(u1, u2);
}


static bool virt_valid(const ag_obj *ctx)
{
}


static size_t virt_sz(const ag_obj *ctx)
{
        const struct payload_derived *p = ag_obj_payload(ctx);

        return ag_mblock_sz(ctx) + ag_mblock_sz(p) + ag_mblock_sz(p->x)
               + ag_mblock_sz(p->y);
}


static size_t virt_len(const ag_obj *ctx)
{
        return 2;
}


static ag_hash virt_hash(const ag_obj *ctx)
{
        const struct payload_derived *p = ag_obj_payload(ctx);
        return ag_hash_new((size_t)p->x);
}


static ag_str *virt_str(const ag_obj *ctx)
{
        return ag_str_new("This is a sample derived object");
}


static void register_base(void)
{
        struct ag_obj_vtable vt = {
                .clone = NULL, .release = NULL, .cmp = NULL,
                .valid = NULL, .sz      = NULL, .len = NULL,
                .hash = NULL,  .str     = NULL,
        };

        ag_obj_registry_set(TYPEID_BASE, &vt);
}


static void register_derived(void)
{
        struct ag_obj_vtable vt = {
                .clone = NULL, .release = NULL, .cmp = NULL,
                .valid = NULL, .sz      = NULL, .len = NULL,
                .hash  = NULL, .str     = NULL,
        };

        ag_obj_registry_set(TYPEID_DERIVED, &vt);
}


extern ag_test_suite *test_suite_obj(void)
{
        ag_test _*test[] = {
        };

        const char *desc[] = {
        };


        ag_test_suite *ctx = ag_test_suite_new("ag_obj interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

