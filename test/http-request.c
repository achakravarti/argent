#include "./object.h"
#include "./test.h"


#define __AG_TEST_SUITE_ID__ 12


#define SAMPLE_CLIENT(tag, ip, port, host, agent, referer)                     \
        static inline ag_http_client *tag(void)                                \
        {                                                                      \
                return ag_http_client_new(ip, port, host, agent, referer);     \
        }


SAMPLE_CLIENT(CLIENT0, "", 0, "", "", "");
SAMPLE_CLIENT(CLIENT1, "192.168.0.1", 0, "host.com", "mozilla", "google.com");
SAMPLE_CLIENT(CLIENT2, "192.168.1.1", 40, "domain.com", "webkit", "");


extern ag_test_suite *
test_suite_http_request(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_request interface");
}

