#include "../include/argent.h"


extern void
ag_exception_regex_hnd(const struct ag_exception *ex, void *opt)
{
        (void)ex;

        struct ag_exception_regex *o = opt;
        const char *msg = "[!] string = %s, regex = %s\n";

        printf(msg, o->str, o->regex);
        printf("\n");
        ag_log_err(msg, o->str, o->regex);

        ag_exit(EXIT_FAILURE);
}


