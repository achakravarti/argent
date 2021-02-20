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


#include "./test.h"
#include "../include/argent.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
        (void) argc;
        (void) argv;

        ag_init();
        (void)system("mkdir -p bld/test");

        ag_test_harness *th = ag_test_harness_new();

        ag_test_suite *log = test_suite_log();
        ag_test_suite *mblock = test_suite_memblock();
        ag_test_suite *str = test_suite_string();
        ag_test_suite *obj = test_suite_object();
        ag_test_suite *val = test_suite_value();
        ag_test_suite *fld = test_suite_field();
        ag_test_suite *lst = test_suite_list();
        ag_test_suite *alst = test_suite_alist();
        ag_test_suite *http_enum = test_suite_http_enum();
        ag_test_suite *url = test_suite_http_url();
        ag_test_suite *client = test_suite_http_client();
        ag_test_suite *req = test_suite_http_request();
        ag_test_suite *resp = test_suite_http_response();
        ag_test_suite *plug = test_suite_plugin();

        ag_test_harness_push(th, log);
        ag_test_harness_push(th, mblock);
        ag_test_harness_push(th, str);
        ag_test_harness_push(th, obj);
        ag_test_harness_push(th, val);
        ag_test_harness_push(th, fld);
        ag_test_harness_push(th, lst);
        ag_test_harness_push(th, alst);
        ag_test_harness_push(th, http_enum);
        ag_test_harness_push(th, url);
        ag_test_harness_push(th, client);
        ag_test_harness_push(th, req);
        ag_test_harness_push(th, resp);
        ag_test_harness_push(th, plug);

        ag_test_suite_release(&log);
        ag_test_suite_release(&mblock);
        ag_test_suite_release(&str);
        ag_test_suite_release(&obj);
        ag_test_suite_release(&val);
        ag_test_suite_release(&fld);
        ag_test_suite_release(&lst);
        ag_test_suite_release(&alst);
        ag_test_suite_release(&http_enum);
        ag_test_suite_release(&url);
        ag_test_suite_release(&client);
        ag_test_suite_release(&req);
        ag_test_suite_release(&resp);
        ag_test_suite_release(&plug);

        ag_test_harness_exec(th);
        ag_test_harness_log(th, stdout);

        ag_test_harness_release(&th);

        (void)system("rm -rf bld/test");
        ag_exit(EXIT_SUCCESS);

        return 0;
}

