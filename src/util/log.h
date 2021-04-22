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

#ifndef __ARGENT_INCLUDE_LOG_H__
#define __ARGENT_INCLUDE_LOG_H__

#ifdef __cplusplus
        extern "C" {
#endif


enum ag_log_level {
        AG_LOG_LEVEL_EMERG,
        AG_LOG_LEVEL_ALERT,
        AG_LOG_LEVEL_CRIT,
        AG_LOG_LEVEL_ERR,
        AG_LOG_LEVEL_WARNING,
        AG_LOG_LEVEL_NOTICE,
        AG_LOG_LEVEL_INFO,
        AG_LOG_LEVEL_DEBUG
};

                
extern void ag_log_emerg(const char *, ...);
extern void ag_log_alert(const char *, ...);
extern void ag_log_crit(const char *, ...);
extern void ag_log_err(const char *, ...);
extern void ag_log_warning(const char *, ...);
extern void ag_log_notice(const char *, ...);
extern void ag_log_info(const char *, ...);
extern void ag_log_debug(const char *, ...);


#ifdef __cplusplus
        }
#endif


#endif /*!__ARGENT_INCLUDE_LOG_H__ */

