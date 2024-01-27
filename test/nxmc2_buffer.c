#include "nxmc2_buffer.h"
#include "util.h"

#include <stdio.h>

#include "nxamf/nxmc2.h"

static bool _test_init(void)
{
    nxmc2_buffer_t buf;
    nxmc2_buffer_init(&buf);

    return true;
}

static bool _test_append(void)
{
    nxmc2_buffer_t buf_;
    nxmc2_buffer_init(&buf_);
    nxamf_buffer_interface_t *buf = (nxamf_buffer_interface_t *)&buf_;

    buf->append(buf, 0xAB);

    return true;
}

static bool _test_deserialize(void)
{
    typedef struct test_case_t
    {
        uint8_t *seq;
        size_t len;
        NxamfGamepadState expected;
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
                           {.seq = (uint8_t[]){0xABU, 0x00U, 0x00U, 0x08U, 0x80U, 0x80U, 0x80U, 0x80U, 0x01U, 0x02U, 0x03U},
                            .len = 11,
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
                                         .l_stick = {.x = NXAMF_STICK_NEUTRAL,
                                                     .y = NXAMF_STICK_NEUTRAL},
                                         .r_stick = {.x = NXAMF_STICK_NEUTRAL,
                                                     .y = NXAMF_STICK_NEUTRAL},
                                         .extension = {1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}}};
    size_t length = sizeof(cases) / sizeof(test_case_t);

    for (size_t i = 0; i < length; i++)
    {
        nxmc2_buffer_t buf_;
        nxmc2_buffer_init(&buf_);
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
    nxmc2_buffer_t buf_;
    nxmc2_buffer_init(&buf_);
    nxamf_buffer_interface_t *buf = (nxamf_buffer_interface_t *)&buf_;

    NxamfGamepadState expected = {.y = NXAMF_BUTTON_RELEASED, .b = NXAMF_BUTTON_RELEASED, .a = NXAMF_BUTTON_RELEASED, .x = NXAMF_BUTTON_RELEASED, .l = NXAMF_BUTTON_RELEASED, .r = NXAMF_BUTTON_RELEASED, .zl = NXAMF_BUTTON_RELEASED, .zr = NXAMF_BUTTON_RELEASED, .minus = NXAMF_BUTTON_RELEASED, .plus = NXAMF_BUTTON_RELEASED, .l_click = NXAMF_BUTTON_RELEASED, .r_click = NXAMF_BUTTON_RELEASED, .home = NXAMF_BUTTON_RELEASED, .capture = NXAMF_BUTTON_RELEASED, .hat = NXAMF_HAT_NEUTRAL, .l_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .r_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    NxamfGamepadState actual;

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

    if (!buf->deserialize(buf, &actual) || !nxamf_gamepad_state_equals(&expected, &actual))
    {
        fprintf(stderr, "[_test_clear] Test failed.\n");
        return false;
    }

    return true;
}

bool test_nxmc2_buffer(void)
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
            fprintf(stderr, "[test_nxmc2_buffer] Test failed at index %d.\n", i);
            return false;
        }
    }

    return true;
}