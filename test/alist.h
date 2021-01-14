#ifndef __AG_ARGENT_METATESTS_ALIST_H__
#define __AG_ARGENT_METATESTS_ALIST_H__


#define AG_METATEST_ALIST_HAS(sample, attr)                             \
        AG_TEST_CASE("ag_alist_has(): " #sample " <- " #attr " => T")   \
        {                                                               \
                AG_AUTO(ag_alist) *a = sample;                          \
                AG_AUTO(ag_field) *f = attr;                            \
                AG_TEST (ag_alist_has(a, f));                           \
        }


#define AG_METATEST_ALIST_HAS_NOT(sample, attr)                         \
        AG_TEST_CASE("ag_alist_has(): " #sample " <- " #attr " => F")   \
        {                                                               \
                AG_AUTO(ag_alist) *a = sample;                          \
                AG_AUTO(ag_field) *f = attr;                            \
                AG_TEST (!ag_alist_has(a, f));                          \
        }


#define AG_METATEST_ALIST_HAS_KEY(sample, key)                          \
        AG_TEST_CASE("ag_alist_has(): " #sample " <- " #key " => T")    \
        {                                                               \
                AG_AUTO(ag_alist) *a = sample;                          \
                AG_AUTO(ag_value) *k = key;                             \
                AG_TEST (ag_alist_has_key(a, k));                       \
        }


#define AG_METATEST_ALIST_HAS_KEY_NOT(sample, key)                      \
        AG_TEST_CASE("ag_alist_has(): " #sample " <- " #key " => F")    \
        {                                                               \
                AG_AUTO(ag_alist) *a = sample;                          \
                AG_AUTO(ag_value) *k = key;                             \
                AG_TEST (!ag_alist_has_key(a, k));                      \
        }


#endif /* !__AG_ARGENT_METATESTS_ALIST_H__ */
