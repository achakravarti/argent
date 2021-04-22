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


#define LOG_WRITE(FMT, LVL)             \
        AG_ASSERT_STR (FMT);            \
        va_list ap;                     \
        va_start(ap, FMT);              \
        ag_log_write(LVL, FMT, ap);     \
        va_end(ap);

/*******************************************************************************
 *                              LOGGING EXTERNALS
 */


extern void 
ag_log_write(enum ag_log_level lvl, const char *fmt, ...)
{
        AG_ASSERT_STR (fmt);

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
        va_start(ap, fmt);
        vsyslog(map[lvl], fmt, ap);
        va_end(ap);

        closelog();
}

                                /* implementation of ag_log_emerg() [AgDM:??] */
extern void
ag_log_emerg(const char *fmt, ...)
{
        LOG_WRITE(fmt, AG_LOG_LEVEL_EMERG);
}


                                /* implementation of ag_log_alert() [AgDM:??] */
extern void
ag_log_alert(const char *fmt, ...)
{
        LOG_WRITE(fmt, AG_LOG_LEVEL_ALERT);
}


                                 /* implementation of ag_log_crit() [AgDM:??] */
extern void
ag_log_crit(const char *fmt, ...)
{
        LOG_WRITE(fmt, AG_LOG_LEVEL_CRIT);

}


                                  /* implementation of ag_log_err() [AgDM:??] */
extern void
ag_log_err(const char *fmt, ...)
{
        LOG_WRITE(fmt, AG_LOG_LEVEL_ERR);
}


                              /* implementation of ag_log_warning() [AgDM:??] */
extern void
ag_log_warning(const char *fmt, ...)
{
        LOG_WRITE(fmt, AG_LOG_LEVEL_WARNING);
}


                               /* implementation of ag_log_notice() [AgDM:??] */
extern void 
ag_log_notice(const char *fmt, ...)
{
        LOG_WRITE(fmt, AG_LOG_LEVEL_NOTICE);
}


                                 /* implementation of ag_log_info() [AgDM:??] */
extern void
ag_log_info(const char *fmt, ...)
{
        LOG_WRITE(fmt, AG_LOG_LEVEL_INFO);
}


                                /* implementation of ag_log_debug() [AgDM:??] */
extern void
ag_log_debug(const char *fmt, ...)
{
        LOG_WRITE(fmt, AG_LOG_LEVEL_DEBUG);
}

