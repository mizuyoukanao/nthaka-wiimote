#include "buffer.h"

#include "nxmc2_buffer.h"
// #include "orca_buffer.h"
// #include "pokecon_buffer.h"
// #include "multi_buffer_manager.h"

#include <stdio.h>

int main(void)
{
    bool (*tests[])(void) = {test_nxmc2_buffer,
                             test_buffer,
                             /*test_pokecon_buffer,*/
                             /*test_multi_buffer_manager,*/
                             /*test_orca_buffer*/};
    size_t length = sizeof(tests) / sizeof(bool (*)(void));
    for (size_t i = 0; i < length; i++)
    {
        if (!tests[i]())
        {
            return 1;
        }
    }

    printf("OK");
    return 0;
}