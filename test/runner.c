#include "../include/argent.h"
#include "./test.h"


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    ag_object_init(32);

    ag_test_memblock();
    ag_test_string();
    ag_test_object();
    ag_test_log();
    ag_test_value();
    ag_test_http_attrib();
    ag_test_http_url();

    ag_object_exit();
    return 0;
}

