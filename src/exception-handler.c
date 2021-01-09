#include "../include/argent.h"

        
extern void
ag_exception_memblock_hnd(const struct ag_exception *ex, void *opt)
{
        (void)ex;
        struct ag_exception_memblock *o = opt;

        if (o->align) {
                printf("[!] requested %lu bytes alignmed to %lu bytes\n",
                    o->sz, o->align);
                ag_log_err("requested %lu bytes aligned to %lu bytes",
                    o->sz, o->align);
        } else {
                printf("[!] requested %lu bytes\n", o->sz);
                ag_log_err("requested %lu bytes", o->sz);
        }

        ag_exit(EXIT_FAILURE);
}


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


