#include <syslog.h>
#include <stdarg.h>
#include "./api.h"


/*******************************************************************************
 *                              LOGGING INTERNALS
 */

                                         /* writes a syslog message [AgDM:??] */
static inline void log_write(int pr, const char *fmt, va_list ap)
{
    openlog(NULL, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
    vsyslog(pr, fmt, ap);
    closelog();
}


/*******************************************************************************
 *                              LOGGING EXTERNALS
 */

                                /* implementation of ag_log_emerg() [AgDM:??] */
extern void ag_log_emerg(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_EMERG, fmt, ap);
    va_end(ap);
}

                                /* implementation of ag_log_alert() [AgDM:??] */
extern void ag_log_alert(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_ALERT, fmt, ap);
    va_end(ap);
}

                                 /* implementation of ag_log_crit() [AgDM:??] */
extern void ag_log_crit(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_CRIT, fmt, ap);
    va_end(ap);
}

                                  /* implementation of ag_log_err() [AgDM:??] */
extern void ag_log_err(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_ERR, fmt, ap);
    va_end(ap);
}

                              /* implementation of ag_log_warning() [AgDM:??] */
extern void ag_log_warning(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_WARNING, fmt, ap);
    va_end(ap);
}

                               /* implementation of ag_log_notice() [AgDM:??] */
extern void ag_log_notice(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_NOTICE, fmt, ap);
    va_end(ap);
}

                                 /* implementation of ag_log_info() [AgDM:??] */
extern void ag_log_info(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_INFO, fmt, ap);
    va_end(ap);
}

                                /* implementation of ag_log_debug() [AgDM:??] */
extern void ag_log_debug(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_DEBUG, fmt, ap);
    va_end(ap);
}

