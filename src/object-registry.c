#include "../include/argent.h"


extern void ag_obj_registry_init(size_t len)
{
        (void)len;
}


extern void ag_obj_registry_exit(void)
{
}


extern const struct ag_obj_vtable *ag_obj_registry_get(size_t typeid)
{
        static struct ag_obj_vtable vt = {
                .vt_clone   = NULL,
                .vt_release = NULL,
                .vt_cmp     = NULL,
                .vt_valid   = NULL,
                .vt_len     = NULL,
                .vt_hash    = NULL,
                .vt_str     = NULL,
        };

        return &vt;
}


extern void ag_obj_registry_set(size_t typeid, const struct ag_obj_vtable *vt)
{
        (void)typeid;
        (void)vt;
}

