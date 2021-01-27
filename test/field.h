#ifndef __AG_TEST_FIELD_H__
#define __AG_TEST_FIELD_H__


#define AG_SAMPLE_FIELD(tag, key, val)                          \
        static inline ag_field *SAMPLE_FIELD_ ## tag(void)      \
        {                                                       \
                AG_AUTO(ag_value) *k = key;                     \
                AG_AUTO(ag_value) *v = val;                     \
                return ag_field_new(k, v);                      \
        }


#endif /* !__AG_TEST_FIELD_H__ */

