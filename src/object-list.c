#include "./api.h"


extern inline ag_object_list *ag_object_list_copy(const ag_object_list *ctx);


extern inline void ag_object_list_dispose(ag_object_list **ctx);


extern inline unsigned ag_object_list_type(const ag_object_list *ctx);


extern inline unsigned ag_object_list_id(const ag_object_list *ctx);


extern inline void ag_object_list_id_set(ag_object_list **ctx, unsigned id);


extern inline size_t ag_object_list_hash(const ag_object_list *ctx);


extern inline size_t ag_object_list_sz(const ag_object_list *ctx);


extern inline size_t ag_object_list_len(const ag_object_list *ctx);


extern inline enum ag_object_cmp ag_object_list_cmp(const ag_object_list *ctx,
        const ag_object_list *cmp);


extern inline bool ag_object_list_lt(const ag_object_list *ctx, 
        const ag_object_list *cmp);


extern inline bool ag_object_list_eq(const ag_object_list *ctx, 
        const ag_object_list *cmp);


extern inline bool ag_object_list_gt(const ag_object_list *ctx, 
        const ag_object_list *cmp);


extern inline const char *ag_object_list_str(const ag_object *ctx);


#define AG_OBJECT_TYPE_OBJECT_LIST 0x1

extern void ag_object_list_register(void)
{
    struct ag_object_method m = {
        .copy = NULL,
        .dispose = NULL,
        .sz = NULL,
        .len = NULL,
        .hash = NULL,
        .cmp = NULL,
        .str = NULL
    };

    ag_object_register(AG_OBJECT_TYPE_OBJECT_LIST, &m);
}


extern ag_object_list *ag_object_list_new(void)
{
    return ag_object_new_noid(AG_OBJECT_TYPE_OBJECT_LIST, payload_new());
}

