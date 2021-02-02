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




extern ag_http_response *
ag_http_response_new(enum ag_http_mime mime, enum ag_http_status status, 
    const char *body)
{
}




extern ag_http_response *
ag_http_response_new_file(enum ag_http_mime mime, enum ag_http_status status,
    const char *path)
{
}




extern ag_http_response *
ag_http_response_new_empty(enum ag_http_mime mime, enum ag_http_status status)
{
}




extern ag_string *
ag_http_response_header(const ag_http_response *ctx);




extern ag_string *
ag_http_response_body(const ag_http_response *ctx)
{
}




extern void
ag_http_response_add(ag_http_response **ctx, const char *body);




extern void
ag_http_response_add_file(ag_http_response **ctx, const char *path)
{
}




extern void             ag_http_response_flush(ag_http_response **);
