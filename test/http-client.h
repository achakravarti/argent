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


#ifndef __ARGENT_TEST_HTTP_CLIENT_H__
#define __ARGENT_TEST_HTTP_CLIENT_H__


/*
 * Define the AG_SAMPLE_HTTP_CLIENT() macro. This macro is responsible for
 * metaprogrammatically generating a static inline function that returns a
 * pointer to a sample HTTP client object with a given set of properties.
 *
 * The first parameter to this macro is a tag used to uniquely identify a given
 * sample HTTP client object, and the other parameters are, in sequence, those
 * of ag_http_url_new(). The name of the generated function is the same as the
 * specified tag.
 */
#define AG_SAMPLE_HTTP_CLIENT(tag, ip, port, host, agent, referer)             \
        static inline ag_http_client *tag(void)                                \
        {                                                                      \
                return ag_http_client_new(ip, port, host, agent, referer);     \
        }


/*
 * Define the __AG_METATEST_HTTP_CLIENT_STR__() helper macro. This macro is used
 * to metaprogrammatically generate the metatests for the non-inherited string
 * property accessor functions of the ag_http_client interface.
 *
 * The first parameter is the suffix of the interface function name, the second
 * parameter is a pointer to the sample client object, and the third parameter
 * is a statically allocated string representing the expected result.
 */
#define __AG_METATEST_HTTP_CLIENT_STR__(sfx, sample, expect)                   \
        AG_TEST_CASE("ag_http_client_" #sfx "(): " #sample " => " expect)      \
        {                                                                      \
                AG_AUTO(ag_http_client) *c = sample;                           \
                AG_AUTO(ag_string) *s = ag_http_client_ ## sfx(c);             \
                AG_TEST (ag_string_eq(s, expect));                             \
        }


/*
 * Define the metatests for the non-inherited functions of the ag_http_client
 * interface that return a string property. Each of these metatests take a
 * pointer to the sample client object as the first parameter and a statically
 * allocated string representing the expected result as the second parameter.
 *
 * These metatests are themselves metaprogrammatically generated through the
 * __AG_METATEST_HTTP_CLIENT_STR__() helper macro. The metatests and their
 * corresponding interface functions which they test are as follows:
 *   - AG_METATEST_HTTP_CLIENT_IP()     : ag_http_client_ip()
 *   - AG_METATEST_HTTP_CLIENT_HOST()   : ag_http_client_host()
 *   - AG_METATEST_HTTP_CLIENT_AGENT()  : ag_http_client_agent()
 *   - AG_METATEST_HTTP_CLIENT_REFERER(): ag_http_client_referer()
 */
#define AG_METATEST_HTTP_CLIENT_IP(sample, expect)              \
        __AG_METATEST_HTTP_CLIENT_STR__(ip, sample, expect)
#define AG_METATEST_HTTP_CLIENT_HOST(sample, expect)            \
        __AG_METATEST_HTTP_CLIENT_STR__(host, sample, expect)
#define AG_METATEST_HTTP_CLIENT_AGENT(sample, expect)           \
        __AG_METATEST_HTTP_CLIENT_STR__(agent, sample, expect)
#define AG_METATEST_HTTP_CLIENT_REFERER(sample, expect)         \
        __AG_METATEST_HTTP_CLIENT_STR__(referer, sample, expect)


/*
 * Define the AG_METATEST_HTTP_CLIENT_PORT() macro. This macro is used to
 * generate metaprogrammatically the test case for the ag_http_client_port()
 * interface function. The first parameter is a pointer to the sample HTTP
 * client object, and the second parameter is the expected unsigned integer
 * result.
 */
#define AG_METATEST_HTTP_CLIENT_PORT(sample, expect)                    \
        AG_TEST_CASE("ag_http_url_client(): " #sample " => " #expect)   \
        {                                                               \
                AG_AUTO(ag_http_client) *c = sample;                    \
                AG_TEST (ag_http_client_port(c) == expect);             \
        }


#endif /* !__ARGENT_TEST_HTTP_CLIENT_H__ */

