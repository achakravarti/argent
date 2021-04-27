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

#include <stdarg.h>
#include <syslog.h>


/*******************************************************************************
 * `LOG_WRITE()` is a helper macro for the `ag_log_write()` wrapper functions.
 * Since these wrapper functions are semantically similar except for the log
 * priority level, `LOG_WRITE()` abstracts the code that is common to them.
 * We've used a macro instead of a helper function in order to avoid the
 * complication of passing around variable argument lists.
 */

#define LOG_WRITE(FMT, LVL)             \
        AG_ASSERT_STR (FMT);            \
        va_list ap;                     \
        va_start(ap, FMT);              \
        ag_log_write(LVL, FMT, ap);     \
        va_end(ap);


/*******************************************************************************
 * `ag_log_write()` writes a formatted message to the system log with a given
 * priority level. The priority level is passed through the first parameter, the
 * message through the second, and the format specifiers through the variable
 * argument list. The format specifiers are rquired only if the message is
 * formatted.
 */

extern void 
ag_log_write(enum ag_log_level lvl, const char *msg, ...)
{
        AG_ASSERT_STR (msg);

        int map[] = {
                LOG_EMERG,
                LOG_ALERT,
                LOG_CRIT,
                LOG_ERR,
                LOG_WARNING,
                LOG_NOTICE,
                LOG_INFO,
                LOG_DEBUG
        };

        openlog(NULL, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);

        va_list ap;
        va_start(ap, msg);
        vsyslog(map[lvl], msg, ap);
        va_end(ap);

        closelog();
}


/*******************************************************************************
 * `ag_log_emerg()` is a convenience wrapper around `ag_log_write()` that logs a
 * formatted emergency message to the system log. The message is passed through
 * the first parameter, and the format specifiers are passed through the
 * variable argument list. The format specifiers are required only if the
 * message is formatted.
 */

extern void
ag_log_emerg(const char *msg, ...)
{
        LOG_WRITE(msg, AG_LOG_LEVEL_EMERG);
}


/*******************************************************************************
 * `ag_log_alert()` is a convenience wraapper around `ag_log_write()` that logs
 * a formatted alert message to the system log. The parameters are semantically
 * the same as `ag_log_emerg()`.
 */

extern void
ag_log_alert(const char *msg, ...)
{
        LOG_WRITE(msg, AG_LOG_LEVEL_ALERT);
}


/*******************************************************************************
 * `ag_log_crit()` is a convenience wrapper around `ag_log_write()` that logs a
 * formatted critical message to the system log. The parameters are semantically
 * the same as `ag_log_emerg()`.
 */

extern void
ag_log_crit(const char *msg, ...)
{
        LOG_WRITE(msg, AG_LOG_LEVEL_CRIT);

}


/*******************************************************************************
 * `ag_log_err()` is a convenience wrapper aournd `ag_log_write()` that logs a
 * formatted error message to the system log. The parameters are semantically
 * the same as `ag_log_emerg()`.
 */

extern void
ag_log_err(const char *msg, ...)
{
        LOG_WRITE(msg, AG_LOG_LEVEL_ERR);
}


/*******************************************************************************
 * `ag_log_warning()` is a convenience wrapper around `ag_log_write()` that logs
 * a formatted warning message to the system log. The parameters are
 * semantically the same as `ag_log_emerg()`.
 */

extern void
ag_log_warning(const char *msg, ...)
{
        LOG_WRITE(msg, AG_LOG_LEVEL_WARNING);
}


/*******************************************************************************
 * `ag_log_notice()` is a convenience wrapper around `ag_log_write()` that logs
 * a formatted notice message to the system log. The parameters are semantically
 * the same as `ag_log_emerg()`.
 */

extern void 
ag_log_notice(const char *fmt, ...)
{
        LOG_WRITE(fmt, AG_LOG_LEVEL_NOTICE);
}


/*******************************************************************************
 * `ag_log_info()` is a convenience wrapper around `ag_log_write()` that logs a
 * formatted information message to the system log. The parameters are
 * semantically the same as `ag_log_emerg()`.
 */

extern void
ag_log_info(const char *msg, ...)
{
        LOG_WRITE(msg, AG_LOG_LEVEL_INFO);
}


/*******************************************************************************
 * `ag_log_debug()` is a convenience wrapper around `ag_log_write()` that logs a
 * formatted debug message to the system log. The parameters are semantically
 * the same as `ag_log_emerg()`.
 */

extern void
ag_log_debug(const char *msg, ...)
{
        LOG_WRITE(msg, AG_LOG_LEVEL_DEBUG);
}
