#include "nthaka.h"

#include <assert.h>
#include <stdio.h>

#include "mock_format.h"
#include "util.h"

static int test_init(void)
{
    int ret = 0;

    typedef struct test_case_t
    {
        nthaka_buffer_t *arg0;
        nthaka_format_handler_t *arg1;

        bool expected;
    } test_case_t;

    nthaka_buffer_t buf;
    nthaka_format_handler_t fmt;

    test_case_t cases[] = {
        {.arg0 = NULL, .arg1 = NULL, .expected = false},
        {.arg0 = &buf, .arg1 = NULL, .expected = false},
        {.arg0 = NULL, .arg1 = &fmt, .expected = false},
        {.arg0 = &buf, .arg1 = &fmt, .expected = true},
    };

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        bool actual = nthaka_buffer_init(case_.arg0, case_.arg1);
        if (actual != case_.expected)
        {
            fprintf(stderr, "index: %d, expected: %s, actual: %s\n", i, bool_(case_.expected), bool_(actual));
            ret++;
        }
    }

    return ret;
}

static int test_append(void)
{
    int ret = 0;

    typedef struct test_case_t
    {
        uint8_t *prev;
        size_t size;
        uint8_t d;

        nthaka_buffer_state_t expected_ret;
        nthaka_gamepad_state_t expected_out;
    } test_case_t;

    test_case_t cases[] = {{.prev = (uint8_t[]){}, .size = 0, .d = 0, .expected_ret = NTHAKA_BUFFER_PENDING},
                           {.prev = (uint8_t[]){}, .size = 0, .d = 1, .expected_ret = NTHAKA_BUFFER_REJECTED},
                           {.prev = (uint8_t[]){0}, .size = 1, .d = 1, .expected_ret = NTHAKA_BUFFER_PENDING},
                           {.prev = (uint8_t[]){0}, .size = 1, .d = 2, .expected_ret = NTHAKA_BUFFER_REJECTED},
                           {.prev = (uint8_t[]){0, 1}, .size = 2, .d = 2, .expected_ret = NTHAKA_BUFFER_ACCEPTED, .expected_out = {.y = NTHAKA_BUTTON_RELEASED, //
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
                                                                                                                                   .l_stick = NTHAKA_STICK_NEUTRAL,
                                                                                                                                   .r_stick = NTHAKA_STICK_NEUTRAL,
                                                                                                                                   .ext = {/**/ 0, 1, 2, /**/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
                           {.prev = (uint8_t[]){0, 1}, .size = 2, .d = 3, .expected_ret = NTHAKA_BUFFER_REJECTED},
                           {.prev = (uint8_t[]){0, 1, 2}, .size = 3, .d = 0, .expected_ret = NTHAKA_BUFFER_REJECTED}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        mock_format_t fmt;
        mock_format_init(&fmt);
        nthaka_buffer_t buf;
        assert(nthaka_buffer_init(&buf, (nthaka_format_handler_t *)&fmt));
        nthaka_gamepad_state_t actual_out;

        for (size_t j = 0; j < case_.size; j++)
        {
            nthaka_buffer_append(&buf, case_.prev[j], NULL);
        }

        nthaka_buffer_state_t actual_ret = nthaka_buffer_append(&buf, case_.d, &actual_out);
        if (actual_ret != case_.expected_ret)
        {
            fprintf(stderr, "index: %d, expected: %s, actual: %s\n", i, nthaka_buffer_state_t_(case_.expected_ret), nthaka_buffer_state_t_(actual_ret));
            ret++;
        }
        else if (actual_ret == NTHAKA_BUFFER_ACCEPTED && !nthaka_gamepad_state_equals(&actual_out, &(case_.expected_out)))
        {
            char str0[NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX];
            char str1[NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX];
            nthaka_gamepad_state_stringify(&(case_.expected_out), str0, NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX);
            nthaka_gamepad_state_stringify(&actual_out, str1, NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX);

            fprintf(stderr, "index: %i, expected: %s, actual: %s\n", i, str0, str1);
            ret++;
        }
    }

    return ret;
}

static int test_clear(void)
{
    int ret = 0;

    mock_format_t fmt;
    mock_format_init(&fmt);
    nthaka_buffer_t buf;
    assert(nthaka_buffer_init(&buf, (nthaka_format_handler_t *)&fmt));

    nthaka_buffer_append(&buf, 0, NULL);
    nthaka_buffer_append(&buf, 1, NULL);
    nthaka_buffer_append(&buf, 2, NULL);

    assert(fmt.s == MOCK_STATE_FINAL);

    nthaka_buffer_clear(&buf);

    assert(fmt.s == MOCK_STATE_INITIAL);

    nthaka_buffer_append(&buf, 0, NULL);
    nthaka_buffer_append(&buf, 1, NULL);
    nthaka_buffer_append(&buf, 2, NULL);

    assert(fmt.s == MOCK_STATE_FINAL);

    return ret;
}

int test_buffer(void)
{
    return run_tests((test_t[]){TEST(test_init),
                                TEST(test_append),
                                TEST(test_clear)},
                     3);
}