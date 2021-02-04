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


#include "./object.h"
#include "./http.h"
#include "./test.h"


/*
 * Define the __AG_TEST_SUITE_ID__ macro. This symbolic constant represents the
 * unique ID for the HTTP URL interface test suite, and is used by the testing
 * macros to metaprogrammatically generate the relevant testing functions.
 */
#define __AG_TEST_SUITE_ID__ 8


/*
 * Declare the prototypes of the helper functions. These functions are used in
 * conjunction with the object metatests for the HTTP URL sample objects
 * defined in this file. These sample objects are generated through the
 * AG_SAMPLE_HTTP_URL macro, and are passed as the first parameter to these
 * functions.
 *
 * sample_str() returns the string representation sample, sample_len() computes
 * the length of a sample, sample_sz() computes the size of a sample, and
 * sample_hash() computes the hash of a sample.
 */
static inline ag_string *sample_str(ag_http_url *);
static inline size_t     sample_len(ag_http_url *);
static inline size_t     sample_sz(ag_http_url *);
static inline ag_hash    sample_hash(ag_http_url *);


/*
 * Generate the function returning the sample HTTP URL objects. We use the macro
 * AG_SAMPLE_HTTP_URL() to do so, and are generating the following functions:
 *   - HTTP_LOCALHOST_8080()     : http://127.0.0.1:8080/
 *   - HTTPS_LOCALHOST_8080()    : https://127.0.0.1:8080/
 *   - HTTP_LOCALHOST_8080_FOO() : http://localhost:8080/foo
 *   - HTTPS_LOCALHOST_8080_FOO(): https://localhost:8080/foo
 *   - HTTPS_DOMAIN_FOO()        : https://www.domain.com/foo
 *   - HTTPS_DOMAIN_FOO_BAR()    : https://www.domain.com/foo/bar
 */
AG_SAMPLE_HTTP_URL(HTTP_LOCALHOST_8080, false, "127.0.0.1", 8080, "");
AG_SAMPLE_HTTP_URL(HTTPS_LOCALHOST_8080, true, "127.0.0.1", 8080, "");
AG_SAMPLE_HTTP_URL(HTTP_LOCALHOST_8080_FOO, false, "localhost", 8080, "foo");
AG_SAMPLE_HTTP_URL(HTTPS_LOCALHOST_8080_FOO, true, "localhost", 8080, "foo");
AG_SAMPLE_HTTP_URL(HTTPS_DOMAIN_FOO, true, "www.domain.com", 0, "/foo");
AG_SAMPLE_HTTP_URL(HTTPS_DOMAIN_FOO_BAR, true, "www.domain.com", 0, "foo/bar");


/*
 * Run the ag_object_copy() metatest for ag_http_url_copy() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_COPY(ag_http_url, HTTP_LOCALHOST_8080());
AG_METATEST_OBJECT_COPY(ag_http_url, HTTPS_LOCALHOST_8080());
AG_METATEST_OBJECT_COPY(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_COPY(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_COPY(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_COPY(ag_http_url, HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_clone() metatest for ag_http_url_clone() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_CLONE(ag_http_url, HTTP_LOCALHOST_8080());
AG_METATEST_OBJECT_CLONE(ag_http_url, HTTPS_LOCALHOST_8080());
AG_METATEST_OBJECT_CLONE(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_CLONE(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_CLONE(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_CLONE(ag_http_url, HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_copy() metatest for ag_http_url_copy() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTP_LOCALHOST_8080());
AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTPS_LOCALHOST_8080());
AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_cmp() metatest for ag_http_url_cmp() with the above sample
 * HTTP URL objects. The first argument is lexicographically less than the
 * second.
 */
AG_METATEST_OBJECT_CMP(ag_http_url, HTTP_LOCALHOST_8080(),
    HTTPS_LOCALHOST_8080());
AG_METATEST_OBJECT_CMP(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_CMP(ag_http_url, HTTPS_DOMAIN_FOO(), HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_lt() metatest for ag_http_url_lt() with the above sample
 * HTTP URL objects. The first argument is lexicographically less than the
 * second.
 */
AG_METATEST_OBJECT_LT(ag_http_url, HTTP_LOCALHOST_8080(),
    HTTPS_LOCALHOST_8080());
AG_METATEST_OBJECT_LT(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_LT(ag_http_url, HTTPS_DOMAIN_FOO(), HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_eq() metatest for ag_http_url_eq() with the above sample
 * HTTP URL objects. The first argument is lexicographically less than the
 * second.
 */
AG_METATEST_OBJECT_EQ(ag_http_url, HTTP_LOCALHOST_8080(),
    HTTPS_LOCALHOST_8080());
AG_METATEST_OBJECT_EQ(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_EQ(ag_http_url, HTTPS_DOMAIN_FOO(), HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_gt() metatest for ag_http_url_gt() with the above sample
 * HTTP URL objects. The first argument is lexicographically less than the
 * second.
 */
AG_METATEST_OBJECT_GT(ag_http_url, HTTP_LOCALHOST_8080(),
    HTTPS_LOCALHOST_8080());
AG_METATEST_OBJECT_GT(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_GT(ag_http_url, HTTPS_DOMAIN_FOO(), HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_empty() metatest for ag_http_url_empty() with the above
 * sample HTTP URL objects. Since URL objects can never be empty, we only
 * perform the AG_METATEST_OBJECT_EMPTY_NOT() metatest.
 */
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTP_LOCALHOST_8080());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTPS_LOCALHOST_8080());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_valid() metatest for ag_http_url_valid() with the above
 * sample HTTP URL objects. Since URL objects can never be invalid if created
 * successfully, we're skipping the AG_METATEST_OBJECT_VALID_NOT() metatest.
 */
AG_METATEST_OBJECT_VALID(ag_http_url, HTTP_LOCALHOST_8080());
AG_METATEST_OBJECT_VALID(ag_http_url, HTTPS_LOCALHOST_8080());
AG_METATEST_OBJECT_VALID(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_VALID(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_VALID(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_VALID(ag_http_url, HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_typeid() metatest for ag_http_url_typeid() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTP_LOCALHOST_8080(),
    AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTPS_LOCALHOST_8080(),
    AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTPS_LOCALHOST_8080_FOO(),
    AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTPS_DOMAIN_FOO(), AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTPS_DOMAIN_FOO_BAR(),
    AG_TYPEID_HTTP_URL);


/*
 * Run the ag_object_uuid() metatest for ag_http_url_uuid() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_UUID(ag_http_url, HTTP_LOCALHOST_8080());
AG_METATEST_OBJECT_UUID(ag_http_url, HTTPS_LOCALHOST_8080());
AG_METATEST_OBJECT_UUID(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_UUID(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_UUID(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_UUID(ag_http_url, HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_refc() metatest for ag_http_url_refc() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_REFC(ag_http_url, HTTP_LOCALHOST_8080());
AG_METATEST_OBJECT_REFC(ag_http_url, HTTPS_LOCALHOST_8080());
AG_METATEST_OBJECT_REFC(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_REFC(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_REFC(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_REFC(ag_http_url, HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_len() metatest for ag_http_url_len() with the above sample
 * HTTP URL objects.
 */
AG_METATEST_OBJECT_LEN(ag_http_url, HTTP_LOCALHOST_8080(),
    sample_len(HTTP_LOCALHOST_8080()));
AG_METATEST_OBJECT_LEN(ag_http_url, HTTPS_LOCALHOST_8080(),
    sample_len(HTTPS_LOCALHOST_8080()));
AG_METATEST_OBJECT_LEN(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    sample_len(HTTP_LOCALHOST_8080_FOO()));
AG_METATEST_OBJECT_LEN(ag_http_url, HTTPS_LOCALHOST_8080_FOO(),
    sample_len(HTTPS_LOCALHOST_8080_FOO()));
AG_METATEST_OBJECT_LEN(ag_http_url, HTTPS_DOMAIN_FOO(),
    sample_len(HTTPS_DOMAIN_FOO()));
AG_METATEST_OBJECT_LEN(ag_http_url, HTTPS_DOMAIN_FOO_BAR(),
    sample_len(HTTPS_DOMAIN_FOO_BAR()));


/*
 * Run the ag_object_url() metatest for ag_http_url_url() with the above sample
 * HTTP URL objects.
 */
AG_METATEST_OBJECT_SZ(ag_http_url, HTTP_LOCALHOST_8080(),
    sample_sz(HTTP_LOCALHOST_8080()));
AG_METATEST_OBJECT_SZ(ag_http_url, HTTPS_LOCALHOST_8080(),
    sample_sz(HTTPS_LOCALHOST_8080()));
AG_METATEST_OBJECT_SZ(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    sample_sz(HTTP_LOCALHOST_8080_FOO()));
AG_METATEST_OBJECT_SZ(ag_http_url, HTTPS_LOCALHOST_8080_FOO(),
    sample_sz(HTTPS_LOCALHOST_8080_FOO()));
AG_METATEST_OBJECT_SZ(ag_http_url, HTTPS_DOMAIN_FOO(),
    sample_sz(HTTPS_DOMAIN_FOO()));
AG_METATEST_OBJECT_SZ(ag_http_url, HTTPS_DOMAIN_FOO_BAR(),
    sample_sz(HTTPS_DOMAIN_FOO_BAR()));


/*
 * Run the ag_object_hash() metatest for ag_http_url_hash() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_HASH(ag_http_url, HTTP_LOCALHOST_8080(),
    sample_hash(HTTP_LOCALHOST_8080()));
AG_METATEST_OBJECT_HASH(ag_http_url, HTTPS_LOCALHOST_8080(),
    sample_hash(HTTPS_LOCALHOST_8080()));
AG_METATEST_OBJECT_HASH(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    sample_hash(HTTP_LOCALHOST_8080_FOO()));
AG_METATEST_OBJECT_HASH(ag_http_url, HTTPS_LOCALHOST_8080_FOO(),
    sample_hash(HTTPS_LOCALHOST_8080_FOO()));
AG_METATEST_OBJECT_HASH(ag_http_url, HTTPS_DOMAIN_FOO(),
    sample_hash(HTTPS_DOMAIN_FOO()));
AG_METATEST_OBJECT_HASH(ag_http_url, HTTPS_DOMAIN_FOO_BAR(),
    sample_hash(HTTPS_DOMAIN_FOO_BAR()));


/*
 * Run the ag_object_str() metatest for ag_http_url_str() with the above sample
 * HTTP URL objects.
 */
AG_METATEST_OBJECT_STR(ag_http_url, HTTP_LOCALHOST_8080(),
    "http://127.0.0.1:8080/");
AG_METATEST_OBJECT_STR(ag_http_url, HTTPS_LOCALHOST_8080(),
    "https://127.0.0.1:8080/");
AG_METATEST_OBJECT_STR(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    "http://localhost:8080/foo");
AG_METATEST_OBJECT_STR(ag_http_url, HTTPS_LOCALHOST_8080_FOO(),
    "https://localhost:8080/foo");
AG_METATEST_OBJECT_STR(ag_http_url, HTTPS_DOMAIN_FOO(),
    "https://www.domain.com/foo");
AG_METATEST_OBJECT_STR(ag_http_url, HTTPS_DOMAIN_FOO_BAR(),
    "https://www.domain.com/foo/bar");


/*
 * Run the metatest for ag_http_url_path() with the sample URL objects defined
 * above. The first parameter to the AG_METATEST_HTTP_URL_PATH() macro is the
 * sample URL object, and the second parameter is the expected string result.
 */
AG_METATEST_HTTP_URL_PATH(HTTP_LOCALHOST_8080(), "/");
AG_METATEST_HTTP_URL_PATH(HTTPS_LOCALHOST_8080(), "/");
AG_METATEST_HTTP_URL_PATH(HTTP_LOCALHOST_8080_FOO(), "/foo");
AG_METATEST_HTTP_URL_PATH(HTTPS_LOCALHOST_8080_FOO(), "/foo");
AG_METATEST_HTTP_URL_PATH(HTTPS_DOMAIN_FOO(), "/foo");
AG_METATEST_HTTP_URL_PATH(HTTPS_DOMAIN_FOO_BAR(), "/foo/bar");


/*
 * Run the metatest for ag_http_url_host() with the sample URL objects defined
 * above. The first parameter to AG_METATEST_HTTP_URL_HOST() is the sample URL,
 * and the second parameter specifies the expected string result.
 */
AG_METATEST_HTTP_URL_HOST(HTTP_LOCALHOST_8080(), "127.0.0.1");
AG_METATEST_HTTP_URL_HOST(HTTPS_LOCALHOST_8080(), "127.0.0.1");
AG_METATEST_HTTP_URL_HOST(HTTP_LOCALHOST_8080_FOO(), "localhost");
AG_METATEST_HTTP_URL_HOST(HTTPS_LOCALHOST_8080_FOO(), "localhost");
AG_METATEST_HTTP_URL_HOST(HTTPS_DOMAIN_FOO(), "www.domain.com");
AG_METATEST_HTTP_URL_HOST(HTTPS_DOMAIN_FOO_BAR(), "www.domain.com");


/*
 * Run the metatest for ag_http_url_secure() with the sample URL objects defined
 * above. The first parameter to the AG_METATEST_HTTP_URL_SECURE() is the sample
 * URL object, and the second parameter is the expected Boolean result.
 */
AG_METATEST_HTTP_URL_SECURE(HTTP_LOCALHOST_8080(), false);
AG_METATEST_HTTP_URL_SECURE(HTTPS_LOCALHOST_8080(), true);
AG_METATEST_HTTP_URL_SECURE(HTTP_LOCALHOST_8080_FOO(), false);
AG_METATEST_HTTP_URL_SECURE(HTTPS_LOCALHOST_8080_FOO(), true);
AG_METATEST_HTTP_URL_SECURE(HTTPS_DOMAIN_FOO(), true);
AG_METATEST_HTTP_URL_SECURE(HTTPS_DOMAIN_FOO_BAR(), true);


/*
 * Run the metatest for ag_http_url_port() with the sample URL objects defined
 * above. The first parameter to the AG_METATEST_HTTP_URL_PORT() macro is the
 * sample URL object, and the second parameter is the expected integer result.
 */
AG_METATEST_HTTP_URL_PORT(HTTP_LOCALHOST_8080(), 8080);
AG_METATEST_HTTP_URL_PORT(HTTPS_LOCALHOST_8080(), 8080);
AG_METATEST_HTTP_URL_PORT(HTTP_LOCALHOST_8080_FOO(), 8080);
AG_METATEST_HTTP_URL_PORT(HTTPS_LOCALHOST_8080_FOO(),8080);
AG_METATEST_HTTP_URL_PORT(HTTPS_DOMAIN_FOO(), 0);
AG_METATEST_HTTP_URL_PORT(HTTPS_DOMAIN_FOO_BAR(), 0);


/*
 * Run the metatests for ag_http_url_parse() with the sample URL objects defined
 * above. The first parameter to the AG_METATEST_HTTP_URL_PARSE() macro is the
 * string to parse, and the second parameter is the expected URL object.
 */
AG_METATEST_HTTP_URL_PARSE("http://127.0.0.1:8080/", HTTP_LOCALHOST_8080());
AG_METATEST_HTTP_URL_PARSE("https://127.0.0.1:8080/", HTTPS_LOCALHOST_8080());
AG_METATEST_HTTP_URL_PARSE("http://localhost:8080/foo",
    HTTP_LOCALHOST_8080_FOO());
AG_METATEST_HTTP_URL_PARSE("https://localhost:8080/foo",
    HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_HTTP_URL_PARSE("https://www.domain.com/foo", HTTPS_DOMAIN_FOO());
AG_METATEST_HTTP_URL_PARSE("https://www.domain.com/foo/bar",
    HTTPS_DOMAIN_FOO_BAR());

/*
 * Define the test_suite_http_url() function. We generate the test cases from
 * the above metatest definitions through a call to AG_TEST_SUITE_GENERATE().
 */
extern ag_test_suite *
test_suite_http_url(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_url interface");
}


/*
 * Define the sample_str() helper function. This function generates the string
 * representation of a sample HTTP URL object generated by the
 * AG_SAMPLE_HTTP_URL() macro.
 */
static inline ag_string *
sample_str(ag_http_url *ctx)
{
        AG_AUTO(ag_http_url) *u = ctx;
        return ag_http_url_str(u);
}


/*
 * Define the sample_len() helper function. This function computes the length of
 * a sample HTTP URL object generated by the AG_SAMPLE_HTTP_URL() macro.
 */
static inline size_t
sample_len(ag_http_url *ctx)
{
        AG_AUTO(ag_string) *s = sample_str(ctx);
        return ag_string_len(s);
}


/*
 * Define the sample_sz() helper function. This function computes the size of a
 * sample HTTP URL object generated by the AG_SAMPLE_HTTP_URL() macro.
 */
static inline size_t
sample_sz(ag_http_url *ctx)
{
        AG_AUTO(ag_string) *s = sample_str(ctx);
        return ag_string_sz(s);
}


/*
 * Define the sample_hash() helper function. This function computes the hash of
 * a sample HTTP URL object generated by the AG_SAMPLE_HTTP_URL() macro.
 */
static inline ag_hash
sample_hash(ag_http_client *ctx)
{
        AG_AUTO(ag_string) *s = sample_str(ctx);
        return ag_hash_new_str(s);
}

