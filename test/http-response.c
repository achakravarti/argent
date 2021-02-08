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


#include "./http.h" 
#include "./object.h"
#include "./test.h"


/**
 * The testing infrastructure requires that every test suite be given a unique
 * integer identifier in order to correctly generate the test cases. This unique
 * identifier is set through the __AG_TEST_SUITE_ID__ symbolic constant.
 **/

#define __AG_TEST_SUITE_ID__ 12


/**
 * These are the prototypes for the helper functions related to the sample HTTP
 * response objects used for testing.
 **/

static inline size_t    sample_len(ag_http_response *);
static inline size_t    sample_sz(ag_http_response *);
static inline ag_hash   sample_hash(ag_http_response *);


/**
 * Using the HTTP response sample generator macros, we generate the following
 * functions returning the sample HTTP response object instances that we use for
 * testing:
 *   - HTML_200_EMPTY(): text/html, 200 (OK), empty body
 *   - JSON_201()      : application/json 201 (Created), JSON body
 **/

AG_SAMPLE_HTTP_RESPONSE_EMPTY(HTML_200_EMPTY, AG_HTTP_MIME_TEXT_HTML,
    AG_HTTP_STATUS_200_OK);
AG_SAMPLE_HTTP_RESPONSE(JSON_201, AG_HTTP_MIME_APPLICATION_JSON,
    AG_HTTP_STATUS_201_CREATED, "{key:foo, val:bar}");
AG_SAMPLE_HTTP_RESPONSE_FILE(TEXT_302_FILE, AG_HTTP_MIME_TEXT_PLAIN,
    AG_HTTP_STATUS_302_FOUND, "bld/data/new.txt");


/**
 * Since ag_http_response is an object, it inherits the polymorphic functions of
 * the object interface. We test these inherited polymorphic functions through
 * the object metatest macros.
 **/

AG_METATEST_OBJECT_COPY(ag_http_response, HTML_200_EMPTY());
AG_METATEST_OBJECT_CLONE(ag_http_response, HTML_200_EMPTY());
AG_METATEST_OBJECT_RELEASE(ag_http_response, HTML_200_EMPTY());

AG_METATEST_OBJECT_COPY(ag_http_response, JSON_201());
AG_METATEST_OBJECT_CLONE(ag_http_response, JSON_201());
AG_METATEST_OBJECT_RELEASE(ag_http_response, JSON_201());

AG_METATEST_OBJECT_COPY(ag_http_response, TEXT_302_FILE());
AG_METATEST_OBJECT_CLONE(ag_http_response, TEXT_302_FILE());
AG_METATEST_OBJECT_RELEASE(ag_http_response, TEXT_302_FILE());

AG_METATEST_OBJECT_COMPARATOR(ag_http_response, HTML_200_EMPTY(), JSON_201());
AG_METATEST_OBJECT_COMPARATOR(ag_http_response, HTML_200_EMPTY(),
    TEXT_302_FILE());
AG_METATEST_OBJECT_COMPARATOR(ag_http_response, TEXT_302_FILE(), JSON_201());

AG_METATEST_OBJECT_PREDICATE(ag_http_response, HTML_200_EMPTY(), true, true);
AG_METATEST_OBJECT_PREDICATE(ag_http_response, JSON_201(), false, true);
AG_METATEST_OBJECT_PREDICATE(ag_http_response, TEXT_302_FILE(), false, true);

AG_METATEST_OBJECT_TYPEID(ag_http_response, HTML_200_EMPTY(),
    AG_TYPEID_HTTP_RESPONSE);
AG_METATEST_OBJECT_TYPEID(ag_http_response, JSON_201(),
    AG_TYPEID_HTTP_RESPONSE);
AG_METATEST_OBJECT_TYPEID(ag_http_response, TEXT_302_FILE(),
    AG_TYPEID_HTTP_RESPONSE);

AG_METATEST_OBJECT_UUID(ag_http_response, HTML_200_EMPTY());
AG_METATEST_OBJECT_UUID(ag_http_response, JSON_201());
AG_METATEST_OBJECT_UUID(ag_http_response, TEXT_302_FILE());

AG_METATEST_OBJECT_REFC(ag_http_response, HTML_200_EMPTY());
AG_METATEST_OBJECT_REFC(ag_http_response, JSON_201());
AG_METATEST_OBJECT_REFC(ag_http_response, TEXT_302_FILE());

AG_METATEST_OBJECT_LEN(ag_http_response, HTML_200_EMPTY(),
    sample_len(HTML_200_EMPTY()));
AG_METATEST_OBJECT_LEN(ag_http_response, JSON_201(), sample_len(JSON_201()));
AG_METATEST_OBJECT_LEN(ag_http_response, TEXT_302_FILE(),
    sample_len(TEXT_302_FILE()));

AG_METATEST_OBJECT_SZ(ag_http_response, HTML_200_EMPTY(),
    sample_sz(HTML_200_EMPTY()));
AG_METATEST_OBJECT_SZ(ag_http_response, JSON_201(), sample_sz(JSON_201()));
AG_METATEST_OBJECT_SZ(ag_http_response, TEXT_302_FILE(),
    sample_sz(TEXT_302_FILE()));

AG_METATEST_OBJECT_HASH(ag_http_response, HTML_200_EMPTY(),
    sample_hash(HTML_200_EMPTY()));
AG_METATEST_OBJECT_HASH(ag_http_response, JSON_201(), sample_hash(JSON_201()));
AG_METATEST_OBJECT_HASH(ag_http_response, TEXT_302_FILE(),
    sample_hash(TEXT_302_FILE()));

AG_METATEST_OBJECT_STR(ag_http_response, HTML_200_EMPTY(),
    "Content-type: text/html; charset=UTF-8\r\nStatus: 200 (OK)\r\n\r\n");
AG_METATEST_OBJECT_STR(ag_http_response, JSON_201(), 
    "Content-type: application/json; charset=UTF-8\r\nStatus: 201 (Created)"
    "\r\n\r\n{key:foo, val:bar}");
AG_METATEST_OBJECT_STR(ag_http_response, TEXT_302_FILE(), 
    "Content-type: text/plain; charset=UTF-8\r\nStatus: 302 (Found)"
    "\r\n\r\nHello, world!\n");


AG_METATEST_HTTP_RESPONSE_HEADER(HTML_200_EMPTY(),
    "Content-type: text/html; charset=UTF-8\r\nStatus: 200 (OK)\r\n\r\n");
AG_METATEST_HTTP_RESPONSE_HEADER(JSON_201(),
    "Content-type: application/json; charset=UTF-8\r\n"
    "Status: 201 (Created)\r\n\r\n");
AG_METATEST_HTTP_RESPONSE_HEADER(TEXT_302_FILE(),
    "Content-type: text/plain; charset=UTF-8\r\n"
    "Status: 302 (Found)\r\n\r\n");

AG_METATEST_HTTP_RESPONSE_BODY(HTML_200_EMPTY(), "");
AG_METATEST_HTTP_RESPONSE_BODY(JSON_201(), "{key:foo, val:bar}");
AG_METATEST_HTTP_RESPONSE_BODY(TEXT_302_FILE(), "Hello, world!\n");


/**
 * A test suite containing the test cases defined above needs to be generated.
 * This is done through the AG_TEST_SUITE_GENERATE() macro, and the generated
 * test suite instance is returned to the test runner for further processing.
 * 
 * However, before we do so, the sample data files that are used for testing are
 * first created:
 *   - bld/data/new.txt: for testing ag_http_response_new_file()
 *   - bld/data/add.txt: for testing ag_http_response_add_file()
 **/

extern ag_test_suite *
test_suite_http_response(void)
{
        (void)system("echo \"Hello, world!\" > bld/data/new.txt");

        return AG_TEST_SUITE_GENERATE("ag_http_response interface");
}


/**
 * The sample_len() helper function makes it convenient to compute the expected
 * length of a sample HTTP response object. The argument to this function is
 * required to be a sample returned by one of the generated sample generator
 * functions defined above.
 *
 * Similarly, the sample_sz() and sample_hash() helper functions are used to
 * help compute the expected size and hash, respectively, of a given generated
 * sample HTTP response object.
 **/

static inline size_t
sample_len(ag_http_response *ctx)
{
        AG_AUTO(ag_http_response) *r = ctx;
        AG_AUTO(ag_string) *s = ag_http_response_body(r);

        return ag_string_len(s);
}

static inline size_t
sample_sz(ag_http_response *ctx)
{
        AG_AUTO(ag_http_response) *r = ctx;
        AG_AUTO(ag_string) *s = ag_http_response_body(r);

        return ag_string_sz(s);
}

static inline ag_hash
sample_hash(ag_http_response *ctx)
{
        AG_AUTO(ag_http_response) *r = ctx;
        AG_AUTO(ag_string) *s = ag_http_response_str(r);

        return ag_hash_new_str(s);
}

