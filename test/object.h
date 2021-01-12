#ifndef __ARGENT_TESTS_OBJECT_H__
#define __ARGENT_TESTS_OBJECT_H__


#define AG_TEST_OBJECT_COPY(type, sample)                               \
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
                AG_TEST (type ## _refc(o) == 2);                        \
        }                                                               \
        AG_TEST_CASE(#type "_copy(): " #sample " => data preserved")    \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_AUTO(type) *cp = type ## _copy(o);                   \
                AG_TEST (type ## _eq(o, cp));                           \
        }


#define AG_TEST_OBJECT_CLONE(type, sample)                              \
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
                AG_TEST (type ## _refc(o) == 1);                        \
        }                                                               \
        AG_TEST_CASE(#type "_clone(): " #sample " => data preserved")   \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_AUTO(type) *cp = type ## _clone(o);                  \
                AG_TEST (type ## _eq(o, cp));                           \
        }


#define AG_TEST_OBJECT_RELEASE(type, sample)                            \
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


#define AG_TEST_OBJECT_CMP(type, small, big)                            \
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


#define AG_TEST_OBJECT_LT(type, small, big)                             \
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


#define AG_TEST_OBJECT_EQ(type, small, big)                             \
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


#define AG_TEST_OBJECT_GT(type, small, big)                             \
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


#define AG_TEST_OBJECT_TYPEID(type, sample, typeid)                     \
        AG_TEST_CASE(#type "_typeid(): " #sample " => " #typeid)        \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_TEST (type ## _typeid(o) == typeid)                  \
        }                                                               \


#define AG_TEST_OBJECT_UUID(type, sample)                       \
        AG_TEST_CASE(#type "_uuid(): " #sample " => UUID")      \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_AUTO(ag_uuid) *u = type ## _uuid(o);         \
                AG_AUTO(ag_string) *s = ag_uuid_str(u);         \
                AG_TEST (!ag_string_empty(s));                  \
        }                                                       \


#define AG_TEST_OBJECT_EMPTY(type, sample)                      \
        AG_TEST_CASE(#type "_empty(): " #sample " => true")     \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (type ## _empty(o))                     \
        }


#define AG_TEST_OBJECT_EMPTY_NOT(type, sample)                  \
        AG_TEST_CASE(#type "_empty(): " #sample " => false")    \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (!type ## _empty(o))                    \
        }


#define AG_TEST_OBJECT_VALID(type, sample)                      \
        AG_TEST_CASE(#type "_valid(): " #sample " => true")     \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (type ## _valid(o))                     \
        }


#define AG_TEST_OBJECT_VALID_NOT(type, sample)                  \
        AG_TEST_CASE(#type "_valid(): " #sample " => false")    \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (!type ## _valid(o))                    \
        }


#define AG_TEST_OBJECT_LEN(type, sample, len)                   \
        AG_TEST_CASE(#type "_len(): " #sample " => " #len)      \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (type ## _len(o) == len)                \
        }


#define AG_TEST_OBJECT_SZ(type, sample, sz)                     \
        AG_TEST_CASE(#type "_sz(): " #sample " => " #sz)        \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (type ## _sz(o) == sz)                  \
        }


#define AG_TEST_OBJECT_HASH(type, sample, hash)                 \
        AG_TEST_CASE(#type "_hash(): " #sample " => " #hash)    \
        {                                                       \
                AG_AUTO(type) *o = sample;                      \
                AG_TEST (type ## _hash(o) == hash)              \
        }


#define AG_TEST_OBJECT_REFC(type, sample)                               \
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
                AG_TEST (type ## _refc(o) == 3);                        \
        }                                                               \
        AG_TEST_CASE(#type "_refc(): deep copy => refc == 1")           \
        {                                                               \
                AG_AUTO(type) *o = sample;                              \
                AG_AUTO(type) *cp = type ## _clone(o);                  \
                AG_AUTO(type) *cp2 = type ## _clone(cp);                \
                AG_TEST (type ## _refc(o) == 1);                        \
        }


#endif /* !__ARGENT_TESTS_OBJECT_H__ */

