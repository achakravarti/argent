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
extern void ag_test_case_free(ag_test_case **);

extern enum ag_test_status ag_test_case_status(const ag_test_case *);

extern void ag_test_case_desc_set(ag_test_case *, const char *);
extern void ag_test_case_exec(ag_test_case *);
extern void ag_test_case_log(const ag_test_case *, FILE *);


/*-
 * Interface: Test Suite
 */


typedef struct ag_test_suite ag_test_suite;

extern ag_test_suite *ag_test_suite_new(const char *);
extern ag_test_suite *ag_test_suite_copy(const ag_test_suite *);
extern void ag_test_suite_free(ag_test_suite **);

extern size_t ag_test_suite_len(const  ag_test_suite *);
extern size_t ag_test_suite_poll(const ag_test_suite *, enum ag_test_status);

extern void ag_test_suite_push(ag_test_suite *, const ag_test_case *);
extern void ag_test_suite_exec(ag_test_suite *);
extern void ag_test_suite_log(const ag_test_suite *, FILE *);


//////////


typedef enum ag_test_status (_ag_test)(void);
typedef struct _ag_test_suite _ag_test_suite;

extern _ag_test_suite *_ag_test_suite_new(const char *, FILE *);
extern _ag_test_suite *_ag_test_suite_copy(const _ag_test_suite *);
extern void ag_test_suties_free(_ag_test_suite **);

extern size_t _ag_test_suite_len(const _ag_test_suite *);
extern size_t _ag_test_suite_poll(const _ag_test_suite *, enum ag_test_status);

extern void _ag_test_suite_push(_ag_test_suite *, _ag_test *, const char *);
extern void _ag_test_suite_push_array(_ag_test_suite *, _ag_test *[],
                const char *[], size_t);
extern void _ag_test_suite_exec(_ag_test_suite *);

/*-
 * Interface: Test Harness
 */


typedef struct ag_test_harness ag_test_harness;

extern ag_test_harness *ag_test_harness_new(void);
extern ag_test_harness *ag_test_harness_copy(const ag_test_harness *);
extern void ag_test_harness_free(ag_test_harness **);

extern int ag_test_harness_len(const ag_test_harness *);
extern size_t ag_test_harness_poll(const ag_test_harness *,
                enum ag_test_status);

extern void ag_test_harness_push(ag_test_harness *, const ag_test_suite *);
extern void ag_test_harness_exec(ag_test_harness *);
extern void ag_test_harness_log(const ag_test_harness *, FILE *);


#ifdef __cplusplus
        }
#endif

#endif /* !__ARGENT_TEST_H__ */

