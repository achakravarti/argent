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


#ifndef __ARGENT_HTTP_H__
#define __ARGENT_HTTP_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "./object.h"


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
 * creates a URL object without a port number.
 */


extern ag_http_url      *ag_http_url_new(bool, const char *, ag_uint,
                            const char *);


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


AG_OBJECT_DECLARE(ag_http_client, AG_TYPEID_HTTP_CLIENT);


extern ag_http_client   *ag_http_client_new(const char *, ag_uint, const char *,
                            const char *, const char *);


extern ag_string        *ag_http_client_ip(const ag_http_client *);
extern ag_uint           ag_http_client_port(const ag_http_client *);
extern ag_string        *ag_http_client_host(const ag_http_client *);
extern ag_string        *ag_http_client_agent(const ag_http_client *);
extern ag_string        *ag_http_client_referer(const ag_http_client *);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_HTTP_H__ */

