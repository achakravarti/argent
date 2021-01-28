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


#include "./object.h"
#include "./http-url.h"
#include "./test.h"


/*
 * Define the __AG_TEST_SUITE_ID__ macro. This symbolic constant represents the
 * unique ID for the HTTP URL interface test suite, and is used by the testing
 * macros to metaprogrammatically generate the relevant testing functions.
 */
#define __AG_TEST_SUITE_ID__ 8


/*
 * Define the sample HTTP URL objects. We're defining, respectively, the
 * following URLS:
 *   - http://127.0.0.1:8080/
 *   - https://127.0.0.1:8080/
 *   - http://localhost:8080/foo
 *   - https://localhost:8080/foo
 *   - https://www.domain.com/foo
 *   - https://www.domain.com/foo/bar
 *
 * We use the SAMPLE() macro defined above to generate the sample HTTP URL
 * objects along with their supporting functions.
 */
AG_SAMPLE_EXT_HTTP_URL(HTTP_LOCALHOST_8080_NOPATH,
    false, "127.0.0.1", 8080, "");
AG_SAMPLE_EXT_HTTP_URL(HTTPS_LOCALHOST_8080_NOPATH,
    true, "127.0.0.1", 8080, "");
AG_SAMPLE_EXT_HTTP_URL(HTTP_LOCALHOST_8080_FOO,
    false, "localhost", 8080, "foo");
AG_SAMPLE_EXT_HTTP_URL(HTTPS_LOCALHOST_8080_FOO,
    true, "localhost", 8080, "foo");
AG_SAMPLE_EXT_HTTP_URL(HTTPS_DOMAIN_FOO,
    true, "www.domain.com", 0, "/foo");
AG_SAMPLE_EXT_HTTP_URL(HTTPS_DOMAIN_FOO_BAR,
    true, "www.domain.com", 0, "foo/bar");


/*
 * Run the ag_object_copy() metatest for ag_http_url_copy() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_COPY(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_COPY(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_COPY(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_COPY(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_COPY(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_COPY(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_clone() metatest for ag_http_url_clone() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_CLONE(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_CLONE(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_CLONE(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_CLONE(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_CLONE(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_CLONE(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_copy() metatest for ag_http_url_copy() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_RELEASE(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_RELEASE(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_RELEASE(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_RELEASE(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_RELEASE(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_RELEASE(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_cmp() metatest for ag_http_url_cmp() with the above sample
 * HTTP URL objects. The first argument is lexicographically less than the
 * second.
 */
AG_METATEST_OBJECT_CMP(ag_http_url, 
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_CMP(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_CMP(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO(), 
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_lt() metatest for ag_http_url_lt() with the above sample
 * HTTP URL objects. The first argument is lexicographically less than the
 * second.
 */
AG_METATEST_OBJECT_LT(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_LT(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_LT(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO(),
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_eq() metatest for ag_http_url_eq() with the above sample
 * HTTP URL objects. The first argument is lexicographically less than the
 * second.
 */
AG_METATEST_OBJECT_EQ(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_EQ(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_EQ(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO(),
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_gt() metatest for ag_http_url_gt() with the above sample
 * HTTP URL objects. The first argument is lexicographically less than the
 * second.
 */
AG_METATEST_OBJECT_GT(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_GT(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_GT(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO(),
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_empty() metatest for ag_http_url_empty() with the above
 * sample HTTP URL objects. Since URL objects can never be empty, we only
 * perform the AG_METATEST_OBJECT_EMPTY_NOT() metatest.
 */
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_valid() metatest for ag_http_url_valid() with the above
 * sample HTTP URL objects. Since URL objects can never be invalid if created
 * successfully, we're skipping the AG_METATEST_OBJECT_VALID_NOT() metatest.
 */
AG_METATEST_OBJECT_VALID(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_VALID(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_VALID(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_VALID(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_VALID(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_VALID(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_typeid() metatest for ag_http_url_typeid() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_TYPEID(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH(), AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH(), AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO(), AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO(), AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO(),
    AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR(),
    AG_TYPEID_HTTP_URL);


/*
 * Run the ag_object_uuid() metatest for ag_http_url_uuid() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_UUID(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_UUID(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_UUID(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_UUID(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_UUID(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_UUID(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_refc() metatest for ag_http_url_refc() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_REFC(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_REFC(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_REFC(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_REFC(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_REFC(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_REFC(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR());


/*
 * Run the ag_object_len() metatest for ag_http_url_len() with the above sample
 * HTTP URL objects.
 */
AG_METATEST_OBJECT_LEN(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH(),
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH_LEN());
AG_METATEST_OBJECT_LEN(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH_LEN());
AG_METATEST_OBJECT_LEN(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO(),
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO_LEN());
AG_METATEST_OBJECT_LEN(ag_http_url, SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO_LEN());
AG_METATEST_OBJECT_LEN(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO(),
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_LEN());
AG_METATEST_OBJECT_LEN(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR(),
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR_LEN());


/*
 * Run the ag_object_url() metatest for ag_http_url_url() with the above sample
 * HTTP URL objects.
 */
AG_METATEST_OBJECT_SZ(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH(),
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH_SZ());
AG_METATEST_OBJECT_SZ(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH_SZ());
AG_METATEST_OBJECT_SZ(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO(),
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO_SZ());
AG_METATEST_OBJECT_SZ(ag_http_url, SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO_SZ());
AG_METATEST_OBJECT_SZ(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO(),
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_SZ());
AG_METATEST_OBJECT_SZ(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR(),
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR_SZ());


/*
 * Run the ag_object_hash() metatest for ag_http_url_hash() with the above
 * sample HTTP URL objects.
 */
AG_METATEST_OBJECT_HASH(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH(),
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH_HASH());
AG_METATEST_OBJECT_HASH(ag_http_url,
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH_HASH());
AG_METATEST_OBJECT_HASH(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO(),
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO_HASH());
AG_METATEST_OBJECT_HASH(ag_http_url, SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO(),
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO_HASH());
AG_METATEST_OBJECT_HASH(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO(),
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_HASH());
AG_METATEST_OBJECT_HASH(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR(),
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR_HASH());


/*
 * Run the ag_object_str() metatest for ag_http_url_str() with the above sample
 * HTTP URL objects.
 */
AG_METATEST_OBJECT_STR(ag_http_url,
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH(), "http://127.0.0.1:8080/");
AG_METATEST_OBJECT_STR(ag_http_url, 
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH(), "https://127.0.0.1:8080/");
AG_METATEST_OBJECT_STR(ag_http_url, SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO(),
    "http://localhost:8080/foo");
AG_METATEST_OBJECT_STR(ag_http_url, SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO(),
    "https://localhost:8080/foo");
AG_METATEST_OBJECT_STR(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO(),
    "https://www.domain.com/foo");
AG_METATEST_OBJECT_STR(ag_http_url, SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR(),
    "https://www.domain.com/foo/bar");


/*
 * Run the metatest for ag_http_url_path() with the sample URL objects defined
 * above. The first parameter to the AG_METATEST_HTTP_URL_PATH() macro is the
 * sample URL object, and the second parameter is the expected string result.
 */
AG_METATEST_HTTP_URL_PATH(SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH(), "/");
AG_METATEST_HTTP_URL_PATH(SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH(), "/");
AG_METATEST_HTTP_URL_PATH(SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO(), "/foo");
AG_METATEST_HTTP_URL_PATH(SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO(), "/foo");
AG_METATEST_HTTP_URL_PATH(SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO(), "/foo");
AG_METATEST_HTTP_URL_PATH(SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR(), "/foo/bar");


/*
 * Run the metatest for ag_http_url_host() with the sample URL objects defined
 * above. The first parameter to AG_METATEST_HTTP_URL_HOST() is the sample URL,
 * and the second parameter specifies the expected string result.
 */
AG_METATEST_HTTP_URL_HOST(SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH(),
    "127.0.0.1");
AG_METATEST_HTTP_URL_HOST(SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH(),
    "127.0.0.1");
AG_METATEST_HTTP_URL_HOST(SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO(),
    "localhost");
AG_METATEST_HTTP_URL_HOST(SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO(),
    "localhost");
AG_METATEST_HTTP_URL_HOST(SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO(),
    "www.domain.com");
AG_METATEST_HTTP_URL_HOST(SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR(),
    "www.domain.com");


/*
 * Run the metatest for ag_http_url_secure() with the sample URL objects defined
 * above. The first parameter to the AG_METATEST_HTTP_URL_SECURE() is the sample
 * URL object, and the second parameter is the expected Boolean result.
 */
AG_METATEST_HTTP_URL_SECURE(SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH(),
    false);
AG_METATEST_HTTP_URL_SECURE(SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH(),
    true);
AG_METATEST_HTTP_URL_SECURE(SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO(), false);
AG_METATEST_HTTP_URL_SECURE(SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO(), true);
AG_METATEST_HTTP_URL_SECURE(SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO(), true);
AG_METATEST_HTTP_URL_SECURE(SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR(), true);


/*
 * Run the metatest for ag_http_url_port() with the sample URL objects defined
 * above. The first parameter to the AG_METATEST_HTTP_URL_PORT() macro is the
 * sample URL object, and the second parameter is the expected integer result.
 */
AG_METATEST_HTTP_URL_PORT(SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH(), 8080);
AG_METATEST_HTTP_URL_PORT(SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH(), 8080);
AG_METATEST_HTTP_URL_PORT(SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO(), 8080);
AG_METATEST_HTTP_URL_PORT(SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO(),8080);
AG_METATEST_HTTP_URL_PORT(SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO(), 0);
AG_METATEST_HTTP_URL_PORT(SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR(), 0);


/*
 * Run the metatests for ag_http_url_parse() with the sample URL objects defined
 * above. The first parameter to the AG_METATEST_HTTP_URL_PARSE() macro is the
 * string to parse, and the second parameter is the expected URL object.
 */
AG_METATEST_HTTP_URL_PARSE("http://127.0.0.1:8080/",
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_HTTP_URL_PARSE("https://127.0.0.1:8080/",
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_HTTP_URL_PARSE("http://localhost:8080/foo",
    SAMPLE_HTTP_URL_HTTP_LOCALHOST_8080_FOO());
AG_METATEST_HTTP_URL_PARSE("https://localhost:8080/foo",
    SAMPLE_HTTP_URL_HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_HTTP_URL_PARSE("https://www.domain.com/foo",
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO());
AG_METATEST_HTTP_URL_PARSE("https://www.domain.com/foo/bar",
    SAMPLE_HTTP_URL_HTTPS_DOMAIN_FOO_BAR());

/*
 * Define the test_suite_http_url() function. We generate the test cases from
 * the above metatest definitions through a call to AG_TEST_SUITE_GENERATE().
 */
extern ag_test_suite *
test_suite_http_url(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_url interface");
}

