#if (!defined ARGENT_TESTS)
#define ARGENT_TESTS


#include "../include/argent.h"


extern void ag_test_memblock(void);

extern void ag_test_string(void);

extern void ag_test_object(void);

extern void ag_test_log(void);

extern void ag_test_value(void);


extern _ag_test_suite *ag_test_suite_mblock(void);


#endif /* !defined ARGENT_TESTS */

