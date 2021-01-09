#include "../include/argent.h"

#include <uuid/uuid.h>


struct ag_uuid {
        uuid_t  uuid;
};


/* Required for FreeBSD */
#ifndef UUID_STR_LEN
#       define UUID_STR_LEN 37
#endif


extern inline bool      ag_uuid_lt(const ag_uuid *, const ag_uuid *);
extern inline bool      ag_uuid_eq(const ag_uuid *, const ag_uuid *);
extern inline bool      ag_uuid_gt(const ag_uuid *, const ag_uuid *);


extern ag_uuid *
ag_uuid_new(void)
{
        ag_uuid *ctx = ag_memblock_new(sizeof *ctx);
        uuid_generate_random(ctx->uuid);

        return (ctx);
}


extern ag_uuid *
ag_uuid_new_empty(void)
{
        ag_uuid *ctx = ag_memblock_new(sizeof *ctx);
        uuid_clear(ctx->uuid);

        return (ctx);
}


extern ag_uuid *
ag_uuid_parse(const char *src)
{
        AG_ASSERT_PTR (src && *src);

        ag_uuid *ctx = ag_memblock_new(sizeof *ctx);
        (void)uuid_parse((char *)src, ctx->uuid);

        return (ctx);
}


extern ag_uuid *
ag_uuid_copy(const ag_uuid *ctx)
{
        AG_ASSERT_PTR (ctx);

        ag_memblock *cp = (ag_memblock *)ctx;
        return (ag_memblock_copy(cp));
}


extern ag_uuid *
ag_uuid_clone(const ag_uuid *ctx)
{
        AG_ASSERT_PTR (ctx);

        return (ag_memblock_clone(ctx));
}


extern void
ag_uuid_release(ag_uuid **ctx)
{
        ag_uuid *u;

        if (AG_LIKELY (ctx && (u = *ctx))) {
                ag_memblock *m = (ag_memblock *)u;
                ag_memblock_release(&m);
                *ctx = m;
        }
}


extern enum ag_cmp
ag_uuid_cmp(const ag_uuid *ctx, const ag_uuid *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);

        return (uuid_compare(ctx->uuid, cmp->uuid));
}


extern ag_hash
ag_uuid_hash(const ag_uuid *ctx)
{
        AG_AUTO(ag_string) *s = ag_uuid_str(ctx);
        return (ag_hash_new_str(s));
}


extern bool
ag_uuid_empty(const ag_uuid *ctx)
{
        AG_ASSERT_PTR (ctx);

        return (uuid_is_null(ctx->uuid));
}


extern ag_string *
ag_uuid_str(const ag_uuid *ctx)
{
        AG_ASSERT_PTR (ctx);

        char bfr[UUID_STR_LEN];
        uuid_unparse_upper(ctx->uuid, bfr);

        return (ag_string_new(bfr));
}

