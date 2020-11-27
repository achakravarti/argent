#include "../include/argent.h"




/*******************************************************************************
 *                              PAYLOAD INTERFNALS
 */


/*
 * struct payload: object payload for IP address.
 */
struct payload {
    uint8_t bytes[4];
};


/*
 * payload_new(): create new IP address payload.
 *
 * @attrib: attribute string.
 *
 * Return: new payload.
 */
static inline struct payload *payload_new(const uint8_t bytes[])
{
    struct payload *p = ag_memblock_new(sizeof *p);
    p->bytes[0] = bytes[0];
    p->bytes[1] = bytes[1];
    p->bytes[2] = bytes[2];
    p->bytes[3] = bytes[3];

    return p;
}


/*
 * payload_copy(): make deep copy of IP address payload.
 *
 * @ctx: contextual payload.
 *
 * Return: copy of @ctx.
 */
static inline void *payload_copy(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return payload_new(p->bytes);
}


/*
 * payload_numeric(): get numeric representation of IP address payload.
 *
 * @ctx: contextual payload.
 *
 * Return: numeric representation of @ctx.
 */
static inline uint64_t payload_numeric(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return p->bytes[3] + (p->bytes[2] * 1000) + (p->bytes[1] * 1000000)
            + p->bytes[0] * 1000000000;
}




/*******************************************************************************
 *                               OBJECT INTERNALS
 */


/*
 * object_cmp(): compare two IP address objects.
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
    uint64_t n1 = payload_numeric(ag_object_payload(ctx));
    uint64_t n2 = payload_numeric(ag_object_payload(cmp));

    return n1 < n2 ? AG_TRISTATE_LO
            : (n1 == n2 ? AG_TRISTATE_GND : AG_TRISTATE_HI);
}


/*
 * object_sz(): get IP address object size.
 *
 * @ctx: contextual object.
 *
 * Return: size of @ctx.
 */
static inline size_t object_sz(const ag_object_t *ctx)
{
    const struct payload *p = ag_object_payload(ctx);
    return sizeof p->bytes;
}


/*
 * object_str(): get IP address object string representation.
 *
 * @ctx: contextual object.
 *
 * Return: string representation of @ctx.
 */
static inline ag_string_t *object_str(const ag_object_t *ctx)
{
    const struct payload *p = ag_object_payload(ctx);
    return ag_string_new_fmt("%d.%d.%d.%d", p->bytes[0], p->bytes[1],
            p->bytes[2], p->bytes[3]);
}


/*
 * object_len(): get IP address object length.
 *
 * @ctx: contextual object.
 *
 * Return: length of @ctx.
 */
static inline size_t object_len(const ag_object_t *ctx) 
{
    ag_string_smart_t *s = object_str(ctx);
    return ag_string_len(s);
}


/*
 * object_hash(): get IP address object hash.
 *
 * @ctx: contextual object.
 *
 * Return: hash of @ctx.
 */
static inline ag_hash_t object_hash(const ag_object_t *ctx)
{
    ag_string_smart_t *s = ag_object_str(ctx);
    return ag_string_hash(s);
}




/*******************************************************************************
 *                              MANAGER EXTERNALS
 */


extern void ag_ip_register(void)
{
    struct ag_object_vtable vt = {
        .copy = &payload_copy,
        .dispose = NULL,
        .id = NULL,
        .sz = &object_sz,
        .len = &object_len,
        .hash = object_hash,
        .cmp = &object_cmp,
        .str = &object_str
    };

    ag_object_register(AG_OBJECT_TYPE_IP, &vt);
}


extern ag_ip_t *ag_ip_new(ag_uint oct1, ag_uint oct2, ag_uint oct3, 
        ag_uint oct4)
{
    ag_assert (oct1 < 256 && oct2 < 256 && oct3 < 256 && oct4 < 256);
    uint8_t bytes[] = {
        (uint8_t) oct1,
        (uint8_t) oct2,
        (uint8_t) oct3,
        (uint8_t) oct4
    };

    return ag_object_new(AG_OBJECT_TYPE_IP, payload_new(bytes));
}


extern ag_ip_t *ag_ip_parse(const char *ip)
{
    ag_assert (ip && *ip);
    ag_string_smart_t *s = ag_string_new(ip);
    ag_require (ag_string_has(s, "."), AG_ERNO_IP, NULL);

    ag_string_smart_t *lhs1 = ag_string_split_left(s, ".");
    ag_string_smart_t *rhs1 = ag_string_split_right(s, ".");
    ag_require (ag_string_has(rhs1, "."), AG_ERNO_IP, NULL);
    
    ag_string_smart_t *lhs2 = ag_string_split_left(rhs1, ".");
    ag_string_smart_t *rhs2 = ag_string_split_right(rhs1, ".");
    ag_require (ag_string_has(rhs2, "."), AG_ERNO_IP, NULL);
    
    ag_string_smart_t *lhs3 = ag_string_split_left(rhs2, ".");
    ag_string_smart_t *rhs3 = ag_string_split_right(rhs2, ".");

    ag_uint oct1 = ag_uint_parse(lhs1);
    ag_require (oct1 < 256, AG_ERNO_IP, NULL);

    ag_uint oct2 = ag_uint_parse(lhs2);
    ag_require (oct2 < 256, AG_ERNO_IP, NULL);

    ag_uint oct3 = ag_uint_parse(lhs3);
    ag_require (oct3 < 256, AG_ERNO_IP, NULL);

    ag_uint oct4 = ag_uint_parse(rhs3);
    ag_require (oct4 < 256, AG_ERNO_IP, NULL);

    uint8_t bytes[] = {
        (uint8_t) oct1,
        (uint8_t) oct2,
        (uint8_t) oct3,
        (uint8_t) oct4
    };

    return ag_object_new(AG_OBJECT_TYPE_IP, payload_new(bytes));
}


extern inline ag_ip_t *ag_ip_copy(const ag_ip_t *ctx);


extern inline void ag_ip_dispose(ag_ip_t **ctx);




/*******************************************************************************
 *                             COMPARATOR EXTERNALS
 */


extern inline enum ag_tristate ag_ip_cmp(const ag_ip_t *ctx,
        const ag_ip_t *cmp);


extern inline bool ag_ip_lt(const ag_ip_t *ctx,
        const ag_ip_t *cmp);


extern inline bool ag_ip_eq(const ag_ip_t *ctx,
        const ag_ip_t *cmp);


extern inline bool ag_ip_gt(const ag_ip_t *ctx,
        const ag_ip_t *cmp);




/*******************************************************************************
 *                              ACCESSOR EXTERNALS
 */


extern inline bool ag_ip_empty(const ag_ip_t *ctx);


extern inline size_t ag_ip_typeid(const ag_ip_t *ctx);


extern inline size_t ag_ip_objid(const ag_ip_t *ctx);


extern inline size_t ag_ip_len(const ag_ip_t *ctx);


extern inline size_t ag_ip_sz(const ag_ip_t *ctx);


extern inline size_t ag_ip_hash(const ag_ip_t *ctx);


extern ag_uint ag_ip_octet(const ag_ip_t *ctx, ag_uint oct)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);

    ag_assert (oct >= 1 && oct <= 4);
    return p->bytes[oct - 1];
}


extern inline ag_string_t *ag_ip_str(const ag_ip_t *ctx);

