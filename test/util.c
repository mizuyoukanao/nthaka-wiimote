#include "util.h"

int run_tests(test_t tests[], size_t size)
{
    int ret = 0;

    for (size_t i = 0; i < size; i++)
        tests[i].test() == 0 ? TEST_PASSED(tests[i].name)
                             : (TEST_FAILED(tests[i].name) && ret++);

    return ret;
}