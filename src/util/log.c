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


#include "../argent.h"

#include <syslog.h>
#include <stdarg.h>


/*******************************************************************************
 *                              LOGGING INTERNALS
 */

                                         /* writes a syslog message [AgDM:??] */
static inline void
log_write(int pr, const char *fmt, va_list ap)
{
    openlog(NULL, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
    vsyslog(pr, fmt, ap);
    closelog();
}


/*******************************************************************************
 *                              LOGGING EXTERNALS
 */

                                /* implementation of ag_log_emerg() [AgDM:??] */
extern void
ag_log_emerg(const char *fmt, ...)
{
    AG_ASSERT_PTR (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_EMERG, fmt, ap);
    va_end(ap);
}


                                /* implementation of ag_log_alert() [AgDM:??] */
extern void
ag_log_alert(const char *fmt, ...)
{
    AG_ASSERT_PTR (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_ALERT, fmt, ap);
    va_end(ap);
}


                                 /* implementation of ag_log_crit() [AgDM:??] */
extern void
ag_log_crit(const char *fmt, ...)
{
    AG_ASSERT_PTR (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_CRIT, fmt, ap);
    va_end(ap);
}


                                  /* implementation of ag_log_err() [AgDM:??] */
extern void
ag_log_err(const char *fmt, ...)
{
    AG_ASSERT_PTR (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_ERR, fmt, ap);
    va_end(ap);
}


                              /* implementation of ag_log_warning() [AgDM:??] */
extern void
ag_log_warning(const char *fmt, ...)
{
    AG_ASSERT_PTR (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_WARNING, fmt, ap);
    va_end(ap);
}


                               /* implementation of ag_log_notice() [AgDM:??] */
extern void 
ag_log_notice(const char *fmt, ...)
{
    AG_ASSERT_PTR (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_NOTICE, fmt, ap);
    va_end(ap);
}


                                 /* implementation of ag_log_info() [AgDM:??] */
extern void
ag_log_info(const char *fmt, ...)
{
    AG_ASSERT_PTR (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_INFO, fmt, ap);
    va_end(ap);
}


                                /* implementation of ag_log_debug() [AgDM:??] */
extern void
ag_log_debug(const char *fmt, ...)
{
    AG_ASSERT_PTR (fmt && *fmt);
    va_list ap;
    va_start(ap, fmt);

    log_write(LOG_DEBUG, fmt, ap);
    va_end(ap);
}

