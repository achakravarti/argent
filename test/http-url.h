/*-
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
 */


#ifndef __ARGENT_TEST_HTTP_URL_H__
#define __ARGENT_TEST_HTTP_URL_H__


/*
 * Define the AG_SAMPLE_HTTP_URL() macro. This macro is responsible for
 * metaprogrammatically generating a static inline function that returns a
 * pointer to a sample HTTP URL object with a given set of properties.
 *
 * The first parameter to this macro is a tag used to uniquely identify a given
 * sample HTTP URL object, and the other parameters are, in sequence, those of
 * ag_http_url_new(). The name of the generated function is the same as the
 * specified tag.
 */
#define AG_SAMPLE_HTTP_URL(tag, secure, host, port, path)               \
        static inline ag_http_url *tag(void)                            \
        {                                                               \
                return port ? ag_http_url_new(secure, host, port, path) \
                    : ag_http_url_new_noport(secure, host, path);       \
        }


/*
 * Define the AG_METATEST_HTTP_URL_PARSE() macro. This macro defines the
 * metatest that is used to metaprogrammatically generate a test case for the
 * ag_http_url_parse() interface function. The first parameter is the statically
 * allocated string that is to be parsed, and the second parameter is the
 * pointer to the sample ag_http_url object instance that is expected after
 * parsing the first parameter.
 */
#define AG_METATEST_HTTP_URL_PARSE(src, expect)                         \
        AG_TEST_CASE("ag_http_url_parse(): " src " => " #expect)        \
        {                                                               \
                AG_AUTO(ag_http_url) *u = ag_http_url_parse(src);       \
                AG_AUTO(ag_http_url) *e = expect;                       \
                AG_TEST (ag_http_url_eq(u, e));                         \
        }


/*
 * Define the AG_METATEST_HTTP_URL_HOST() macro. This macro defines the metatest
 * that is used to metaprogrammatically generate a test case for the
 * ag_http_url_host() interface function. The first parameter is a pointer to
 * the sample HTTP URL object, and the second parameter is a statically
 * allocated string representing the result that is expected to be returned by
 * ag_http_url_host().
 */
#define AG_METATEST_HTTP_URL_HOST(sample, expect)                       \
        AG_TEST_CASE("ag_http_url_host(): " #sample " => " expect)      \
        {                                                               \
                AG_AUTO(ag_http_url) *u = sample;                       \
                AG_AUTO(ag_string) *h = ag_http_url_host(u);            \
                AG_TEST (ag_string_eq(h, expect));                      \
        }


/*
 * Define the AG_METATEST_HTTP_URL_SECURE() macro. This macro defines the
 * metatest that is used to metaprogrammatically generate a test case for the
 * ag_http_url_secure() interface function. The first parameter is a pointer to
 * the sample HTTP URL object, and second parameter is the Boolean value that is
 * expected to be returned by ag_http_url_secure().
 */
#define AG_METATEST_HTTP_URL_SECURE(sample, expect)                     \
        AG_TEST_CASE("ag_http_url_secure(): " #sample " => " #expect)   \
        {                                                               \
                AG_AUTO(ag_http_url) *u = sample;                       \
                AG_TEST (ag_http_url_secure(u) == expect);              \
        }


/*
 * Define the AG_METATEST_HTTP_URL_PORT() macro. This macro defines the metatest
 * that is used to metaprogrammatically generate a test case for the
 * ag_http_url_port() interface function. The first parameter is a pointer to
 * the sample HTTP URL object, and the second parameter represents the integer
 * result that is expected to be returned by ag_http_url_port().
 */
#define AG_METATEST_HTTP_URL_PORT(sample, expect)                       \
        AG_TEST_CASE("ag_http_url_port(): " #sample " => " #expect)     \
        {                                                               \
                AG_AUTO(ag_http_url) *u = sample;                       \
                AG_TEST (ag_http_url_port(u) == expect);                \
        }


/*
 * Define the AG_METATEST_HTTP_URL_PATH() macro. This macro defines the metatest
 * that is used to metaprogrammatically generate a test case for the
 * ag_http_url_path() interface function. The first parameter is a pointer to
 * the sample HTTP URL object, and the second parameter is a statically
 * allocated string which represents the return value that is expected from
 * ag_http_url_path().
 */
#define AG_METATEST_HTTP_URL_PATH(sample, expect)                       \
        AG_TEST_CASE("ag_http_url_path(): " #sample " => " expect)      \
        {                                                               \
                AG_AUTO(ag_http_url) *u = sample;                       \
                AG_AUTO(ag_string) *p = ag_http_url_path(u);            \
                AG_TEST (ag_string_eq(p, expect));                      \
        }


#endif /* !__AG_HTTP_REQUEST_H__ */

