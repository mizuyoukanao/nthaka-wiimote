#include "nxmc2_buffer.h"
#include "pokecon_buffer.h"

#include <stdio.h>

int main(void)
{
    bool (*tests[])(void) = {
        test_nxmc2_buffer, test_pokecon_buffer};
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