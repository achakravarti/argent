#include "../include/argent.h"
#include "./test.h"

#include <stdlib.h>


#define cmd_journalctl(log) "journalctl -t ag-tests -p \"" log  \
        "\" -S \"5 sec ago\" | grep \"No entries\""

#define log_check(level) system(cmd_journalctl(level))


AG_TEST_INIT(emerg_01, "ag_log_emerg() logs an emergency record") {
        ag_log_emerg("Testing ag_log_emerg()...");
        AG_TEST_ASSERT (log_check("emerg"));
} AG_TEST_EXIT();


AG_TEST_INIT(alert_01, "ag_log_alert() logs an alert record") {
        ag_log_alert("Testing ag_log_alert()...");
        AG_TEST_ASSERT (log_check("alert"));
} AG_TEST_EXIT();


AG_TEST_INIT(crit_01, "ag_log_crit() logs a critical record") {
        ag_log_crit("Testing ag_log_crit()...");
        AG_TEST_ASSERT (log_check("crit"));
} AG_TEST_EXIT();


AG_TEST_INIT(err_01, "ag_log_err() logs an error record") {
        ag_log_err("Testing ag_log_err()...");
        AG_TEST_ASSERT (log_check("err"));
} AG_TEST_EXIT();


AG_TEST_INIT(warning_01, "ag_log_warning() logs a warning record") {
        ag_log_warning("Testing ag_log_warning()...");
        AG_TEST_ASSERT (log_check("warning"));
} AG_TEST_EXIT();


AG_TEST_INIT(notice_01, "ag_log_notice() logs a notice record") {
        ag_log_notice("Testing ag_log_notice()...");
        AG_TEST_ASSERT (log_check("notice"));
} AG_TEST_EXIT();


AG_TEST_INIT(info_01, "ag_log_info() logs an information record") {
        ag_log_info("Testing ag_log_info()...");
        AG_TEST_ASSERT (log_check("info"));
} AG_TEST_EXIT();


AG_TEST_INIT(debug_01, "ag_log_debug() logs a debug record") {
        ag_log_debug("Testing ag_log_debug()...");
        AG_TEST_ASSERT (log_check("debug"));
} AG_TEST_EXIT();



extern ag_test_suite *test_log(void)
{
        ag_test *test[] = {
                emerg_01, alert_01, crit_01, err_01, warning_01, notice_01,
                info_01, debug_01,
        };

        const char *desc[] = {
                emerg_01_desc, alert_01_desc, crit_01_desc, err_01_desc,
                warning_01_desc, notice_01_desc, info_01_desc, debug_01_desc,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_log interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}



