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


#ifndef __ARGENT_INCLUDE_HTTP_H__
#define __ARGENT_INCLUDE_HTTP_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "./alist.h"
#include "./object.h"


/*
 * Define the HTTP method enumeration. This enumeration defines the common HTTP
 * methods used to make HTTP requests.
 */
enum ag_http_method {
        AG_HTTP_METHOD_GET,     /* GET    */
        AG_HTTP_METHOD_POST,    /* POST   */
        AG_HTTP_METHOD_PUT,     /* PUT    */
        AG_HTTP_METHOD_PATCH,   /* PATCH  */
        AG_HTTP_METHOD_DELETE,  /* DELETE */
};


/*
 * Declare the function prototypes of the HTTP method enumeration. There are
 * only two functions in this interface, which are used to convert to and from
 * the string representation of this enumeration. ag_http_method_parse() parses
 * a string to determine the HTTP method enumerator, and ag_http_method_str()
 * does the converse, generating the string representation of an HTTP method
 * enumerator.
 */
extern enum ag_http_method       ag_http_method_parse(const char *);
extern ag_string                *ag_http_method_str(enum ag_http_method);


/*
 * Define the HTTP MIME type enumeration. This enumeration defines the common
 * HTTP MIME types that are used most often. This enumeration is only a subset
 * of all possible MIME types, and may be expanded in future if required. See
 * the following URLs for a more complete list:
 *   - https://github.com/cujojs/rest/blob/master/docs/mime.md
 *   - https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/
 *     MIME_typesCommon_types
 */
enum ag_http_mime {
        AG_HTTP_MIME_APPLICATION_FORM,  /* application/x-www-form-urlencoded */
        AG_HTTP_MIME_APPLICATION_JSON,  /* application/json                  */
        AG_HTTP_MIME_APPLICATION_OCTET, /* application/octet-stream          */
        AG_HTTP_MIME_APPLICATION_XML,   /* application/xml                   */
        AG_HTTP_MIME_MULTIPART_FORM,    /* multipart/form-data               */
        AG_HTTP_MIME_TEXT_CSS,          /* text/css                          */
        AG_HTTP_MIME_TEXT_CSV,          /* text/csv                          */
        AG_HTTP_MIME_TEXT_HTML,         /* text/html                         */
        AG_HTTP_MIME_TEXT_JS,           /* text/javascript                   */
        AG_HTTP_MIME_TEXT_PLAIN,        /* text/plain                        */
        AG_HTTP_MIME_TEXT_XML,          /* text/xml                          */
};


/*
 * Declare the function prototypes of the MIME enumeration. There are only two
 * functions in this interface, which are used to convert to and from string
 * representations. ag_http_mime_parse() parses a string to get the MIME enum it
 * represents, and ag_http_mime_str() does the opposite, converting a MIME enum
 * to its string representation.
 */
extern enum ag_http_mime         ag_http_mime_parse(const char *);
extern ag_string                *ag_http_mime_str(enum ag_http_mime);


/*
 * Declare the ag_http_url type. The ag_http_url type is an object with the
 * type ID of AG_TYPEID_HTTP_URL, and reifies an HTTP URL. An HTTP URL comprises
 * of four parts:
 *   1. The protocol (HTTP/HTTPS)
 *   2. The host
 *   3. The port number
 *   4. The path
 *
 * Only the port number is optional, and the ag_http_url type uses a port number
 * of 0 to indicate that it is not applicable. If nothing is specified for the
 * path, then the ag_http_url object defaults its path to the root (/).
 *
 * We use the AG_OBJECT_DECLARE() macro to declare ag_http_url as an object, and
 * to declare its inherited functions.  The following functions are declared
 * metaprogrammatically as part of the ag_http_url interface:
 *   - ag_http_url_copy()
 *   - ag_http_url_clone()
 *   - ag_http_url_release()
 *   - ag_http_url_cmp()
 *   - ag_http_url_lt()
 *   - ag_http_url_eq()
 *   - ag_http_url_gt()
 *   - ag_http_url_empty()
 *   - ag_http_url_typeid()
 *   - ag_http_url_uuid()
 *   - ag_http_url_valid()
 *   - ag_http_url_sz()
 *   - ag_http_url_refc()
 *   - ag_http_url_len()
 *   - ag_http_url_hash()
 *   - ag_http_url_str()
 */
AG_OBJECT_DECLARE(ag_http_url, AG_TYPEID_HTTP_URL);


/*
 * Declare the non-inherited manager interface for the ag_http_url object.
 * ag_http_url_new() creates a new URL object, and ag_http_url_new_noport()
 * creates a URL object without a port number. ag_http_url_parse() parses a
 * given string into an HTTP URL object.
 */
extern ag_http_url      *ag_http_url_new(bool, const char *, ag_uint,
                            const char *);
extern ag_http_url      *ag_http_url_new_noport(bool, const char *,
                            const char *);
extern ag_http_url      *ag_http_url_parse(const char *);


/*
 * Declare the non-inherited accessor interface for the ag_http_url object.
 * ag_http_url_secure() determines if a given URL object is secure, i.e.,
 * whether it is using the HTTPS protocol. ag_http_url_host() gets the hostname
 * component of a URL object, ag_http_url_port() gets the port number (0 if not
 * applicable), and ag_http_url_path() gets the path component of the URL.
 */
extern bool              ag_http_url_secure(const ag_http_url *ctx);
extern ag_string        *ag_http_url_host(const ag_http_url *ctx);
extern ag_uint           ag_http_url_port(const ag_http_url *ctx);
extern ag_string        *ag_http_url_path(const ag_http_url *ctx);


/*
 * Declare the ag_http_client object with the typeid AG_TYPEID_HTTP_CLIENT. This
 * object represents a client (human or otherwise) making an HTTP request. The
 * client is represented by five properties: the IP address, the port number,
 * the hostname, the user agent, and the URL from where the request originated.
 *
 * Since we're using the AG_OBJECT_DECLARE() macro to declare the client object,
 * the following inherited functions are also metaprogrammatically declared as
 * part of the ag_http_client interface:
 *   - ag_http_client_copy()
 *   - ag_http_client_clone()
 *   - ag_http_client_release()
 *   - ag_http_client_cmp()
 *   - ag_http_client_lt()
 *   - ag_http_client_eq()
 *   - ag_http_client_gt()
 *   - ag_http_client_empty()
 *   - ag_http_client_typeid()
 *   - ag_http_client_uuid()
 *   - ag_http_client_valid()
 *   - ag_http_client_sz()
 *   - ag_http_client_refc()
 *   - ag_http_client_len()
 *   - ag_http_client_hash()
 *   - ag_http_client_str()
 */
AG_OBJECT_DECLARE(ag_http_client, AG_TYPEID_HTTP_CLIENT);


/*
 * Declare the non-inherited manager interface for the ag_http_client object.
 * There is only one function in this interface, ag_http_client_new(), which is
 * responsible for creating a new client object.
 */
extern ag_http_client   *ag_http_client_new(const char *, ag_uint, const char *,
                            const char *, const char *);


/*
 * Declare the non-inherited accessor interface for the ag_http_client object.
 * Each of the five functions that make up this interface get a relevant
 * property of the contextual client object.
 *
 * ag_http_client_ip() gets the IP address of a client, ag_http_client_port()
 * the port number, ag_http_client_host() the hostname, ag_http_client_agent()
 * the user agent, and ag_http_client_referer() the referring URL from where the
 * client made the HTTP request.
 */
extern ag_string        *ag_http_client_ip(const ag_http_client *);
extern ag_uint           ag_http_client_port(const ag_http_client *);
extern ag_string        *ag_http_client_host(const ag_http_client *);
extern ag_string        *ag_http_client_agent(const ag_http_client *);
extern ag_string        *ag_http_client_referer(const ag_http_client *);


/*
 * Declare the HTTP request interface. An HTTP request is one half of HTTP,
 * representing a message sent by the client to the server for some resource.
 * The Argent Library reifies HTTP requests through the ag_http_request object
 * with the type ID AG_TYPEID_HTTP_REQUEST. 
 *
 * The ag_http_request object does not implement the HTTP request strictly
 * according to specification, and encapsulates only a subset of the properties
 * which are considered most useful.
 *
 * There is only one manager function, ag_http_request_new(), and the remaining
 * functions declared below are all accessors. Since the ag_http_request object
 * has been declared through the AG_OBJECT_DECLARE() macro, its inherited object
 * methods are metaprogrammatically declared.
 *
 * See the following files for more details:
 *   - include/typeid.h
 *   - include/object.h
 *   - src/http-request.c
 */


AG_OBJECT_DECLARE(ag_http_request, AG_TYPEID_HTTP_REQUEST);

extern ag_http_request  *ag_http_request_new(enum ag_http_method,
                            enum ag_http_mime, const ag_http_url *,
                            const ag_http_client *, const ag_alist *);


extern enum ag_http_mime         ag_http_request_mime(const ag_http_request *);
extern enum ag_http_method       ag_http_request_method(const ag_http_request *);
extern ag_http_client           *ag_http_request_client(const ag_http_request *);
extern ag_http_url              *ag_http_request_url(const ag_http_request *);
extern ag_alist                 *ag_http_request_param(const ag_http_request *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_INCLUDE_HTTP_H__ */

