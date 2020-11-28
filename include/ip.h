#ifndef __ARGENT_IP_H__
#define __ARGENT_IP_H__


#include "./object.h"


#if (defined __GNUC__ || defined __clang__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wattributes"
#endif




/*******************************************************************************
 *                                    TYPES
 */


/*
 * ag_ip_t: IP address.
 */
#define ag_ip_t ag_object_smart_t


/*
 * ag_ip_static_t: non-smart version of ag_ip_t.
 */
typedef ag_object_t ag_ip_static_t;




/*******************************************************************************
 *                              MANAGER INTERFACE
 */


/*
 * ag_p_register(): register IP addresses as objects.
 */
extern void ag_ip_register(void);


/*
 * ag_ip_new(): create new IP address.
 *
 * @oct1: octet 1.
 * @oct2: octet 2.
 * @oct3: octet 3.
 * @oct4: octet 4.
 *
 * Return: new IP address.
 */
extern ag_ip_t *ag_ip_new(ag_uint oct1, ag_uint oct2, ag_uint oct3, 
        ag_uint oct4);


/*
 * ag_ip_parse(): create new IP address from string.
 *
 * @ip: IP address string.
 *
 * Return: new IP address.
 */
extern ag_ip_t *ag_ip_parse(const char *ip);


/*
 * ag_ip_copy(): make shallow copy of IP address.
 *
 * @ctx: contextual IP address.
 *
 * Return: copy of @ctx.
 */
inline ag_ip_t *ag_ip_copy(const ag_ip_t *ctx)
{
    return ag_object_copy(ctx);
}


/*
 * ag_ip_dispose(): destroy IP address.
 *
 * @ctx: contextual IP address.
 */
inline void ag_ip_dispose(ag_ip_t **ctx)
{
    ag_object_dispose(ctx);
}




/*******************************************************************************
 *                             COMPARATOR INTERFACE
 */


/*
 * ag_ip_cmp(): compare two IP addresss.
 *
 * @ctx: contextual IP address.
 * @cmp: IP address to compare against.
 *
 * Return: AG_TRISTATE_LO - @ctx < @cmp,
 *         AG_TRISTATE_EQ - @ctx == @cmp,
 *         AG_TRISTATE_HI - @ctx > @cmp.
 */
inline enum ag_tristate ag_ip_cmp(const ag_ip_t *ctx, const ag_ip_t *cmp)
{
    return ag_object_cmp(ctx, cmp);
}


/*
 * ag_ip_lt(): check if IP address is less than another.
 *
 * @ctx: contextual IP address.
 * @cmp: IP address to compare against.
 *
 * Return: true  - @ctx < @cmp,
 *         false - @ctx >= @cmp.
 */
inline bool ag_ip_lt(const ag_ip_t *ctx, const ag_ip_t *cmp)
{
    return ag_object_lt(ctx, cmp);
}


/*
 * ag_ip_eq(): check if IP address is equal to another.
 *
 * @ctx: contextual IP address.
 * @cmp: IP address to compare against.
 *
 * Return: true  - @ctx == @cmp,
 *         false - @ctx != @cmp.
 */
inline bool ag_ip_eq(const ag_ip_t *ctx, const ag_ip_t *cmp)
{
    return ag_object_eq(ctx, cmp);
}


/*
 * ag_ip_gt(): check if IP address is greater than another.
 *
 * @ctx: contextual IP address.
 * @cmp: IP address to compare against.
 *
 * Return: true  - @ctx > @cmp,
 *         false - @ctx <= @cmp.
 */
inline bool ag_ip_gt(const ag_ip_t *ctx, const ag_ip_t *cmp)
{
    return ag_object_gt(ctx, cmp);
}




/*******************************************************************************
 *                              ACCESSOR INTERFACE
 */


/*
 * ag_ip_empty(): check if IP address is empty.
 *
 * @ctx: contextual IP address.
 *
 * Return: true  - @ctx is empty,
 *         false - @ctx is not empty.
 */
inline bool ag_ip_empty(const ag_ip_t *ctx)
{
    return ag_object_empty(ctx);
}


/*
 * ag_ip_typeid(): get type ID of IP address.
 *
 * @ctx: contextual IP address.
 *
 * Return: type ID of @ctx.
 */
inline size_t ag_ip_typeid(const ag_ip_t *ctx)
{
    return ag_object_type(ctx);
}


/*
 * ag_ip_objid(): get object ID of IP address.
 *
 * @ctx: contextual IP address.
 *
 * Return: object ID of @ctx.
 */
inline size_t ag_ip_objid(const ag_ip_t *ctx)
{
    return ag_object_id(ctx);
}


/*
 * ag_ip_hash(): get IP address hash.
 *
 * @ctx: contextual IP address.
 *
 * Return: hash of @ctx.
 */
inline ag_hash_t ag_ip_hash(const ag_ip_t *ctx)
{
    return ag_object_hash(ctx);
}


/*
 * ag_ip_sz(): get size in bytes of IP address.
 *
 * @ctx: contextual IP address.
 *
 * Return: size of @ctx.
 */
inline size_t ag_ip_sz(const ag_ip_t *ctx)
{
    return ag_object_sz(ctx);
}


/*
 * ag_ip_len(): get length of IP address.
 *
 * @ctx: contextual IP address.
 *
 * Return: length of @ctx.
 */
inline size_t ag_ip_len(const ag_ip_t *ctx)
{
    return ag_object_len(ctx);
}


/*
 * ag_ip_octet(): get octet value of IP address.
 *
 * @ctx: contextual IP address.
 * @oct: 1-based octet number.
 *
 * Return: value of @oct in @ctx.
 */
extern ag_uint ag_ip_octet(const ag_ip_t *ctx, ag_uint oct);


/*
 * ag_ip_str(): get string representation of IP address.
 *
 * @ctx: contextual IP address.
 *
 * Return: string representation of @ctx.
 */
inline ag_string_t *ag_ip_str(const ag_ip_t *ctx)
{
    return ag_object_str(ctx);
}




#if (defined __GNUC__ || defined __clang__)
#   pragma GCC diagnostic pop
#endif


#endif /* !__ARGENT_IP_H__ */

