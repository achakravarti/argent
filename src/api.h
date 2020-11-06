/*******************************************************************************
 *                        __   ____   ___  ____  __ _  ____ 
 *                       / _\ (  _ \ / __)(  __)(  ( \(_  _)
 *                      /    \ )   /( (_ \ ) _) /    /  )(  
 *                      \_/\_/(__\_) \___/(____)\_)__) (__)                     
 *
 * Argent Library
 * Copyright (c) 2020 Abhishek Chakravarti <abhishek@taranjali.org>
 *
 * This file is part of the Argent Library. It declares application programming
 * interface of the Argent Library.
 *
 * The contents of this file are released under the GPLv3 License. See the
 * accompanying LICENSE file or the generated Developer Manual (section I:?) for 
 * complete licensing details.
 *
 * BY CONTINUING TO USE AND/OR DISTRIBUTE THIS FILE, YOU ACKNOWLEDGE THAT YOU
 * HAVE UNDERSTOOD THESE LICENSE TERMS AND ACCEPT TO BE LEGALLY BOUND BY THEM.
 ******************************************************************************/



#include "../include/compiler.h"
#include "../include/log.h"
#include "../include/exception.h"
#include "../include/memblock.h"
#include "../include/primitives.h"
#include "../include/string.h"
#include "../include/object.h"
#include "../include/value.h"
#include "../include/list.h"
#include "../include/http/primitives.h"
#include "../include/http/url.h"
#include "../include/http/user.h"
#include "../include/http/cookie.h"
#include "../include/http/response.h"

























typedef void (ag_http_handler)(void);

extern void ag_http_init(void);
extern void ag_http_exit(void);
extern void ag_http_register(ag_http_handler *req);
extern void ag_http_run(void);

#if 0
extern void ag_http_cookie_param_set(const char *name, const char *val);
extern void ag_http_cookie_domain_set(const char *val);
extern void ag_http_cookie_path_set(const char *val);
extern void ag_http_cookie_expires_set(const char *val);
extern void ag_http_cookie_secure_set(void);
extern const char *ag_http_cookie_val(void);
#endif


extern enum ag_http_method ag_http_request_method(void);
extern enum ag_http_mime ag_http_request_type(void);
extern ag_string_t *ag_http_request_referer(void);

extern ag_string_t *ag_http_request_user_agent(void);
extern ag_string_t *ag_http_request_user_ip(void);
extern ag_string_t *ag_http_request_user_host(void);
extern ag_string_t *ag_http_request_user_port(void);

extern bool ag_http_request_url_secure(void);
extern ag_string_t *ag_http_request_url_host(void);
extern ag_string_t *ag_http_request_url_port(void);
extern ag_string_t *ag_http_request_url_path(void);

//extern ag_string_t *ag_http_request_param(const char *key);


extern void ag_http_response_begin(enum ag_http_mime type, 
        enum ag_http_status code);
extern void ag_http_response_string(const char *str);
extern void ag_http_response_file(const char *fpath);
#define ag_http_response_end()

extern ag_http_url_t *ag_http_request_url(void);


extern void ag_http_response_respond(const ag_http_response_t *resp);

