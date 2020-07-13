#include <syslog.h>
#include <stdarg.h>
#include "./api.h"


/*******************************************************************************
 *                              LOGGING EXTERNALS
 */


static inline void log_write(int pr, const char *fmt, va_list ap)
{
    openlog(NULL, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
    vsyslog(pr, fmt, ap);
    closelog();
}


/*******************************************************************************
 *                              LOGGING EXTERNALS
 */


extern void ag_log_emergency(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_EMERG, fmt, ap);
    va_end(ap);
}


extern void ag_log_alert(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_ALERT, fmt, ap);
    va_end(ap);
}


extern void ag_log_critical(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_CRIT, fmt, ap);
    va_end(ap);
}


extern void ag_log_error(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_ERR, fmt, ap);
    va_end(ap);
}


extern void ag_log_warning(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_WARNING, fmt, ap);
    va_end(ap);
}


extern void ag_log_notice(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_NOTICE, fmt, ap);
    va_end(ap);
}


extern void ag_log_info(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_INFO, fmt, ap);
    va_end(ap);
}


extern void ag_log_debug(const char *fmt, ...)
{
    ag_assert (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_DEBUG, fmt, ap);
    va_end(ap);
}

