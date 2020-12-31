#ifndef __ARGENT_LIST_H__
#define __ARGENT_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "./argent.h"


typedef ag_object ag_list;

extern ag_list *ag_list_new(void);

inline ag_list *ag_list_copy(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_copy(ctx);
}

inline ag_list *ag_list_clone(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_clone(ctx);
}

inline void ag_list_release(ag_list **ctx)
{
        ag_object_release(ctx);
}


inline enum ag_cmp ag_list_cmp(const ag_list *ctx, const ag_list *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_LIST);

        return ag_object_cmp(ctx, cmp);
}

inline bool ag_list_lt(const ag_list *ctx, const ag_list *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_LIST);

        return ag_object_lt(ctx, cmp);
}

inline bool ag_list_eq(const ag_list *ctx, const ag_list *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);
        AG_ASSERT (ag_object_typeid(cmp) == AG_TYPEID_LIST);

        return ag_object_eq(ctx, cmp);
}

inline bool ag_list_gt(const ag_list *ctx, const ag_list *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_gt(ctx, cmp);
}

inline bool ag_list_empty(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_empty(ctx);
}

inline ag_typeid ag_list_typeid(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_typeid(ctx);
}

inline ag_uuid *ag_list_uuid(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_uuid(ctx);
}

inline bool ag_list_valid(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_valid(ctx);
}

inline size_t ag_list_sz(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_sz(ctx);
}

inline size_t ag_list_refc(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_refc(ctx);
}

inline size_t ag_list_len(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_len(ctx);
}

inline ag_hash ag_list_hash(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_hash(ctx);
}

inline ag_string *ag_list_str(const ag_list *ctx)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT (ag_object_typeid(ctx) == AG_TYPEID_LIST);

        return ag_object_str(ctx);
}


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_LIST_H__ */

