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


#include "./field.h"
#include "./http-client.h"
#include "./http-url.h"
#include "./object.h"
#include "./test.h"
#include "./value.h"


/*
 * Define the __AG_TEST_SUITE_ID__ symbolic constant. This constant represents
 * the unique ID of the HTTP request interface test suite, and is required by
 * the testing interface to metaprogrammatically generate the relevant testing
 * functions.
 */
#define __AG_TEST_SUITE_ID__ 12


#define SAMPLE_REQUEST(tag, method, mime, url, client, param)           \
        static inline ag_http_request *REQUEST_ ## tag(void)            \
        {                                                               \
                AG_AUTO(ag_http_url) *u = url;                          \
                AG_AUTO(ag_http_client) *c = client;                    \
                AG_AUTO(ag_alist) *p = param;                           \
                return ag_http_request_new(method, mime, u, c, p);      \
        }                                                               \
        static inline ag_hash REQUEST_ ## tag ## _HASH(void)            \
        {                                                               \
                AG_AUTO(ag_http_request) *r = REQUEST_ ## tag();        \
                AG_AUTO(ag_string) *s = ag_http_request_str(r);         \
                return ag_hash_new_str(s);                              \
        }                                                               \
        static inline size_t REQUEST_ ## tag ## _SZ(void)               \
        {                                                               \
                AG_AUTO(ag_http_request) *r = REQUEST_ ## tag();        \
                return ag_http_request_sz(r);                           \
        }                                                               \


AG_SAMPLE_HTTP_CLIENT(CLIENT_0, "", 0, "", "", "");
AG_SAMPLE_HTTP_CLIENT(CLIENT_1, "192.168.0.1", 0, "host.com", "mozilla",
    "google.com");
AG_SAMPLE_HTTP_CLIENT(CLIENT_2, "192.168.1.1", 40, "domain.com", "webkit", "");


AG_SAMPLE_HTTP_URL(HTTP_LOCALHOST_8080, false, "127.0.0.1", 8080, "");
AG_SAMPLE_HTTP_URL(HTTPS_LOCALHOST_8080, true, "127.0.0.1", 8080, "");
AG_SAMPLE_HTTP_URL(HTTP_LOCALHOST_8080_FOO, false, "localhost", 8080, "foo");
//SAMPLE_URL(HTTPS_LOCALHOST_8080_FOO, true, "localhost", 8080, "foo");
//SAMPLE_URL(HTTPS_DOMAIN_FOO, true, "www.domain.com", 0, "/foo");
//SAMPLE_URL(HTTPS_DOMAIN_FOO_BAR, true, "www.domain.com", 0, "foo/bar");


/*
 * Define a few sample string values for generating sample fields through the
 * AG_SAMPLE_VALUE_STRING() macro. The following functions returning a pointer
 * to a string value are generated:
 *   - VALUE_KEY()  : string value set to "key"
 *   - VALUE_VAL()  : string value set to "val"
 *   - VALUE_FOO()  : string value set to "foo"
 *   - VALUE_EMPTY(): empty string value
 */
AG_SAMPLE_VALUE_STRING(VALUE_KEY, "key");
AG_SAMPLE_VALUE_STRING(VALUE_VAL, "val");
AG_SAMPLE_VALUE_STRING(VALUE_FOO, "foo");
AG_SAMPLE_VALUE_STRING(VALUE_BAR, "bar");
AG_SAMPLE_VALUE_STRING(VALUE_EMPTY, "");


/*
 * Define a few sample fields containing string key-value pairs for preparing
 * the sample parameter lists. We use the AG_SAMPLE_FIELD() macro in conjunction
 * with the sample string value functions generated above by the macro
 * AG_SAMPLE_VALUE_STRING(). The sample field functions generated are:
 *   - SAMPLE_FIELD_KEYVAL(): (key:val)
 *   - SAMPLE_FIELD_FOOBAR(): (foo:bar)
 *   - SAMPLE_FIELD_FOO()   : (foo:)
 */
AG_SAMPLE_FIELD(KEYVAL, VALUE_KEY(), VALUE_VAL());
AG_SAMPLE_FIELD(FOOBAR, VALUE_FOO(), VALUE_BAR());
AG_SAMPLE_FIELD(FOO, VALUE_FOO(), VALUE_EMPTY());


static inline ag_list *param_empty(void)
{
        return ag_alist_new_empty();
}


static inline ag_list *param_single(void)
{
        AG_AUTO(ag_field) *f = SAMPLE_FIELD_KEYVAL();
        return ag_alist_new(f);
}


static ag_list *param_array(void)
{
        AG_AUTO(ag_field) *f1 = SAMPLE_FIELD_KEYVAL();
        AG_AUTO(ag_field) *f2 = SAMPLE_FIELD_FOOBAR();
        AG_AUTO(ag_field) *f3 = SAMPLE_FIELD_FOO();

        const ag_field *f[] = {f1, f2, f3};
        return ag_alist_new_array(f, 3);
}


/*
 * Define the sample HTTP request objects to be used for testing. We use the
 * SAMPLE_REQUEST() macro to generate the supporting functions for these sample
 * objects. The following functions are metaprogrammatically generated to return
 * the sample objects:
 *   - REQUEST_GET0(): GET text/html http://127.0.0.1:8080 no params
 *   - REQUEST_GET1(): GET text/plain https://127.0.0.1:8080 single param
 *   - REQUEST_GET2(): GET text/css https://localhost:8080/foo multiple params
 *
 * These sample HTTP request objects are not meant to be realistic; their
 * properties have been selected randomly. Each of these sample HTTP request
 * objects also has a corresponding function metaprogrammatically generated to
 * determine its size and hash.
 */
SAMPLE_REQUEST(GET0, AG_HTTP_METHOD_GET, AG_HTTP_MIME_TEXT_HTML,
    HTTP_LOCALHOST_8080(), CLIENT_0(), param_empty());
SAMPLE_REQUEST(GET1, AG_HTTP_METHOD_GET, AG_HTTP_MIME_TEXT_PLAIN,
    HTTPS_LOCALHOST_8080(), CLIENT_1(), param_single());
SAMPLE_REQUEST(GET2, AG_HTTP_METHOD_GET, AG_HTTP_MIME_TEXT_CSS,
    HTTP_LOCALHOST_8080_FOO(), CLIENT_2(), param_array());


/*
 * Run the ag_object_copy() metatest for ag_http_request_copy() with the sample
 * HTTP request objects defined above.
 */
AG_METATEST_OBJECT_COPY(ag_http_request, REQUEST_GET0());
AG_METATEST_OBJECT_COPY(ag_http_request, REQUEST_GET1());
AG_METATEST_OBJECT_COPY(ag_http_request, REQUEST_GET2());


/*
 * Run the ag_object_clone() metatest for ag_http_request_clone() with the
 * sample HTTP request objects defined above.
 */
AG_METATEST_OBJECT_CLONE(ag_http_request, REQUEST_GET0());
AG_METATEST_OBJECT_CLONE(ag_http_request, REQUEST_GET1());
AG_METATEST_OBJECT_CLONE(ag_http_request, REQUEST_GET2());


/*
 * Run the ag_object_release() metatest for ag_http_request_release() with the
 * sample HTTP request objects defined above.
 */
AG_METATEST_OBJECT_RELEASE(ag_http_request, REQUEST_GET0());
AG_METATEST_OBJECT_RELEASE(ag_http_request, REQUEST_GET1());
AG_METATEST_OBJECT_RELEASE(ag_http_request, REQUEST_GET2());


/*
 * Run the ag_object_cmp() metatest for ag_http_request_cmp() with the sample
 * HTTP request objects defined above.
 *
 * TODO: Rethink implementation ag_http_request_cmp().
 */
AG_METATEST_OBJECT_CMP(ag_http_request, REQUEST_GET0(), REQUEST_GET1());
AG_METATEST_OBJECT_CMP(ag_http_request, REQUEST_GET2(), REQUEST_GET0());
AG_METATEST_OBJECT_CMP(ag_http_request, REQUEST_GET2(), REQUEST_GET1());


/*
 * Run the ag_object_lt() metatest for ag_http_request_lt() with the sample HTTP
 * request objects defined above.
 *
 * TODO: Rethink implementation ag_http_request_lt().
 */
AG_METATEST_OBJECT_LT(ag_http_request, REQUEST_GET0(), REQUEST_GET1());
AG_METATEST_OBJECT_LT(ag_http_request, REQUEST_GET2(), REQUEST_GET0());
AG_METATEST_OBJECT_LT(ag_http_request, REQUEST_GET2(), REQUEST_GET1());


/*
 * Run the ag_object_gt() metatest for ag_http_request_gt() with the sample HTTP
 * request objects defined above.
 *
 * TODO: Rethink implementation ag_http_request_gt().
 */
AG_METATEST_OBJECT_GT(ag_http_request, REQUEST_GET0(), REQUEST_GET1());
AG_METATEST_OBJECT_GT(ag_http_request, REQUEST_GET2(), REQUEST_GET0());
AG_METATEST_OBJECT_GT(ag_http_request, REQUEST_GET2(), REQUEST_GET1());


/*
 * Run the ag_object_eq() metatest for ag_http_request_eq() with the sample HTTP
 * request objects defined above.
 *
 * TODO: Rethink implementation ag_http_request_eq().
 */
AG_METATEST_OBJECT_EQ(ag_http_request, REQUEST_GET0(), REQUEST_GET1());
AG_METATEST_OBJECT_EQ(ag_http_request, REQUEST_GET2(), REQUEST_GET0());
AG_METATEST_OBJECT_EQ(ag_http_request, REQUEST_GET2(), REQUEST_GET1());


/*
 * Run the ag_object_empty() metatest for ag_http_request_empty() with the
 * sample HTTP request objects defined above. Since HTTP request objects can
 * never be empty, we run only AG_METATEST_OBJECT_EMPTY_NOT() and skip out on
 * AG_METATEST_OBJECT_EMPTY().
 */
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_request, REQUEST_GET0());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_request, REQUEST_GET1());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_request, REQUEST_GET2());


/*
 * Run the ag_object_valid() metatest for ag_http_request_valid() with the
 * sample HTTP request objects defined above. Since HTTP request objects are
 * guaranteed to be in a valid state when constructed through their interface,
 * we run only AG_METATEST_OBJECT_VALID() and skip out on
 * AG_METATEST_OBJECT_VALID_NOT().
 */
AG_METATEST_OBJECT_VALID(ag_http_request, REQUEST_GET0());
AG_METATEST_OBJECT_VALID(ag_http_request, REQUEST_GET1());
AG_METATEST_OBJECT_VALID(ag_http_request, REQUEST_GET2());


/* * Run the ag_object_typeid() metatest for ag_http_request_typeid() with the
 * sample HTTP request objects defined above.
 */
AG_METATEST_OBJECT_TYPEID(ag_http_request, REQUEST_GET0(),
    AG_TYPEID_HTTP_REQUEST);
AG_METATEST_OBJECT_TYPEID(ag_http_request, REQUEST_GET1(),
    AG_TYPEID_HTTP_REQUEST);
AG_METATEST_OBJECT_TYPEID(ag_http_request, REQUEST_GET2(),
    AG_TYPEID_HTTP_REQUEST);


/*
 * Run the ag_object_uuid() metatest for ag_http_request_uuid() with the sample
 * HTTP request objects defined above.
 */
AG_METATEST_OBJECT_UUID(ag_http_request, REQUEST_GET0());
AG_METATEST_OBJECT_UUID(ag_http_request, REQUEST_GET1());
AG_METATEST_OBJECT_UUID(ag_http_request, REQUEST_GET2());


/*
 * Run the ag_object_refc() metatest for ag_http_request_refc() with the sample
 * HTTP request objects defined above.
 */
AG_METATEST_OBJECT_REFC(ag_http_request, REQUEST_GET0());
AG_METATEST_OBJECT_REFC(ag_http_request, REQUEST_GET1());
AG_METATEST_OBJECT_REFC(ag_http_request, REQUEST_GET2());


/*
 * Run the ag_object_len() metatest for ag_http_request_len() with the sample
 * HTTP request objects defined above. The length of an HTTP request object is
 * always 1.
 */
AG_METATEST_OBJECT_LEN(ag_http_request, REQUEST_GET0(), 1);
AG_METATEST_OBJECT_LEN(ag_http_request, REQUEST_GET1(), 1);
AG_METATEST_OBJECT_LEN(ag_http_request, REQUEST_GET2(), 1);


/*
 * Run the ag_object_sz() metatest for ag_http_request_sz() with the sample HTTP
 * request objects defined above.
 */
AG_METATEST_OBJECT_SZ(ag_http_request, REQUEST_GET0(), REQUEST_GET0_SZ());
AG_METATEST_OBJECT_SZ(ag_http_request, REQUEST_GET1(), REQUEST_GET1_SZ());
AG_METATEST_OBJECT_SZ(ag_http_request, REQUEST_GET2(), REQUEST_GET2_SZ());


/*
 * Run the ag_object_hash() metatest for ag_http_request_hash() with the sample
 * HTTP request objects defined above.
 */
AG_METATEST_OBJECT_HASH(ag_http_request, REQUEST_GET0(), REQUEST_GET0_HASH());
AG_METATEST_OBJECT_HASH(ag_http_request, REQUEST_GET1(), REQUEST_GET1_HASH());
AG_METATEST_OBJECT_HASH(ag_http_request, REQUEST_GET2(), REQUEST_GET2_HASH());


/*
 * Run the ag_object_str() metatest for ag_http_request_str() with the sample
 * HTTP request objects defined above.
 */
AG_METATEST_OBJECT_STR(ag_http_request, REQUEST_GET0(),
    "[GET] text/html: url=http://127.0.0.1:8080/, client=[] host=, agent=, "
    "referer=, param=()");
AG_METATEST_OBJECT_STR(ag_http_request, REQUEST_GET1(),
    "[GET] text/plain: url=https://127.0.0.1:8080/, client=[192.168.0.1] "
    "host=host.com, agent=mozilla, referer=google.com, param=((key:val))");
AG_METATEST_OBJECT_STR(ag_http_request, REQUEST_GET2(),
    "[GET] text/css: url=http://localhost:8080/foo, client=[192.168.1.1:40] "
    "host=domain.com, agent=webkit, referer=, "
    "param=((key:val) (foo:bar) (foo:))");


/*
 * Define the test_suite_http_request() function. We generate the test cases
 * from the above metatest definitions through a call to
 * AG_TEST_SUITE_GENERATE().
 */
extern ag_test_suite *
test_suite_http_request(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_request interface");
}

