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


#ifndef __ARGENT_TEST_OBJECT_H__
#define __ARGENT_TEST_OBJECT_H__


#define AG_METATEST_OBJECT_COPY(type, sample)                           \
        AG_TEST_CASE(#type "_copy(): " #sample " => shallow copy")      \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_AUTO(type) *cp = type ## _copy(o);                   \
                AG_TEST (cp == o);                                      \
        }                                                               \
        AG_TEST_CASE(#type "_copy(): " #sample " => refc +1")           \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_AUTO(type) *cp = type ## _copy(o);                   \
                (void)cp;                                               \
                AG_TEST (type ## _refc(o) == 2);                        \
        }                                                               \
        AG_TEST_CASE(#type "_copy(): " #sample " => data preserved")    \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_AUTO(type) *cp = type ## _copy(o);                   \
                AG_TEST (type ## _eq(o, cp));                           \
        }


#define AG_METATEST_OBJECT_CLONE(type, sample)                          \
        AG_TEST_CASE(#type "_clone(): " #sample " => deep copy")        \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_AUTO(type) *cp = type ## _clone(o);                  \
                AG_TEST (o != cp);                                      \
        }                                                               \
        AG_TEST_CASE(#type "_clone(): " #sample " => refc no effect")   \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_AUTO(type) *cp = type ## _clone(o);                  \
                (void)cp;                                               \
                AG_TEST (type ## _refc(o) == 1);                        \
        }                                                               \
        AG_TEST_CASE(#type "_clone(): " #sample " => data preserved")   \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_AUTO(type) *cp = type ## _clone(o);                  \
                AG_TEST (type ## _eq(o, cp));                           \
        }


#define AG_METATEST_OBJECT_RELEASE(type, sample)                        \
        AG_TEST_CASE(#type "_release(): pptr NULL => no-op")            \
        {                                                               \
                type ## _release(NULL);                                 \
                AG_TEST (true);                                         \
        }                                                               \
        AG_TEST_CASE(#type "_release(): pptr ~NULL ptr NULL => no-op")  \
        {                                                               \
                AG_AUTO(type) *o = NULL;                                \
                type ## _release(&o);                                   \
                AG_TEST (true);                                         \
        }                                                               \
        AG_TEST_CASE(#type "_release(): " #sample " => dispose ptr")    \
        {                                                               \
                type *o = sample;                                       \
                type ## _release(&o);                                   \
                AG_TEST (!o);                                           \
        }                                                               \
        AG_TEST_CASE(#type "_release(): " #sample " => refc -1")        \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_AUTO(type) *cp = type ## _copy(o);                   \
                type *cp2 = type ## _copy(cp);                          \
                type ## _release(&cp2);                                 \
                AG_TEST (type ## _refc(o) == 2);                        \
        }


#define AG_METATEST_OBJECT_CMP(type, small, big)                        \
        AG_TEST_CASE(#type "_cmp(): ctx < cmp => AG_CMP_LT")            \
        {                                                               \
                AG_AUTO(type) *s = small;                               \
                AG_AUTO(type) *b = big;                                 \
                AG_TEST (type ## _cmp(s, b) == AG_CMP_LT);              \
        }                                                               \
        AG_TEST_CASE(#type "_cmp(): ctx == cmp => AG_CMP_EQ")           \
        {                                                               \
                AG_AUTO(type) *s = big;                                 \
                AG_AUTO(type) *b = big;                                 \
                AG_TEST (type ## _cmp(s, b) == AG_CMP_EQ);              \
        }                                                               \
        AG_TEST_CASE(#type "_cmp(): ctx > cmp => AG_CMP_GT")            \
        {                                                               \
                AG_AUTO(type) *s = small;                               \
                AG_AUTO(type) *b = big;                                 \
                AG_TEST (type ## _cmp(b, s) == AG_CMP_GT);              \
        }


#define AG_METATEST_OBJECT_LT(type, small, big)                         \
        AG_TEST_CASE(#type "_lt(): ctx < cmp => true")                  \
        {                                                               \
                AG_AUTO(type) *s = small;                               \
                AG_AUTO(type) *b = big;                                 \
                AG_TEST (type ## _lt(s, b));                            \
        }                                                               \
        AG_TEST_CASE(#type "_lt(): ctx == cmp => false")                \
        {                                                               \
                AG_AUTO(type) *s = small;                               \
                AG_AUTO(type) *b = small;                               \
                AG_TEST (!type ## _lt(s, b));                           \
        }                                                               \
        AG_TEST_CASE(#type "_lt(): ctx > cmp => false")                 \
        {                                                               \
                AG_AUTO(type) *s = small;                               \
                AG_AUTO(type) *b = big;                                 \
                AG_TEST (!type ## _lt(b, s));                           \
        }                                                               \


#define AG_METATEST_OBJECT_EQ(type, small, big)                         \
        AG_TEST_CASE(#type "_eq(): ctx < cmp => false")                 \
        {                                                               \
                AG_AUTO(type) *s = small;                               \
                AG_AUTO(type) *b = big;                                 \
                AG_TEST (!type ## _eq(s, b));                           \
        }                                                               \
        AG_TEST_CASE(#type "_eq(): ctx == cmp => true")                 \
        {                                                               \
                AG_AUTO(type) *s = small;                               \
                AG_AUTO(type) *b = small;                               \
                AG_TEST (type ## _eq(s, b));                            \
        }                                                               \
        AG_TEST_CASE(#type "_eq(): ctx > cmp => false")                 \
        {                                                               \
                AG_AUTO(type) *s = small;                               \
                AG_AUTO(type) *b = big;                                 \
                AG_TEST (!type ## _eq(b, s));                           \
        }                                                               \


#define AG_METATEST_OBJECT_GT(type, small, big)                         \
        AG_TEST_CASE(#type "_gt(): ctx < cmp => false")                 \
        {                                                               \
                AG_AUTO(type) *s = small;                               \
                AG_AUTO(type) *b = big;                                 \
                AG_TEST (!type ## _gt(s, b));                           \
        }                                                               \
        AG_TEST_CASE(#type "_gt(): ctx == cmp => false")                \
        {                                                               \
                AG_AUTO(type) *s = small;                               \
                AG_AUTO(type) *b = small;                               \
                AG_TEST (!type ## _gt(s, b));                           \
        }                                                               \
        AG_TEST_CASE(#type "_gt(): ctx > cmp => true")                  \
        {                                                               \
                AG_AUTO(type) *s = small;                               \
                AG_AUTO(type) *b = big;                                 \
                AG_TEST (type ## _gt(b, s));                            \
        }                                                               \


#define AG_METATEST_OBJECT_EMPTY(type, sample)                  \
        AG_TEST_CASE(#type "_empty(): " #sample " => true")     \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (type ## _empty(o))                     \
        }


#define AG_METATEST_OBJECT_EMPTY_NOT(type, sample)              \
        AG_TEST_CASE(#type "_empty(): " #sample " => false")    \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (!type ## _empty(o))                    \
        }


#define AG_METATEST_OBJECT_VALID(type, sample)                  \
        AG_TEST_CASE(#type "_valid(): " #sample " => true")     \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (type ## _valid(o))                     \
        }


#define AG_METATEST_OBJECT_VALID_NOT(type, sample)              \
        AG_TEST_CASE(#type "_valid(): " #sample " => false")    \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (!type ## _valid(o))                    \
        }


#define AG_METATEST_OBJECT_TYPEID(type, sample, typeid)                 \
        AG_TEST_CASE(#type "_typeid(): " #sample " => " #typeid)        \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_TEST (type ## _typeid(o) == typeid)                  \
        }                                                               \


#define AG_METATEST_OBJECT_UUID(type, sample)                   \
        AG_TEST_CASE(#type "_uuid(): " #sample " => UUID")      \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_AUTO(ag_uuid) *u = type ## _uuid(o);         \
                AG_AUTO(ag_string) *s = ag_uuid_str(u);         \
                AG_TEST (!ag_string_empty(s));                  \
        }                                                       \


#define AG_METATEST_OBJECT_LEN(type, sample, len)               \
        AG_TEST_CASE(#type "_len(): " #sample " => " #len)      \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (type ## _len(o) == len)                \
        }


#define AG_METATEST_OBJECT_SZ(type, sample, sz)                 \
        AG_TEST_CASE(#type "_sz(): " #sample " => " #sz)        \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (type ## _sz(o) == sz)                  \
        }


#define AG_METATEST_OBJECT_HASH(type, sample, hash)             \
        AG_TEST_CASE(#type "_hash(): " #sample " => " #hash)    \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (type ## _hash(o) == hash)              \
        }


#define AG_METATEST_OBJECT_STR(type, sample, str)               \
        AG_TEST_CASE(#type "_str(): " #sample " => " #str)      \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_AUTO(ag_string) *s = type ## _str(o);        \
                AG_TEST (ag_string_eq(s, str));                 \
        }


#define AG_METATEST_OBJECT_STR_HAS(type, sample, str)           \
        AG_TEST_CASE(#type "_str(): " #sample " => has " #str)  \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_AUTO(ag_string) *s = type ## _str(o);        \
                AG_TEST (ag_string_has(s, str));                \
        }


#define AG_METATEST_OBJECT_JSON(T, SMP, JSTR)           \
        AG_TEST_CASE(#T "_json(): " #SMP " => " #JSTR)  \
        {                                               \
                AG_AUTO(T) *o = SMP;                    \
                AG_AUTO(ag_string) *j = T##_json(o);    \
                AG_TEST (ag_string_eq(j, JSTR));        \
        }


#define AG_METATEST_OBJECT_JSON_HAS(T, SMP, JSTR)       \
        AG_TEST_CASE(#T "_json(): " #SMP " => " #JSTR)  \
        {                                               \
                AG_AUTO(T) *o = SMP;                    \
                AG_AUTO(ag_string) *j = T##_json(o);    \
                AG_TEST (ag_string_has(j, JSTR));       \
        }


#define AG_METATEST_OBJECT_REFC(type, sample)                           \
        AG_TEST_CASE(#type "_refc(): single instance => refc == 1")     \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_TEST (type ## _refc(o) == 1);                        \
        }                                                               \
        AG_TEST_CASE(#type "_refc(): shallow copy => refc > 1")         \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_AUTO(type) *cp = type ## _copy(o);                   \
                AG_AUTO(type) *cp2 = type ## _copy(cp);                 \
                (void)cp2;                                              \
                AG_TEST (type ## _refc(o) == 3);                        \
        }                                                               \
        AG_TEST_CASE(#type "_refc(): deep copy => refc == 1")           \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_AUTO(type) *cp = type ## _clone(o);                  \
                AG_AUTO(type) *cp2 = type ## _clone(cp);                \
                (void)cp2;                                              \
                AG_TEST (type ## _refc(o) == 1);                        \
        }


#endif /* !__ARGENT_TEST_OBJECT_H__ */

