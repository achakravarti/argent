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


/**
 * For testing purposes, we need to work with sample objects. The following four 
 * macros help metaprogrammatically generate functions that return pointers to
 * sample objects:
 *   - AG_SAMPLE_HTTP_CLIENT()        : pointer to sample HTTP client
 *   - AG_SAMPLE_HTTP_REQUEST()       : pointer to sample HTTP request
 *   - AG_SAMPLE_HTTP_RESPONSE()      : pointer to sample HTTP response
 *   - AG_SAMPLE_HTTP_URL()           : pointer to sample HTTP URL object
 *
 * Of these, the AG_SAMPLE_HTTP_RESPONSE() macro is special in that it provides
 * two additional overloaded forms:
 *   - AG_SAMPLE_HTTP_RESPONSE_EMPTY()
 *   - AG_SAMPLE_HTTP_RESPONSE_FILE()
 *
 * Each of these macros takes a tag as its first parameter. The tag is used as
 * the name of the generated function, and helps uniquely identify the generated
 * sample object. The remaining parameters are the same as the corresponding
 * constructors:
 *   - AG_SAMPLE_HTTP_CLIENT()        : ag_http_client_new()
 *   - AG_SAMPLE_HTTP_REQUEST()       : ag_http_request_new()
 *   - AG_SAMPLE_HTTP_RESPONSE()      : ag_http_response_new()
 *   - AG_SAMPLE_HTTP_RESPONSE_EMPTY(): ag_http_response_new_empty()
 *   - AG_SAMPLE_HTTP_RESPONSE_FILE() : ag_http_response_new_file()
 *   - AG_SAMPLE_HTTP_URL()           : ag_http_url_new()
 *
 * See the following files for more details:
 *   - test/http-client.c
 *   - test/http-request.c
 *   - test/http-response.c
 *   - test/http-url.c
 **/

#define AG_SAMPLE_HTTP_CLIENT(tag, ip, port, host, agent, referer)             \
        static inline ag_http_client *tag(void)                                \
        {                                                                      \
                return ag_http_client_new(ip, port, host, agent, referer);     \
        }

#define AG_SAMPLE_HTTP_REQUEST(tag, method, mime, url, client, param)   \
        static inline ag_http_request *tag(void)                        \
        {                                                               \
                AG_AUTO(ag_http_url) *u = url;                          \
                AG_AUTO(ag_http_client) *c = client;                    \
                AG_AUTO(ag_alist) *p = param;                           \
                return ag_http_request_new(method, mime, u, c, p);      \
        }

#define AG_SAMPLE_HTTP_RESPONSE(tag, mime, status, body)                \
        static inline ag_http_response *tag(void)                       \
        {                                                               \
                return ag_http_response_new(mime, status, body);        \
        }

#define AG_SAMPLE_HTTP_RESPONSE_EMPTY(tag, mime, status)                \
        static inline ag_http_response *tag(void)                       \
        {                                                               \
                return ag_http_response_new_empty(mime, status);        \
        }

#define AG_SAMPLE_HTTP_RESPONSE_FILE(tag, mime, status, path)           \
        static inline ag_http_response *tag(void)                       \
        {                                                               \
                return ag_http_response_new_file(mime, status, path);   \
        }


#define AG_SAMPLE_HTTP_URL(tag, secure, host, port, path)               \
        static inline ag_http_url *tag(void)                            \
        {                                                               \
                return port ? ag_http_url_new(secure, host, port, path) \
                    : ag_http_url_new_noport(secure, host, path);       \
        }


/**
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
 *
 * See the following files for more details:
 *   - include/http.h
 *   - test/http-enum.c
 **/

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


/**
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
 *
 * See the following files for more details:
 *   - include/http.h
 *   - test/http-enum.c
 **/

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


/**
 * There are two macros provided by the ag_http_status testing interface:
 *   1. AG_METATEST_HTTP_STATUS_PARSE()
 *   2. AG_METATEST_HTTP_STATUS_STR()
 *
 * The first macro generates a test case to determine whether the interface
 * function ag_http_status_parse() returns the expected ag_http_status
 * enumerator for a given sample string. The sample string is passed through the
 * first parameter, and the expected enumerator through the second.
 *
 * The second macro generates a test case that checks the corollary. It tests
 * whether ag_http_status_str() returns the expected string representation of a
 * given sample ag_http_status enumerator. The sample is passed through the
 * first parameter, and the expected return value through the second parameter.
 **/

#define AG_METATEST_HTTP_STATUS_PARSE(sample, expect)                   \
        AG_TEST_CASE("ag_http_status_parse(): " sample " => " #expect)  \
        {                                                               \
                AG_TEST (ag_http_status_parse(sample) == expect);       \
        }

#define AG_METATEST_HTTP_STATUS_STR(sample, expect)                                       \
        AG_TEST_CASE("ag_http_status_str(): " #sample " => " expect)    \
        {                                                               \
                AG_AUTO(ag_string) *s = ag_http_status_str(sample);     \
                AG_TEST (ag_string_eq(s, expect));                      \
        }


/**
 * The ag_http_client testing interface provides five macros for generating
 * metaprogrammatically the test cases for its non-inherited accessor functions:
 *   - AG_METATEST_HTTP_CLIENT_IP()     : ag_http_client_ip()
 *   - AG_METATEST_HTTP_CLIENT_HOST()   : ag_http_clienmt_host()
 *   - AG_METATEST_HTTP_CLIENT_AGENT()  : ag_http_client_agent()
 *   - AG_METATEST_HTTP_CLIENT_REFERER(): ag_http_client_referer()
 *   - AG_METATEST_HTTP_CLIENT_PORT()   : ag_http_client_port()
 *
 * The first parameter for each of these macros is the sample HTTP client object
 * generated by the AG_SAMPLE_HTTP_CLIENT() macro, and the second parameter is
 * the expected return value from the accessor function being tested.
 *
 * The __AG_METATEST_HTTP_CLIENT_STR__() helper macro is used to generate
 * metaprogrammatically the metatest macros for the accessor functions that
 * return a string value, and is **not** part of the ag_http_client testing
 * interface.
 *
 * See the following files for more details:
 *   - include/http.h
 *   - test/http-client.c
 **/

#define __AG_METATEST_HTTP_CLIENT_STR__(sfx, sample, expect)                   \
        AG_TEST_CASE("ag_http_client_" #sfx "(): " #sample " => " expect)      \
        {                                                                      \
                AG_AUTO(ag_http_client) *c = sample;                           \
                AG_AUTO(ag_string) *s = ag_http_client_ ## sfx(c);             \
                AG_TEST (ag_string_eq(s, expect));                             \
        }

#define AG_METATEST_HTTP_CLIENT_IP(sample, expect)              \
        __AG_METATEST_HTTP_CLIENT_STR__(ip, sample, expect)
#define AG_METATEST_HTTP_CLIENT_HOST(sample, expect)            \
        __AG_METATEST_HTTP_CLIENT_STR__(host, sample, expect)
#define AG_METATEST_HTTP_CLIENT_AGENT(sample, expect)           \
        __AG_METATEST_HTTP_CLIENT_STR__(agent, sample, expect)
#define AG_METATEST_HTTP_CLIENT_REFERER(sample, expect)         \
        __AG_METATEST_HTTP_CLIENT_STR__(referer, sample, expect)

#define AG_METATEST_HTTP_CLIENT_PORT(sample, expect)                    \
        AG_TEST_CASE("ag_http_url_client(): " #sample " => " #expect)   \
        {                                                               \
                AG_AUTO(ag_http_client) *c = sample;                    \
                AG_TEST (ag_http_client_port(c) == expect);             \
        }


/**
 * There are five macros available for metaprogrammatically generating the test
 * cases of the non-inherited functions of the ag_http_url interface:
 *   - AG_METATEST_HTTP_URL_PARSE() : ag_http_url_parse()
 *   - AG_METATEST_HTTP_URL_HOST()  : ag_http_url_host()
 *   - AG_METATEST_HTTP_URL_SECURE(); ag_http_url_secure()
 *   - AG_METATEST_HTTP_URL_PATH()  : ag_http_url_path()
 *   - AG_METATEST_HTTP_URL_PORT()  : ag_http_url_port()
 *
 * Each of these macros generates a test case that checks whether the
 * corresponding interface function returns the expected value for a given
 * sample. The first parameter for each of these macros is the sample, and the
 * second parameter is the expected return value.
 *
 * In the case of AG_METATEST_HTTP_URL_PARSE(), the sample is a statically
 * allocated const string, whereas for the others the sample is the function
 * generated by the AG_SAMPLE_HTTP_URL() macro.
 *
 * See the following files for more details:
 *   - include/http.h
 *   - test/http-url.c
 **/

#define AG_METATEST_HTTP_URL_PARSE(src, expect)                         \
        AG_TEST_CASE("ag_http_url_parse(): " src " => " #expect)        \
        {                                                               \
                AG_AUTO(ag_http_url) *u = ag_http_url_parse(src);       \
                AG_AUTO(ag_http_url) *e = expect;                       \
                AG_TEST (ag_http_url_eq(u, e));                         \
        }

#define AG_METATEST_HTTP_URL_HOST(sample, expect)                       \
        AG_TEST_CASE("ag_http_url_host(): " #sample " => " expect)      \
        {                                                               \
                AG_AUTO(ag_http_url) *u = sample;                       \
                AG_AUTO(ag_string) *h = ag_http_url_host(u);            \
                AG_TEST (ag_string_eq(h, expect));                      \
        }

#define AG_METATEST_HTTP_URL_SECURE(sample, expect)                     \
        AG_TEST_CASE("ag_http_url_secure(): " #sample " => " #expect)   \
        {                                                               \
                AG_AUTO(ag_http_url) *u = sample;                       \
                AG_TEST (ag_http_url_secure(u) == expect);              \
        }

#define AG_METATEST_HTTP_URL_PORT(sample, expect)                       \
        AG_TEST_CASE("ag_http_url_port(): " #sample " => " #expect)     \
        {                                                               \
                AG_AUTO(ag_http_url) *u = sample;                       \
                AG_TEST (ag_http_url_port(u) == expect);                \
        }

#define AG_METATEST_HTTP_URL_PATH(sample, expect)                       \
        AG_TEST_CASE("ag_http_url_path(): " #sample " => " expect)      \
        {                                                               \
                AG_AUTO(ag_http_url) *u = sample;                       \
                AG_AUTO(ag_string) *p = ag_http_url_path(u);            \
                AG_TEST (ag_string_eq(p, expect));                      \
        }


/**
 * The ag_http_request testing interface provides five macros for generating
 * metaprogrammatically the test cases that check whether their corresponding
 * ag_http_request interface accessor functions return the expected result for a
 * given sample:
 *   - AG_METATEST_HTTP_REQUEST_MIME()  : ag_http_request_mime()
 *   - AG_METATEST_HTTP_REQUEST_METHOD(): ag_http_request_method()
 *   - AG_METATEST_HTTP_REQUEST_CLIENT(): ag_http_request_client()
 *   - AG_METATEST_HTTP_REQUEST_URL()   : ag_http_request_url()
 *   - AG_METATEST_HTTP_REQUEST_PARAM() : ag_http_request_param()
 *
 * The argument to the first parameter for each of theses functions is the
 * function generated by the AG_SAMPLE_HTTP_REQUEST() macro, and the expected
 * return value is passed through the second parameter. Note that if the
 * expected return value is an object, then the argument to the second parameter
 * is automatically released.
 **/

#define AG_METATEST_HTTP_REQUEST_MIME(sample, expect)                   \
        AG_TEST_CASE("ag_http_request_mime(): " #sample " => " #expect) \
        {                                                               \
                AG_AUTO(ag_http_request) *r = sample;                   \
                AG_TEST (ag_http_request_mime(r) == expect);            \
        }

#define AG_METATEST_HTTP_REQUEST_METHOD(sample, expect)                        \
        AG_TEST_CASE("ag_http_request_method(): " #sample " => " #expect)      \
        {                                                                      \
                AG_AUTO(ag_http_request) *r = sample;                          \
                AG_TEST (ag_http_request_method(r) == expect);                 \
        }

#define AG_METATEST_HTTP_REQUEST_CLIENT(sample, expect)                        \
        AG_TEST_CASE("ag_http_request_client(): " #sample " => " #expect)      \
        {                                                                      \
                AG_AUTO(ag_http_request) *r = sample;                          \
                AG_AUTO(ag_http_client) *c = expect;                           \
                AG_AUTO(ag_http_client) *c2 = ag_http_request_client(r);       \
                AG_TEST (ag_http_client_eq(c, c2));                            \
        }

#define AG_METATEST_HTTP_REQUEST_URL(sample, expect)                    \
        AG_TEST_CASE("ag_http_request_url(): " #sample " => " #expect)  \
        {                                                               \
                AG_AUTO(ag_http_request) *r = sample;                   \
                AG_AUTO(ag_http_url) *u = expect;                       \
                AG_AUTO(ag_http_url) *u2 = ag_http_request_url(r);      \
                AG_TEST (ag_http_url_eq(u, u2));                        \
        }

#define AG_METATEST_HTTP_REQUEST_PARAM(sample, expect)                         \
        AG_TEST_CASE("ag_http_request_param(): " #sample " => " #expect)       \
        {                                                                      \
                AG_AUTO(ag_http_request) *r = sample;                          \
                AG_AUTO(ag_alist) *p = expect;                                 \
                AG_AUTO(ag_alist) *p2 = ag_http_request_param(r);              \
                AG_TEST (ag_alist_eq(p, p2));                                  \
        }


#define AG_METATEST_HTTP_RESPONSE_HEADER(sample, expect)                       \
        AG_TEST_CASE("ag_http_response_header(): " #sample " => " expect)      \
        {                                                                      \
                AG_AUTO(ag_http_response) *r = sample;                         \
                AG_AUTO(ag_string) *h = ag_http_response_header(r);            \
                AG_TEST (ag_string_eq(h, expect));                             \
        }

#define AG_METATEST_HTTP_RESPONSE_BODY(sample, expect)                  \
        AG_TEST_CASE("ag_http_response_body(): " #sample " => " expect) \
        {                                                               \
                AG_AUTO(ag_http_response) *r = sample;                  \
                AG_AUTO(ag_string) *b = ag_http_response_body(r);       \
                AG_TEST (ag_string_eq(b, expect));                      \
        }


#define AG_METATEST_HTTP_RESPONSE_ADD(sample, add, expect)                     \
        AG_TEST_CASE("ag_http_response_add(): " #sample " + " add " => "       \
            expect)                                                            \
        {                                                                      \
                AG_AUTO(ag_http_response) *r = sample;                         \
                ag_http_response_add(&r, add);                                 \
                AG_AUTO(ag_string) *b = ag_http_response_body(r);              \
                AG_TEST (ag_string_eq(b, expect));                             \
        }


#endif /* !__ARGENT_TEST_HTTP_H__ */

