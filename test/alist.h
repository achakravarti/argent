#ifndef __ARGENT_TEST_ALIST_H__
#define __ARGENT_TEST_ALIST_H__


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


#define AG_METATEST_ALIST_GET(sample, itr, attr)                        \
        AG_TEST_CASE("ag_alist_get(): iteration #" #itr " => " #attr)   \
        {                                                               \
                AG_AUTO(ag_alist) *a = sample;                          \
                AG_AUTO(ag_field) *f = attr;                            \
                ag_alist_start(&a);                                     \
                for (register size_t i = 0; i < itr; i++)               \
                        ag_alist_next(&a);                              \
                AG_AUTO(ag_field) *f2 = ag_alist_get(a);                \
                AG_TEST (ag_field_eq(f, f2));                           \
        }


#define AG_METATEST_ALIST_GET_AT(sample, idx, attr)                     \
        AG_TEST_CASE("ag_alist_get(): index #" #idx " => " #attr)       \
        {                                                               \
                AG_AUTO(ag_alist) *a = sample;                          \
                AG_AUTO(ag_field) *f = attr;                            \
                AG_AUTO(ag_field) *f2 = ag_alist_get_at(a, idx);        \
                AG_TEST (ag_field_eq(f, f2));                           \
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


#define AG_METATEST_ALIST_SET(sample, itr, attr)                        \
        AG_TEST_CASE("ag_list_set(): iteration #" #itr " => " #attr)    \
        {                                                               \
                AG_AUTO(ag_alist) *a = sample;                          \
                AG_AUTO(ag_field) *f = attr;                            \
                ag_alist_start(&a);                                     \
                for (register size_t i = 0; i < itr; i++)               \
                        ag_list_next(&a);                               \
                ag_alist_set(&a, f);                                    \
                AG_AUTO(ag_field) *f2 = ag_alist_get_at(a, itr + 1);    \
                AG_TEST (ag_field_eq(f, f2));                           \
        }


#define AG_METATEST_ALIST_SET_AT(sample, idx, attr)                     \
        AG_TEST_CASE("ag_list_set(): index #" #idx " => " #attr)        \
        {                                                               \
                AG_AUTO(ag_alist) *a = sample;                          \
                AG_AUTO(ag_field) *f = attr;                            \
                ag_alist_set_at(&a, f, idx);                            \
                ag_alist_start(&a);                                     \
                for (register size_t i = 0; i < idx - 1; i++)           \
                        ag_list_next(&a);                               \
                AG_AUTO(ag_field) *f2 = ag_alist_get(a);                \
                AG_TEST (ag_field_eq(f, f2));                           \
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


#endif /* !__ARGENT_TEST_ALIST_H__ */
