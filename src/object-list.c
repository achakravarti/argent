#include "./api.h"




/*******************************************************************************
 *                               TYPE DEFINITIONS
 */


#define AG_OBJECT_TYPE_OBJECT_LIST 0x1


struct node {
    ag_object *val;
    struct node *nxt;
};


struct payload {
    struct node *head;
    struct node *tail;
    struct node *itr;
    size_t len;
    size_t sz;
};




/*******************************************************************************
 *                            HELPER IMPLEMENTATION
 */


static void payload_push(struct payload *ctx, const ag_object *val)
{
    struct node *n = ag_mempool_new(sizeof *n);
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


static struct payload *payload_new(const struct node *head)
{
    struct payload *p = ag_mempool_new(sizeof *p);
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


static inline void *method_copy(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return payload_new(p->head);
}


static void method_dispose(void *ctx)
{
    struct payload *p = (struct payload *) ctx;
    register struct node *i = p->head;
    struct node *tmp;

    while (i) {
        tmp = i;
        i = i->nxt;

        ag_object_dispose(&tmp->val);
        ag_mempool_free((void **) &tmp);
    };
}


static inline size_t method_sz(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return p->sz;
}


static inline size_t method_len(const void *ctx)
{
    const struct payload *p = (const struct payload *) ctx;
    return p->len;
}




/*******************************************************************************
 *                        INLINE INTERFACE DECLARATIONS
 */


                            /* declaration of ag_object_list_copy() [AgDM:??] */
extern inline ag_object_list *ag_object_list_copy(const ag_object_list *ctx);


                         /* declaration of ag_object_list_dispose() [AgDM:??] */
extern inline void ag_object_list_dispose(ag_object_list **ctx);


                            /* declaration of ag_object_list_type() [AgDM:??] */
extern inline unsigned ag_object_list_type(const ag_object_list *ctx);


                              /* declaration of ag_object_list_id() [AgDM:??] */
extern inline unsigned ag_object_list_id(const ag_object_list *ctx);


                          /* declaration of ag_object_list_id_set() [AgDM:??] */
extern inline void ag_object_list_id_set(ag_object_list **ctx, unsigned id);


                            /* declaration of ag_object_list_hash() [AgDM:??] */
extern inline size_t ag_object_list_hash(const ag_object_list *ctx);


                              /* declaration of ag_object_list_sz() [AgDM:??] */
extern inline size_t ag_object_list_sz(const ag_object_list *ctx);


                             /* declaration of ag_object_list_len() [AgDM:??] */
extern inline size_t ag_object_list_len(const ag_object_list *ctx);


                             /* declaration of ag_object_list_cmp() [AgDM:??] */
extern inline enum ag_object_cmp ag_object_list_cmp(const ag_object_list *ctx,
        const ag_object_list *cmp);


                              /* declaration of ag_object_list_lt() [AgDM:??] */
extern inline bool ag_object_list_lt(const ag_object_list *ctx, 
        const ag_object_list *cmp);


                              /* declaration of ag_object_list_eq() [AgDM:??] */
extern inline bool ag_object_list_eq(const ag_object_list *ctx, 
        const ag_object_list *cmp);


                              /* declaration of ag_object_list_gt() [AgDM:??] */
extern inline bool ag_object_list_gt(const ag_object_list *ctx, 
        const ag_object_list *cmp);


                             /* declaration of ag_object_list_str() [AgDM:??] */
extern inline const char *ag_object_list_str(const ag_object *ctx);




/*******************************************************************************
 *                           INTERFACE IMPLEMENTATION
 */


extern void ag_object_list_register(void)
{
    struct ag_object_method m = {
        .copy = &method_copy,
        .dispose = &method_dispose,
        .sz = &method_sz,
        .len = method_len,
        .hash = NULL,
        .cmp = NULL,
        .str = NULL
    };

    ag_object_register(AG_OBJECT_TYPE_OBJECT_LIST, &m);
}


extern ag_object_list *ag_object_list_new(void)
{
    return ag_object_new_noid(AG_OBJECT_TYPE_OBJECT_LIST, payload_new(NULL));
}

