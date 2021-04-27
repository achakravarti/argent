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


#include "../src/argent.h"
#include "./test.h"


/*
 * Define the ID of the test suite for the log interface. We need this ID for
 * the testing macros to correctly generate the boilerplate testing code.
 */


#define __AG_TEST_SUITE_ID__ 0


/*
 * The following macros work in conjunction to help determine whether a log
 * entry of a given level has been written to the systemd log. We use the
 * journalctl and grep commands to do so, which are available in both GNU/Linux
 * and FreeBSD. If journalctl can't find a specified log entry, it returns "No
 * entries". Therefore, if a log entry has been written, grep'ing for "No
 * entries" on journalctl will be invalid, and a wrapping system() call will
 * return a non-zero value.
 */

#define cmd_journalctl(level) "journalctl -t ./bld/ag-tests -p \"" level  \
        "\" -S \"5 sec ago\" | grep \"No entries\""

#define log_check(level) system(cmd_journalctl(level))


/* 
 * The following unit tests test out the logging interface of the Argent Library
 * by writing a one liner to the systemd log for each of the standard log
 * levels. We check that the log entry of the given log level has been written
 * through the log_check() macro. We know that lob_check() will evaluate to
 * non-zero if it is able to find the log entry of the given level.
 */


AG_TEST_CASE("ag_log_emerg() logs an emergency record")
{
        ag_log_emerg("Testing ag_log_emerg()...");
        AG_TEST (log_check("emerg"));
}


AG_TEST_CASE("ag_log_alert() logs an alert record")
{
        ag_log_alert("Testing ag_log_alert()...");
        AG_TEST (log_check("alert"));
}


AG_TEST_CASE("ag_log_crit() logs a critical record") 
{
        ag_log_crit("Testing ag_log_crit()...");
        AG_TEST (log_check("crit"));
}


AG_TEST_CASE("ag_log_err() logs an error record") 
{
        ag_log_err("Testing ag_log_err()...");
        AG_TEST (log_check("err"));
}


AG_TEST_CASE("ag_log_warning() logs a warning record") 
{
        ag_log_warning("Testing ag_log_warning()...");
        AG_TEST (log_check("warning"));
}


AG_TEST_CASE("ag_log_notice() logs a notice record") 
{
        ag_log_notice("Testing ag_log_notice()...");
        AG_TEST (log_check("notice"));
}


AG_TEST_CASE("ag_log_info() logs an information record") 
{
        ag_log_info("Testing ag_log_info()...");
        AG_TEST (log_check("info"));
}


AG_TEST_CASE("ag_log_debug() logs a debug record") 
{
        ag_log_debug("Testing ag_log_debug()...");
        AG_TEST (log_check("debug"));
}


/*
 * Generate a test suite containing the above unit tests and return it so that
 * it can be excecuted by a test harness.
 */


extern ag_test_suite *test_suite_log(void)
{
        return AG_TEST_SUITE_GENERATE("ag_log interface");
}

