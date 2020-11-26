#if (!defined __ARGENT_OBJECT_H__)
#define __ARGENT_OBJECT_H__


#include <stddef.h>
#include <stdbool.h>
#include "./compiler.h"
#include "./primitives.h"
#include "./memblock.h"
#include "./string.h"



/*******************************************************************************
 *                                OBJECT MODEL
 */


                                        /* base object for all ADTs [AgDM:??] */
typedef struct ag_object_t ag_object_t;


                                      /* smart version of ag_object [AgDM:??] */
#if (defined __GNUC__ || defined __clang__)
#   define ag_object_smart_t \
            __attribute__((cleanup(ag_object_dispose))) ag_object_t
#else
#   define ag_object_smart_t ag_object_t
#   warning "[!] ag_object_smart_t leaks memory on current compiler"
#endif


                                           /* reserved object types [AgDM:??] */
#define AG_OBJECT_TYPE_OBJECT ((size_t) 0x0)
#define AG_OBJECT_TYPE_LIST ((size_t) 0x1)
#define AG_OBJECT_TYPE_IP ((size_t) 0x2)
#define AG_OBJECT_TYPE_URL ((size_t) 0x3)
#define AG_OBJECT_TYPE_RESPONSE ((size_t) 0x4)
#define AG_OBJECT_TYPE_HTTP_USER ((size_t) 0x5)
#define AG_OBJECT_TYPE_HTTP_COOKIE ((size_t) 0x6)
#define AG_OBJECT_TYPE_HTTP_REQUEST ((size_t) 0x7)
#define AG_OBJECT_TYPE_HTTP_BODY ((size_t) 0x8)
#define AG_OBJECT_TYPE_HTTP_ATTRIB ((size_t) 0x9)
#define AG_OBJECT_TYPE_HTTP_URL ((size_t) 0xa)


                                       /* v-table of object methods [AgDM:??] */
struct ag_object_vtable {
    ag_memblock_t *(*copy)(const ag_memblock_t *payload);
    void (*dispose)(ag_memblock_t *payload);
    size_t (*id)(const ag_object_t *obj);
    size_t (*sz)(const ag_object_t *obj);
    size_t (*len)(const ag_object_t *obj);
    size_t (*hash)(const ag_object_t *obj);
    enum ag_tristate (*cmp)(const ag_object_t *lhs, const ag_object_t *rhs);
    ag_string_t *(*str)(const ag_object_t *obj);
};


                                       /* initialises object system [AgDM:??] */
extern void ag_object_init(size_t len);


                                        /* shuts down object system [AgDM:??] */
extern void ag_object_exit(void);


                                                /* registers object [AgDM:??] */
extern void ag_object_register(size_t type, const struct ag_object_vtable *vt);


                                              /* creates new object [AgDM:??] */
extern ag_hot ag_object_t *ag_object_new(size_t type, ag_memblock_t *payload);


                                                  /* copies object [AgDM:??] */
extern ag_hot ag_object_t *ag_object_copy(const ag_object_t *ctx);


                                                 /* disposes object [AgDM:??] */
extern ag_hot void ag_object_dispose(ag_object_t **ctx);


                                                /* gets object type [AgDM:??] */
extern ag_pure size_t ag_object_type(const ag_object_t *ctx);


                                     /* gets object reference count [AgDM:??] */
extern size_t ag_object_refc(const ag_object_t *ctx);


                                                  /* gets object ID [AgDM:??] */
extern ag_pure size_t ag_object_id(const ag_object_t *ctx);


                                             /* gets hash of object [AgDM:??] */
extern ag_pure ag_hash_t ag_object_hash(const ag_object_t *ctx);


                                                /* gets object size [AgDM:??] */
extern ag_pure size_t ag_object_sz(const ag_object_t *ctx);


                                              /* gets object length [AgDM:??] */
extern ag_pure size_t ag_object_len(const ag_object_t *ctx);


                                       /* checks if object is empty [AgDM:??] */
inline bool ag_object_empty(const ag_object_t *ctx)
{
    return !ag_object_len(ctx);
}


                                            /* compares two objects [AgDM:??] */
extern ag_pure enum ag_tristate ag_object_cmp(const ag_object_t *ctx, 
        const ag_object_t *cmp);


                           /* checks if object is less than another [AgDM:??] */
inline bool ag_object_lt(const ag_object_t *ctx, const ag_object_t *cmp)
{
    return ag_object_cmp(ctx, cmp) == AG_TRISTATE_LO;
}


                       /* checks if object is equivalent to another [AgDM:??] */
inline bool ag_object_eq(const ag_object_t *ctx, const ag_object_t *cmp)
{
    return ag_object_cmp(ctx, cmp) == AG_TRISTATE_GND;
}


                        /* checks if object is greater than another [AgDM:??] */
inline bool ag_object_gt(const ag_object_t *ctx, const ag_object_t *cmp)
{
    return ag_object_cmp(ctx, cmp) == AG_TRISTATE_HI;
}


                         /* gets read-only handle to object payload [AgDM:??] */
extern ag_hot ag_pure const ag_memblock_t *ag_object_payload(
        const ag_object_t *ctx);


                        /* gets read-write handle to object payload [AgDM:??] */
extern ag_hot ag_memblock_t *ag_object_payload_mutable(ag_object_t **ctx);


                            /* gets string representation of object [AgDM:??] */
extern ag_string_t *ag_object_str(const ag_object_t *ctx);

#endif /* !defined __ARGENT_OBJECT_H__ */

