#include "../include/argent.h"
#include <fcgiapp.h>


struct node {
        ag_string       *path;
        ag_string       *dso;
        ag_string       *sym;
        struct node     *next;
};

static struct node      *node_new(const char *, const char *, const char *);
static void              node_release(struct node *);


#define REGISTRY_BUCKETS        sizeof(int)

static inline struct node      **registry_new(void);
static void                      registry_release(struct node **);

static void     registry_push(struct node *);


static AG_THREADLOCAL struct {
        FCGX_Request     *req;
        struct node     **reg;
} *g_http = NULL;

#define HTTP_LISTENSOCK_FILENO  0
#define HTTP_LISTENSOCK_FLAGS   0


extern void
ag_http_server_init(void)
{
        AG_ASSERT (!g_http);

        g_http = ag_memblock_new(sizeof *g_http);
        g_http->reg = registry_new();

        AG_REQUIRE (!FCGX_Init(), AG_ERNO_HTTP);
        AG_REQUIRE (!FCGX_InitRequest(g_http->req, HTTP_LISTENSOCK_FILENO,
            HTTP_LISTENSOCK_FLAGS), AG_ERNO_HTTP);
}


extern void
ag_http_server_exit(void)
{
        if (AG_UNLIKELY (!g_http))
                return;

        registry_release(g_http->reg);
}


extern void
ag_http_server_register(const char *path, const char *sym)
{
        AG_ASSERT_STR (path);
        AG_ASSERT_STR (sym);
        AG_ASSERT_PTR (g_http);

        registry_push(node_new(path, NULL, sym));
}

extern void
ag_http_server_register_dso(const char *path, const char *dso, const char *sym)
{
        AG_ASSERT_STR (path);
        AG_ASSERT_STR (dso);
        AG_ASSERT_STR (sym);
        AG_ASSERT_PTR (g_http);

        registry_push(node_new(path, dso, sym));
}


extern void
ag_http_server_run(void)
{
        AG_ASSERT_PTR (g_http);

        while (FCGX_Accept_r(g_http->req) >= 0) {
                FCGX_Finish_r(g_http->req);
        }
}


static struct node *
node_new(const char *path, const char *dso, const char *sym)
{
        AG_ASSERT_STR (path);
        AG_ASSERT_STR (sym);

        struct node *n = ag_memblock_new(sizeof *n);

        n->path = ag_string_new(path);
        n->dso = ag_string_new(dso);
        n->sym = ag_string_new(sym);
        n->next = NULL;

        return n;
}

static void
node_release(struct node *ctx)
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
registry_push(struct node *node)
{
        AG_ASSERT_PTR (node);
        AG_ASSERT_PTR (g_http);

        ag_hash h = ag_hash_new_str(node->path);
        register struct node *i = g_http->reg[h % REGISTRY_BUCKETS];

        if (i) {
                while (i && i->next)
                        i = i->next;

                i->next = node;
        } else
                i = node;
}

        
static inline struct node **
registry_new(void)
{
        return ag_memblock_new(sizeof(struct node *) * REGISTRY_BUCKETS);
}

static void
registry_release(struct node **ctx)
{
        for (register size_t i = 0; i < REGISTRY_BUCKETS; i++) {
                register struct node *n = ctx[i];

                while (n) {
                        register struct node *n2 = n->next;
                        node_release(n);
                        n = n2;
                }
        }
}

