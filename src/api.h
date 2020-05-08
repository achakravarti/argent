/******************************************************************************
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

