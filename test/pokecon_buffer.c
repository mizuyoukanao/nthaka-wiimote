#include "pokecon_buffer.h"
#include "util.h"

#include <stdio.h>

#include "nxamf/pokecon.h"

static bool _test_init(void)
{
    pokecon_buffer_t buf;
    pokecon_buffer_init(&buf);
    return true;
}

static bool _test_append(void)
{
    pokecon_buffer_t buf_;
    pokecon_buffer_init(&buf_);
    nxamf_buffer_interface_t *buf = (nxamf_buffer_interface_t *)&buf_;

    buf->append(buf, '0');

    return true;
}

static bool _test_deserialize(void)
{
    typedef struct test_case_t
    {
        char *seq;
        size_t len;
        NxamfGamepadState expected;
    } test_case_t;

    test_case_t cases[] = {
        {.seq = "0 0\n", .len = 4, .expected = {.y = NXAMF_BUTTON_RELEASED, .b = NXAMF_BUTTON_RELEASED, .a = NXAMF_BUTTON_RELEASED, .x = NXAMF_BUTTON_RELEASED, .l = NXAMF_BUTTON_RELEASED, .r = NXAMF_BUTTON_RELEASED, .zl = NXAMF_BUTTON_RELEASED, .zr = NXAMF_BUTTON_RELEASED, .minus = NXAMF_BUTTON_RELEASED, .plus = NXAMF_BUTTON_RELEASED, .l_click = NXAMF_BUTTON_RELEASED, .r_click = NXAMF_BUTTON_RELEASED, .home = NXAMF_BUTTON_RELEASED, .capture = NXAMF_BUTTON_RELEASED, .hat = NXAMF_HAT_UP, .l_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .r_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.seq = "0x0004 8\r\n", .len = 10, .expected = {.y = NXAMF_BUTTON_PRESSED, .b = NXAMF_BUTTON_RELEASED, .a = NXAMF_BUTTON_RELEASED, .x = NXAMF_BUTTON_RELEASED, .l = NXAMF_BUTTON_RELEASED, .r = NXAMF_BUTTON_RELEASED, .zl = NXAMF_BUTTON_RELEASED, .zr = NXAMF_BUTTON_RELEASED, .minus = NXAMF_BUTTON_RELEASED, .plus = NXAMF_BUTTON_RELEASED, .l_click = NXAMF_BUTTON_RELEASED, .r_click = NXAMF_BUTTON_RELEASED, .home = NXAMF_BUTTON_RELEASED, .capture = NXAMF_BUTTON_RELEASED, .hat = NXAMF_HAT_NEUTRAL, .l_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .r_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.seq = "0x0002 8 26 da\r\n", .len = 16, .expected = {.y = NXAMF_BUTTON_RELEASED, .b = NXAMF_BUTTON_RELEASED, .a = NXAMF_BUTTON_RELEASED, .x = NXAMF_BUTTON_RELEASED, .l = NXAMF_BUTTON_RELEASED, .r = NXAMF_BUTTON_RELEASED, .zl = NXAMF_BUTTON_RELEASED, .zr = NXAMF_BUTTON_RELEASED, .minus = NXAMF_BUTTON_RELEASED, .plus = NXAMF_BUTTON_RELEASED, .l_click = NXAMF_BUTTON_RELEASED, .r_click = NXAMF_BUTTON_RELEASED, .home = NXAMF_BUTTON_RELEASED, .capture = NXAMF_BUTTON_RELEASED, .hat = NXAMF_HAT_NEUTRAL, .l_stick = {.x = 0x26, .y = 0xda}, .r_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.seq = "0x0001 8 26 26\r\n", .len = 16, .expected = {.y = NXAMF_BUTTON_RELEASED, .b = NXAMF_BUTTON_RELEASED, .a = NXAMF_BUTTON_RELEASED, .x = NXAMF_BUTTON_RELEASED, .l = NXAMF_BUTTON_RELEASED, .r = NXAMF_BUTTON_RELEASED, .zl = NXAMF_BUTTON_RELEASED, .zr = NXAMF_BUTTON_RELEASED, .minus = NXAMF_BUTTON_RELEASED, .plus = NXAMF_BUTTON_RELEASED, .l_click = NXAMF_BUTTON_RELEASED, .r_click = NXAMF_BUTTON_RELEASED, .home = NXAMF_BUTTON_RELEASED, .capture = NXAMF_BUTTON_RELEASED, .hat = NXAMF_HAT_NEUTRAL, .l_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .r_stick = {.x = 0x26, .y = 0x26}, .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.seq = "3 8 0x5a 0x6 80 80\r\n", .len = 20, .expected = {.y = NXAMF_BUTTON_RELEASED, .b = NXAMF_BUTTON_RELEASED, .a = NXAMF_BUTTON_RELEASED, .x = NXAMF_BUTTON_RELEASED, .l = NXAMF_BUTTON_RELEASED, .r = NXAMF_BUTTON_RELEASED, .zl = NXAMF_BUTTON_RELEASED, .zr = NXAMF_BUTTON_RELEASED, .minus = NXAMF_BUTTON_RELEASED, .plus = NXAMF_BUTTON_RELEASED, .l_click = NXAMF_BUTTON_RELEASED, .r_click = NXAMF_BUTTON_RELEASED, .home = NXAMF_BUTTON_RELEASED, .capture = NXAMF_BUTTON_RELEASED, .hat = NXAMF_HAT_NEUTRAL, .l_stick = {.x = 0x5a, .y = 0x6}, .r_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.seq = "3 8 80 80 0xd 0x48\r\n", .len = 20, .expected = {.y = NXAMF_BUTTON_RELEASED, .b = NXAMF_BUTTON_RELEASED, .a = NXAMF_BUTTON_RELEASED, .x = NXAMF_BUTTON_RELEASED, .l = NXAMF_BUTTON_RELEASED, .r = NXAMF_BUTTON_RELEASED, .zl = NXAMF_BUTTON_RELEASED, .zr = NXAMF_BUTTON_RELEASED, .minus = NXAMF_BUTTON_RELEASED, .plus = NXAMF_BUTTON_RELEASED, .l_click = NXAMF_BUTTON_RELEASED, .r_click = NXAMF_BUTTON_RELEASED, .home = NXAMF_BUTTON_RELEASED, .capture = NXAMF_BUTTON_RELEASED, .hat = NXAMF_HAT_NEUTRAL, .l_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .r_stick = {.x = 0xd, .y = 0x48}, .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.seq = "3 8 80 80\r\n", .len = 11, .expected = {.y = NXAMF_BUTTON_RELEASED, .b = NXAMF_BUTTON_RELEASED, .a = NXAMF_BUTTON_RELEASED, .x = NXAMF_BUTTON_RELEASED, .l = NXAMF_BUTTON_RELEASED, .r = NXAMF_BUTTON_RELEASED, .zl = NXAMF_BUTTON_RELEASED, .zr = NXAMF_BUTTON_RELEASED, .minus = NXAMF_BUTTON_RELEASED, .plus = NXAMF_BUTTON_RELEASED, .l_click = NXAMF_BUTTON_RELEASED, .r_click = NXAMF_BUTTON_RELEASED, .home = NXAMF_BUTTON_RELEASED, .capture = NXAMF_BUTTON_RELEASED, .hat = NXAMF_HAT_NEUTRAL, .l_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .r_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}}};
    size_t length = sizeof(cases) / sizeof(test_case_t);

    for (size_t i = 0; i < length; i++)
    {
        pokecon_buffer_t buf_;
        pokecon_buffer_init(&buf_);
        nxamf_buffer_interface_t *buf = (nxamf_buffer_interface_t *)&buf_;
        NxamfGamepadState actual;

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
    pokecon_buffer_t buf_;
    pokecon_buffer_init(&buf_);
    nxamf_buffer_interface_t *buf = (nxamf_buffer_interface_t *)&buf_;

    NxamfGamepadState expected = {.y = NXAMF_BUTTON_RELEASED, .b = NXAMF_BUTTON_RELEASED, .a = NXAMF_BUTTON_RELEASED, .x = NXAMF_BUTTON_RELEASED, .l = NXAMF_BUTTON_RELEASED, .r = NXAMF_BUTTON_RELEASED, .zl = NXAMF_BUTTON_RELEASED, .zr = NXAMF_BUTTON_RELEASED, .minus = NXAMF_BUTTON_RELEASED, .plus = NXAMF_BUTTON_RELEASED, .l_click = NXAMF_BUTTON_RELEASED, .r_click = NXAMF_BUTTON_RELEASED, .home = NXAMF_BUTTON_RELEASED, .capture = NXAMF_BUTTON_RELEASED, .hat = NXAMF_HAT_UP, .l_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .r_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    NxamfGamepadState actual;

    buf->clear(buf);

    buf->append(buf, '0');
    buf->append(buf, ' ');
    buf->append(buf, '0');
    buf->append(buf, '\n');

    if (!buf->deserialize(buf, &actual) || !nxamf_gamepad_state_equals(&expected, &actual))
    {
        fprintf(stderr, "[_test_clear] Test failed.\n");
        return false;
    }

    return true;
}

bool test_pokecon_buffer(void)
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
            fprintf(stderr, "[test_pokecon_buffer] Test failed at index %d.\n", i);
            return false;
        }
    }

    return true;
}