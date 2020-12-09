#ifndef __ARGENT_PRIMITIVES_H__
#define __ARGENT_PRIMITIVES_H__


#ifdef __cplusplus
        extern "C" {
#endif


#if (defined __GNUC__ || defined __clang__)
#   define ag_pure __attribute__((pure))
#else
#   define ag_pure
#   warning "[!] ag_pure not supported by current compiler"
#endif


#if (defined __GNUC__ || defined __clang__)
#   define ag_hot __attribute__((hot))
#else
#   define ag_hot
#   warning "[!] ag_hot not supported by current compiler"
#endif


#if (defined __GNUC__ || defined __clang__)
#   define ag_cold __attribute__((cold))
#else
#   define ag_cold
#   warning "[!] ag_cold not supported by current compiler"
#endif


#if (defined __GNUC__ || defined __clang__)
#   define ag_likely(p) (__builtin_expect(!!(p), 1))
#else
#   define ag_likely(p) (p)
#   warning "[!] ag_likely() not supported by current compiler"
#endif


#if (defined __GNUC__ || defined __clang__)
#   define ag_unlikely(p) (__builtin_expect(!!(p), 0))
#else
#   define ag_unlikely(p) (p)
#   warning "[!] ag_unlikely not supported by current compiler"
#endif


#if (defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L)
#   include <threads.h>
#   define ag_threadlocal thread_local
#elif (defined __GNUC__ || defined __clang__)
#   define ag_threadlocal __thread
#else
#    define ag_threadlocal
#    warning "[!] ag_threadlocal not supported by current compiler"
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

