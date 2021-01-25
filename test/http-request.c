#include "./object.h"
#include "./test.h"


#define __AG_TEST_SUITE_ID__ 12


extern ag_test_suite *
test_suite_http_request(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_request interface");
}

