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


static AG_THREADLOCAL bool g_init = false;


/*******************************************************************************
 * `LOG_WRITE()` is a helper macro for the log writing functions.  Since these
 * functions are semantically similar except for the log priority level,
 * `LOG_WRITE()` abstracts the code that is common to them.  We've used a macro
 * instead of a helper function in order to avoid the complication of passing
 * around variable argument lists.
 */

#define LOG_WRITE(MSG, LVL)     \
        AG_ASSERT_STR (MSG);    \
        va_list ap;             \
        va_start(ap, MSG);      \
        vsyslog(LVL, MSG, ap);  \
        va_end(ap);


extern void
ag_log_init(const char *ident)
{
        AG_ASSERT_STR (ident);

        openlog(ident, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
        g_init = true;
        ag_log_info("starting log");
}


extern void
ag_log_exit(void)
{
        AG_ASSERT_TAG ("LOG_INIT", g_init);

        ag_log_info("stopping log");
        closelog();
        g_init = false;
}


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
        LOG_WRITE(msg, lvl);
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

