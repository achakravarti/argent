/*-
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Argent - infrastructure for building web services
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
 */


#ifndef __AG_HTTP_METHOD_METATESTS_H__
#define __AG_HTTP_METHOD_METATESTS_H__


/*
 * Define the AG_METATEST_HTTP_METHOD_PARSE() macro. This macro is used to
 * metaprogrammatically generate the test case to test whether calling
 * ag_http_method_parse() with a given string results in the expected
 * ag_http_method enumerator.
 */
#define AG_METATEST_HTTP_METHOD_PARSE(str, enm)                         \
        AG_TEST_CASE("ag_http_method_parse(): " str " => " #enm)        \
`       {                                                               \
                AG_TEST (ag_http_method_parse(str) == enm);             \
        }


/*
 * Define the AG_METATEST_HTTP_METHOD_STR() macro. This macro is used to
 * metaprogrammatically generate the test case to check whether calling the
 * ag_http_method_str() interface function with a given ag_http_method
 * enumerator results in the expected string representation.
 *
 * Since the expected string representation is simply the suffix of the
 * enumerator (i.e., the token after the last underscore in the enumerator
 * symbol), we only require for the suffix to be passed to this macro.
 */
#define AG_METATEST_HTTP_METHOD_STR(sfx)                                       \
        AG_TEST_CASE("ag_http_method_str(): AG_HTTP_METHOD_" #sfx " => " #sfx) \
        {                                                                      \
                AG_AUTO(ag_string) *s;                                         \
                s = ag_http_method_str(AG_HTTP_METHOD_ ## sfx);                \
                AG_TEST (ag_string_eq(s, "AG_HTTP_METHOD_" #sfx));             \
        }


#endif /* !__AG_HTTP_METHOD_METATESTS_H__ */

