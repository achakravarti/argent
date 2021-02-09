#include "../include/argent.h"


struct registry_node {
        ag_string               *path;
        ag_string               *dso;
        ag_string               *hnd;
        struct registry_node    *next;
};

static struct registry_node     *registry_node_new(const char *, const char *,
                                    const char *);
static void                      registry_node_release(struct registry_node *);


static AG_THREADLOCAL struct registry_node **g_registry = NULL;

#define REGISTRY_BUCKETS sizeof(int)


static struct registry_node *
registry_node_new(const char *path, const char *dso, const char *hnd)
{
        AG_ASSERT_STR (path);
        AG_ASSERT_STR (hnd);

        struct registry_node *n = ag_memblock_new(sizeof *n);

        n->path = ag_string_new(path);
        n->dso = ag_string_new(dso);
        n->hnd = ag_string_new(hnd);
        n->next = NULL;

        return n;
}

static void
registry_node_release(struct registry_node *ctx)
{
        if (AG_LIKELY (ctx)) {
                ag_string_release(&ctx->path);
                ag_string_release(&ctx->dso);
                ag_string_release(&ctx->hnd);

                ag_memblock *hnd = &ctx;
                ag_memblock_release(hnd);
        }
}


extern void
ag_http_server_init(void)
{
        AG_ASSERT (!g_registry);

        g_registry = ag_memblock_new(sizeof *g_registry * REGISTRY_BUCKETS);
}


extern void
ag_http_server_exit(void)
{
        if (AG_UNLIKELY (!g_registry))
                return;

        for (register size_t i = 0; i < REGISTRY_BUCKETS; i++) {
                register struct registry_node *n = g_registry[i];

                while (n) {
                        register struct registry_node *n2 = n->next;
                        registry_node_release(n);
                        n = n2;
                }
        }
}

