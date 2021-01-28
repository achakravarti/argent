#ifndef __ARGENT_INCLUDE_LOG_H__
#define __ARGENT_INCLUDE_LOG_H__

#ifdef __cplusplus
        extern "C" {
#endif

                
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

