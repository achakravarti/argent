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


#include "./http-mime.h"
#include "./test.h"


/*
 * Define the __AG_TEST_SUITE_ID__ symbolic constant. This constant is required
 * by the testing interface to metaprogrammatically generate the supporting
 * testing functionality. It represents the unique ID of the test suite for the
 * HTTP MIME type enumeration test suite.
 */
#define __AG_TEST_SUITE_ID__ 11


/*
 * Run the metatest for ag_http_mime_parse() for several possible string
 * representations, including uppercase, lowercase, proper case, and invalid
 * forms.
 */
AG_METATEST_HTTP_MIME_PARSE("foo", AG_HTTP_MIME_TEXT_PLAIN);
AG_METATEST_HTTP_MIME_PARSE("application/x-www-form-urlencoded",
    AG_HTTP_MIME_APPLICATION_FORM);
AG_METATEST_HTTP_MIME_PARSE("APPLICATION/X-WWW-FORM-URLENCODED",
    AG_HTTP_MIME_APPLICATION_FORM);
AG_METATEST_HTTP_MIME_PARSE("Application/X-Www-Form-Urlencoded",
    AG_HTTP_MIME_APPLICATION_FORM);
AG_METATEST_HTTP_MIME_PARSE("application/json", AG_HTTP_MIME_APPLICATION_JSON);
AG_METATEST_HTTP_MIME_PARSE("APPLICATION/JSON", AG_HTTP_MIME_APPLICATION_JSON);
AG_METATEST_HTTP_MIME_PARSE("Application/Json", AG_HTTP_MIME_APPLICATION_JSON);
AG_METATEST_HTTP_MIME_PARSE("application/octet-stream",
    AG_HTTP_MIME_APPLICATION_OCTET);
AG_METATEST_HTTP_MIME_PARSE("APPLICATION/OCTET-STREAM",
    AG_HTTP_MIME_APPLICATION_OCTET);
AG_METATEST_HTTP_MIME_PARSE("Application/Octet-stream",
    AG_HTTP_MIME_APPLICATION_OCTET);
AG_METATEST_HTTP_MIME_PARSE("application/xml", AG_HTTP_MIME_APPLICATION_XML);
AG_METATEST_HTTP_MIME_PARSE("APPLICATION/XML", AG_HTTP_MIME_APPLICATION_XML);
AG_METATEST_HTTP_MIME_PARSE("Application/Xml", AG_HTTP_MIME_APPLICATION_XML);
AG_METATEST_HTTP_MIME_PARSE("multipart/form-data", AG_HTTP_MIME_MULTIPART_FORM);
AG_METATEST_HTTP_MIME_PARSE("MULTIPART/FORM-DATA", AG_HTTP_MIME_MULTIPART_FORM);
AG_METATEST_HTTP_MIME_PARSE("Multipart/fOrm-data", AG_HTTP_MIME_MULTIPART_FORM);
AG_METATEST_HTTP_MIME_PARSE("text/css", AG_HTTP_MIME_TEXT_CSS);
AG_METATEST_HTTP_MIME_PARSE("TEXT/CSS", AG_HTTP_MIME_TEXT_CSS);
AG_METATEST_HTTP_MIME_PARSE("Text/Css", AG_HTTP_MIME_TEXT_CSS);
AG_METATEST_HTTP_MIME_PARSE("text/csv", AG_HTTP_MIME_TEXT_CSV);
AG_METATEST_HTTP_MIME_PARSE("TEXT/CSV", AG_HTTP_MIME_TEXT_CSV);
AG_METATEST_HTTP_MIME_PARSE("Text/Csv", AG_HTTP_MIME_TEXT_CSV);
AG_METATEST_HTTP_MIME_PARSE("text/html", AG_HTTP_MIME_TEXT_HTML);
AG_METATEST_HTTP_MIME_PARSE("TEXT/HTML", AG_HTTP_MIME_TEXT_HTML);
AG_METATEST_HTTP_MIME_PARSE("Text/Html", AG_HTTP_MIME_TEXT_HTML);
AG_METATEST_HTTP_MIME_PARSE("text/javascript", AG_HTTP_MIME_TEXT_JS);
AG_METATEST_HTTP_MIME_PARSE("TEXT/JAVASCRIPT", AG_HTTP_MIME_TEXT_JS);
AG_METATEST_HTTP_MIME_PARSE("Text/Javascript", AG_HTTP_MIME_TEXT_JS);
AG_METATEST_HTTP_MIME_PARSE("text/plain", AG_HTTP_MIME_TEXT_PLAIN);
AG_METATEST_HTTP_MIME_PARSE("TEXT/PLAIN", AG_HTTP_MIME_TEXT_PLAIN);
AG_METATEST_HTTP_MIME_PARSE("Text/Plain", AG_HTTP_MIME_TEXT_PLAIN);
AG_METATEST_HTTP_MIME_PARSE("text/xml", AG_HTTP_MIME_TEXT_XML);
AG_METATEST_HTTP_MIME_PARSE("TEXT/XML", AG_HTTP_MIME_TEXT_XML);
AG_METATEST_HTTP_MIME_PARSE("Text/Xml", AG_HTTP_MIME_TEXT_XML);


/*
 * Run the metatest for ag_http_mime_str() for each of the enumerators in the
 * ag_http_mime enumeration.
 */
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_APPLICATION_FORM,
    "application/x-www-form-urlencoded");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_APPLICATION_JSON, "application/json");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_APPLICATION_OCTET,
    "application/octet-stream");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_APPLICATION_XML, "application/xml");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_MULTIPART_FORM, "multipart/form-data");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_TEXT_CSS, "text/css");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_TEXT_CSV, "text/csv");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_TEXT_HTML, "text/html");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_TEXT_JS, "text/javascript");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_TEXT_PLAIN, "text/plain");
AG_METATEST_HTTP_MIME_STR(AG_HTTP_MIME_TEXT_XML, "text/xml");


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

