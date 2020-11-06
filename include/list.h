#if (!defined __ARGENT_LIST_H__)
#define __ARGENT_LIST_H__


#include <stdbool.h>
#include "./object.h"


/*******************************************************************************
 *                              SINGLY LINKED LIST
 */


                                   /* singly linked list of objects [AgDM:??] */
typedef ag_object_t ag_list_t;


                                 /* smart version of ag_list [AgDM:??] */
#define ag_list_smart_t ag_object_smart_t


                                                  /* registers list [AgDM:??] */
extern void ag_list_register(void);


                                                /* creates new list [AgDM:??] */
extern ag_list_t *ag_list_new(void);


                                                     /* copies list [AgDM:??] */
inline ag_list_t *ag_list_copy(const ag_list_t *ctx)
{
    return ag_object_copy(ctx);
}


                                                   /* disposes list [AgDM:??] */
inline void ag_list_dispose(ag_list_t **ctx)
{
    ag_object_dispose(ctx);
}


                                        /* gets object type of list [AgDM:??] */
inline size_t ag_list_type(const ag_list_t *ctx)
{
    return ag_object_type(ctx);
}


                                          /* gets object ID of list [AgDM:??] */
inline size_t ag_list_id(const ag_list_t *ctx)
{
    return ag_object_id(ctx);
}


                                               /* gets hash of list [AgDM:??] */
inline size_t ag_list_hash(const ag_list_t *ctx)
{
    return ag_object_hash(ctx);
}


                            /* gets sum of all object sizes in list [AgDM:??] */
inline size_t ag_list_sz(const ag_list_t *ctx)
{
    return ag_object_sz(ctx);
}


                                            /* gets number of objects in list */
inline size_t ag_list_len(const ag_list_t *ctx)
{
    return ag_object_len(ctx);
}


                                         /* checks if list is empty [AgDM:??] */
inline bool ag_list_empty(const ag_list_t *ctx)
{
    return ag_object_empty(ctx);
}


                                              /* compares two lists [AgDM:??] */
inline enum ag_tristate ag_list_cmp(const ag_list_t *ctx, 
        const ag_list_t *cmp)
{
    return ag_object_cmp(ctx, cmp);
}


                             /* checks if list is less than another [AgDM:??] */
inline bool ag_list_lt(const ag_list_t *ctx, const ag_list_t*cmp)
{
    return ag_object_lt(ctx, cmp);
}


                         /* checks if list is equivalent to another [AgDM:??] */
inline bool ag_list_eq(const ag_list_t *ctx, const ag_list_t *cmp)
{
    return ag_object_eq(ctx, cmp);
}


                          /* checks if list is greater than another [AgDM:??] */
inline bool ag_list_gt(const ag_list_t *ctx, const ag_list_t *cmp)
{
    return ag_object_gt(ctx, cmp);
}


                              /* gets string representation of list [AgDM:??] */
inline const char *ag_list_str(const ag_object_t *ctx)
{
    return ag_object_str(ctx);
}

                                /* starts internal iterator of list [AgDM:??] */
extern void ag_list_start(ag_list_t **ctx);


                                /* moves list iterator to next node [AgDM:??] */
extern bool ag_list_next(ag_list_t **ctx);


                  /* gets object at currently iterated node of list [AgDM:??] */
extern ag_object_t *ag_list_get(const ag_list_t *ctx);


                            /* gets object at 1-based index of list [AgDM:??] */
extern ag_object_t *ag_list_get_at(const ag_list_t *ctx, size_t idx);


                  /* sets object at currently iterated node of list [AgDM:??] */
extern void ag_list_set(ag_list_t **ctx, const ag_object_t *val);


                            /* sets object at 1-based index of list [AgDM:??] */
extern void ag_list_set_at(ag_list_t **ctx, size_t idx, const ag_object_t *val);


                                    /* pushes object to end of list [AgDM:??] */
extern void ag_list_push(ag_list_t **ctx, const ag_object_t *val);


                                  /* iterates through nodes of list [AgDM:??] */
extern void ag_list_map(const ag_list_t *ctx, void (*cbk)(
            const ag_object_t *node, void *opt), void *opt);


                          /* iterates mutably through nodes of list [AgDM:??] */
extern void ag_list_map_mutable(ag_list_t **ctx, void (*cbk)(ag_object_t **node,
        void *opt), void *opt);



#endif /* !defined __ARGENT_LIST_H__ */

