#include "../include/argent.h"


extern void
ag_exception_regex_hnd(const struct ag_exception *ex, void *opt)
{
        struct ag_exception_regex *x = opt;
        
        printf("[!] %d [%s(), %s:%lu]: %s\n", ex->erno, ex->func, ex->file,
            ex->line, ag_exception_registry_msg(ex->erno));
        
        ag_log_err("%d [%s(), %s:%lu]: %s", ex->erno, ex->func, ex->file,
            ex->line, ag_exception_registry_msg(ex->erno));

        const char *msg = "[!] string = %s, regex = %s\n";
        printf(msg, x->str, x->regex);
        ag_log_err(msg, x->str, x->regex);

        ag_exit(EXIT_FAILURE);
}


