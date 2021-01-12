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


#endif /* !__ARGENT_TESTS_OBJECT_H__ */

