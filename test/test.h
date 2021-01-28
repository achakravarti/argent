#ifndef __ARGENT_TEST_TEST_H__
#define __ARGENT_TEST_TEST_H__

#include "../include/argent.h"


extern ag_test_suite    *test_suite_log(void);
extern ag_test_suite    *test_suite_memblock(void);
extern ag_test_suite    *test_suite_string(void);
extern ag_test_suite    *test_suite_object(void);
extern ag_test_suite    *test_suite_value(void);
extern ag_test_suite    *test_suite_field(void);
extern ag_test_suite    *test_suite_list(void);
extern ag_test_suite    *test_suite_alist(void);
extern ag_test_suite    *test_suite_http_url(void);
extern ag_test_suite    *test_suite_http_client(void);
extern ag_test_suite    *test_suite_http_method(void);
extern ag_test_suite    *test_suite_http_mime(void);
extern ag_test_suite    *test_suite_http_request(void);


#endif /* !__ARGENT_TEST_TEST_H__ */

