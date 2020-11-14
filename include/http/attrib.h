#if (!defined __ARGENT_HTTP_ATTRIB_H__)
#define __ARGENT_HTTP_ATTRIB_H__

#include "../object.h"


typedef ag_object_t ag_http_attrib_t;
#define ag_http_attrib_smart_t ag_object_smart_t




/*******************************************************************************
 *                              MANAGER INTERFACE
 */


/*
 * ag_http_attrib_register(): register HTTP attributes as objects.
 */
extern void ag_http_attrib_register(void);


/*
 * ag_http_attrib_new(): create new HTTP attribute.
 *
 * @name: attribute name.
 * @val: attribute value.
 *
 * Return: new HTTP attribute.
 */
extern ag_http_attrib_t *ag_http_attrib_new(const char *name, const char *val);


/*
 * ag_http_attrib_new(): create new HTTP attribute with only a name.
 *
 * @name: attribute name.
 *
 * Return: new HTTP attribute.
 */
extern ag_http_attrib_t *ag_http_attrib_new_nameonly(const char *name);


/*
 * ag_http_attrib_copy(): make shallow copy of HTTP attribute.
 *
 * @ctx: contextual attribute.
 *
 * Return: copy of HTTP attribute.
 */
inline ag_http_attrib_t *ag_http_attrib_copy(const ag_http_attrib_t *ctx)
{
    return ag_object_copy(ctx);
}


/*
 * ag_http_attrib_dispose(): destroy HTTP attribute.
 *
 * @ctx: contextual attribute.
 */
inline void ag_http_attrib_dispose(ag_http_attrib_t **ctx)
{
    ag_object_dispose(ctx);
}




/*******************************************************************************
 *                             COMPARATOR INTERFACE
 */


/*
 * ag_http_attrib_cmp(): compare two HTTP attributes.
 *
 * @ctx: contextual attribute.
 * @cmp: attribute to compare against.
 *
 * Return: AG_TRISTATE_LO - @ctx < @cmp,
 *         AG_TRISTATE_EQ - @ctx == @cmp,
 *         AG_TRISTATE_HI - @ctx > @cmp.
 */
inline enum ag_tristate ag_http_attrib_cmp(const ag_http_attrib_t *ctx,
        const ag_http_attrib_t *cmp)
{
    return ag_object_cmp(ctx, cmp);
}


/*
 * ag_http_attrib_lt(): check if HTTP attribute is less than another.
 *
 * @ctx: contextual attribute.
 * @cmp: attribute to compare against.
 *
 * Return: true  - @ctx < @cmp,
 *         false - @ctx >= @cmp.
 */
inline bool ag_http_attrib_lt(const ag_http_attrib_t *ctx,
        const ag_http_attrib_t *cmp)
{
    return ag_object_lt(ctx, cmp);
}


/*
 * ag_http_attrib_eq(): check if HTTP attribute is equal to another.
 *
 * @ctx: contextual attribute.
 * @cmp: attribute to compare against.
 *
 * Return: true  - @ctx == @cmp,
 *         false - @ctx != @cmp.
 */
inline bool ag_http_attrib_eq(const ag_http_attrib_t *ctx,
        const ag_http_attrib_t *cmp)
{
    return ag_object_eq(ctx, cmp);
}


/*
 * ag_http_attrib_gt(): check if HTTP attribute is greater than another.
 *
 * @ctx: contextual attribute.
 * @cmp: attribute to compare against.
 *
 * Return: true  - @ctx > @cmp,
 *         false - @ctx <= @cmp.
 */
inline bool ag_http_attrib_gt(const ag_http_attrib_t *ctx,
        const ag_http_attrib_t *cmp)
{
    return ag_object_gt(ctx, cmp);
}




/*******************************************************************************
 *                              ACCESSOR INTERFACE
 */


/*
 * ag_http_attrib_empty(): check if HTTP attribute is empty.
 *
 * @ctx: contextual attribute.
 *
 * Return: true  - @ctx is empty,
 *         false - @ctx is not empty.
 */
inline bool ag_http_attrib_empty(const ag_http_attrib_t *ctx)
{
    return ag_object_empty(ctx);
}


/*
 * ag_http_attrib_nameonly(): check if HTTP attribute has only name.
 *
 * @ctx: contextual attribute.
 *
 * Return: true  - @ctx only has name,
 *         false - @ctx has name-value pair.
 */
extern bool ag_http_attrib_nameonly(const ag_http_attrib_t *ctx);


/*
 * ag_http_attrib_typeid(): get type ID of HTTP attribute.
 *
 * @ctx: contextual attribute.
 *
 * Return: type ID of @ctx.
 */
inline size_t ag_http_attrib_typeid(const ag_http_attrib_t *ctx)
{
    return ag_object_type(ctx);
}


/*
 * ag_http_attrib_objid(): get object ID of HTTP attribute.
 *
 * @ctx: contextual attribute.
 *
 * Return: object ID of @ctx.
 */
inline size_t ag_http_attrib_objid(const ag_http_attrib_t *ctx)
{
    return ag_object_id(ctx);
}


/*
 * ag_http_attrib_hash(): get HTTP attribute hash.
 *
 * @ctx: contextual attribute.
 *
 * Return: hash of @ctx.
 */
inline ag_hash_t ag_http_attrib_hash(const ag_http_attrib_t *ctx)
{
    return ag_object_hash(ctx);
}


/*
 * ag_http_attrib_sz(): get size in bytes of HTTP attribute.
 *
 * @ctx: contextual attribute.
 *
 * Return: size of @ctx.
 */
inline size_t ag_http_attrib_sz(const ag_http_attrib_t *ctx)
{
    return ag_object_sz(ctx);
}


/*
 * ag_http_attrib_len(): get legnth of HTTP attribute.
 *
 * @ctx: contextual attribute.
 *
 * Return: length of @ctx.
 */
inline size_t ag_http_attrib_len(const ag_http_attrib_t *ctx)
{
    return ag_object_len(ctx);
}


/*
 * ag_http_attrib_name(): get name of HTTP attribute.
 *
 * @ctx: contextual attribute.
 *
 * Return: name of @ctx.
 */
extern ag_string_t *ag_http_attrib_name(const ag_http_attrib_t *ctx);


/*
 * ag_http_attrib_value(): get value of HTTP attribute.
 *
 * @ctx: contextual attribute.
 *
 * Return: value of @ctx.
 */
extern ag_string_t *ag_http_attrib_value(const ag_http_attrib_t *ctx);


/*
 * ag_http_attrib_str(): get string representation of HTTP attribute.
 *
 * @ctx: contextual attribute.
 *
 * Return: string representation of @ctx.
 */
inline ag_string_t *ag_http_attrib_str(const ag_http_attrib_t *ctx)
{
    return ag_object_str(ctx);
}


#endif /* !defined __ARGENT_HTTP_ATTRIB_H__ */

