#include "../include/argent.h"

#include <stdlib.h>

#define AG_ERNO_MBLOCK_NEW ((ag_erno) -1)
#define AG_ERNO_MBLOCK_NEW_ALIGN ((ag_erno) -2)
#define AG_ERNO_MBLOCK_RESIZE ((ag_erno) -3)


#define MSG_MBLOCK_NEW "AG_ERNO_MBLOCK_NEW: failed to create new memory block"
#define MSG_MBLOCK_NEW_ALIGN "AG_ERNO_MBLOCK_NEW_ALIGN: failed to create new" \
                " aligned memory block"
#define MSG_MBLOCK_RESIZE "AG_ERNO_MBLOCK_RESIZE: failed to resize memory block"


static void eh_mblock_new(const struct ag_exception *, void *);
static void eh_mblock_new_align(const struct ag_exception *, void *);
static void eh_mblock_resize(const struct ag_exception *, void *);


extern void ag_init(void)
{
        ag_exception_registry_init(32);

        ag_exception_registry_set(AG_ERNO_MBLOCK_NEW, MSG_MBLOCK_NEW,
                        &eh_mblock_new);
        ag_exception_registry_set(AG_ERNO_MBLOCK_NEW_ALIGN, MSG_MBLOCK_NEW_ALIGN,
                        &eh_mblock_new_align);
        ag_exception_registry_set(AG_ERNO_MBLOCK_RESIZE, MSG_MBLOCK_RESIZE,
                        &eh_mblock_resize);
}


extern void ag_exit(int status)
{
        ag_exception_registry_exit();
        exit(status);
}


static void eh_mblock_new(const struct ag_exception *ex, void *opt)
{
        (void) ex;
        (void) opt;
        
        printf("[!] %d [%s(), %s:%lu]: %s\n", ex->erno, ex->func, ex->file,
                        ex->line, ag_exception_registry_msg(ex->erno));
        
        ag_log_err("%d [%s(), %s:%lu]: %s", ex->erno, ex->func, ex->file,
                        ex->line, ag_exception_registry_msg(ex->erno));

        ag_exit(EXIT_FAILURE);
}


static void eh_mblock_new_align(const struct ag_exception *ex, void *opt)
{
        (void) ex;
        (void) opt;
        
        printf("[!] %d [%s(), %s:%lu]: %s\n", ex->erno, ex->func, ex->file,
                        ex->line, ag_exception_registry_msg(ex->erno));

        ag_log_err("%d [%s(), %s:%lu]: %s", ex->erno, ex->func, ex->file,
                        ex->line, ag_exception_registry_msg(ex->erno));

        ag_exit(EXIT_FAILURE);
}


static void eh_mblock_resize(const struct ag_exception *ex, void *opt)
{
        (void) ex;
        (void) opt;
        
        ag_log_err("%d [%s(), %s:%lu]: %s", ex->erno, ex->func, ex->file,
                        ex->line, ag_exception_registry_msg(ex->erno));
        
        ag_log_err("%d [%s(), %s:%lu]: %s", ex->erno, ex->func, ex->file,
                        ex->line, ag_exception_registry_msg(ex->erno));

        ag_exit(EXIT_FAILURE);
}

