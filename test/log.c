#if 0
#include "../src/api.h"
#include "./test.h"


/*******************************************************************************
 *                                  TEST CASES
 */

                                    /* test case for ag_log_emerg() [AgDM:??] */
static void emerg_test(void)
{
    printf("ag_log_emerg() logs an emergency message");
    ag_log_emerg("Testing ag_log_emerg()...");
    
    char *cmd = "journalctl -t ag-tests -p \"emerg\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                    /* test case for ag_log_alert() [AgDM:??] */
static void alert_test(void)
{
    printf("ag_log_alert() logs an alert message");
    ag_log_alert("Testing ag_log_alert()...");
    
    char *cmd = "journalctl -t ag-tests -p \"alert\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                     /* test case for ag_log_crit() [AgDM:??] */
static void crit_test(void)
{
    printf("ag_log_crit() logs a critical message");
    ag_log_crit("Testing ag_log_crit()...");
    
    char *cmd = "journalctl -t ag-tests -p \"crit\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                      /* test case for ag_log_err() [AgDM:??] */
static void err_test(void)
{
    printf("ag_log_err() logs an error message");
    ag_log_err("Testing ag_log_err()...");
    
    char *cmd = "journalctl -t ag-tests -p \"err\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                  /* test case for ag_log_warning() [AgDM:??] */
static void warning_test(void)
{
    printf("ag_log_warning() logs a warning message");
    ag_log_warning("Testing ag_log_warning()...");
    
    char *cmd = "journalctl -t ag-tests -p \"warning\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                   /* test case for ag_log_notice() [AgDM:??] */
static void notice_test(void)
{
    printf("ag_log_notice() logs a notice message");
    ag_log_notice("Testing ag_log_notice()...");

    char *cmd = "journalctl -t ag-tests -p \"notice\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                     /* test case for ag_log_info() [AgDM:??] */
static void info_test(void)
{
    printf("ag_log_info() logs an information message");
    ag_log_info("Testing ag_log_info()...");
    
    char *cmd = "journalctl -t ag-tests -p \"info\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);

    printf("...OK\n");
}

                                    /* test case for ag_log_debug() [AgDM:??] */
static void debug_test(void)
{
    printf("ag_log_debug() logs a debug message");
    ag_log_debug("Testing ag_log_debug()...");

    char *cmd = "journalctl -t ag-tests -p \"debug\" -S \"5 sec ago\""
            " | grep \"No entries\"";
    ag_require (system(cmd), AG_ERNO_TEST, NULL);


    printf("...OK\n");
}


/*******************************************************************************
 *                                  TEST SUITE
 */

                            /* implementation of logging test suite [AgDM:??] */
extern void ag_test_log(void)
{
    printf("===============================================================\n");
    printf("Starting logging interface test suite...\n\n");

    emerg_test();
    alert_test();
    crit_test();
    err_test();
    warning_test();
    notice_test();
    info_test();
    debug_test();
    
    printf("\n");
}
#endif
