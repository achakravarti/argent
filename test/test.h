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


#ifndef __ARGENT_TEST_TEST_H__
#define __ARGENT_TEST_TEST_H__

#include "../include/argent.h"


extern ag_test_suite    *test_suite_log(void);
extern ag_test_suite    *test_suite_memblock(void);
extern ag_test_suite    *test_suite_string(void);
extern ag_test_suite    *test_suite_object(void);
extern ag_test_suite    *test_suite_value(void);
extern ag_test_suite    *test_suite_field(void);
extern ag_test_suite    *test_suite_list(void);
extern ag_test_suite    *test_suite_alist(void);
extern ag_test_suite    *test_suite_http_url(void);
extern ag_test_suite    *test_suite_http_client(void);
extern ag_test_suite    *test_suite_http_method(void);
extern ag_test_suite    *test_suite_http_mime(void);
extern ag_test_suite    *test_suite_http_request(void);


#endif /* !__ARGENT_TEST_TEST_H__ */

