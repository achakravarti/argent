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
                AG_TEST (ag_memblock_eq(o, cp));                        \
        }


#endif /* !__ARGENT_TESTS_OBJECT_H__ */

