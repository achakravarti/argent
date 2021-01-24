/*-
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Argent - infrastructure for building web services
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


#include "./http-mime.h"
#include "./test.h"


/*
 * Define the __AG_TEST_SUITE_ID__ symbolic constant. This constant is required
 * by the testing interface to metaprogrammatically generate the supporting
 * testing functionality. It represents the unique ID of the test suite for the
 * HTTP MIME type enumeration test suite.
 */
#define __AG_TEST_SUITE_ID__ 11


AG_METATEST_HTTP_MIME_STR(APPLICATION_FORM);
AG_METATEST_HTTP_MIME_STR(APPLICATION_JSON);
AG_METATEST_HTTP_MIME_STR(APPLICATION_OCTET);
AG_METATEST_HTTP_MIME_STR(APPLICATION_XML);
AG_METATEST_HTTP_MIME_STR(MULTIPART_FORM);
AG_METATEST_HTTP_MIME_STR(TEXT_CSS);
AG_METATEST_HTTP_MIME_STR(TEXT_CSV);
AG_METATEST_HTTP_MIME_STR(TEXT_HTML);
AG_METATEST_HTTP_MIME_STR(TEXT_JS);
AG_METATEST_HTTP_MIME_STR(TEXT_PLAIN);
AG_METATEST_HTTP_MIME_STR(TEXT_XML);


/*
 * Define the test_suite_http_mime() function. We generate the test cases from
 * the above metatest definitions through a call to the AG_TEST_SUITE_GENERATE()
 * macro.
 */
extern ag_test_suite *
test_suite_http_mime(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_mime interface");
}

