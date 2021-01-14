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


#define AG_METATEST_ALIST_HAS_KEY(sample, key)                                 \
        AG_TEST_CASE("ag_alist_has_key(): " #sample " <- " #key " => T")       \
        {                                                                      \
                AG_AUTO(ag_alist) *a = sample;                                 \
                AG_AUTO(ag_value) *k = key;                                    \
                AG_TEST (ag_alist_has_key(a, k));                              \
        }


#define AG_METATEST_ALIST_HAS_KEY_NOT(sample, key)                             \
        AG_TEST_CASE("ag_alist_has_key(): " #sample " <- " #key " => F")       \
        {                                                                      \
                AG_AUTO(ag_alist) *a = sample;                                 \
                AG_AUTO(ag_value) *k = key;                                    \
                AG_TEST (!ag_alist_has_key(a, k));                             \
        }


#define AG_METATEST_ALIST_HAS_VAL(sample, val)                                 \
        AG_TEST_CASE("ag_alist_has_val(): " #sample " <- " #val " => T")       \
        {                                                                      \
                AG_AUTO(ag_alist) *a = sample;                                 \
                AG_AUTO(ag_value) *v = val;                                    \
                AG_TEST (ag_alist_has_val(a, v));                              \
        }


#define AG_METATEST_ALIST_HAS_VAL_NOT(sample, val)                             \
        AG_TEST_CASE("ag_alist_has_val(): " #sample " <- " #val " => F")       \
        {                                                                      \
                AG_AUTO(ag_alist) *a = sample;                                 \
                AG_AUTO(ag_value) *v = val;                                    \
                AG_TEST (!ag_alist_has_val(a, v));                             \
        }


#define AG_METATEST_ALIST_VAL(sample, key, val)                 \
        AG_TEST_CASE("ag_alist_val(): " #key " => " #val)       \
        {                                                       \
                AG_AUTO(ag_alist) *a = sample;                  \
                AG_AUTO(ag_value) *k = key;                     \
                AG_AUTO(ag_value) *v = val;                     \
                AG_AUTO(ag_value) *v2 = ag_alist_val(a, k);     \
                AG_TEST (ag_value_eq(v, v2));                   \
        }


#define AG_METATEST_ALIST_VAL_SET(sample, key, val)             \
        AG_TEST_CASE("ag_alist_val_set(): " #key " => " #val)   \
        {                                                       \
                AG_AUTO(ag_alist) *a = sample;                  \
                AG_AUTO(ag_value) *k = key;                     \
                AG_AUTO(ag_value) *v = val;                     \
                ag_alist_val_set(&a, k, v);                     \
                AG_AUTO(ag_value) *v2 = ag_alist_val(a, k);     \
                AG_TEST (ag_value_eq(v, v2));                   \
        }


#endif /* !__AG_ARGENT_METATESTS_ALIST_H__ */
