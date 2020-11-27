#ifndef __ARGENT_HTTP_URL_H__
#define __ARGENT_HTTP_URL_H__


#include "../object.h"


#if (defined __GNUC__ || defined __clang__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wattributes"
#endif




/*******************************************************************************
 *                                    TYPES
 */


/*
 * ag_htp_url_t: HTTP URL.
 */
#define ag_http_url_t ag_object_smart_t


/*
 * ag_http_url_static_t: non-smart version of ag_http_url_t.
 */
typedef ag_object_t ag_http_url_static_t;




/*******************************************************************************
 *                              MANAGER INTERFACE
 */


/*
 * ag_http_url_register(): register HTTP URLs as objects.
 */
extern void ag_http_url_register(void);


/*
 * ag_http_url_new(): create new HTTP URL.
 *
 * @secure: HTTPS protocol used.
 * @host  : URL host.
 * @port  : URL port.
 * @path  : URL path.
 *
 * Return: new HTTP URL.
 */
extern ag_http_url_t *ag_http_url_new(bool secure, const char *host,
        ag_uint port, const char *path);


/*
 * ag_http_url_copy(): make shallow copy of HTTP URL.
 *
 * @ctx: contextual URL.
 *
 * Return: copy of @ctx.
 */
inline ag_http_url_t *ag_http_url_copy(const ag_http_url_t *ctx)
{
    return ag_object_copy(ctx);
}


/*
 * ag_http_url_dispose(): destroy HTTP URL.
 *
 * @ctx: contextual URL.
 */
inline void ag_http_url_dispose(ag_http_url_t **ctx)
{
    ag_object_dispose(ctx);
}




/*******************************************************************************
 *                             COMPARATOR INTERFACE
 */


/*
 * ag_http_url_cmp(): compare two HTTP URLs.
 *
 * @ctx: contextual URL.
 * @cmp: comparison URL.
 *
 * Return: AG_TRISTATE_LO - @ctx < @cmp,
 *         AG_TRISTATE_EQ - @ctx == @cmp,
 *         AG_TRISTATE_HI - @ctx > @cmp.
 */
inline enum ag_tristate ag_http_url_cmp(const ag_http_url_t *ctx,
        const ag_http_url_t *cmp)
{
    return ag_object_cmp(ctx, cmp);
}


/*
 * ag_http_url_lt(): check if HTTP URL is less than another.
 *
 * @ctx: contextual URL.
 * @cmp: comparison URL.
 *
 * Return: true  - @ctx < @cmp,
 *         false - @ctx >= @cmp.
 */
inline bool ag_http_url_lt(const ag_http_url_t *ctx, const ag_http_url_t *cmp)
{
    return ag_object_lt(ctx, cmp);
}


/*
 * ag_http_url_eq(): check if HTTP URL is equal to another.
 *
 * @ctx: contextual URL.
 * @cmp: comparison URL.
 *
 * Return: true  - @ctx == @cmp,
 *         false - @ctx != @cmp.
 */
inline bool ag_http_url_eq(const ag_http_url_t *ctx, const ag_http_url_t *cmp)
{
    return ag_object_eq(ctx, cmp);
}


/*
 * ag_http_url_gt(): check if HTTP URL is greater than another.
 *
 * @ctx: contextual URL.
 * @cmp: comparison URL.
 *
 * Return: true  - @ctx > @cmp,
 *         false - @ctx <= @cmp.
 */
inline bool ag_http_url_gt(const ag_http_url_t *ctx, const ag_http_url_t *cmp)
{
    return ag_object_gt(ctx, cmp);
}




/*******************************************************************************
 *                              ACCESSOR INTERFACE
 */


/*
 * ag_http_url_empty(): check if HTTP URL is empty.
 *
 * @ctx: contextual URL
 *
 * Return: true  - @ctx is empty,
 *         false - @ctx is not empty.
 */
inline bool ag_http_url_empty(const ag_http_url_t *ctx)
{
    return ag_object_empty(ctx);
}


/*
 * ag_http_url_secure(): check if HTTP URL uses HTTPS.
 *
 * @ctx: contextual URL.
 *
 * Return: true  - @ctx uses HTTPS.
 *         false - @ctx does not use HTTPS.
 */
extern bool ag_http_url_secure(const ag_http_url_t *ctx);


/*
 * ag_http_url_typeid(): get type ID of HTTP URL.
 *
 * @ctx: contextual URL.
 *
 * Return: type ID of @ctx.
 */
inline size_t ag_http_url_typeid(const ag_http_url_t *ctx)
{
    return ag_object_type(ctx);
}


/*
 * ag_http_url_objid(): get object ID of HTTP URL.
 *
 * @ctx: contextual URL.
 *
 * Return: object ID of @ctx.
 */
inline size_t ag_http_url_objid(const ag_http_url_t *ctx)
{
    return ag_object_id(ctx);
}


/*
 * ag_http_url_hash(): get HTTP URL hash.
 *
 * @ctx: contextual URL.
 *
 * Return: hash of @ctx.
 */
inline ag_hash_t ag_http_url_hash(const ag_http_url_t *ctx)
{
    return ag_object_hash(ctx);
}


/*
 * ag_http_url_sz(): get size in bytes of HTTP URL.
 *
 * @ctx: contextual URL.
 *
 * Return: size of @ctx.
 */
inline size_t ag_http_url_sz(const ag_http_url_t *ctx)
{
    return ag_object_sz(ctx);
}


/*
 * ag_http_url_len(): get length of HTTP URL.
 *
 * @ctx: contextual URL.
 *
 * Return: size of @ctx.
 */
inline size_t ag_http_url_len(const ag_http_url_t *ctx)
{
    return ag_object_len(ctx);
}


/*
 * ag_http_url_port(): get port of HTTP URL.
 *
 * @ctx: contextual URL.
 *
 * Return: port of @ctx.
 */
extern ag_uint ag_http_url_port(const ag_http_url_t *ctx);


/*
 * ag_http_url_str(): get string representation of HTTP URL.
 *
 * @ctx: contextual URL.
 *
 * Return: string representation of @ctx.
 */
inline ag_string_t *ag_http_url_str(const ag_http_url_t *ctx)
{
    return ag_object_str(ctx);
}


/*
 * ag_http_url_host(): get host of HTTP URL.
 *
 * @ctx: contextual URL.
 *
 * Return: host of @ctx.
 */
extern ag_string_t *ag_http_url_host(const ag_http_url_t *ctx);


/*
 * ag_http_url_path(): get path of HTTP URL.
 *
 * @ctx: contextual URL.
 *
 * Return: path of @ctx.
 */
extern ag_string_t *ag_http_url_path(const ag_http_url_t *ctx);




#if (defined __GNUC__ || defined __clang__)
#   pragma GCC diagnostic pop
#endif


#endif /* !__ARGENT_HTTP_URL_H__ */

