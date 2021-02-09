/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Argent---infrastructure for building web services
 * Copyright (C) 2020 Abhishek Chakravarti
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact Abhishek Chakravarti at <abhishek@taranjali.org>.
 ******************************************************************************/


#ifndef __ARGENT_INCLUDE_ERNO_H__
#define __ARGENT_INCLUDE_ERNO_H__


#ifdef __cplusplus
        extern "C" {
#endif


/* 
 * Since we don't have exceptions per se in C, we need to rely on error codes to
 * signal exceptions. The Argent Library doesn't rely on the global `errno` in
 * order to prevent potential conflicts with error codes defined by the relevant
 * standards. We adopt the convention that negative error codes are reserved for
 * use by the Argent Library, and positive ones by client code.
 */


typedef int ag_erno;


/* 
 * The following symbolic constants are the error codes used in the Argent
 * Library. Apart from AG_ERNO_NULL, all of these error codes are negative to
 * distinguish them from error codes used by client code.
 */
#define AG_ERNO_NULL    ((ag_erno) 0)
#define AG_ERNO_MBLOCK  ((ag_erno) -1)
#define AG_ERNO_REGEX   ((ag_erno) -2)
#define AG_ERNO_PARSE   ((ag_erno) -3)
#define AG_ERNO_HTTP    ((ag_erno) -4)


/* 
 * The following symbolic constants are the short messages associated with the
 * above error codes. These symbolic constants are used in conjunction with the
 * AG_ERNO_MSG() macro defined below to generate a captioned error string.
 */
#define AG_ERNO_NULL_MSG        "no error has occurred"
#define AG_ERNO_MBLOCK_MSG      "failed to allocate memory block"
#define AG_ERNO_REGEX_MSG       "failed to process regular expression"
#define AG_ERNO_PARSE_MSG       "failed to parse string"
#define AG_ERNO_HTTP_MSG        "error in HTTP server"


/*
 * For debugging purposes, it is helpful to have error messages captioned with
 * the symbolic constant aliasing the error code, instead of just the error
 * code. We're using the AG_ERNO_MSG() macro to help generate these captioned
 * error messages from the above error codes and their associated messages. For
 * example, calling AG_ERNO_MSG(AG_ERNO_MBLOCK) will lead to the generation of
 * the captioned error string "AG_ERNO_MBLOCK: failed to allocate memory block".
 */
#define AG_ERNO_MSG(p) #p ": " p##_MSG


#ifdef __cplusplus
        }
#endif


#endif /* !__ARGENT_INCLUDE_ERNO_H__ */

