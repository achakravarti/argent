#include "../include/argent.h"




/*******************************************************************************
 *                              PAYLOAD INTERFNALS
 */


/*
 * struct payload: object payload for HTTP attribute.
 */
struct payload {
    ag_string_t *attrib;
};


/*
 * payload_new(): create new HTTP attribute payload.
 *
 * @attrib: attribute string.
 *
 * Return: new payload.
 */
static inline struct payload *payload_new(const char *attrib)
{
    struct payload *p = ag_memblock_new(sizeof *p);
    p->attrib = ag_string_new(attrib);
    return p;
}


/*
 * payload_copy(): make deep copy of HTTP attribute payload.
 *
 * @ctx: contextual payload.
 *
 * Return: copy of @ctx.
 */
static inline void *payload_copy(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return payload_new(p->attrib);
}


/*
 * payload_dispose(): free up HTTP attribute payload resources.
 *
 * @ctx: contextual payload.
 */
static inline void payload_dispose(void *ctx)
{
    struct payload *p = (struct payload *) ctx;
    ag_string_dispose(&p->attrib);
}




/*******************************************************************************
 *                               OBJECT INTERNALS
 */


/*
 * object_cmp(): compare two HTTP attribute objects.
 *
 * @ctx: contextual object.
 * @cmp: comparison object.
 *
 * Return: AG_TRISTATE_LO - @ctx < @cmp,
 *         AG_TRISTATE_EQ - @ctx == @cmp,
 *         AG_TRISTATE_HI - @ctx > @cmp.
 */
static inline enum ag_tristate object_cmp(const ag_object_t *ctx,
        const ag_object_t *cmp)
{
    const struct payload *p1 = ag_object_payload(ctx);
    const struct payload *p2 = ag_object_payload(cmp);
    return ag_string_cmp(p1->attrib, p2->attrib);
}


/*
 * object_sz(): get HTTP attribute object size.
 *
 * @ctx: contextual object.
 *
 * Return: size of @ctx.
 */
static inline size_t object_sz(const ag_object_t *ctx)
{
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_sz(p->attrib);
}


/*
 * object_len(): get HTTP attribute object length.
 *
 * @ctx: contextual object.
 *
 * Return: length of @ctx.
 */
static inline size_t object_len(const ag_object_t *ctx)
{
    const struct payload *p = ag_object_payload(ctx);
    ag_string_smart_t *val = ag_string_split_right(p->attrib, "=");
    return ag_string_len(val);
}


/*
 * object_hash(): get HTTP attribute object hash.
 *
 * @ctx: contextual object.
 *
 * Return: hash of @ctx.
 */
static inline ag_hash_t object_hash(const ag_object_t *ctx)
{
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_hash(p->attrib);
}


/*
 * object_str(): get HTTP attribute object string representation.
 *
 * @ctx: contextual object.
 *
 * Return: string representation of @ctx.
 */
static inline ag_string_t *object_str(const ag_object_t *ctx)
{
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_copy(p->attrib);
}




/*******************************************************************************
 *                              MANAGER EXTERNALS
 */


extern void ag_http_attrib_register(void)
{
    struct ag_object_vtable vt = {
        .copy = &payload_copy,
        .dispose = &payload_dispose,
        .id = NULL,
        .sz = &object_sz,
        .len = &object_len,
        .hash = &object_hash,
        .cmp = &object_cmp,
        .str = &object_str,
    };

    ag_object_register(AG_OBJECT_TYPE_HTTP_ATTRIB, &vt);
}


extern ag_http_attrib_t *ag_http_attrib_new(const char *key, const char *val)
{
    ag_assert (key && *key);
    ag_assert (val);

    ag_string_smart_t *s;
    if (*val) {
        ag_string_smart_t *v1 = ag_string_new(val);
        ag_string_smart_t *v2 = ag_string_url_encode(v1);
        s = ag_string_new_fmt("%s=%s", key, v2);
    } else
        s = ag_string_new(key);

    return ag_object_new(AG_OBJECT_TYPE_HTTP_ATTRIB, payload_new(s));
}


extern ag_http_attrib_t *ag_http_attrib_new_empty(const char *key)
{
    ag_assert (key && *key);

    return ag_object_new(AG_OBJECT_TYPE_HTTP_ATTRIB, payload_new(key));
}


extern inline ag_http_attrib_t *ag_http_attrib_copy(
        const ag_http_attrib_t *ctx);


extern inline void ag_http_attrib_dispose(ag_http_attrib_t **ctx);




/*******************************************************************************
 *                             COMPARATOR EXTERNALS
 */


extern inline enum ag_tristate ag_http_attrib_cmp(const ag_http_attrib_t *ctx,
        const ag_http_attrib_t *cmp);


extern inline bool ag_http_attrib_lt(const ag_http_attrib_t *ctx,
        const ag_http_attrib_t *cmp);


extern inline bool ag_http_attrib_eq(const ag_http_attrib_t *ctx,
        const ag_http_attrib_t *cmp);


extern inline bool ag_http_attrib_gt(const ag_http_attrib_t *ctx,
        const ag_http_attrib_t *cmp);




/*******************************************************************************
 *                              ACCESSOR EXTERNALS
 */


extern inline bool ag_http_attrib_empty(const ag_http_attrib_t *ctx);


extern inline size_t ag_http_attrib_typeid(const ag_http_attrib_t *ctx);


extern inline size_t ag_http_attrib_objid(const ag_http_attrib_t *ctx);


extern inline ag_hash_t ag_http_attrib_hash(const ag_http_attrib_t *ctx);


extern inline size_t ag_http_attrib_sz(const ag_http_attrib_t *ctx);


extern inline size_t ag_http_attrib_len(const ag_http_attrib_t *ctx);


extern ag_string_t *ag_http_attrib_key(const ag_http_attrib_t *ctx)
{
    ag_assert (ctx);

    const struct payload *p = ag_object_payload(ctx);
    return ag_string_has(p->attrib, "=") ? ag_string_split_left(p->attrib, "=")
            : ag_string_copy(p->attrib);
}


extern ag_string_t *ag_http_attrib_val(const ag_http_attrib_t *ctx)
{
    ag_assert (ctx);

    const struct payload *p = ag_object_payload(ctx);

    if (ag_string_has(p->attrib, "=")) {
        ag_string_smart_t *val = ag_string_split_right(p->attrib, "=");
        return ag_string_url_decode(val);
    }

    return ag_string_new_empty();
}


extern inline ag_string_t *ag_http_attrib_str(const ag_http_attrib_t *ctx);

