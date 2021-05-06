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
 * The global variable `g_init` is used to track whether or not the logging unit
 * has been initialised, and is required only for release builds. As is the norm
 * for Argent, we specify every static global variable to be thread-local.
 */

#ifndef NDEBUG
        static AG_THREADLOCAL bool      g_init = false;
#endif


/*******************************************************************************
 * `LOG_WRITE()` is a helper macro for the log writing functions.  Since these
 * functions are semantically similar except for the log priority level,
 * `LOG_WRITE()` abstracts the code that is common to them.  We've used a macro
 * instead of a helper function in order to avoid the complication of passing
 * around variable argument lists.
 */

#define WRITE_NOMETA(L, M) do {                                 \
        AG_ASSERT (g_init && "logging unit initialised");       \
        AG_ASSERT (*M && "message valid string");               \
        va_list ap;                                             \
        va_start(ap, M);                                        \
        vsyslog(L, M, ap);                                      \
        va_end(ap);                                             \
} while (0)


#define WRITE_META(L, FN, FL, LN, M) do {                       \
        AG_ASSERT (*FN && "function name valid string");        \
        AG_ASSERT (*FL && "file path valid string");            \
        AG_ASSERT (*M && "log message valid string");           \
        char meta[1024];                                        \
        snprintf(meta, 1024, "[%s() @ %s:%d]", FN, FL, LN);     \
        char body[1024];                                        \
        va_list ap;                                             \
        va_start(ap, M);                                        \
        vsnprintf(body, 1024, M, ap);                           \
        va_end(ap);                                             \
        syslog(L, "%s %s", body, meta);                         \
} while (0)


/*******************************************************************************
 * `ag_log_init()` initialises the logging unit of the Argent Library. The
 * log writing functions will not work as expected unless this function is first
 * called. The identity of the client program is passed through the only
 * parameter. `ag_init()` calls this function with `argv[0]`, so client code
 * does not need to call this function directly.
 */

void
ag_log_init(const char *ident)
{
        AG_ASSERT (!g_init && "logging unit not initialised");
        AG_ASSERT (*ident && "log identity valid string");

        openlog(ident, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
        g_init = true;

        ag_log_info("log started");
}


/*******************************************************************************
 * `ag_log_exit()` shuts down the logging unit of the Argent Library, and is
 * expected to be called as part of the finalisation of a program. Client code
 * does not need to call this function directly since `ag_exit()` already does
 * so.
 */

void
ag_log_exit(void)
{
        AG_ASSERT (g_init && "logging unit initialised");

        ag_log_info("stopping log");

        closelog();
        g_init = false;
}


/*******************************************************************************
 * `ag_log_emerg()` is a convenience wrapper around `ag_log_write()` that logs a
 * formatted emergency message to the system log. The message is passed through
 * the first parameter, and the format specifiers are passed through the
 * variable argument list. The format specifiers are required only if the
 * message is formatted.
 */

void
ag_log_emerg(const char *msg, ...)
{
        WRITE_NOMETA(AG_LOG_LEVEL_EMERG, msg);
}


/*******************************************************************************
 * `ag_log_crit()` is a convenience wrapper around `ag_log_write()` that logs a
 * formatted critical message to the system log. The parameters are semantically
 * the same as `ag_log_emerg()`.
 */

void
ag_log_crit(const char *msg, ...)
{
        WRITE_NOMETA(AG_LOG_LEVEL_CRIT, msg);

}


/*******************************************************************************
 * `ag_log_warning()` is a convenience wrapper around `ag_log_write()` that logs
 * a formatted warning message to the system log. The parameters are
 * semantically the same as `ag_log_emerg()`.
 */

void
ag_log_warning(const char *msg, ...)
{
        WRITE_NOMETA(AG_LOG_LEVEL_WARNING, msg);
}


/*******************************************************************************
 * `ag_log_notice()` is a convenience wrapper around `ag_log_write()` that logs
 * a formatted notice message to the system log. The parameters are semantically
 * the same as `ag_log_emerg()`.
 */

void 
ag_log_notice(const char *msg, ...)
{
        WRITE_NOMETA(AG_LOG_LEVEL_NOTICE, msg);
}


/*******************************************************************************
 * `ag_log_info()` is a convenience wrapper around `ag_log_write()` that logs a
 * formatted information message to the system log. The parameters are
 * semantically the same as `ag_log_emerg()`.
 */

void
ag_log_info(const char *msg, ...)
{
        WRITE_NOMETA(AG_LOG_LEVEL_INFO, msg);
}


/*******************************************************************************
 * `ag_log_alert()` is a convenience wraapper around `ag_log_write()` that logs
 * a formatted alert message to the system log. The parameters are semantically
 * the same as `ag_log_emerg()`.
 */

void
__ag_log_alert__(const char *func, const char *file, int line, const char *msg,
    ...)
{
        WRITE_META(AG_LOG_LEVEL_ALERT, func, file, line, msg);
}


/*******************************************************************************
 * `ag_log_err()` is a convenience wrapper aournd `ag_log_write()` that logs a
 * formatted error message to the system log. The parameters are semantically
 * the same as `ag_log_emerg()`.
 */

void
__ag_log_err__(const char *func, const char *file, int line, const char *msg,
    ...)
{
        WRITE_META(AG_LOG_LEVEL_ERR, func, file, line, msg);
}


/*******************************************************************************
 * `ag_log_debug()` is a convenience wrapper around `ag_log_write()` that logs a
 * formatted debug message to the system log. The parameters are semantically
 * the same as `ag_log_emerg()`. This function is only available for release
 * builds.
 */

#ifndef NDEBUG
void
__ag_log_debug__(const char *func, const char *file, int line, const char *msg,
    ...)
{
        WRITE_META(AG_LOG_LEVEL_DEBUG, func, file, line, msg);
}
#endif

