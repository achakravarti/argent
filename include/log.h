#if (!defined __ARGENT_LOG_H__)
#define __ARGENT_LOG_H__

/*******************************************************************************
 *                                   LOGGING
 */



                                       /* logs an emergency message [AgDM:??] */
extern void ag_log_emerg(const char *fmt, ...);

                                           /* logs an alert message [AgDM:??] */
extern void ag_log_alert(const char *fmt, ...);

                                         /* logs a critical message [AgDM:??] */
extern void ag_log_crit(const char *fmt, ...);

                                           /* logs an error message [AgDM:??] */
extern void ag_log_err(const char *fmt, ...);

                                          /* logs a warning message [AgDM:??] */
extern void ag_log_warning(const char *fmt, ...);

                                           /* logs a notice message [AgDM:??] */
extern void ag_log_notice(const char *fmt, ...);

                                      /* logs an information message [AgDM:??] */
extern void ag_log_info(const char *fmt, ...);

                                            /* logs a debug message [AgDM:??] */
extern void ag_log_debug(const char *fmt, ...);


#endif /* !defined __ARGENT_LOG_H__ */

