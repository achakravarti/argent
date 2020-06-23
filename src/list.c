#include "./api.h"




/*******************************************************************************
 *                                LIST INTERNALS
 */


                               /* singly linked list node of object [AgDM:??] */
struct node {
    ag_object_t *val;
    struct node *nxt;
};


                                             /* object list payload [AgDM:??] */
struct payload {
    struct node *head;
    struct node *tail;
    struct node *itr;
    size_t len;
    size_t sz;
};


                                 /* pushes object into payload list [AgDM:??] */
static void payload_push(struct payload *ctx, const ag_object_t *val)
{
    struct node *n = ag_memblock_new(sizeof *n);
    n->val = ag_object_copy(val);
    n->nxt = NULL;

    if (ag_likely (ctx->tail))
        ctx->tail->nxt = n;
    else
        ctx->head = n;

    ctx->tail = n;
    ctx->len++;
    ctx->sz += ag_object_sz(val);
}


                                    /* creates new payload instance [AgDM:??] */
static struct payload *payload_new(const struct node *head)
{
    struct payload *p = ag_memblock_new(sizeof *p);
    p->len = p->sz = 0;

    if (!head) {
        p->head = p->tail = p->itr = NULL;
        return p;
    }

    register const struct node *i = head;
    while (i) {
        payload_push(p, i->val);
        i = i->nxt;
    };

    return p;
}


                                      /* method to copy object list [AgDM:??] */
static inline void *method_copy(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return payload_new(p->head);
}


                                        /* method to dispose object [AgDM:??] */
static void method_dispose(void *ctx)
{
    struct payload *p = (struct payload *) ctx;
    register struct node *i = p->head;
    struct node *tmp;

    while (i) {
        tmp = i;
        i = i->nxt;

        ag_object_dispose(&tmp->val);
        ag_memblock_free((ag_memblock_t **) &tmp);
    };
}


                                       /* method to get object size [AgDM:??] */
static inline size_t method_sz(const ag_object_t *obj)
{
    const struct payload *p = ag_object_payload(obj);
    return p->sz;
}


                                               /* method to get object length */
static inline size_t method_len(const ag_object_t *obj)
{
    const struct payload *p = ag_object_payload(obj);
    return p->len;
}




/*******************************************************************************
 *                                LIST EXTERNALS
 */


                            /* declaration of ag_list_copy() [AgDM:??] */
extern inline ag_list_t *ag_list_copy(const ag_list_t *ctx);


                         /* declaration of ag_list_dispose() [AgDM:??] */
extern inline void ag_list_dispose(ag_list_t **ctx);


                            /* declaration of ag_list_type() [AgDM:??] */
extern inline size_t ag_list_type(const ag_list_t *ctx);


                              /* declaration of ag_list_id() [AgDM:??] */
extern inline size_t ag_list_id(const ag_list_t *ctx);


                            /* declaration of ag_list_hash() [AgDM:??] */
extern inline size_t ag_list_hash(const ag_list_t *ctx);


                              /* declaration of ag_list_sz() [AgDM:??] */
extern inline size_t ag_list_sz(const ag_list_t *ctx);


                             /* declaration of ag_list_len() [AgDM:??] */
extern inline size_t ag_list_len(const ag_list_t *ctx);


                           /* declaration of ag_list_empty() [AgDM:??] */
extern inline bool ag_list_empty(const ag_list_t *ctx);


                             /* declaration of ag_list_cmp() [AgDM:??] */
extern inline enum ag_tristate ag_list_cmp(const ag_list_t *ctx,
        const ag_list_t *cmp);


                              /* declaration of ag_list_lt() [AgDM:??] */
extern inline bool ag_list_lt(const ag_list_t *ctx, 
        const ag_list_t *cmp);


                              /* declaration of ag_list_eq() [AgDM:??] */
extern inline bool ag_list_eq(const ag_list_t *ctx, 
        const ag_list_t *cmp);


                              /* declaration of ag_list_gt() [AgDM:??] */
extern inline bool ag_list_gt(const ag_list_t *ctx, 
        const ag_list_t *cmp);


                             /* declaration of ag_list_str() [AgDM:??] */
extern inline const char *ag_list_str(const ag_object_t *ctx);


                     /* implementation of ag_list_register() [AgDM:??] */
extern void ag_list_register(void)
{
    struct ag_object_vtable vt = {
        .copy = &method_copy,
        .dispose = &method_dispose,
        .id = NULL,
        .sz = &method_sz,
        .len = method_len,
        .hash = NULL,
        .cmp = NULL,
        .str = NULL
    };

    ag_object_register(AG_OBJECT_TYPE_LIST, &vt);
}


                          /* implementation of ag_list_new() [AgDM:??] */
extern ag_list_t *ag_list_new(void)
{
    return ag_object_new(AG_OBJECT_TYPE_LIST, payload_new(NULL));
}


                        /* implementation of ag_list_start() [AgDM:??] */
extern void ag_list_start(ag_list_t **ctx)
{
    ag_assert (ctx);
    struct payload *p = ag_object_payload_mutable(ctx);

    ag_assert (p->len);
    p->itr = p->head;
}


                         /* implementation of ag_list_next() [AgDM:??] */
extern bool ag_list_next(ag_list_t **ctx)
{
    ag_assert (ctx);
    struct payload *p = ag_object_payload_mutable(ctx);

    p->itr = p->itr->nxt;
    return p->itr->nxt;
}


                          /* implementation of ag_list_get() [AgDM:??] */
extern ag_object_t *ag_list_get(const ag_list_t *ctx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);

    ag_assert (p->itr);
    return ag_object_copy(p->itr->val);
}


                       /* implementation of ag_list_get_at() [AgDM:??] */
extern ag_object_t *ag_list_get_at(const ag_list_t *ctx, size_t idx)
{
    ag_assert (ctx);
    const struct payload *p = ag_object_payload(ctx);

    ag_assert (idx && idx <= p->len);
    register const struct node *n = p->head;
    for (register size_t i = 1; i < idx; i++)
        n = n->nxt;

    return ag_object_copy(n->val);
}


                          /* implementation of ag_list_set() [AgDM:??] */
extern void ag_list_set(ag_list_t **ctx, const ag_object_t *val)
{
    ag_assert (ctx && *ctx);
    struct payload *p = ag_object_payload_mutable(ctx);

    ag_assert (p->itr);
    ag_object_dispose(&p->itr->val);
    p->itr->val = ag_object_copy(val);
}


                       /* implementation of ag_list_set_at() [AgDM:??] */
extern void ag_list_set_at(ag_list_t **ctx, size_t idx, const ag_object_t *val)
{
    ag_assert (ctx && *ctx);
    struct payload *p = ag_object_payload_mutable(ctx);
    
    ag_assert (idx && idx <= p->len);
    register struct node *n = p->head;
    for (register size_t i = 1; i < idx; i++)
        n = n->nxt;

    ag_assert (val);
    ag_object_dispose(&n->val);
    n->val = ag_object_copy(val);
}


                         /* implementation of ag_list_push() [AgDM:??] */
extern void ag_list_push(ag_list_t **ctx, const ag_object_t *val)
{
    ag_assert (ctx && *ctx);
    struct payload *p = ag_object_payload_mutable(ctx);

    ag_assert (val);
    payload_push(p, val);
}


                      /* implementation of ag_list_map() [AgDM:??] */
extern void ag_list_map(const ag_list_t *ctx, void (*cbk)(
            const ag_object_t *node, void *opt), void *opt)
{
    ag_assert (ctx);
    if (ag_unlikely (ag_list_empty(ctx)))
            return;

    const struct payload *p = ag_object_payload(ctx);
    register const struct node *n = p->head;
    
    while (n) {
        cbk(ag_object_copy(n->val), opt);
        n = n->nxt;
    }
}


              /* implementation of ag_list_map_mutable() [AgDM:??] */
extern void ag_list_map_mutable(ag_list_t **ctx, void (*cbk)(ag_object_t **node,
        void *opt), void *opt)
{
    ag_assert (ctx);
    if (ag_unlikely (ag_list_empty(*ctx)))
            return;

    struct payload *p = ag_object_payload_mutable(ctx);
    register struct node *n = p->head;
    
    while (n) {
        cbk(&n->val, opt);
        n = n->nxt;
    }
}

