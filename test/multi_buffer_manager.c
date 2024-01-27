#include "multi_buffer_manager.h"

#include <stdio.h>

#include "nxamf.h"
#include "nxamf/nxmc2.h"
#include "nxamf/pokecon.h"

static bool _test_init(void)
{
    nxamf_buffer_interface_t *bufs[] = {};
    nxamf_multi_buffer_manager_t buf;
    nxamf_multi_buffer_manager_init(&buf, bufs, 0);

    return true;
}

static bool _test_append(void)
{
    nxamf_buffer_interface_t *bufs[] = {};
    nxamf_multi_buffer_manager_t buf_;
    nxamf_multi_buffer_manager_init(&buf_, bufs, 0);
    nxamf_buffer_interface_t *buf = (nxamf_buffer_interface_t *)&buf_;

    buf->append(buf, 0);

    return true;
}

static bool _test_deserialize(void)
{
    typedef struct test_case_t
    {
        uint8_t *seq;
        size_t len;
        nxamf_gamepad_state_t expected;
    } test_case_t;

    test_case_t cases[] = {{.seq = (uint8_t[]){0xABU, 0x01U, 0x20U, 0x08U, 0x80U, 0x80U, 0x80U, 0x80U, 0x00U, 0x00U, 0x00U},
                            .len = 11,
                            .expected = {.y = NXAMF_BUTTON_PRESSED,
                                         .b = NXAMF_BUTTON_RELEASED,
                                         .a = NXAMF_BUTTON_RELEASED,
                                         .x = NXAMF_BUTTON_RELEASED,
                                         .l = NXAMF_BUTTON_RELEASED,
                                         .r = NXAMF_BUTTON_RELEASED,
                                         .zl = NXAMF_BUTTON_RELEASED,
                                         .zr = NXAMF_BUTTON_RELEASED,
                                         .minus = NXAMF_BUTTON_RELEASED,
                                         .plus = NXAMF_BUTTON_RELEASED,
                                         .l_click = NXAMF_BUTTON_RELEASED,
                                         .r_click = NXAMF_BUTTON_RELEASED,
                                         .home = NXAMF_BUTTON_RELEASED,
                                         .capture = NXAMF_BUTTON_PRESSED,
                                         .hat = NXAMF_HAT_NEUTRAL,
                                         .l_stick = {.x = NXAMF_STICK_NEUTRAL,
                                                     .y = NXAMF_STICK_NEUTRAL},
                                         .r_stick = {.x = NXAMF_STICK_NEUTRAL,
                                                     .y = NXAMF_STICK_NEUTRAL},
                                         .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
                           {.seq = "0x0000 8\r\n",
                            .len = 10,
                            .expected = {.y = NXAMF_BUTTON_RELEASED,
                                         .b = NXAMF_BUTTON_RELEASED,
                                         .a = NXAMF_BUTTON_RELEASED,
                                         .x = NXAMF_BUTTON_RELEASED,
                                         .l = NXAMF_BUTTON_RELEASED,
                                         .r = NXAMF_BUTTON_RELEASED,
                                         .zl = NXAMF_BUTTON_RELEASED,
                                         .zr = NXAMF_BUTTON_RELEASED,
                                         .minus = NXAMF_BUTTON_RELEASED,
                                         .plus = NXAMF_BUTTON_RELEASED,
                                         .l_click = NXAMF_BUTTON_RELEASED,
                                         .r_click = NXAMF_BUTTON_RELEASED,
                                         .home = NXAMF_BUTTON_RELEASED,
                                         .capture = NXAMF_BUTTON_RELEASED,
                                         .hat = NXAMF_HAT_NEUTRAL,
                                         .l_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL},
                                         .r_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL},
                                         .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}}};
    size_t length = sizeof(cases) / sizeof(test_case_t);

    for (size_t i = 0; i < length; i++)
    {
        nxmc2_buffer_t nxmc2;
        nxmc2_buffer_init(&nxmc2);
        pokecon_buffer_t pokecon;
        pokecon_buffer_init(&pokecon);
        nxamf_buffer_interface_t *bufs[] = {(nxamf_buffer_interface_t *)&nxmc2, (nxamf_buffer_interface_t *)&pokecon};

        nxamf_multi_buffer_manager_t buf_;
        nxamf_multi_buffer_manager_init(&buf_, bufs, 2);
        nxamf_buffer_interface_t *buf = (nxamf_buffer_interface_t *)&buf_;
        nxamf_gamepad_state_t actual;

        for (size_t j = 0; j < cases[i].len; j++)
        {
            buf->append(buf, (uint8_t)cases[i].seq[j]);
        }
        if (!buf->deserialize(buf, &actual) || !nxamf_gamepad_state_equals(&(cases[i].expected), &actual))
        {
            fprintf(stderr, "[_test_deserialize] Test failed at index %d.\n", i);
            return false;
        }
    }

    return true;
}

static bool _test_clear(void)
{
    nxmc2_buffer_t nxmc2;
    nxmc2_buffer_init(&nxmc2);
    nxamf_buffer_interface_t *bufs[] = {(nxamf_buffer_interface_t *)&nxmc2};

    nxamf_multi_buffer_manager_t buf_;
    nxamf_multi_buffer_manager_init(&buf_, bufs, 1);
    nxamf_buffer_interface_t *buf = (nxamf_buffer_interface_t *)&buf_;

    nxamf_gamepad_state_t expected = {.y = NXAMF_BUTTON_RELEASED, .b = NXAMF_BUTTON_RELEASED, .a = NXAMF_BUTTON_RELEASED, .x = NXAMF_BUTTON_RELEASED, .l = NXAMF_BUTTON_RELEASED, .r = NXAMF_BUTTON_RELEASED, .zl = NXAMF_BUTTON_RELEASED, .zr = NXAMF_BUTTON_RELEASED, .minus = NXAMF_BUTTON_RELEASED, .plus = NXAMF_BUTTON_RELEASED, .l_click = NXAMF_BUTTON_RELEASED, .r_click = NXAMF_BUTTON_RELEASED, .home = NXAMF_BUTTON_RELEASED, .capture = NXAMF_BUTTON_RELEASED, .hat = NXAMF_HAT_NEUTRAL, .l_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .r_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    nxamf_gamepad_state_t actual;

    buf->clear(buf);

    buf->append(buf, 0xAB);
    buf->append(buf, 0);
    buf->append(buf, 0);
    buf->append(buf, 8);
    buf->append(buf, 128);
    buf->append(buf, 128);
    buf->append(buf, 128);
    buf->append(buf, 128);
    buf->append(buf, 0);
    buf->append(buf, 0);
    buf->append(buf, 0);

    bool ret = buf->deserialize(buf, &actual);
    if (!ret || !nxamf_gamepad_state_equals(&expected, &actual))
    {
        fprintf(stderr, "[_test_clear] Test failed.\n");
        return false;
    }

    return true;
}

bool test_multi_buffer_manager(void)
{
    bool (*tests[])(void) = {
        _test_init,
        _test_append,
        _test_deserialize,
        _test_clear};
    size_t length = sizeof(tests) / sizeof(bool (*)(void));
    for (size_t i = 0; i < length; i++)
    {
        if (!tests[i]())
        {
            fprintf(stderr, "[test_multi_buffer_manager] Test failed at index %d.\n", i);
            return false;
        }
    }

    return true;
}