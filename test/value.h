#ifndef __ARGENT_TEST_VALUE_H__
#define __ARGENT_TEST_VALUE_H__


#define AG_SAMPLE_VALUE_INT(tag, val)                           \
        static inline ag_value *SAMPLE_VALUE_ ## tag(void)      \
        {                                                       \
                return ag_value_new_int(val);                   \
        }


#define AG_SAMPLE_VALUE_UINT(tag, val)                          \
        static inline ag_value *SAMPLE_VALUE_ ## tag(void)      \
        {                                                       \
                return ag_value_new_uint(val);                  \
        }


#define AG_SAMPLE_VALUE_FLOAT(tag, val)                         \
        static inline ag_value *SAMPLE_VALUE_ ## tag(void)      \
        {                                                       \
                return ag_value_new_float(val);                 \
        }


#define AG_SAMPLE_VALUE_STRING(tag, val)                        \
        static inline ag_value *SAMPLE_VALUE_ ## tag(void)      \
        {                                                       \
                AG_AUTO(ag_string) *s = ag_string_new(val);     \
                return ag_value_new_string(s);                  \
        }


#define AG_SAMPLE_VALUE_OBJECT(tag, type, val)                  \
        static inline ag_value *SAMPLE_VALUE_ ## tag(void)      \
        {                                                       \
                AG_AUTO(type) *o = val;                         \
                return ag_value_new_object(val);                \
        }


#endif /* !__ARGENT_TEST_VALUE_H__ */

