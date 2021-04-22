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


/*******************************************************************************
 * The global variable `g_init` is used to track whether or not the logging unit
 * has been initialised, and is required only for release builds. As is the norm
 * for Argent, we specify every static global variable to be thread-local.
 */

#ifndef NDEBUG
        static AG_THREADLOCAL bool      g_init = false;
#endif


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
 * `__ag_log_write__()` is a protected helper function used for writing a log
 * message without any code location metadata. Although this function has extern
 * visibility, it is __not__ part of the logging unit interface; it is called by
 * the following interface macros:
 *   - `ag_log_crit()`
 *   - `ag_log_warning()`
 *   - `ag_log_notice()`
 *   - `ag_log_info()`
 *
 * The log priority level is provided through the first parameter, and the log
 * message (which may be formatted) is passed through the second parameter. In
 * case the log message is formatted, then the format specifiers are passed
 * through the variable argument list.
 */

void
__ag_log_write__(int pr, const char *msg, ...)
{
        AG_ASSERT (g_init && "logging unit initialised");
        AG_ASSERT (*msg && "log message valid string");

        va_list ap;
        va_start(ap, msg);
        vsyslog(pr, msg, ap);
        va_end(ap);
}


/*******************************************************************************
 * `__ag_log_write_meta__()` is similar to `__ag_log_write__()`, except that it
 * appends the code location metadata when writing the log message. It is used
 * by the following interface macros:
 *   - `ag_log_emerg()`
 *   - `ag_log_alert()`
 *   - `ag_log_err()`
 *   - `ag_log_debug()`
 *
 * Since these particular macros are used to log messages for diagnosing adverse
 * conditions, referring to the code location metadata (function, file and line
 * number) is useful. The function name is especially useful for pin-pointing
 * metaprogrammatically generated code.
 *
 * The code location metadata is passed through the first three parameters, and
 * the remaining parameters are the same as in the case of `__ag_log_write__()`.
 */

void
__ag_log_write_meta__(const char *func, const char *file, int line, int pr,
    const char *msg, ...)
{
        AG_ASSERT (g_init && "logging unit initialised");
        AG_ASSERT (*func && "function name valid string");
        AG_ASSERT (*file && "file path valid string");
        AG_ASSERT (line && "file line valid natural number");
        AG_ASSERT (*msg && "log message valid string");
        
        char meta[1024];
        snprintf(meta, 1024, "[%s() @ %s:%d]", func, file, line);
        
        char body[1024];
        va_list ap;
        va_start(ap, msg);
        vsnprintf(body, 1024, msg, ap);
        va_end(ap);
        
        syslog(pr, "%s %s", body, meta);
}

