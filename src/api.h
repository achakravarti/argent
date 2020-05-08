/*******************************************************************************
 *                        __   ____   ___  ____  __ _  ____ 
 *                       / _\ (  _ \ / __)(  __)(  ( \(_  _)
 *                      /    \ )   /( (_ \ ) _) /    /  )(  
 *                      \_/\_/(__\_) \___/(____)\_)__) (__)                     
 *
 * Argent Library
 * Copyright (c) 2020 Abhishek Chakravarti <abhishek@taranjali.org>
 *
 * This file is part of the Argent Library. It defines application programming
 * interface of the Argent Library.
 *
 * The contents of this file are released under the GPLv3 License. See the
 * accompanying LICENSE file or the generated Developer Manual (section I:?) for 
 * complete licensing details.
 *
 * BY CONTINUING TO USE AND/OR DISTRIBUTE THIS FILE, YOU ACKNOWLEDGE THAT YOU
 * HAVE UNDERSTOOD THESE LICENSE TERMS AND ACCEPT TO BE LEGALLY BOUND BY THEM.
 ******************************************************************************/



/*******************************************************************************
 *                             COMPILER EXTENSIONS
 */


/*
 *      The ag_likely() macro provides a branch prediction hint that a predicate
 *      is likely to be true [DM:??]
 */
#if (defined __GNUC__ || defined __clang__)
#   define ag_likely(p) (__builtin_expect(!!(p), 1))
#else
#   define ag_likely(p) (p)
#   warning "[!] ag_likely() not supported by current compiler"
#endif


/*
 *      The ag_unlikely() macro provides a branch prediction hint that a 
 *      predicate is likely to be false [DM:??]
 */
#if (defined __GNUC__ || defined __clang__)
#   define ag_unlikely(p) (__builtin_expect(!!(p), 0))
#else
#   define ag_unlikely(p) (p)
#   warning "[!] ag_unlikely not supported by current compiler"
#endif


/*
 *      The ag_pure macro hints that a given function is pure, having no side
 *      effects [DM:??]
 */
#if (defined __GNUC__ || defined __clang__)
#   define ag_pure __attribute__((pure))
#else
#   define ag_pure
#   warning "[!] ag_pure not supported by current compiler"
#endif


/*
 *      The ag_hot macro hints that a given function is called frequently
 *      [DM:??].
 */
#if (defined __GNUC__ || defined __clang__)
#   define ag_hot __attribute__((hot))
#else
#   define ag_hot
#   warning "[!] ag_hot not supported by current compiler"
#endif


/*
 *      The ag_cold macro hints that a given function is called infrequently
 *      [DM:??].
 */
#if (defined __GNUC__ || defined __clang__)
#   define ag_cold __attribute__((cold))
#else
#   define ag_cold
#   warning "[!] ag_cold not supported by current compiler"
#endif

