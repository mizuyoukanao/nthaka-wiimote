#include "buffer.h"
#include "nxmc2.h"

#include "util.h"

// #include "nxmc2_buffer.h"
// #include "orca_buffer.h"
// #include "pokecon_buffer.h"
// #include "multi_buffer_manager.h"

#include <stdio.h>

int main(void)
{
    int ret = 0;

    test_t tests[] = {{.name = "test_buffer", .test = test_buffer},
                      {.name = "test_nxmc2", .test = test_nxmc2}};

    for (size_t i = 0; i < SIZE_OF(tests); i++)
    {
        printf("%s:\n", tests[i].name);
        if (tests[i].test() != 0)
        {
            ret++;
        }
        printf("\n");
    }

    if (ret == 0)
    {
        printf("OK");
    }
    return ret;
}