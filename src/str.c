#include "../include/argent.h"

#include <ctype.h>
#include <string.h>
#include <stdarg.h>


#ifndef NDEBUG
static inline bool is_string_valid(const char *);
static inline bool is_string_not_empty(const char *);
#endif


extern inline ag_str *ag_str_new_empty(void);
extern inline bool ag_str_lt(const char *, const char *);
extern inline bool ag_str_eq(const char *, const char *);
extern inline bool ag_str_gt(const char *, const char *);
extern inline bool ag_str_empty(const ag_str *);


extern ag_str *ag_str_new(const char *src)
{
        AG_ASSERT (is_string_valid(src));

        size_t sz = strlen(src);
        char *s = ag_mblock_new(sz + 1);

        strncpy(s, src, sz);
        s[sz] = '\0';
        return s;
}



extern ag_str *ag_str_new_fmt(const char *fmt, ...)
{
        AG_ASSERT (is_string_not_empty(fmt));

        va_list args;
        va_start(args, fmt);
        char *bfr = ag_mblock_new(vsnprintf(NULL, 0, fmt, args));
        va_end(args);

        va_start(args, fmt);
        (void)vsprintf(bfr, fmt, args);
        va_end(args);

        char *s = ag_str_new(bfr);
        ag_mblock_release((ag_mblock **)&bfr);
        return s;
}


extern ag_str *ag_str_copy(const ag_str *ctx)
{
        AG_ASSERT (is_string_valid(ctx));

        ag_str *cp = (ag_str *)ctx;
        ag_mblock_retain(cp);
        return cp;
}


extern void ag_str_release(ag_str **ctx)
{
        ag_mblock_release((ag_mblock **)ctx);
}


extern enum ag_cmp ag_str_cmp(const char *ctx,  const char *cmp)
{
        AG_ASSERT (is_string_valid(ctx));
        AG_ASSERT (is_string_valid(cmp));

        return strcmp(ctx, cmp);
}


extern bool ag_str_has(const ag_str *ctx, const char *tgt)
{
        AG_ASSERT (is_string_valid(ctx));
        AG_ASSERT (is_string_not_empty(tgt));

        return strstr(ctx, tgt);
}


extern size_t ag_str_len(const ag_str *ctx)
{
        AG_ASSERT (is_string_valid(ctx));

        register size_t i = 0, len = 0;

        while (ctx[i]) {
                if ((ctx[i] & 0xC0) != 0x80)
                        len++;
                
                i++;
        }

        return len;
}


extern size_t ag_str_sz(const ag_str *ctx)
{
        AG_ASSERT (is_string_valid(ctx));

        return ag_mblock_sz(ctx);
}



extern size_t ag_str_refc(const ag_str *ctx)
{
        AG_ASSERT (is_string_valid(ctx));

        return ag_mblock_refc(ctx);
}


extern ag_str *ag_str_lower(const char *ctx)
{
        AG_ASSERT (is_string_valid(ctx));

        ag_str *s = ag_str_new(ctx);
        register char *c = s;

        while (*c) {
                *c = tolower(*c);
                c++;
        }

        return s;
}



extern ag_str *ag_str_upper(const char *ctx)
{
        AG_ASSERT (is_string_valid(ctx));
        
        ag_str *s = ag_str_new(ctx);
        register char *c = s;

        while (*c) {
                *c = toupper(*c);
                c++;
        }

        return s;
}


extern ag_str *ag_str_proper(const char *ctx)
{
        AG_ASSERT (is_string_valid(ctx));
        
        ag_str *s = ag_str_new(ctx);
        register size_t sz = ag_mblock_sz(s);

        for (register size_t i = 0; i < sz; i++) {
                s[i] = (!i || s[i - 1] == ' ' || s[i - 1] == '.')
                        ? toupper(s[i]) : tolower(s[i]);
        }

        return s;
}


extern ag_str *ag_str_split(const char *ctx, const char *pvt)
{
        AG_ASSERT (is_string_valid(ctx));
        AG_ASSERT (is_string_not_empty(pvt));

        if (AG_UNLIKELY (!(*ctx && strstr(ctx, pvt))))
                return ag_str_new_empty();

        char *save;
        ag_str_auto *s = ag_str_new(ctx);
        return ag_str_new(strtok_r(s, pvt, &save));
}


extern ag_str *ag_str_split_right(const char *ctx, const char *pvt)
{
        AG_ASSERT (is_string_valid(ctx));
        AG_ASSERT (is_string_not_empty(pvt));

        if (AG_UNLIKELY (!(*ctx && strstr(ctx, pvt))))
                return ag_str_new_empty();

        char *save;
        ag_str_auto *s = ag_str_new(ctx);
        (void)strtok_r(s, pvt, &save);
        return ag_str_new(save);
}

        

#ifndef NDEBUG
static inline bool is_string_valid(const char *ctx)
{
        return ctx;
}
#endif
        

#ifndef NDEBUG
static inline bool is_string_not_empty(const char *ctx)
{
        return ctx && *ctx;
}
#endif

