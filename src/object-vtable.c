#include "./api.h"


struct node {
    unsigned key;
    struct ag_object_method *val;
    struct node *nxt;
};


#define VTABLE_BUCKETS 64


ag_threadlocal struct node **vtable = NULL;


static inline struct node *node_new(unsigned key, 
        const struct ag_object_method *val, struct node *nxt)
{
    struct node *n = ag_mempool_new(sizeof *n);
    n->key = key;
    n->nxt = nxt;

    n->val = ag_mempool_new(sizeof *n->val);
    n->val->copy = val->copy;
    n->val->dispose = val->dispose;
    n->val->sz = val->sz;
    n->val->len = val->len;
    n->val->hash = val->hash;
    n->val->cmp = val->cmp;
    n->val->str = val->str;

    return n;
}


static inline void node_free(struct node *n)
{
    ag_mempool_free((void **) &n->val);
    ag_mempool_free((void **) &n);
}


extern void ag_object_vtable_init(void)
{
    if (ag_likely (!vtable))
        vtable = ag_mempool_new(sizeof *vtable * VTABLE_BUCKETS);
}


extern void ag_object_vtable_exit(void)
{
    struct node *n, *nxt;

    for (register size_t i = 0; i < VTABLE_BUCKETS; i++) {
        if ((n = vtable[i])) {
            do {
                nxt = n->nxt;
                node_free(n);
                n = nxt;
            } while (n);
        }
    }
}


extern bool ag_object_vtable_exists(unsigned type)
{
    ag_assert (type);
    unsigned hash = type % VTABLE_BUCKETS;

    ag_assert (vtable);
    struct node *n = vtable[hash];

    while (n) {
        if (n->key == type)
            return true;

        n = n->nxt;
    }

    return false;
}


extern const struct ag_object_method *ag_object_vtable_get(unsigned type)
{
    ag_assert (type);
    unsigned hash = type % VTABLE_BUCKETS;

    ag_assert (vtable);
    struct node *n = vtable[hash];

    while (n) {
        if (n->key == type)
            return n->val;

        n = n->nxt;
    }

    ag_assert (n);
    return NULL;
}


extern void ag_object_vtable_set(unsigned type, 
        const struct ag_object_method *meth)
{
    ag_assert (type);
    unsigned hash = type % VTABLE_BUCKETS;

    ag_assert (vtable && !ag_object_vtable_exists(type));
    struct node *n = node_new(type, meth, vtable[hash]);
    vtable[hash] = n;
}

