#include "./test.h"


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    ag_test_memblock();
    ag_test_string();
    ag_test_object();

    return 0;
}

