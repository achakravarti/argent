#ifndef __ARGENT_PRIMITIVES_H__
#define __ARGENT_PRIMITIVES_H__


#ifdef __cplusplus
        extern "C" {
#endif


#if (defined __GNUC__ || defined __clang__)
#   define AG_PURE __attribute__((pure))
#else
#   define AG_PURE
#   warning "[!] AG_PURE not supported by current compiler"
#endif


#if (defined __GNUC__ || defined __clang__)
#   define AG_HOT __attribute__((hot))
#else
#   define AG_HOT
#   warning "[!] AG_HOT not supported by current compiler"
#endif


#if (defined __GNUC__ || defined __clang__)
#   define AG_COLD __attribute__((cold))
#else
#   define AG_COLD
#   warning "[!] AG_COLD not supported by current compiler"
#endif


#if (defined __GNUC__ || defined __clang__)
#   define AG_LIKELY(p) (__builtin_expect(!!(p), 1))
#else
#   define AG_LIKELY(p) (p)
#   warning "[!] AG_LIKELY() not supported by current compiler"
#endif


#if (defined __GNUC__ || defined __clang__)
#   define AG_UNLIKELY(p) (__builtin_expect(!!(p), 0))
#else
#   define AG_UNLIKELY(p) (p)
#   warning "[!] AG_UNLIKELY() not supported by current compiler"
#endif


#if (defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L)
#   include <threads.h>
#   define ag_threadlocal thread_local
#elif (defined __GNUC__ || defined __clang__)
#   define AG_THREADLOCAL __thread
#else
#    define AG_THREADLOCAL
#    warning "[!] AG_THREADLOCAL not supported by current compiler"
#endif


enum ag_cmp {
        AG_CMP_LT = -1,
        AG_CMP_EQ,
        AG_CMP_GT
};

#ifdef __cplusplus
        }
#endif

#endif /* !__ARGENT_PRIMITIVE_H__ */

