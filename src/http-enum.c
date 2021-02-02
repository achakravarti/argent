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


#include "../include/argent.h"


/*
 * Define the g_method string array. This array holds the string representations
 * of the enumerators of the ag_http_method enumeration. There is a one-to-one
 * correspondence between the enumerators and their string representations, with
 * the array member index being the same as the corresponding enumerator.
 */
static AG_THREADLOCAL const char *g_method[] = {
        "GET",
        "POST",
        "PUT",
        "PATCH",
        "DELETE",
};


/*
 * Define the g_mime string array. Similar to the g_method string array, this
 * array holds the string represetations of the individual enumerators of the
 * ag_http_mime enumeration, with the index of each array member being the same
 * as the corresponding enumerator that it represents.
 */
static AG_THREADLOCAL const char *g_mime[] = {
        "application/x-www-form-urlencoded",
        "application/json",
        "application/octet-stream",
        "application/xml",
        "multipart/form-data",
        "text/css",
        "text/csv",
        "text/html",
        "text/javascript",
        "text/plain",
        "text/xml",
};


static AG_THREADLOCAL const char *g_status[] = {
        "200 (OK)",
        "201 (Created)",
        "202 (Accepted)",
        "204 (No Content)",
        "301 (Moved Permanently)",
        "302 (Found)",
        "303 (See Other)",
        "304 (Not Modified)",
        "307 (Temporary Redirect)",
        "400 (Bad Request)",
        "401 (Unauthorized)",
        "403 (Forbidden)",
        "404 (Not Found)",
        "405 (Method Not Allowed)",
        "406 (Not Acceptable)",
        "412 (Precondition Failed)",
        "415 (Unsupported Media Type)",
        "500 (Internal Server Error)",
        "501 (Not Implemented)",
};



/*
 * Define the ag_http_method_parse() interface function. This function is
 * responsible for parsing a given string and returning the HTTP method
 * enumerator represented by the string. 
 *
 * In case the string contains something not recognised as a valid enumerator,
 * then the AG_ERNO_PARSE exception is raised. The final return statement is
 * never reached, and is placed only for syntactic reasons.
 */
extern enum ag_http_method
ag_http_method_parse(const char *str)
{
        AG_ASSERT_STR (str);

        AG_AUTO(ag_string) *s = ag_string_new(str);
        AG_AUTO(ag_string) *u = ag_string_upper(s);

        for (register int i = 0; i <= AG_HTTP_METHOD_DELETE; i++)
                if (ag_string_eq(u, g_method[i]))
                        return i;

        struct ag_exception_parse x = {.str = str, .ctx = "ag_http_method"};
        AG_REQUIRE_OPT (false, AG_ERNO_PARSE, &x);
        return AG_HTTP_METHOD_GET;
}


/*
 * Define the ag_http_method_str() interface function. This function gets the
 * string representation of a contextual HTTP method enumerator.
 */
extern ag_string *
ag_http_method_str(enum ag_http_method meth)
{
        AG_ASSERT (meth >= AG_HTTP_METHOD_GET && meth <= AG_HTTP_METHOD_DELETE);

        return ag_string_new(g_method[meth]);
}


/*
 * Define the ag_http_mime_parse() interface function. This function parses a
 * given string and returns the ag_http_mime enumerator corresponding to the
 * string. 
 *
 * If no match is found, then the AG_ERNO_PARSE exception is raised. The final
 * return statement is never reached, and is placed only to prevent compiler
 * warnings.
 */
extern enum ag_http_mime
ag_http_mime_parse(const char *str)
{
        AG_ASSERT_STR (str);

        AG_AUTO(ag_string) *s = ag_string_new(str);
        AG_AUTO(ag_string) *l = ag_string_lower(s);

        for (register int i = 0; i <= AG_HTTP_MIME_TEXT_XML; i++)
                if (ag_string_eq(l, g_mime[i]))
                        return i;

        struct ag_exception_parse x = {.str = str, .ctx = "ag_http_mime"};
        AG_REQUIRE_OPT (false, AG_ERNO_PARSE, &x);
        return AG_HTTP_MIME_TEXT_PLAIN;
}


/*
 * Define the ag_http_mime_str() interface function. This function returns the
 * string representation of a given ag_http_mime enumerator.
 */
extern ag_string *
ag_http_mime_str(enum ag_http_mime mime)
{
        AG_ASSERT (mime >= AG_HTTP_MIME_APPLICATION_FORM && 
            mime <= AG_HTTP_MIME_TEXT_XML);

        return ag_string_new(g_mime[mime]);
}


extern enum ag_http_status
ag_http_status_parse(const char *str)
{
        AG_ASSERT_PTR (str);

        AG_AUTO(ag_string) *s = ag_string_new(str);
        AG_AUTO(ag_string) *p = ag_string_proper(s);

        for (register int i = 0; i <= AG_HTTP_STATUS_501_NOT_IMPLEMENTED; i++)
                if (ag_string_eq(p, g_status[i]))
                        return i;

        struct ag_exception_parse x = {.str = str, .ctx = "ag_http_status"};
        AG_REQUIRE_OPT (false, AG_ERNO_PARSE, &x);
        return AG_HTTP_STATUS_500_INTERNAL_SERVER_ERROR;
}


extern ag_string *
ag_http_status_str(enum ag_http_status status)
{
        AG_ASSERT (status >= AG_HTTP_STATUS_200_OK &&
            status <= AG_HTTP_STATUS_501_NOT_IMPLEMENTED);

        return ag_string_new(g_status[status]);
}


