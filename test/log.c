#include "../src/api.h"
#include "./test.h"


/*******************************************************************************
 *                                  TEST CASES
 */

static void emergency_test(void)
{
    printf("ag_log_emergency() logs an emergency message");
    ag_log_emergency("Testing ag_log_emergency()...");
    
    char *cmd = "journaltcl -t ag-tests -p \"emerg\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void alert_test(void)
{
    printf("ag_log_alert() logs an alert message");
    ag_log_alert("Testing ag_log_alert()...");
    
    char *cmd = "journaltcl -t ag-tests -p \"alert\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void critical_test(void)
{
    printf("ag_log_critical() logs a critical message");
    ag_log_critical("Testing ag_log_critical()...");
    
    char *cmd = "journaltcl -t ag-tests -p \"crit\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void error_test(void)
{
    printf("ag_log_error() logs an error message");
    ag_log_error("Testing ag_log_error()...");
    
    char *cmd = "journaltcl -t ag-tests -p \"err\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void warning_test(void)
{
    printf("ag_log_warning() logs a warning message");
    ag_log_warning("Testing ag_log_warning()...");
    
    char *cmd = "journaltcl -t ag-tests -p \"warning\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void notice_test(void)
{
    printf("ag_log_notice() logs a notice message");
    ag_log_notice("Testing ag_log_notice()...");

    char *cmd = "journaltcl -t ag-tests -p \"notice\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void info_test(void)
{
    printf("ag_log_info() logs an information message");
    ag_log_info("Testing ag_log_info()...");
    
    char *cmd = "journaltcl -t ag-tests -p \"info\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

static void debug_test(void)
{
    printf("ag_log_debug() logs a debug message");
    ag_log_debug("Testing ag_log_debug()...");

    char *cmd = "journaltcl -t ag-tests -p \"debug\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);


    printf("...OK\n");
}


/*******************************************************************************
 *                                  TEST SUITE
 */

                             /* implementation of string test suite [AgDM:??] */
extern void ag_test_log(void)
{
    printf("===============================================================\n");
    printf("Starting logging interface test suite...\n\n");

    emergency_test();
    alert_test();
    critical_test();
    error_test();
    warning_test();
    notice_test();
    info_test();
    debug_test();
    
    printf("\n");
}

