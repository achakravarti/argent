#include "../include/argent.h"

#include <stdlib.h>


struct node {
        ag_erno                  erno;
        char                    *msg;
        ag_exception_handler    *hnd;
};


extern void
ag_init(void)
{
        struct node list[] = {
                {
                        .erno = AG_ERNO_MBLOCK, 
                        .msg = AG_ERNO_MSG(AG_ERNO_MBLOCK),
                        .hnd = &ag_memblock_exception_handler
                },
        };

        ag_exception_registry_init(32);
        ag_object_registry_init();

        register struct node n;
        register size_t len = sizeof list / sizeof *list;

        for (register size_t i = 0; i < len; i++) {
                n = list[i];
                ag_exception_registry_set(n.erno, n.msg, n.hnd);
        }
}


extern void
ag_exit(int status)
{
        ag_object_registry_exit();
        ag_exception_registry_exit();

        exit(status);
}

