/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Argent---infrastructure for building web services
 * Copyright (C) 2020 Abhishek Chakravarti
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact Abhishek Chakravarti at <abhishek@taranjali.org>.
 ******************************************************************************/


#include "../argent.h"

#ifdef __FreeBSD__
#       include <malloc_np.h>
#else
#       include <malloc.h>
#endif


/*******************************************************************************
 *
 */

static AG_NONULL inline size_t  *meta_head(const ag_memblock *ctx);
static AG_NONULL inline size_t   meta_sz(const ag_memblock *ctx);
static AG_NONULL inline size_t   meta_refc(const ag_memblock *ctx);


/*******************************************************************************
 *
 */

extern inline bool      ag_memblock_lt(const ag_memblock *, 
                            const ag_memblock *);
extern inline bool      ag_memblock_eq(const ag_memblock *,
                            const ag_memblock *);
extern inline bool      ag_memblock_gt(const ag_memblock *,
                            const ag_memblock *);


/*******************************************************************************
 *
 */

#define ASSERT_SZ(S)    AG_ASSERT (S && "memory size valid")
#define ASSERT_ALIGN(A) AG_ASSERT (A && !(A % 2) && "memory alignment valid")
#define ASSERT_HND(H)   AG_ASSERT (*H && "memory handle valid")


/*******************************************************************************
 *
 */

ag_memblock *
ag_memblock_new(size_t sz)
{
        ASSERT_SZ (sz);

        size_t sz2 = sz + sizeof(size_t) * 2;
        size_t *ctx = malloc(sz2);

        struct ag_exception_memblock x = { .sz = sz, .align = 0 };
        AG_REQUIRE_OPT (ctx, AG_ERNO_MBLOCK, &x);

        memset(ctx, 0, sz2);
        ctx[0] = 1;
        ctx[1] = sz;

        return (ag_memblock *)&(ctx[2]);
}


/*******************************************************************************
 *
 */

ag_memblock *
ag_memblock_new_align(size_t sz, size_t align)
{
        ASSERT_SZ (sz);
        ASSERT_ALIGN (align);
       
        size_t sz2 = sz + sizeof(size_t) * 2; 
        size_t *ctx;
        (void)posix_memalign((void **)&ctx, align, sz2);

        struct ag_exception_memblock x = { .sz = sz, .align = align };
        AG_REQUIRE_OPT (ctx, AG_ERNO_MBLOCK, &x);
        
        memset(ctx, 0, sz2);
        ctx[0] = 1;
        ctx[1] = sz;

        return (ag_memblock *)&(ctx[2]);
}


/*******************************************************************************
 *
 */

ag_memblock *
ag_memblock_copy(const ag_memblock *ctx)
{
        ag_memblock *cp = (ag_memblock *)ctx;
        ((size_t *)cp)[-2]++;

        return cp;
}


/*******************************************************************************
 *
 */

ag_memblock *
ag_memblock_clone(const ag_memblock *ctx)
{
        size_t sz = meta_sz(ctx);
        ag_memblock *cp = ag_memblock_new(sz);
        memcpy(cp, ctx, sz);

        return cp;
}


/*******************************************************************************
 *
 */

ag_memblock *
ag_memblock_clone_align(const ag_memblock *ctx, size_t align)
{
        ASSERT_ALIGN (align);

        size_t sz = meta_sz(ctx);
        ag_memblock *cp = ag_memblock_new_align(sz, align);
        memcpy(cp, ctx, sz);

        return cp;
}


/*******************************************************************************
 *
 */

void
ag_memblock_release(ag_memblock **ctx)
{
        size_t *hnd;

        if (AG_LIKELY (ctx && (hnd = (size_t *)*ctx))) {
                if (!(--hnd[-2]))
                        free(&hnd[-2]);
                        
                *ctx = NULL;
        }
}


/*******************************************************************************
 *
 */

enum ag_cmp
ag_memblock_cmp(const ag_memblock *ctx, const ag_memblock *cmp)
{
        return ctx == cmp ? AG_CMP_EQ : memcmp(ctx, cmp, meta_sz(ctx));
}


/*******************************************************************************
 *
 */

size_t
ag_memblock_sz(const ag_memblock *ctx)
{
        return meta_sz(ctx);
}


/*******************************************************************************
 *
 */

size_t
ag_memblock_sz_total(const ag_memblock *ctx)
{
        return malloc_usable_size(meta_head(ctx));
}


/*******************************************************************************
 *
 */

size_t
ag_memblock_refc(const ag_memblock *ctx)
{
        return meta_refc(ctx);
}


/*******************************************************************************
 *
 */

bool
ag_memblock_aligned(const ag_memblock *ctx, size_t align)
{
        ASSERT_ALIGN (align);

        return !((uintptr_t)meta_head(ctx) & (align - 1));
}


/*******************************************************************************
 *
 */

void
ag_memblock_resize(ag_memblock **ctx, size_t sz)
{
        ASSERT_HND (ctx);
        ASSERT_SZ (sz);

        ag_memblock *hnd = *ctx;
        size_t oldsz = meta_sz(hnd);

        ag_memblock *cp = ag_memblock_new(sz);
        memcpy(cp, hnd, sz < oldsz ? sz : oldsz);
        
        ag_memblock_release(ctx);
        *ctx = cp;
}


/*******************************************************************************
 *
 */

void
ag_memblock_resize_align(ag_memblock **ctx, size_t sz, size_t align)
{
        ASSERT_HND (ctx);
        ASSERT_SZ (sz);
        ASSERT_ALIGN (align);

        ag_memblock *hnd = *ctx;
        size_t oldsz = meta_sz(hnd);

        ag_memblock *cp = ag_memblock_new_align(sz, align);
        memcpy(cp, hnd, sz < oldsz ? sz : oldsz);
        
        ag_memblock_release(ctx);
        *ctx = cp;
}


/*******************************************************************************
 *
 */

ag_string *
ag_memblock_str(const ag_memblock *ctx)
{
        return (ag_string_new_fmt("address = %p, data sz = %lu,"
            " total data = %lu, refc = %lu", (void *)meta_head(ctx),
            meta_sz(ctx), ag_memblock_sz_total(ctx), meta_refc(ctx)));

}


/*******************************************************************************
 *
 */

size_t *
meta_head(const ag_memblock *ctx)
{
        return &((size_t *)ctx)[-2];
}


/*******************************************************************************
 *
 */

size_t
meta_sz(const ag_memblock *ctx)
{
        return ((size_t *)ctx)[-1];
}


/*******************************************************************************
 *
 */

size_t
meta_refc(const ag_memblock *ctx)
{
        return ((size_t *)ctx)[-2];
}

