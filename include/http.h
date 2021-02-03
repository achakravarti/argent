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




#ifndef __ARGENT_INCLUDE_HTTP_H__
#define __ARGENT_INCLUDE_HTTP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "./alist.h"
#include "./object.h"




/*******************************************************************************
 * The ag_http_method enumeration defines the common HTTP methods (verbs) used
 * to make HTTP requests. We have also declared the prototypes of two supporting
 * functions, ag_http_method_parse() and ag_http_method_str(), which are used to
 * convert from and to the string representation of this enumeration.
 *
 * See the following files for more details:
 *   - src/http-enum.c
 *   - test/http-method.c
 */

enum ag_http_method {
        AG_HTTP_METHOD_GET,     /* GET    */
        AG_HTTP_METHOD_POST,    /* POST   */
        AG_HTTP_METHOD_PUT,     /* PUT    */
        AG_HTTP_METHOD_PATCH,   /* PATCH  */
        AG_HTTP_METHOD_DELETE,  /* DELETE */
};

extern enum ag_http_method       ag_http_method_parse(const char *);
extern ag_string                *ag_http_method_str(enum ag_http_method);




/*******************************************************************************
 * The ag_http_mime enumeration represents the common HTTP MIME types that are
 * used most often. As of now, this enumeration is only a subset of all possible
 * MIME types, and may be expanded in future if required.
 *
 * See the following URLs for a more complete list:
 *   - https://github.com/cujojs/rest/blob/master/docs/mime.md
 *   - https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/
 *     MIME_typesCommon_types
 *
 * We also declare the prototypes of the supporting functions, which are used to
 * convert from and to the string representation of the individual enumerators.
 *
 * See the following files for more details:
 *   - src/http-enum.c
 *   - test/http-mime.c
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

extern enum ag_http_mime         ag_http_mime_parse(const char *);
extern ag_string                *ag_http_mime_str(enum ag_http_mime);




/*******************************************************************************
 * The ag_http_status enumeration represents the commonly used subset of HTTP
 * response codes. Even from this subset, we anticipate that most would be
 * unused; however, they have been provided in case required by client code.
 *
 * Supporting the ag_http_status enumeration are two functions that help to
 * convert from and to the string representations of the individual enumerators:
 * ag_http_status_parse() and ag_http_status_str().
 *
 * See the following files for more details:
 *   - src/http-enum.c
 *   - test/http-status.c
 */

enum ag_http_status {
        AG_HTTP_STATUS_200_OK,                          /* 200 (OK)           */
        AG_HTTP_STATUS_201_CREATED,                     /* 201 (Created)      */
        AG_HTTP_STATUS_202_ACCEPTED,                    /* 202 (Accepted)     */
        AG_HTTP_STATUS_204_NO_CONTENT,                  /* 204 (No Content)   */
        AG_HTTP_STATUS_301_MOVED_PERMANENTLY,           /* 301 (Moved
                                                           Permanently)       */
        AG_HTTP_STATUS_302_FOUND,                       /* 302 (Found)        */
        AG_HTTP_STATUS_303_SEE_OTHER,                   /* 303 (See Other)    */
        AG_HTTP_STATUS_304_NOT_MODIFIED,                /* 304 (Not Modified) */
        AG_HTTP_STATUS_307_TEMPORARY_REDIRECT,          /* 307 (Temporary
                                                           Redirect)          */
        AG_HTTP_STATUS_400_BAD_REQUEST,                 /* 400 (Bad Request)  */
        AG_HTTP_STATUS_401_UNAUTHORIZED,                /* 401 (Unauthorized) */
        AG_HTTP_STATUS_403_FORBIDDEN,                   /* 403 (Forbidden)    */
        AG_HTTP_STATUS_404_NOT_FOUND,                   /* 404 (Not Found)    */
        AG_HTTP_STATUS_405_METHOD_NOT_ALLOWED,          /* 405 (Method Not
                                                           Allowed)           */
        AG_HTTP_STATUS_406_NOT_ACCEPTABLE,              /* 406 (Not
                                                           Acceptable)        */
        AG_HTTP_STATUS_412_PRECONDITION_FAILED,         /* 412 (Precondition
                                                           Failed)            */
        AG_HTTP_STATUS_415_UNSUPPORTED_MEDIA_TYPE,      /* 415 (Unsupported
                                                           Media Type)        */
        AG_HTTP_STATUS_500_INTERNAL_SERVER_ERROR,       /* 500 (Internal Server
                                                           Error)             */
        AG_HTTP_STATUS_501_NOT_IMPLEMENTED,             /* 501 (Not
                                                           Implemented)       */
};

extern enum ag_http_status       ag_http_status_parse(const char *);
extern ag_string                *ag_http_status_str(enum ag_http_status);




/*******************************************************************************
 * Declare the HTTP URL interface. An HTTP URL is the URL of the resource
 * requested by the client to the server, and makes up part of the HTTP request
 * specification. The Argent Library reifies HTTP URLs through the ag_http_url
 * object with the type ID AG_TYPEID_HTTP_URL.
 *
 * The ag_http_url object encapsulates the following four properties:
 *   1. The protocol (HTTP/HTTPS)
 *   2. The host
 *   3. The port number
 *   4. The path to the resource
 *
 * Only the port number is optional, and the ag_http_url object uses a port
 * number of 0 to indicate that is not applicable and/or available. If nothing
 * is specified for the path, then the ag_http_url object defaults its path to
 * the root (/).
 *
 * There are three manager functions which are responsible for creating a new
 * ag_http_url object instance:
 *   1. ag_http_url_new()
 *   2. ag_http_url_new_noport()
 *   3. ag_http_url_parse()
 *
 * The remaining functions are accessors to the properties of the ag_http_url
 * object. Since the AG_OBJECT_DECLARE() macro is used to declare the HTTP URL
 * object, the inhertied object functions are metaprogrammatically introduced.
 *
 * See the following files for more details:
 *   - include/typeid.h
 *   - include/object.h
 *   - src/http-url.c
 *   - test/http-url.c
 */

AG_OBJECT_DECLARE(ag_http_url, AG_TYPEID_HTTP_URL);

extern ag_http_url      *ag_http_url_new(bool, const char *, ag_uint,
                            const char *);
extern ag_http_url      *ag_http_url_new_noport(bool, const char *,
                            const char *);
extern ag_http_url      *ag_http_url_parse(const char *);

extern bool              ag_http_url_secure(const ag_http_url *ctx);
extern ag_string        *ag_http_url_host(const ag_http_url *ctx);
extern ag_uint           ag_http_url_port(const ag_http_url *ctx);
extern ag_string        *ag_http_url_path(const ag_http_url *ctx);




/*******************************************************************************
 * Declare the HTTP client interface. An HTTP client represents the client
 * making the HTTP request. Although the HTTP specification does not explicitly
 * mention a client type, we have introduced it for convenience. The Argent
 * Library refies HTTP clients through the ag_http_client object with the type
 * ID AG_TYPEID_HTTP_CLIENT.
 *
 * The ag_http_client object encapsulates the following properties:
 *   1. the IP address of the client
 *   2. the port number
 *   3. the user agent
 *   4. the referring URL
 *
 * All of these four properties have been made optional for the time being,
 * though in future this might change. A value of 0 indicates that the port
 * number is not available, and empty strings are used to denote that the other
 * properties are not available.
 *
 * There is only one manager function, ag_http_client_new(), which creates new
 * ag_http_client object instances from a given set of properties. The remaining
 * declared functions are all accessors to the properties. Since the HTTP client
 * object is declared through the AG_OBJECT_DECLARE() macro, the inherited
 * object methods are metaprogrammatically included.
 *
 * See the following files for more information:
 *   - include/typeid.h
 *   - include/object.h
 *   - src/http-client.c
 *   - test/http-client.c
 */

AG_OBJECT_DECLARE(ag_http_client, AG_TYPEID_HTTP_CLIENT);

extern ag_http_client   *ag_http_client_new(const char *, ag_uint, const char *,
                            const char *, const char *);

extern ag_string        *ag_http_client_ip(const ag_http_client *);
extern ag_uint           ag_http_client_port(const ag_http_client *);
extern ag_string        *ag_http_client_host(const ag_http_client *);
extern ag_string        *ag_http_client_agent(const ag_http_client *);
extern ag_string        *ag_http_client_referer(const ag_http_client *);




/*******************************************************************************
 * An HTTP request is one half of the HTTP protocol, representing a message sent
 * by the client to the server for some resource. The Argent Library reifies
 * HTTP requests through the ag_http_request object with the type ID
 * AG_TYPEID_HTTP_REQUEST.
 *
 * The ag_http_request object does not implement the HTTP request strictly
 * according to specification, and encapsulates only a subset of the properties
 * which are considered most useful. These are:
 *   1. The MIME type
 *   2. The HTTP method (verb)
 *   3. The HTTP client
 *   4. The HTTP URL
 *   5. The HTTP body key-value parameters
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
 *   - test/http-request.c
 */

AG_OBJECT_DECLARE(ag_http_request, AG_TYPEID_HTTP_REQUEST);

extern ag_http_request  *ag_http_request_new(enum ag_http_method,
                            enum ag_http_mime, const ag_http_url *,
                            const ag_http_client *, const ag_alist *);


extern enum ag_http_mime         ag_http_request_mime(const ag_http_request *);
extern enum ag_http_method       ag_http_request_method(
                                    const ag_http_request *);
extern ag_http_client           *ag_http_request_client(
                                    const ag_http_request *);
extern ag_http_url              *ag_http_request_url(const ag_http_request *);
extern ag_alist                 *ag_http_request_param(const ag_http_request *);




/*******************************************************************************
 * The other half of the HTTP protocol is the response sent by the server to the
 * client. The Argent Library reifies the HTTP response through the object
 * ag_http_response with type ID AG_TYPEID_HTTP_REQUEST.
 *
 * The ag_http_response object only has two properties: the header and the body.
 * The header specifies the response MIME type and the response status. The body
 * consists of the string representation of the resource to be returned.
 *
 * Supporting the ag_http_response object are the three manager functions
 * ag_http_response_new() and its two overloaded forms, the two accessor
 * functions ag_http_respone_header() and ag_http_response_body(), and the three
 * mutator functions ag_http_response_add(), ag_http_response_add_file(), and
 * ag_http_response_flush(). Since the ag_http_request object has been declared
 * through the AG_OBJECT_DECLARE() macro, its inherited object methods are added
 * metaprogrammatically.
 *
 * See the following files for more details:
 *   - include/typeid.h
 *   - include/object.h
 *   - src/http-response.c
 *   - test/http-response.c
 */

AG_OBJECT_DECLARE(ag_http_response, AG_TYPEID_HTTP_RESPONSE);

extern ag_http_response *ag_http_response_new(enum ag_http_mime,
                            enum ag_http_status, const char *);
extern ag_http_response *ag_http_response_new_file(enum ag_http_mime,
                            enum ag_http_status, const char *);
extern ag_http_response *ag_http_response_new_empty(enum ag_http_mime,
                            enum ag_http_status);

extern ag_string        *ag_http_response_header(const ag_http_response *);
extern ag_string        *ag_http_response_body(const ag_http_response *);

extern void             ag_http_response_add(ag_http_response **, const char *);
extern void             ag_http_response_add_file(ag_http_response **,
                            const char *);
extern void             ag_http_response_flush(ag_http_response **);




#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_INCLUDE_HTTP_H__ */

