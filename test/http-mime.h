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


#ifndef __ARGENT_TEST_HTTP_MIME_H__
#define __ARGENT_TEST__HTTP_MIME_H__


/*
 * Define the AG_METATEST_HTTP_MIME_PARSE() macro. This macro is used to
 * metaprogrammatically generate the test case to test whether calling
 * ag_http_mime_parse() with a given string results in the expected ag_http_mime
 * enumerator.
 */
#define AG_METATEST_HTTP_MIME_PARSE(str, enm)                   \
        AG_TEST_CASE("ag_http_mime_parse(): " str " => " #enm)  \
        {                                                       \
                AG_TEST (ag_http_mime_parse(str) == enm);       \
        }


/*
 * Define the AG_METATEST_HTTP_MIME_STR() macro. This macro is used to generate
 * metaprogrammatically a test case that checks whether ag_http_mime_str()
 * returns the correct string representation of a given ag_http_mime enumerator.
 * 
 * The first parameter of this macro is the ag_http_mime enumerator to test, and
 * the second parameter is the statically allocated string that holds the
 * expected string representation.
 */
#define AG_METATEST_HTTP_MIME_STR(enm, str)                     \
        AG_TEST_CASE("ag_http_mime_str(): " #enm " => " str)    \
        {                                                       \
                AG_AUTO(ag_string) *s = ag_http_mime_str(enm);  \
                AG_TEST (ag_string_eq(s, str));                 \
        }


#endif /* !__ARGENT_TEST_HTTP_MIME_H__ */

