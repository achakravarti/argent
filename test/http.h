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




#ifndef __ARGENT_TEST_HTTP_H__
#define __ARGENT_TEST_HTTP_H__




/*******************************************************************************
 * There are two metatest macros available for the ag_http_mime interface:
 *   1. AG_METATEST_HTTP_MIME_PARSE()
 *   2. AG_METATEST_HTTP_MIME_STR()
 *
 * The first metatest macro generates a test case to determine whether the
 * ag_http_mime_parse() interface function is able to correctly parse a given
 * string. The string to be parsed is passed through the first parameter, and
 * the expected ag_http_mime enumerator is passed through the second.
 *
 * The second metatest macro generates a test case to check whether the
 * ag_http_mime_str() interface function generates the expected string
 * representation of a given ag_http_mime enumerator. The first parameter is the
 * enumerator for which the string representation must be generated, and the
 * second parameter is the expected string representation.
 */


#define AG_METATEST_HTTP_MIME_PARSE(sample, expect)                     \
        AG_TEST_CASE("ag_http_mime_parse(): " sample " => " #expect)    \
        {                                                               \
                AG_TEST (ag_http_mime_parse(sample) == expect);         \
        }


#define AG_METATEST_HTTP_MIME_STR(sample, expect)                       \
        AG_TEST_CASE("ag_http_mime_str(): " #sample" => " expect)       \
        {                                                               \
                AG_AUTO(ag_string) *s = ag_http_mime_str(sample);       \
                AG_TEST (ag_string_eq(s, expect));                      \
        }




/*******************************************************************************
 * There are two metatest macros available for the ag_http_method interface:
 *   1. AG_METATEST_HTTP_METHOD_PARSE()
 *   2. AG_METATEST_HTTP_METHOD_STR()
 *
 * The first metatest macro generates a test case to check whether the
 * ag_http_method_parse() interface function is able to correctly parse a sample
 * string to the expected ag_http_method enumerator. The sample is passed
 * through the first parameter, and the expected value through the second.
 *
 * The second metatest macro generates a test case to check whether the
 * ag_http_method_str() interface function returns the expected string
 * representation of a given sample ag_http_method enumerator. The sample is
 * passed through the first parameter, and the expected string through the
 * second.
 */


#define AG_METATEST_HTTP_METHOD_PARSE(sample, expect)                   \
        AG_TEST_CASE("ag_http_method_parse(): " sample " => " #expect)  \
        {                                                               \
                AG_TEST (ag_http_method_parse(sample) == expect);       \
        }


#define AG_METATEST_HTTP_METHOD_STR(sample, expect)                                       \
        AG_TEST_CASE("ag_http_method_str(): " #sample " => " expect)    \
        {                                                               \
                AG_AUTO(ag_string) *s = ag_http_method_str(sample);     \
                AG_TEST (ag_string_eq(s, expect));                      \
        }


#endif /* !__ARGENT_TEST_HTTP_H__ */

