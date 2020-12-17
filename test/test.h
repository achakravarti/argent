#ifndef __ARGENT_TESTS_H__
#define __ARGENT_TESTS_H__

#include "../include/argent.h"


/*extern void ag_test_memblock(void);

extern void ag_test_string(void);

extern void ag_test_object(void);

extern void ag_test_log(void);

extern void ag_test_value(void);*/

extern ag_test_suite *test_log(void);
extern ag_test_suite *ag_test_suite_mblock(void);
extern ag_test_suite *test_suite_str(void);


#endif /* !__ARGENT_TEST_H__ */

