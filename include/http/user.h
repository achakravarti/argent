#ifndef __ARGENT_HTTP_USER_H__
#define __ARGENT_HTTP_USER_H__


#include "../object.h"




/*******************************************************************************
 *                                    TYPES
 */



/*
 * ag_http_user_t: HTTP user.
 */
typedef ag_object_t ag_http_user_t;


/*
 * ag_http_user_smart_t: smart version of ag_http_user_t.
 */
#define ag_http_user_smart_t ag_object_smart_t




/*******************************************************************************
 *                              MANAGER INTERFACE
 */


/*
 * ag_http_user_register(): register HTTP users as objects.
 */
extern void ag_http_user_register(void);


/*
 * ag_http_user_new(): create new HTTP user.
 *
 * @agent: user agent.
 * @ip   : user IP address.
 * @port : user port number.
 * @host : user hostname.
 *
 * Return: new HTTP user.
 */
extern ag_http_user_t *ag_http_user_new(const char *agent, const char *ip,
        ag_uint port, const char *host);


/*
 * ag_http_user_copy(): make shallow copy of HTTP user.
 *
 * @ctx: contextual user.
 *
 * Return: copy of @ctx.
 */
inline ag_http_user_t *ag_http_user_copy(const ag_http_user_t *ctx)
{
    return ag_object_copy(ctx);
}


/*
 * ag_http_user_dispose(): destroy HTTP user.
 *
 * @ctx: contextual user.
 */
inline void ag_http_user_dispose(ag_http_user_t **ctx)
{
    ag_object_dispose(ctx);
}




/*******************************************************************************
 *                             COMPARATOR INTERFACE
 */


/*
 * ag_http_user_cmp(): compare two HTTP users.
 *
 * @ctx: contextual user.
 * @cmp: comparison user.
 *
 * Return: AG_TRISTATE_LO - @ctx < @cmp,
 *         AG_TRISTATE_EQ - @ctx == @cmp,
 *         AG_TRISTATE_HI - @ctx > @cmp.
 */
inline enum ag_tristate ag_http_user_cmp(const ag_http_user_t *ctx,
        const ag_http_user_t *cmp)
{
    return ag_object_cmp(ctx, cmp);
}


/*
 * ag_http_user_lt(): check if HTTP user is less than another.
 *
 * @ctx: contextual user.
 * @cmp: comparison user.
 *
 * Return: true  - @ctx < @cmp,
 *         false - @ctx >= @cmp.
 */
inline bool ag_http_user_lt(const ag_http_user_t *ctx,
        const ag_http_user_t *cmp)
{
    return ag_object_lt(ctx, cmp);
}


/*
 * ag_http_user_eq(): check if HTTP user is equal to another.
 *
 * @ctx: contextual user.
 * @cmp: comparison user.
 *
 * Return: true  - @ctx == @cmp,
 *         false - @ctx != @cmp.
 */
inline bool ag_http_user_eq(const ag_http_user_t *ctx,
        const ag_http_user_t *cmp)
{
    return ag_object_eq(ctx, cmp);
}


/*
 * ag_http_user_gt(): check if HTTP user is greater than another.
 *
 * @ctx: contextual user.
 * @cmp: comparison user.
 *
 * Return: true  - @ctx > @cmp,
 *         false - @ctx <= @cmp.
 */
inline bool ag_http_user_gt(const ag_http_user_t *ctx,
        const ag_http_user_t *cmp)
{
    return ag_object_gt(ctx, cmp);
}




/*******************************************************************************
 *                              ACCESSOR INTERFACE
 */


/*
 * ag_http_user_empty(): check if HTTP user is empty.
 *
 * @ctx: contextual user.
 *
 * Return: true  - @ctx is empty,
 *         false - @ctx is not empty.
 */
inline bool ag_http_user_empty(const ag_http_user_t *ctx)
{
    return ag_object_empty(ctx);
}


/*
 * ag_http_user_typeid(): get type ID of HTTP user.
 *
 * @ctx: contextual user.
 *
 * Return: type ID of @ctx.
 */
inline size_t ag_http_user_typeid(const ag_http_user_t *ctx)
{
    return ag_object_type(ctx);
}


/*
 * ag_http_user_objid(): get object ID of HTTP user.
 *
 * @ctx: contextual user.
 *
 * Return: object ID of @ctx.
 */
inline size_t ag_http_user_objid(const ag_http_user_t *ctx)
{
    return ag_object_id(ctx);
}


/*
 * ag_http_user_len(): get length of HTTP user.
 *
 * @ctx: contextual user.
 *
 * Return: length of @ctx.
 */
inline size_t ag_http_user_len(const ag_http_user_t *ctx)
{
    return ag_object_len(ctx);
}


/*
 * ag_http_user_sz(): get size in bytes of HTTP user.
 *
 * @ctx: contextual user.
 *
 * Return: size of @ctx.
 */
inline size_t ag_http_user_sz(const ag_http_user_t *ctx)
{
    return ag_object_sz(ctx);
}


/*
 * ag_http_user_hash(): get HTTP user hash.
 *
 * @ctx: contextual user.
 *
 * Return: hash of @ctx.
 */
inline size_t ag_http_user_hash(const ag_http_user_t *ctx)
{
    return ag_object_hash(ctx);
}


/*
 * ag_http_user_str(): get string representation of HTTP user.
 *
 * @ctx: contextual user.
 *
 * Return: string representation of @ctx.
 */
inline ag_string_t *ag_http_user_str(const ag_http_user_t *ctx)
{
    return ag_object_str(ctx);
}


/*
 * ag_http_user_agent(): get user agent of HTTP user.
 *
 * @ctx: contextual user.
 *
 * Return: user agent of @ctx.
 */
extern ag_string_t *ag_http_user_agent(const ag_http_user_t *ctx);


/*
 * ag_http_user_ip(): get IP address of HTTP user.
 *
 * @ctx: contextual user.
 *
 * Return: IP address of @ctx.
 */
extern ag_string_t *ag_http_user_ip(const ag_http_user_t *ctx);


/*
 * ag_http_user_port(): get port number of HTTP user.
 *
 * @ctx: contextual user.
 *
 * Return: port of @ctx.
 */
extern ag_uint ag_http_user_port(const ag_http_user_t *ctx);


/*
 * ag_http_user_host(): get hostname of HTTP user.
 *
 * @ctx: contextual user.
 *
 * Return: hostname of @ctx.
 */
extern ag_string_t *ag_http_user_host(const ag_http_user_t *ctx);




#endif /* !__ARGENT_HTTP_USER_H__ */

