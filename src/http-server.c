#include "../include/argent.h"


struct registry_node {
        ag_string               *path;
        ag_string               *dso;
        ag_string               *sym;
        struct registry_node    *next;
};

static struct registry_node     *registry_node_new(const char *, const char *,
                                    const char *);
static void                      registry_node_release(struct registry_node *);


static AG_THREADLOCAL struct registry_node      **g_registry = NULL;

#define REGISTRY_BUCKETS        sizeof(int)

static void     registry_push(struct registry_node *);


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


extern void
ag_http_server_register(const char *path, const char *sym)
{
        AG_ASSERT_STR (path);
        AG_ASSERT_STR (sym);
        AG_ASSERT_PTR (g_registry);

        registry_push(registry_node_new(path, NULL, sym));
}

extern void
ag_http_server_register_dso(const char *path, const char *dso, const char *sym)
{
        AG_ASSERT_STR (path);
        AG_ASSERT_STR (dso);
        AG_ASSERT_STR (sym);
        AG_ASSERT_PTR (g_registry);

        registry_push(registry_node_new(path, dso, sym));
}


static struct registry_node *
registry_node_new(const char *path, const char *dso, const char *sym)
{
        AG_ASSERT_STR (path);
        AG_ASSERT_STR (sym);

        struct registry_node *n = ag_memblock_new(sizeof *n);

        n->path = ag_string_new(path);
        n->dso = ag_string_new(dso);
        n->sym = ag_string_new(sym);
        n->next = NULL;

        return n;
}

static void
registry_node_release(struct registry_node *ctx)
{
        if (AG_LIKELY (ctx)) {
                ag_string_release(&ctx->path);
                ag_string_release(&ctx->dso);
                ag_string_release(&ctx->sym);

                ag_memblock *hnd = &ctx;
                ag_memblock_release(hnd);
        }
}


static void
registry_push(struct registry_node *node)
{
        AG_ASSERT (node);

        ag_hash h = ag_hash_new_str(node->path);
        register struct registry_node *i = g_registry[h % REGISTRY_BUCKETS];

        if (i) {
                while (i && i->next)
                        i = i->next;

                i->next = node;
        } else
                i = node;
}

