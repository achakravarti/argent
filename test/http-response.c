#include "./http.h"
#include "./object.h"
#include "./test.h"




#define __AG_TEST_SUITE_ID__ 13

        
        
        
extern ag_test_suite *
test_suite_http_response(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_response interface");
}
