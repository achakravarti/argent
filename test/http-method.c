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


#include "./http-method.h"
#include "./test.h"


/*
 * Define the __AG_TEST_SUITE_ID__ symbolic constant. This constant is required
 * by the testing interface to metaprogrammatically generate the supporting
 * testing functionality. It represents the unique ID of the test suite for the
 * HTTP method enumeration test suite.
 */
#define __AG_TEST_SUITE_ID__ 10


/*
 * Run the metatest for ag_http_method_parse() for possible string
 * representations, including uppercase, lowercase, and proper case forms.
 */
AG_METATEST_HTTP_METHOD_PARSE("get", AG_HTTP_METHOD_GET);
AG_METATEST_HTTP_METHOD_PARSE("GET", AG_HTTP_METHOD_GET);
AG_METATEST_HTTP_METHOD_PARSE("Get", AG_HTTP_METHOD_GET);
AG_METATEST_HTTP_METHOD_PARSE("post", AG_HTTP_METHOD_POST);
AG_METATEST_HTTP_METHOD_PARSE("POST", AG_HTTP_METHOD_POST);
AG_METATEST_HTTP_METHOD_PARSE("Post", AG_HTTP_METHOD_POST);
AG_METATEST_HTTP_METHOD_PARSE("PUT", AG_HTTP_METHOD_PUT);
AG_METATEST_HTTP_METHOD_PARSE("put", AG_HTTP_METHOD_PUT);
AG_METATEST_HTTP_METHOD_PARSE("Put", AG_HTTP_METHOD_PUT);
AG_METATEST_HTTP_METHOD_PARSE("PATCH", AG_HTTP_METHOD_PATCH);
AG_METATEST_HTTP_METHOD_PARSE("patch", AG_HTTP_METHOD_PATCH);
AG_METATEST_HTTP_METHOD_PARSE("Patch", AG_HTTP_METHOD_PATCH);
AG_METATEST_HTTP_METHOD_PARSE("DELETE", AG_HTTP_METHOD_DELETE);
AG_METATEST_HTTP_METHOD_PARSE("delete", AG_HTTP_METHOD_DELETE);
AG_METATEST_HTTP_METHOD_PARSE("Delete", AG_HTTP_METHOD_DELETE);


/*
 * Run the metatest for ag_http_method_str() for each of the enumerators in the
 * ag_http_method enumeration.
 */
AG_METATEST_HTTP_METHOD_STR(GET);
AG_METATEST_HTTP_METHOD_STR(POST);
AG_METATEST_HTTP_METHOD_STR(PUT);
AG_METATEST_HTTP_METHOD_STR(PATCH);
AG_METATEST_HTTP_METHOD_STR(DELETE);


/*
 * Define the test_suite_http_method() function. We generate the test cases from
 * the above metatest definitions through a call to the AG_TEST_SUITE_GENERATE()
 * macro.
 */
extern ag_test_suite *
test_suite_http_method(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_method interface");
}

