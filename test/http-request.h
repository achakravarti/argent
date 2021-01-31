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


#ifndef __ARGENT_TEST_HTTP_REQUEST_H__
#define __ARGENT_TEST_HTTP_REQUEST_H__


/*
 * Define the AG_SAMPLE_HTTP_REQUESTT() macro. This macro is responsible for
 * metaprogrammatically generating a static inline function that returns a
 * pointer to a sample HTTP request object with a given set of properties.
 *
 * The first parameter to this macro is a tag used to uniquely identify a given
 * sample HTTP client object, and the other parameters are, in sequence, those
 * of ag_http_request_new(). The name of the generated function is the same as
 * the specified tag.
 */
#define AG_SAMPLE_HTTP_REQUEST(tag, method, mime, url, client, param)   \
        static inline ag_http_request *tag(void)                        \
        {                                                               \
                AG_AUTO(ag_http_url) *u = url;                          \
                AG_AUTO(ag_http_client) *c = client;                    \
                AG_AUTO(ag_alist) *p = param;                           \
                return ag_http_request_new(method, mime, u, c, p);      \
        }


#endif /* !__AG_HTTP_REQUEST_H__ */
