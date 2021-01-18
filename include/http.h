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


AG_OBJECT_DECLARE(ag_http_url, AG_TYPEID_HTTP_URL);
extern ag_http_url      *ag_http_url_new(bool, const char *, ag_uint,
                            const char *);


extern bool              ag_http_url_secure(const ag_http_url *ctx);
extern ag_string        *ag_http_url_host(const ag_http_url *ctx);
extern ag_uint           ag_http_url_port(const ag_http_url *ctx);
extern ag_string        *ag_http_url_path(const ag_http_url *ctx);


#ifdef __cplusplus
}
#endif

#endif /* !__ARGENT_HTTP_H__ */

