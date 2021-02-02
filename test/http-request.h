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


#ifndef __ARGENT_TEST_HTTP_REQUEST_H__
#define __ARGENT_TEST_HTTP_REQUEST_H__


/*
 * Define the AG_SAMPLE_HTTP_REQUEST() macro. This macro is responsible for
 * metaprogrammatically generating a static inline function that returns a
 * pointer to a sample HTTP request object with a given set of properties.
 *
 * The first parameter to this macro is a tag used to uniquely identify a given
 * sample HTTP client object, and the other parameters are, in sequence, those
 * of ag_http_request_new(). The name of the generated function is the same as
 * the specified tag.
 */
#define AG_SAMPLE_HTTP_REQUEST(tag, method, mime, url, client, param)   \
        static inline ag_http_request *tag(void)                        \
        {                                                               \
                AG_AUTO(ag_http_url) *u = url;                          \
                AG_AUTO(ag_http_client) *c = client;                    \
                AG_AUTO(ag_alist) *p = param;                           \
                return ag_http_request_new(method, mime, u, c, p);      \
        }



/*
 * Define the AG_METATEST_HTTP_REQUEST_MIME() macro. This macro is used to
 * metaprogrammatically generate the test case for the ag_http_request_mime()
 * interface function. The first parameter is a pointer to the sample HTTP
 * request object, and the second parameter is the expected return value of
 * ag_http_request_mime().
 */
#define AG_METATEST_HTTP_REQUEST_MIME(sample, expect)                   \
        AG_TEST_CASE("ag_http_request_mime(): " #sample " => " #expect) \
        {                                                               \
                AG_AUTO(ag_http_request) *r = sample;                   \
                AG_TEST (ag_http_request_mime(r) == expect);            \
        }


/*
 * Define the AG_METATEST_HTTP_REQUEST_METHOD() macro. This macro is used to
 * metaprogrammatically generate the test case for the ag_http_request_method()
 * interface function. The first parameter is a pointer to the sample HTTP
 * request object, and the second parameter is the expected return value of
 * ag_http_request_method().
 */
#define AG_METATEST_HTTP_REQUEST_METHOD(sample, expect)                        \
        AG_TEST_CASE("ag_http_request_method(): " #sample " => " #expect)      \
        {                                                                      \
                AG_AUTO(ag_http_request) *r = sample;                          \
                AG_TEST (ag_http_request_method(r) == expect);                 \
        }


/*
 * Define the AG_METATEST_HTTP_REQUEST_CLIENT() macro. This macro is used to
 * metaprogrammatically generate the test case for the ag_http_request_client()
 * interface function. The first parameter is a pointer to the sample HTTP
 * request object, and the second parameter is the expected return value of
 * ag_http_reequest_client().
 */
#define AG_METATEST_HTTP_REQUEST_CLIENT(sample, expect)                        \
        AG_TEST_CASE("ag_http_request_client(): " #sample " => " #expect)      \
        {                                                                      \
                AG_AUTO(ag_http_request) *r = sample;                          \
                AG_AUTO(ag_http_client) *c = expect;                           \
                AG_AUTO(ag_http_client) *c2 = ag_http_request_client(r);       \
                AG_TEST (ag_http_client_eq(c, c2));                            \
        }


/*
 * Define the AG_METATEST_HTTP_REQUEST_URL() macro. This macro is used to
 * metaprogrammatically generate the test case for the ag_http_request_url()
 * interface function. The first parameter is a pointer to the sample HTTP
 * request object, and the second parameter is the expected return value of
 * ag_http_request_url().
 */
#define AG_METATEST_HTTP_REQUEST_URL(sample, expect)                    \
        AG_TEST_CASE("ag_http_request_url(): " #sample " => " #expect)  \
        {                                                               \
                AG_AUTO(ag_http_request) *r = sample;                   \
                AG_AUTO(ag_http_url) *u = expect;                       \
                AG_AUTO(ag_http_url) *u2 = ag_http_request_url(r);      \
                AG_TEST (ag_http_url_eq(u, u2));                        \
        }


/*
 * Define the AG_METATEST_HTTP_REQUEST_PARAM() macro. This macro is used to
 * metaprogrammatically generate the test case for the ag_http_request_param()
 * interface function. The first parameter is a pointer to the sample HTTP
 * request object, and the second parameter is the expected return value of
 * ag_http_request_param().
 */
#define AG_METATEST_HTTP_REQUEST_PARAM(sample, expect)                         \
        AG_TEST_CASE("ag_http_request_param(): " #sample " => " #expect)       \
        {                                                                      \
                AG_AUTO(ag_http_request) *r = sample;                          \
                AG_AUTO(ag_alist) *p = expect;                                 \
                AG_AUTO(ag_alist) *p2 = ag_http_request_param(r);              \
                AG_TEST (ag_alist_eq(p, p2));                                  \
        }


#endif /* !__AG_HTTP_REQUEST_H__ */
