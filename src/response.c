#include "./api.h"

extern void ag_response_register(void);

extern ag_response_t *ag_response_new(enum ag_http_mime type, 
        enum ag_http_status code);

extern inline ag_response_t *ag_response_copy(const ag_response_t *ctx);


extern inline void ag_response_dispose(ag_response_t **ctx);

extern inline size_t ag_response_typeid(const ag_response_t *ctx);

extern inline size_t ag_response_objid(const ag_response_t *ctx);

extern inline size_t ag_response_hash(const ag_response_t *ctx);

extern inline size_t ag_response_sz(const ag_response_t *ctx);

extern inline size_t ag_response_len(const ag_response_t *ctx);

extern inline bool ag_response_empty(const ag_response_t *ctx);

extern inline enum ag_tristate ag_response_cmp(const ag_response_t *ctx, 
        const ag_response_t *cmp);

extern inline bool ag_response_lt(const ag_response_t *ctx, 
        const ag_response_t *cmp);

extern inline bool ag_response_eq(const ag_response_t *ctx, 
        const ag_response_t *cmp);

extern inline bool ag_response_gt(const ag_response_t *ctx, 
        const ag_response_t *cmp);

extern inline ag_string_t *ag_response_str(const ag_response_t *ctx);


extern void ag_response_add(const ag_response_t *ctx);
extern void ag_response_add_file(const char *fpath);

