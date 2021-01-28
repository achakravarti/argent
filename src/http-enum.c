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


/*
 * Define the ag_http_method_parse() interface function. This function is
 * responsible for parsing a given string and returning the HTTP method
 * enumerator represented by the string. In case the string contains something
 * not recognised as a valid enumerator, then AG_HTTP_METHOD_GET is returned.
 *
 * TODO: explore the possiblity of raising a runtime exception.
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
 * string. If no match is found, then AG_HTTP_MIME_TEXT_PLAIN is returned.
 *
 * TODO: explore throwing a runtime exception for parse failure.
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

