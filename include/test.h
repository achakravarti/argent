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


/*-
 * Module: Testing
 *
 * The Testing Module is responsible for providing an interface for running unit
 * tests in a standardised manner, both for the Argent Library itself and for
 * client code.
 *
 * The Testing Module provides the following features:
 *      - hierarchical organisation of tests
 *      - skipping of debug tests
 *      - reporting of tests passed, failed and skipped
 *      - logging of test results
 *      - catching signals such as SIGABRT and SIGSEGV
 *
 * The Testing Module is organised around a hierarchy of entities, the most
 * fundamental being unit tests. Each unit test is wrapped within a test case,
 * and related test cases are collected together in a test suite. The individual
 * test suites are in turn collected within a test harness. Each of these
 * entities is reified as a type, and supported by a public interface.
 *
 * The implementation of the interfaces of this Module is divided logically
 * across the argent/src/test-*.c files.
 */


#ifndef __ARGENT_TEST_H__
#define __ARGENT_TEST_H__

#ifdef __cplusplus
        extern "C" {
#endif

#include <stddef.h>
#include <stdio.h>


/*-
 * Interface: Test Primitives
 */


enum ag_test_status {
        AG_TEST_STATUS_OK = 0,
        AG_TEST_STATUS_WAIT,
        AG_TEST_STATUS_SKIP,
        AG_TEST_STATUS_FAIL,
        AG_TEST_STATUS_SIGABRT,
        AG_TEST_STATUS_SIGSEGV,
        __AG_TEST_STATUS_LEN__
};


/*-
 * Interface: Test Case
 */


typedef struct ag_test_case ag_test_case;

typedef enum ag_test_status (ag_test)(ag_test_case *);

#define ag_test_assert(p) ((p) ? AG_TEST_STATUS_OK : AG_TEST_STATUS_FAIL)

#ifdef NDEBUG
#       define ag_test_assert_debug(p) AG_TEST_STATUS_SKIP
#else
#       define ag_test_assert_debug(p) ag_test_assert(p)
#endif

extern ag_test_case *ag_test_case_new(ag_test *);
extern ag_test_case *ag_test_case_copy(const ag_test_case *);
extern void ag_test_case_dispose(ag_test_case **);

extern enum ag_test_status ag_test_case_status(const ag_test_case *);
extern char *ag_test_case_desc(const ag_test_case *);
extern char *ag_test_case_str(const ag_test_case *);

extern void ag_test_case_desc_set(ag_test_case *, const char *);
extern void ag_test_case_exec(ag_test_case *);


/*-
 * Interface: Test Suite
 */


typedef struct ag_test_suite ag_test_suite;

extern ag_test_suite *ag_test_suite_new(const char *);
extern ag_test_suite *ag_test_suite_copy(const ag_test_suite *);
extern void ag_test_suite_dispose(ag_test_suite **);

extern size_t ag_test_suite_len(const  ag_test_suite *);
extern int ag_test_suite_pass(const ag_test_suite *);
extern int ag_test_suite_skip(const ag_test_suite *);
extern int ag_test_suite_fail(const ag_test_suite *);
extern char *ag_test_suite_str(const ag_test_suite *);

extern void ag_test_suite_push(ag_test_suite *, const ag_test_case *);
extern void ag_test_suite_exec(ag_test_suite *);
extern void ag_test_suite_exec_console(ag_test_suite *);
extern void ag_test_suite_exec_file(ag_test_suite *ctx, const char *);


/*-
 * Interface: Test Harness
 */


typedef struct ag_test_harness ag_test_harness;

extern ag_test_harness *ag_test_harness_new(void);
extern ag_test_harness *ag_test_harness_copy(const ag_test_harness *);
extern void ag_test_harness_dispose(ag_test_harness **);

extern int ag_test_harness_len(const ag_test_harness *);
extern int ag_test_harness_total(const ag_test_harness *);
extern int ag_test_harness_pass(const ag_test_harness *);
extern int ag_test_harness_skip(const ag_test_harness *);
extern int ag_test_harness_fail(const ag_test_harness *);
extern char *ag_test_harness_str(const ag_test_harness *);

extern void ag_test_harness_push(ag_test_harness *, const ag_test_suite *);
extern void ag_test_harness_exec(ag_test_harness *);
extern void ag_test_harness_exec_console(ag_test_harness *);
extern void ag_test_harness_exec_file(ag_test_harness *, const char *);



#ifdef __cplusplus
        }
#endif

#endif /* !__ARGENT_TEST_H__ */

