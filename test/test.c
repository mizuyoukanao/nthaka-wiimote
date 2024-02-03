#include "buffer.h"
#include "dol.h"
#include "nxmc2.h"
#include "orca.h"
#include "pokecon.h"
#include "multi_format.h"

#include "util.h"

#include <stdio.h>

int main(void)
{
    int ret = 0;

    test_t tests[] = {TEST(test_dol),
                      TEST(test_buffer),
                      TEST(test_nxmc2),
                      TEST(test_orca),
                      TEST(test_pokecon),
                      TEST(test_multi_format)};

    for (size_t i = 0; i < SIZE_OF(tests); i++)
    {
        printf("%s:\n", tests[i].name);
        ret += tests[i].test();
    }

    if (ret == 0)
    {
        printf("OK");
    }
    return ret;
}