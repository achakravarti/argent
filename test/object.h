#ifndef __ARGENT_TESTS_OBJECT_H__
#define __ARGENT_TESTS_OBJECT_H__


#define AG_TESTS_OBJECT_COPY(type, sample)                              \
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


#define AG_TESTS_OBJECT_CLONE(type, sample)                             \
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


#define AG_TESTS_OBJECT_RELEASE(type, sample)                           \
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


#define AG_TESTS_OBJECT_CMP(type, small, big)                           \
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


#define AG_TESTS_OBJECT_LT(type, small, big)                            \
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


#define AG_TESTS_OBJECT_EQ(type, small, big)                            \
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


#define AG_TESTS_OBJECT_GT(type, small, big)                            \
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


#endif /* !__ARGENT_TESTS_OBJECT_H__ */

