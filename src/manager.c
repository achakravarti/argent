#include "../include/argent.h"

#include <stdlib.h>



#define MSG_MBLOCK "AG_ERNO_MBLOCK: failed to allocated heap memory block"


extern void ag_init(void)
{
        ag_exception_registry_init(32);

        ag_exception_registry_set(AG_ERNO_MBLOCK, MSG_MBLOCK,
                        &ag_mblock_exception_handler);
}


extern void ag_exit(int status)
{
        ag_exception_registry_exit();
        exit(status);
}

