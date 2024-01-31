#include "nxmc2_buffer.h"

#include <stdio.h>

#include "nthaka/nxmc2.h"

#include "util.h"

static bool _test_init(void)
{
    nxmc2_buffer_t buf;
    nxmc2_buffer_init(&buf);

    return true;
}

static bool _test_append(void)
{
    typedef struct test_case_t
    {
        uint8_t *pre;
        size_t len;
        uint8_t d;
        bool expected_ret;
        nthaka_gamepad_state_t expected_out;
    } test_case_t;

    test_case_t cases[] = {
        {.pre = (uint8_t[]){}, .len = 0, .d = 0xAB, .expected_ret = false},
        {.pre = (uint8_t[]){}, .len = 0, .d = 0x80, .expected_ret = false},
        {.pre = (uint8_t[]){0xAB, 0, 0, 8, 128, 128, 128, 128, 0, 0}, .len = 10, .d = 0, .expected_ret = true, .expected_out = NTHAKA_GAMEPAD_STATE_NEUTRAL},
        {.pre = (uint8_t[]){0xAB, 1, 1, 8, 129, 129, 127, 127, 1, 2}, .len = 10, .d = 3, .expected_ret = true, .expected_out = {.y = NTHAKA_BUTTON_PRESSED, //
                                                                                                                                .b = NTHAKA_BUTTON_RELEASED,
                                                                                                                                .a = NTHAKA_BUTTON_RELEASED,
                                                                                                                                .x = NTHAKA_BUTTON_RELEASED,
                                                                                                                                .l = NTHAKA_BUTTON_RELEASED,
                                                                                                                                .r = NTHAKA_BUTTON_RELEASED,
                                                                                                                                .zl = NTHAKA_BUTTON_RELEASED,
                                                                                                                                .zr = NTHAKA_BUTTON_RELEASED,
                                                                                                                                .minus = NTHAKA_BUTTON_PRESSED,
                                                                                                                                .plus = NTHAKA_BUTTON_RELEASED,
                                                                                                                                .l_click = NTHAKA_BUTTON_RELEASED,
                                                                                                                                .r_click = NTHAKA_BUTTON_RELEASED,
                                                                                                                                .home = NTHAKA_BUTTON_RELEASED,
                                                                                                                                .capture = NTHAKA_BUTTON_RELEASED,
                                                                                                                                .hat = NTHAKA_HAT_NEUTRAL,
                                                                                                                                .l_stick = {.x = 129, .y = 129},
                                                                                                                                .r_stick = {.x = 127, .y = 127},
                                                                                                                                .extension = {1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}}};
    size_t length = sizeof(cases) / sizeof(test_case_t);

    for (size_t i = 0; i < length; i++)
    {
        nxmc2_buffer_t buf_;
        nxmc2_buffer_init(&buf_);
        nthaka_buffer_interface_t *buf = (nthaka_buffer_interface_t *)&buf_;
        nthaka_gamepad_state_t actual;

        for (size_t j = 0; j < cases[i].len; j++)
        {
            buf->append(buf, cases[i].pre[j], NULL);
        }

        bool ret = buf->append(buf, cases[i].d, &actual);
        if (ret != cases[i].expected_ret)
        {
            fprintf(stderr, "[_test_append] Test failed at index %d.\n", i);
            return false;
        }
        if (ret && !nthaka_gamepad_state_equals(&(cases[i].expected_out), &actual))
        {
            fprintf(stderr, "[_test_append] Test failed at index %d.\n", i);

            char str[NTHAKA_GAMEPAD_STATE_STRING_LENGTH_MAX];
            size_t len = sizeof(str) / sizeof(char);
            nthaka_gamepad_state_stringify(&(cases[i].expected_out), str, len);
            fprintf(stderr, "expected: %s\n", str);

            nthaka_gamepad_state_stringify(&actual, str, len);
            fprintf(stderr, "actual  : %s\n", str);

            return false;
        }
    }

    return true;
}

// static bool _test_deserialize(void)
// {
//     typedef struct test_case_t
//     {
//         uint8_t *seq;
//         size_t len;
//         nthaka_gamepad_state_t expected;
//     } test_case_t;

//     test_case_t cases[] = {{.seq = (uint8_t[]){0xABU, 0x01U, 0x20U, 0x08U, 0x80U, 0x80U, 0x80U, 0x80U, 0x00U, 0x00U, 0x00U},
//                             .len = 11,
//                             .expected = {.y = NTHAKA_BUTTON_PRESSED,
//                                          .b = NTHAKA_BUTTON_RELEASED,
//                                          .a = NTHAKA_BUTTON_RELEASED,
//                                          .x = NTHAKA_BUTTON_RELEASED,
//                                          .l = NTHAKA_BUTTON_RELEASED,
//                                          .r = NTHAKA_BUTTON_RELEASED,
//                                          .zl = NTHAKA_BUTTON_RELEASED,
//                                          .zr = NTHAKA_BUTTON_RELEASED,
//                                          .minus = NTHAKA_BUTTON_RELEASED,
//                                          .plus = NTHAKA_BUTTON_RELEASED,
//                                          .l_click = NTHAKA_BUTTON_RELEASED,
//                                          .r_click = NTHAKA_BUTTON_RELEASED,
//                                          .home = NTHAKA_BUTTON_RELEASED,
//                                          .capture = NTHAKA_BUTTON_PRESSED,
//                                          .hat = NTHAKA_HAT_NEUTRAL,
//                                          .l_stick = {.x = NTHAKA_STICK_NEUTRAL,
//                                                      .y = NTHAKA_STICK_NEUTRAL},
//                                          .r_stick = {.x = NTHAKA_STICK_NEUTRAL,
//                                                      .y = NTHAKA_STICK_NEUTRAL},
//                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
//                            {.seq = (uint8_t[]){0xABU, 0x00U, 0x00U, 0x08U, 0x80U, 0x80U, 0x80U, 0x80U, 0x01U, 0x02U, 0x03U},
//                             .len = 11,
//                             .expected =
//                            {.seq = (uint8_t[]){0xABU, 0x00U, 0x00U, 0x08U, 0x80U, 0x80U, 0x80U, 0x80U, 0x01U, 0x02U, 0x03U, /**/ 0xABU, 0x00U, 0x00U, 0x08U, 0x80U, 0x80U, 0x80U, 0x80U, 0x00U, 0x00U, 0x00U},
//                             .len = 22,
//                             .expected = NTHAKA_GAMEPAD_STATE_NEUTRAL}};
//     size_t length = sizeof(cases) / sizeof(test_case_t);

//     for (size_t i = 0; i < length; i++)
//     {
//         nxmc2_buffer_t buf_;
//         nxmc2_buffer_init(&buf_);
//         nthaka_buffer_interface_t *buf = (nthaka_buffer_interface_t *)&buf_;
//         nthaka_gamepad_state_t actual;

//         for (size_t j = 0; j < cases[i].len; j++)
//         {
//             buf->append(buf, (uint8_t)cases[i].seq[j]);
//         }
//         if (!buf->deserialize(buf, &actual) || !nthaka_gamepad_state_equals(&(cases[i].expected), &actual))
//         {
//             fprintf(stderr, "[_test_deserialize] Test failed at index %d.\n", i);
//             return false;
//         }
//     }

//     return true;
// }

static bool _test_clear(void)
{
    nxmc2_buffer_t buf_;
    nxmc2_buffer_init(&buf_);
    nthaka_buffer_interface_t *buf = (nthaka_buffer_interface_t *)&buf_;

    nthaka_gamepad_state_t expected = {.y = NTHAKA_BUTTON_PRESSED,
                                       .b = NTHAKA_BUTTON_RELEASED,
                                       .a = NTHAKA_BUTTON_RELEASED,
                                       .x = NTHAKA_BUTTON_RELEASED,
                                       .l = NTHAKA_BUTTON_RELEASED,
                                       .r = NTHAKA_BUTTON_RELEASED,
                                       .zl = NTHAKA_BUTTON_RELEASED,
                                       .zr = NTHAKA_BUTTON_RELEASED,
                                       .minus = NTHAKA_BUTTON_RELEASED,
                                       .plus = NTHAKA_BUTTON_RELEASED,
                                       .l_click = NTHAKA_BUTTON_RELEASED,
                                       .r_click = NTHAKA_BUTTON_RELEASED,
                                       .home = NTHAKA_BUTTON_RELEASED,
                                       .capture = NTHAKA_BUTTON_RELEASED,
                                       .hat = NTHAKA_HAT_NEUTRAL,
                                       .l_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
                                       .r_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
                                       .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    nthaka_gamepad_state_t actual;

    buf->clear(buf);

    buf->append(buf, 0xAB, NULL);
    buf->append(buf, 1, NULL);
    buf->append(buf, 0, NULL);
    buf->append(buf, 8, NULL);
    buf->append(buf, 128, NULL);
    buf->append(buf, 128, NULL);
    buf->append(buf, 128, NULL);
    buf->append(buf, 128, NULL);
    buf->append(buf, 0, NULL);
    buf->append(buf, 0, NULL);
    bool ret = buf->append(buf, 0, &actual);

    if (!ret || !nthaka_gamepad_state_equals(&expected, &actual))
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
        /*_test_deserialize,*/
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