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


#include "../argent.h"

#include <stdlib.h>


struct node {
        ag_erno                  erno;
        char                    *msg;
        ag_exception_hnd        *hnd;
};


extern void
ag_init(void)
{
        struct node list[] = {
                {
                        .erno = AG_ERNO_MBLOCK, 
                        .msg = AG_ERNO_MSG(AG_ERNO_MBLOCK),
                        .hnd = &ag_exception_hnd_memblock
                },
                {
                        .erno = AG_ERNO_REGEX, 
                        .msg = AG_ERNO_MSG(AG_ERNO_REGEX),
                        .hnd = &ag_exception_hnd_regex,
                },
                {
                        .erno = AG_ERNO_PARSE, 
                        .msg = AG_ERNO_MSG(AG_ERNO_PARSE),
                        .hnd = &ag_exception_hnd_parse,
                },
        };

        ag_exception_registry_init();
        ag_object_registry_init();

        register struct node n;
        register size_t len = sizeof list / sizeof *list;

        for (register size_t i = 0; i < len; i++) {
                n = list[i];
                ag_exception_registry_set(n.erno, n.msg, n.hnd);
        }

        AG_OBJECT_REGISTER(ag_field);
        AG_OBJECT_REGISTER(ag_list);
        AG_OBJECT_REGISTER(ag_alist);
        AG_OBJECT_REGISTER(ag_http_url);
        AG_OBJECT_REGISTER(ag_http_client);
        AG_OBJECT_REGISTER(ag_http_request);
        AG_OBJECT_REGISTER(ag_http_response);
        AG_OBJECT_REGISTER(ag_plugin);
}


extern void
ag_exit(int status)
{
        ag_object_registry_exit();
        ag_exception_registry_exit();

        exit(status);
}

