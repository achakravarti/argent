/*******************************************************************************
 *                        __   ____   ___  ____  __ _  ____ 
 *                       / _\ (  _ \ / __)(  __)(  ( \(_  _)
 *                      /    \ )   /( (_ \ ) _) /    /  )(  
 *                      \_/\_/(__\_) \___/(____)\_)__) (__)                     
 *
 * Argent Library
 * Copyright (c) 2020 Abhishek Chakravarti <abhishek@taranjali.org>
 *
 * This file is part of the Argent Library. It implements the exception handling
 * interface of the Argent Library.
 *
 * The contents of this file are released under the GPLv3 License. See the
 * accompanying LICENSE file or the generated Developer Manual (section I:?) for 
 * complete licensing details.
 *
 * BY CONTINUING TO USE AND/OR DISTRIBUTE THIS FILE, YOU ACKNOWLEDGE THAT YOU
 * HAVE UNDERSTOOD THESE LICENSE TERMS AND ACCEPT TO BE LEGALLY BOUND BY THEM.
 ******************************************************************************/


#include "./api.h"




/*******************************************************************************
 *                             HELPER PROTOTYPES
 */


/*
 *      The eh_default() helper function is the default exception handler used
 *      in case none is supplied by the client code [DM:??].
 */
static void eh_default(const struct ag_exception *ex, void *opt);




/*******************************************************************************
 *                              GLOBAL VARIABLES
 */


/*
 *      The g_eh thread-local callback holds the exception handler that is to be
 *      invoked in case of an exception [DM:??].
 */
static ag_threadlocal ag_exception_handler *g_eh = eh_default;


/*
 *      The g_emsg thread-local array stores the error messages corresponding to
 *      each enum ag_erno enumerator [DM:??]
 */
static ag_threadlocal const char *g_emsg[] = {
    "AG_ERNO_NULL: no error has occurred",
    "AG_ERNO_MEMPOOL_NEW: failed to allocate new heap memory",
    "AG_ERNO_MEMPOOL_RESIZE: failed to resize existing heap memory buffer"
    "AG_ERNO_MEMBLOCK_NEW: failed to allocate new heap block",
    "AG_ERNO_MEMBLOCK_RESIZE: failed to resize existing heap block",
    "AG_ERNO_TEST: failed to pass unit test",
    "AG_ERNO_HTTP_INIT: failed to initalise HTTP server",
    "AG_ERNO_HTTP_PARAM: failed to read HTTP parameters",
    "AG_ERNO_HTTP_FILE: failed to serve HTTP file",
};




/*******************************************************************************
 *                          INTERFACE IMPLEMENTATION
 */


/*
 *      Implementation of the ag_exception_message() interface function [DM:??].
 */
extern const char *ag_exception_message(enum ag_erno erno)
{
    return ag_likely (erno >= AG_ERNO_NULL && erno < AG_ERNO_LEN) 
            ? g_emsg[erno] : "[!] undefined error message";
}


/*
 *      Implementation of the ag_exception_handler_get() interface function
 *      [DM:??].
 */
extern ag_exception_handler *ag_exception_handler_get(void)
{
    return g_eh;
}


/*
 *      Implementation of the ag_exception_handler_set() interface function
 *      [DM:??].
 */
extern void ag_exception_handler_set(ag_exception_handler *eh)
{
    g_eh = ag_likely (eh) ? eh : eh_default;
}




/*******************************************************************************
 *                           HELPER IMPLEMENTATION
 */


/*
 *      Implementation of the eh_default() helper function [DM:??].
 */
static void eh_default(const struct ag_exception *ex, void *opt)
{
    (void) opt;

    printf("[!] unhandled exception: 0x%x [%s(), %s:%lu]\n%s\n", ex->erno, 
            ex->func, ex->file, ex->line, ag_exception_message(ex->erno));
    ag_log_err("unhandled exception: 0x%x [%s(), %s:%lu]\n%s\n", ex->erno, 
            ex->func, ex->file, ex->line, ag_exception_message(ex->erno));

    exit(EXIT_FAILURE);
}

