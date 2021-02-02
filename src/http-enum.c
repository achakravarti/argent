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




/*******************************************************************************
 * The following string arrays hold the string representations of the individual
 * enumerators of the HTTP interface. g_method, g_mime and g_status hold the
 * string representations of the ag_http_method, ag_http_mime and ag_http_status
 * enumerations respectively.
 *
 * There is a one-to-one correspondence between the enumerators and their string
 * representations, with the array member index being the same as the value of
 * the corresponding enumerator.
 */

static AG_THREADLOCAL const char *g_method[] = {
        "GET",
        "POST",
        "PUT",
        "PATCH",
        "DELETE",
};

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




/*******************************************************************************
 * The ag_http_method_parse() interface function is responsible for parsing a
 * given string and returning the HTTP method enumerator represented by that
 * string. 
 *
 * The parsing is performed by looping through the g_method string array and
 * returning the index of the matching string.  We need to enusre that the
 * string argument is first transformed into uppercase so that a proper
 * comparison is performed.
 *
 * In case the string contains
 * something not recognised as a valid enumerator, then the AG_ERNO_PARSE
 * exception is raised. The final return statement is never reached, and is
 * placed only to prevent compiler warnings.
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




/*******************************************************************************
 * The ag_http_method_str() interface function returns the string representation
 * of a given ag_http_method enumerator. The implementation is straight-forward,
 * simply requiring us to return the string contained in the g_method array at
 * the index corresponding to the enumerator.
 */

extern ag_string *
ag_http_method_str(enum ag_http_method meth)
{
        AG_ASSERT (meth >= AG_HTTP_METHOD_GET && meth <= AG_HTTP_METHOD_DELETE);

        return ag_string_new(g_method[meth]);
}




/*******************************************************************************
 * The ag_http_mime_parse() interface function is responsible for parsing a
 * given string and returning the HTTP MIME enumerator represented by that
 * string.
 *
 * The parsing is performed by looping through the g_mime string array and
 * returning the index of the matching string. We need to take care to first
 * transform the string argument to lowercase to ensure that a proper comparison
 * takes place.
 *
 * In case the string contains something not recognised as a valid enumerator,
 * then the AG_ERNO_PARSE exception is raised. The final return statement is
 * never reached, and placed only to preven compiler warnings.
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




/*******************************************************************************
 * The ag_http_mime_str() interface function returns the string representation
 * of a given ag_http_mime enumerator. We simply return the string contained in
 * the g_mime array at the index corresponding to the enumerator.
 */

extern ag_string *
ag_http_mime_str(enum ag_http_mime mime)
{
        AG_ASSERT (mime >= AG_HTTP_MIME_APPLICATION_FORM && 
            mime <= AG_HTTP_MIME_TEXT_XML);

        return ag_string_new(g_mime[mime]);
}




/*******************************************************************************
 * The ag_http_status_parse() interface function is responsible for parsing a
 * given string and returning the HTTP status enumerator represented by that
 * string.
 *
 * The parsing is performed by looping through the g_status string array and
 * returning the index of the matching string. We need to take care to first
 * transform the string argument to proper case to ensure that a proper
 * comparison takes place.
 *
 * In case the string contains something not recognised as a valid enumerator,
 * then the AG_ERNO_PARSE exception is raised. The final return statement is
 * never reached, and placed only to preven compiler warnings.
 */


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




/*******************************************************************************
 * The ag_http_status_str() interface function returns the string representation
 * of a given ag_http_status enumerator. We simply return the string contained
 * in the g_status array at the index corresponding to the enumerator.
 */

extern ag_string *
ag_http_status_str(enum ag_http_status status)
{
        AG_ASSERT (status >= AG_HTTP_STATUS_200_OK &&
            status <= AG_HTTP_STATUS_501_NOT_IMPLEMENTED);

        return ag_string_new(g_status[status]);
}


