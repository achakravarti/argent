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


#ifndef __ARGENT_INCLUDE_OBJECT_H__
#define __ARGENT_INCLUDE_OBJECT_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "../src/ex/exception.h"
#include "../src/util/memblock.h"
#include "../src/util/hash.h"
#include "./typeid.h"
#include "../src/util/uuid.h"


typedef struct ag_object ag_object;

#define AG_OBJECT_DECLARE(T, TID)                                       \
        typedef ag_object T;                                            \
        extern const ag_typeid __##T##_tid__;                           \
        inline T *T##_copy(const T *hnd)                                \
        {                                                               \
                AG_ASSERT_PTR (hnd);                                    \
                AG_ASSERT (ag_object_typeid(hnd) == __##T##_tid__);     \
                return ag_object_copy(hnd);                             \
        }                                                               \
        inline T *T##_clone(const T *hnd)                               \
        {                                                               \
                AG_ASSERT_PTR (hnd);                                    \
                AG_ASSERT (ag_object_typeid(hnd) == __##T##_tid__);     \
                return ag_object_clone(hnd);                            \
        }                                                               \
        inline void T##_release(T **hnd)                                \
        {                                                               \
                ag_object_release(hnd);                                 \
        }                                                               \
        inline enum ag_cmp T##_cmp(const T *lhs, const T *rhs)          \
        {                                                               \
                AG_ASSERT_PTR (lhs);                                    \
                AG_ASSERT_PTR (rhs);                                    \
                AG_ASSERT (ag_object_typeid(lhs) == __##T##_tid__);     \
                AG_ASSERT (ag_object_typeid(rhs) == __##T##_tid__);     \
                return ag_object_cmp(lhs, rhs);                         \
        }                                                               \
        inline bool T##_lt(const T *lhs, const T *rhs)                  \
        {                                                               \
                AG_ASSERT_PTR (lhs);                                    \
                AG_ASSERT_PTR (rhs);                                    \
                AG_ASSERT (ag_object_typeid(lhs) == __##T##_tid__);     \
                AG_ASSERT (ag_object_typeid(rhs) == __##T##_tid__);     \
                return ag_object_lt(lhs, rhs);                          \
        }                                                               \
        inline bool T##_eq(const T *lhs, const T *rhs)                  \
        {                                                               \
                AG_ASSERT_PTR (lhs);                                    \
                AG_ASSERT_PTR (rhs);                                    \
                AG_ASSERT (ag_object_typeid(lhs) == __##T##_tid__);     \
                AG_ASSERT (ag_object_typeid(rhs) == __##T##_tid__);     \
                return ag_object_eq(lhs, rhs);                          \
        }                                                               \
        inline bool T##_gt(const T *lhs, const T *rhs)                  \
        {                                                               \
                AG_ASSERT_PTR (lhs);                                    \
                AG_ASSERT_PTR (rhs);                                    \
                AG_ASSERT (ag_object_typeid(lhs) == __##T##_tid__);     \
                AG_ASSERT (ag_object_typeid(rhs) == __##T##_tid__);     \
                return ag_object_gt(lhs, rhs);                          \
        }                                                               \
        inline bool T##_empty(const T *hnd)                             \
        {                                                               \
                AG_ASSERT_PTR (hnd);                                    \
                AG_ASSERT (ag_object_typeid(hnd) == __##T##_tid__);     \
                return ag_object_empty(hnd);                            \
        }                                                               \
        inline ag_typeid T##_typeid(const T *hnd)                       \
        {                                                               \
                AG_ASSERT_PTR (hnd);                                    \
                AG_ASSERT (ag_object_typeid(hnd) == __##T##_tid__);     \
                return ag_object_typeid(hnd);                           \
        }                                                               \
        inline ag_uuid *T ## _uuid(const T *ctx)                        \
        {                                                               \
                AG_ASSERT_PTR (ctx);                                    \
                AG_ASSERT (ag_object_typeid(ctx) == __##T##_tid__);     \
                return ag_object_uuid(ctx);                             \
        }                                                               \
        inline bool T ## _valid(const T *ctx)                           \
        {                                                               \
                AG_ASSERT_PTR (ctx);                                    \
                AG_ASSERT (ag_object_typeid(ctx) == __##T##_tid__);     \
                return ag_object_valid(ctx);                            \
        }                                                               \
        inline size_t T ## _sz(const T *ctx)                            \
        {                                                               \
                AG_ASSERT_PTR (ctx);                                    \
                AG_ASSERT (ag_object_typeid(ctx) == __##T##_tid__);     \
                return ag_object_sz(ctx);                               \
        }                                                               \
        inline size_t T ## _refc(const T *ctx)                          \
        {                                                               \
                AG_ASSERT_PTR (ctx);                                    \
                AG_ASSERT (ag_object_typeid(ctx) == __##T##_tid__);     \
                return ag_object_refc(ctx);                             \
        }                                                               \
        inline size_t T ## _len(const T *ctx)                           \
        {                                                               \
                AG_ASSERT_PTR (ctx);                                    \
                AG_ASSERT (ag_object_typeid(ctx) == __##T##_tid__);     \
                return ag_object_len(ctx);                              \
        }                                                               \
        inline ag_hash T ## _hash(const T *ctx)                         \
        {                                                               \
                AG_ASSERT_PTR (ctx);                                    \
                AG_ASSERT (ag_object_typeid(ctx) == __##T##_tid__);     \
                return ag_object_hash(ctx);                             \
        }                                                               \
        inline ag_string *T ## _str(const T *ctx)                       \
        {                                                               \
                AG_ASSERT_PTR (ctx);                                    \
                AG_ASSERT (ag_object_typeid(ctx) == __##T##_tid__);     \
                return ag_object_str(ctx);                              \
        }                                                               \
        inline ag_string *T ## _json(const T *ctx)                      \
        {                                                               \
                AG_ASSERT_PTR (ctx);                                    \
                AG_ASSERT (ag_object_typeid(ctx) == __##T##_tid__);     \
                return ag_object_json(ctx);                             \
        }                                                               \
        extern void __ ## T ## _register__(void)


#define AG_OBJECT_DEFINE_CLONE(T, CLOS)         \
        ag_memblock *                           \
        __##T##_clone__(const ag_memblock *_p_) \
        {                                       \
                AG_ASSERT_PTR (_p_);            \
                CLOS                            \
        }


#define AG_OBJECT_DEFINE_RELEASE(T, CLOS)       \
        void                                    \
        __##T##_release__(ag_memblock *_p_)     \
        {                                       \
                AG_ASSERT_PTR (_p_);            \
                CLOS                            \
        }


#define AG_OBJECT_DEFINE_CMP(T, CLOS)                                   \
        enum ag_cmp                                                     \
        __##T##_cmp__(const ag_object *_o1_, const ag_object *_o2_)     \
        {                                                               \
                AG_ASSERT_PTR (_o1_);                                   \
                AG_ASSERT_PTR (_o2_);                                   \
                AG_ASSERT (ag_object_typeid(_o1_) == __##T##_tid__);    \
                AG_ASSERT (ag_object_typeid(_o2_) == __##T##_tid__);    \
                CLOS                                                    \
        }


#define AG_OBJECT_DEFINE_VALID(T, CLOS)                                 \
        bool                                                            \
        __##T##_valid__(const ag_object *_o_)                           \
        {                                                               \
                AG_ASSERT_PTR (_o_);                                    \
                AG_ASSERT (ag_object_typeid(_o_) == __##T##_tid__);     \
                CLOS                                                    \
        }


#define AG_OBJECT_DEFINE_SZ(T, CLOS)                                    \
        size_t                                                          \
        __##T##_sz__(const ag_object *_o_)                              \
        {                                                               \
                AG_ASSERT_PTR (_o_);                                    \
                AG_ASSERT (ag_object_typeid(_o_) == __##T##_tid__);     \
                CLOS                                                    \
        }


#define AG_OBJECT_DEFINE_LEN(T, CLOS)                                   \
        size_t                                                          \
        __##T##_len__(const ag_object *_o_)                             \
        {                                                               \
                AG_ASSERT_PTR (_o_);                                    \
                AG_ASSERT (ag_object_typeid(_o_) == __##T##_tid__);     \
                CLOS                                                    \
        }


#define AG_OBJECT_DEFINE_HASH(T, CLOS)                                  \
        ag_hash                                                         \
        __##T##_hash__(const ag_object *_o_)                            \
        {                                                               \
                AG_ASSERT_PTR (_o_);                                    \
                AG_ASSERT (ag_object_typeid(_o_) == __##T##_tid__);     \
                CLOS                                                    \
        }


#define AG_OBJECT_DEFINE_STR(T, CLOS)                                   \
        ag_string *                                                     \
        __##T##_str__(const ag_object *_o_)                             \
        {                                                               \
                AG_ASSERT_PTR (_o_);                                    \
                AG_ASSERT (ag_object_typeid(_o_) == __##T##_tid__);     \
                CLOS                                                    \
        }


#define AG_OBJECT_DEFINE(T, TID)                                        \
        const ag_typeid __##T##_tid__ = TID;                            \
        extern inline T *T##_copy(const T *);                           \
        extern inline T *T##_clone(const T *);                          \
        extern inline void T##_release(T **);                           \
        extern inline enum ag_cmp T##_cmp(const T *, const T *);        \
        extern inline bool T##_lt(const T *, const T *);                \
        extern inline bool T##_eq(const T *, const T *);                \
        extern inline bool T##_gt(const T *, const T *);                \
        extern inline bool T##_empty(const T *);                        \
        extern inline ag_typeid T##_typeid(const T *);                  \
        extern inline ag_uuid *T##_uuid(const T *);                     \
        extern inline bool T##_valid(const T *);                        \
        extern inline size_t T##_sz(const T *);                         \
        extern inline size_t T##_refc(const T *);                       \
        extern inline size_t T##_len(const T *);                        \
        extern inline ag_hash T##_hash(const T *);                      \
        extern inline ag_string *T##_str(const T *);                    \
        extern inline ag_string *T##_json(const T *);                   \
        extern void __##T##_register__(void)                            \
        {                                                               \
                __ag_object_register__(#T, TID);                        \
        }

#define AG_OBJECT_REGISTER(T) __##T##_register__()

extern ag_object                *ag_object_new(ag_typeid, ag_memblock *);
extern ag_object                *ag_object_copy(const ag_object *);
extern ag_object                *ag_object_clone(const ag_object *);
extern void                      ag_object_release(ag_object **);
extern enum ag_cmp               ag_object_cmp(const ag_object *,
                                    const ag_object *);
extern ag_typeid                 ag_object_typeid(const ag_object *);
extern ag_uuid                  *ag_object_uuid(const ag_object *);
extern bool                      ag_object_valid(const ag_object *);
extern size_t                    ag_object_sz(const ag_object *);
extern size_t                    ag_object_refc(const ag_object *);
extern size_t                    ag_object_len(const ag_object *);
extern ag_hash                   ag_object_hash(const ag_object *);
extern ag_string                *ag_object_str(const ag_object *);
extern ag_string                *ag_object_json(const ag_object *);
extern const ag_memblock        *ag_object_payload(const ag_object *);
extern ag_memblock              *ag_object_payload_mutable(ag_object **);
extern void                      __ag_object_register__(const char *,
                                    ag_typeid);

inline bool
ag_object_lt(const ag_object *ctx, const ag_object *cmp)
{
        return ag_object_cmp(ctx, cmp) == AG_CMP_LT;
}

inline bool
ag_object_eq(const ag_object *ctx, const ag_object *cmp)
{
        return ag_object_cmp(ctx, cmp) == AG_CMP_EQ;
}

inline bool
ag_object_gt(const ag_object *ctx, const ag_object *cmp)
{
        return ag_object_cmp(ctx, cmp) == AG_CMP_GT;
}

inline bool
ag_object_empty(const ag_object *ctx)
{
        return !ag_object_len(ctx);
}


typedef ag_memblock     *(ag_object_clone_virt)(const ag_memblock *);
typedef void             (ag_object_release_virt)(ag_memblock *);
typedef enum ag_cmp      (ag_object_cmp_virt)(const ag_object *,
                            const ag_object *);
typedef bool             (ag_object_valid_virt)(const ag_object *);
typedef size_t           (ag_object_sz_virt)(const ag_object *);
typedef size_t           (ag_object_len_virt)(const ag_object *);
typedef ag_hash          (ag_object_hash_virt)(const ag_object *);
typedef ag_string       *(ag_object_str_virt)(const ag_object *);
typedef ag_string       *(ag_object_json_virt)(const ag_object *);


struct ag_object_vtable {
        ag_object_clone_virt    *clone;   /* Clone callback               */
        ag_object_release_virt  *release; /* Release callback             */
        ag_object_cmp_virt      *cmp;     /* Comparison callback          */
        ag_object_valid_virt    *valid;   /* Validation callback          */
        ag_object_sz_virt       *sz;      /* Size computation callback    */
        ag_object_len_virt      *len;     /* Length computation callback  */
        ag_object_hash_virt     *hash;    /* Hash computation callback    */
        ag_object_str_virt      *str;     /* String generation callback   */
        ag_object_json_virt     *json;    /* JSON representation callback */
};


extern void                              ag_object_registry_init(void);
extern void                              ag_object_registry_exit(void);
extern const struct ag_object_vtable    *ag_object_registry_get(ag_typeid);
extern void                              ag_object_registry_push(ag_typeid,
                                            const struct ag_object_vtable *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_INCLUDE_OBJECT_H__ */

