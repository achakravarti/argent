#include "../include/argent.h"

#include <uuid/uuid.h>

struct ag_uuid {
        uuid_t uuid;
};


extern inline bool ag_uuid_lt(const ag_uuid *, const ag_uuid *);
extern inline bool ag_uuid_eq(const ag_uuid *, const ag_uuid *);
extern inline bool ag_uuid_gt(const ag_uuid *, const ag_uuid *);


extern ag_uuid *ag_uuid_new(void)
{
        ag_uuid *ctx = ag_mblock_new(sizeof *ctx);
        uuid_generate_random(ctx->uuid);

        return ctx;
}


extern ag_uuid *ag_uuid_new_empty(void)
{
        ag_uuid *ctx = ag_mblock_new(sizeof *ctx);
        uuid_clear(ctx->uuid);

        return ctx;
}


extern ag_uuid *ag_uuid_parse(const char *src)
{
        AG_ASSERT_PTR (src && *src);

        ag_uuid *ctx = ag_mblock_new(sizeof *ctx);
        (void)uuid_parse((char *)src, ctx->uuid);

        return ctx;
}


extern ag_uuid *ag_uuid_copy(const ag_uuid *ctx)
{
        AG_ASSERT_PTR (ctx);

        ag_mblock *cp = (ag_mblock *)ctx;
        return ag_mblock_copy(cp);
}


extern ag_uuid *ag_uuid_clone(const ag_uuid *ctx)
{
        AG_ASSERT_PTR (ctx);

        return ag_mblock_clone(ctx);
}


extern void ag_uuid_release(ag_uuid **ctx)
{
        ag_uuid *u;

        if (AG_LIKELY (ctx && (u = *ctx))) {
                ag_mblock *m = (ag_mblock *)u;
                ag_mblock_release(&m);
                *ctx = m;
        }
}


extern enum ag_cmp ag_uuid_cmp(const ag_uuid *ctx, const ag_uuid *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);

        return uuid_compare(ctx->uuid, cmp->uuid);
}


extern bool ag_uuid_empty(const ag_uuid *ctx)
{
        AG_ASSERT_PTR (ctx);

        return uuid_is_null(ctx->uuid);
}


extern ag_str *ag_uuid_str(const ag_uuid *ctx)
{
        AG_ASSERT_PTR (ctx);

        char bfr[37];
        uuid_unparse_upper(ctx->uuid, bfr);

        return ag_str_new(bfr);
}

