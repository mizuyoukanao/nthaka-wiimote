#include "buffer.h"
#include "nxmc2.h"
#include "orca.h"
#include "pokecon.h"

#include "util.h"

#include <stdio.h>

int main(void)
{
    int ret = 0;

    test_t tests[] = {TEST(test_buffer),
                      TEST(test_nxmc2),
                      TEST(test_orca),
                      TEST(test_pokecon)};

    for (size_t i = 0; i < SIZE_OF(tests); i++)
    {
        printf("%s:\n", tests[i].name);
        ret += tests[i].test();
        printf("\n");
    }

    if (ret == 0)
    {
        printf("OK");
    }
    return ret;
}