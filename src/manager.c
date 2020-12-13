#include "../include/argent.h"

#include <stdlib.h>



extern void ag_init(void)
{
        ag_exception_registry_init(32);

        ag_exception_registry_set(AG_ERNO_MBLOCK, AG_ERNO_MBLOCK_MSG,
                        &ag_mblock_exception_handler);
}


extern void ag_exit(int status)
{
        ag_exception_registry_exit();
        exit(status);
}

