#include "nthaka/orca.h"

#include <assert.h>
#include <stdio.h>

#include "util.h"

static int test_init(void)
{
    int ret = 0;

    typedef struct test_case_t
    {
        orca_format_t *arg0;

        bool expected;
    } test_case_t;

    orca_format_t orca;

    test_case_t cases[] = {
        {.arg0 = NULL, .expected = false},
        {.arg0 = &orca, .expected = true},
    };

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        bool actual = orca_format_init(case_.arg0);
        if (actual != case_.expected)
        {
            fprintf(stderr, "index: %d, expected: %s, actual: %s\n", i, bool_(case_.expected), bool_(actual));
            ret++;
        }
    }

    return ret;
}

static int test_update(void)
{
    int ret = 0;

    typedef struct test_case_t
    {
        uint8_t *seq;
        size_t size;
        nthaka_buffer_state_t *expected;
    } test_case_t;

    test_case_t cases[] = {{.seq = (uint8_t[]){0}, //
                            .size = 1,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_REJECTED}},
                           {.seq = (uint8_t[]){0, 0x80}, //
                            .size = 2,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_REJECTED, NTHAKA_BUFFER_REJECTED}},
                           {.seq = (uint8_t[]){'@'}, //
                            .size = 1,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_ACCEPTED}},
                           {.seq = (uint8_t[]){0x80, 0, 0, 0}, //
                            .size = 4,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED, NTHAKA_BUFFER_REJECTED}},
                           {.seq = (uint8_t[]){0x80, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0}, //
                            .size = 7,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED}}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        orca_format_t orca;
        assert(orca_format_init(&orca));
        nthaka_format_t *fmt = (nthaka_format_t *)&orca;

        for (size_t j = 0; j < case_.size; j++)
        {
            nthaka_buffer_state_t actual = nthaka_format_update(fmt, case_.seq[j]);
            if (actual != case_.expected[j])
            {
                fprintf(stderr, "index: %d, %d, expected: %s, actual: %s\n", i, j, nthaka_buffer_state_t_(case_.expected[j]), nthaka_buffer_state_t_(actual));
                ret++;
                break;
            }
        }
    }

    return ret;
}

static int test_deserialize(void)
{
    int ret = 0;

    typedef struct test_case_t
    {
        uint8_t *buf;
        size_t size;

        nthaka_gamepad_state_t expected;
    } test_case_t;

    test_case_t cases[] = {
        {.buf = (uint8_t[]){0x80, 0, 0}, .size = 3, .expected = NTHAKA_GAMEPAD_STATE_NEUTRAL},
        {.buf = (uint8_t[]){'@'}, .size = 1, .expected = {.y = NTHAKA_BUTTON_STATE_RELEASED, //
                                                          .b = NTHAKA_BUTTON_STATE_RELEASED,
                                                          .a = NTHAKA_BUTTON_STATE_RELEASED,
                                                          .x = NTHAKA_BUTTON_STATE_RELEASED,
                                                          .l = NTHAKA_BUTTON_STATE_RELEASED,
                                                          .r = NTHAKA_BUTTON_STATE_RELEASED,
                                                          .zl = NTHAKA_BUTTON_STATE_RELEASED,
                                                          .zr = NTHAKA_BUTTON_STATE_RELEASED,
                                                          .minus = NTHAKA_BUTTON_STATE_RELEASED,
                                                          .plus = NTHAKA_BUTTON_STATE_RELEASED,
                                                          .l_click = NTHAKA_BUTTON_STATE_RELEASED,
                                                          .r_click = NTHAKA_BUTTON_STATE_RELEASED,
                                                          .home = NTHAKA_BUTTON_STATE_PRESSED,
                                                          .capture = NTHAKA_BUTTON_STATE_RELEASED,
                                                          .hat = NTHAKA_HAT_STATE_NEUTRAL,
                                                          .l_stick = NTHAKA_STICK_STATE_NEUTRAL,
                                                          .r_stick = NTHAKA_STICK_STATE_NEUTRAL,
                                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.buf = (uint8_t[]){0x80, /*0b00010101*/ 21, 21}, .size = 3, .expected = {.y = NTHAKA_BUTTON_STATE_RELEASED, //
                                                                                  .b = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                  .a = NTHAKA_BUTTON_STATE_PRESSED,
                                                                                  .x = NTHAKA_BUTTON_STATE_PRESSED,
                                                                                  .l = NTHAKA_BUTTON_STATE_PRESSED,
                                                                                  .r = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                  .zl = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                  .zr = NTHAKA_BUTTON_STATE_PRESSED,
                                                                                  .minus = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                  .plus = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                  .l_click = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                  .r_click = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                  .home = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                  .capture = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                  .hat = NTHAKA_HAT_STATE_UPLEFT,
                                                                                  .l_stick = NTHAKA_STICK_STATE_NEUTRAL,
                                                                                  .r_stick = NTHAKA_STICK_STATE_NEUTRAL,
                                                                                  .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.buf = (uint8_t[]){0x80, 0xFF, /* 0b00101010 */ 42, 0xFF, 0xFF, 42}, .size = 6, .expected = {.y = NTHAKA_BUTTON_STATE_PRESSED, //
                                                                                                      .b = NTHAKA_BUTTON_STATE_PRESSED,
                                                                                                      .a = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                                      .x = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                                      .l = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                                      .r = NTHAKA_BUTTON_STATE_PRESSED,
                                                                                                      .zl = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                                      .zr = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                                      .minus = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                                      .plus = NTHAKA_BUTTON_STATE_PRESSED,
                                                                                                      .l_click = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                                      .r_click = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                                      .home = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                                      .capture = NTHAKA_BUTTON_STATE_RELEASED,
                                                                                                      .hat = NTHAKA_HAT_STATE_DOWNRIGHT,
                                                                                                      .l_stick = NTHAKA_STICK_STATE_NEUTRAL,
                                                                                                      .r_stick = NTHAKA_STICK_STATE_NEUTRAL,
                                                                                                      .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        orca_format_t orca;
        assert(orca_format_init(&orca));
        nthaka_format_t *fmt = (nthaka_format_t *)&orca;

        for (size_t j = 0; j < case_.size - 1; j++)
        {
            assert(nthaka_format_update(fmt, case_.buf[j]) == NTHAKA_BUFFER_PENDING);
        }
        assert(nthaka_format_update(fmt, case_.buf[case_.size - 1]) == NTHAKA_BUFFER_ACCEPTED);

        nthaka_gamepad_state_t actual_out;
        assert(nthaka_format_deserialize(fmt, case_.buf, case_.size, &actual_out));
        if (!nthaka_gamepad_state_equals(&case_.expected, &actual_out))
        {
            char str0[NTHAKA_GAMEPAD_STATE_STRING_LENGTH_MAX];
            char str1[NTHAKA_GAMEPAD_STATE_STRING_LENGTH_MAX];
            nthaka_gamepad_state_stringify(&(case_.expected), str0, NTHAKA_GAMEPAD_STATE_STRING_LENGTH_MAX);
            nthaka_gamepad_state_stringify(&actual_out, str1, NTHAKA_GAMEPAD_STATE_STRING_LENGTH_MAX);

            fprintf(stderr, "index: %d, expected: %s, actual: %s\n", i, str0, str1);
            ret++;
        }
    }

    return ret;
}

static int test_hold_previous_state(void)
{
    int ret = 0;

    typedef struct test_case_t
    {
        uint8_t *prev;
        size_t prev_size;

        uint8_t *buf;
        size_t size;

        nthaka_gamepad_state_t expected;
    } test_case_t;

    test_case_t cases[] = {{.prev = (uint8_t[]){0x80, 1, 0}, //
                            .prev_size = 3,
                            .buf = (uint8_t[]){'@'},
                            .size = 1,
                            .expected = {.y = NTHAKA_BUTTON_STATE_RELEASED, //
                                         .b = NTHAKA_BUTTON_STATE_RELEASED,
                                         .a = NTHAKA_BUTTON_STATE_PRESSED,
                                         .x = NTHAKA_BUTTON_STATE_RELEASED,
                                         .l = NTHAKA_BUTTON_STATE_RELEASED,
                                         .r = NTHAKA_BUTTON_STATE_RELEASED,
                                         .zl = NTHAKA_BUTTON_STATE_RELEASED,
                                         .zr = NTHAKA_BUTTON_STATE_RELEASED,
                                         .minus = NTHAKA_BUTTON_STATE_RELEASED,
                                         .plus = NTHAKA_BUTTON_STATE_RELEASED,
                                         .l_click = NTHAKA_BUTTON_STATE_RELEASED,
                                         .r_click = NTHAKA_BUTTON_STATE_RELEASED,
                                         .home = NTHAKA_BUTTON_STATE_PRESSED,
                                         .capture = NTHAKA_BUTTON_STATE_RELEASED,
                                         .hat = NTHAKA_HAT_STATE_NEUTRAL,
                                         .l_stick = NTHAKA_STICK_STATE_NEUTRAL,
                                         .r_stick = NTHAKA_STICK_STATE_NEUTRAL,
                                         .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
                           {.prev = (uint8_t[]){'@'}, //
                            .prev_size = 1,
                            .buf = (uint8_t[]){0x80, 1, 0},
                            .size = 3,
                            .expected = {.y = NTHAKA_BUTTON_STATE_RELEASED, //
                                         .b = NTHAKA_BUTTON_STATE_RELEASED,
                                         .a = NTHAKA_BUTTON_STATE_PRESSED,
                                         .x = NTHAKA_BUTTON_STATE_RELEASED,
                                         .l = NTHAKA_BUTTON_STATE_RELEASED,
                                         .r = NTHAKA_BUTTON_STATE_RELEASED,
                                         .zl = NTHAKA_BUTTON_STATE_RELEASED,
                                         .zr = NTHAKA_BUTTON_STATE_RELEASED,
                                         .minus = NTHAKA_BUTTON_STATE_RELEASED,
                                         .plus = NTHAKA_BUTTON_STATE_RELEASED,
                                         .l_click = NTHAKA_BUTTON_STATE_RELEASED,
                                         .r_click = NTHAKA_BUTTON_STATE_RELEASED,
                                         .home = NTHAKA_BUTTON_STATE_RELEASED,
                                         .capture = NTHAKA_BUTTON_STATE_RELEASED,
                                         .hat = NTHAKA_HAT_STATE_NEUTRAL,
                                         .l_stick = NTHAKA_STICK_STATE_NEUTRAL,
                                         .r_stick = NTHAKA_STICK_STATE_NEUTRAL,
                                         .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        orca_format_t orca;
        assert(orca_format_init(&orca));
        nthaka_format_t *fmt = (nthaka_format_t *)&orca;

        for (size_t j = 0; j < case_.prev_size - 1; j++)
        {
            assert(nthaka_format_update(fmt, case_.prev[j]) == NTHAKA_BUFFER_PENDING);
        }
        assert(nthaka_format_update(fmt, case_.prev[case_.prev_size - 1]) == NTHAKA_BUFFER_ACCEPTED);
        assert(nthaka_format_deserialize(fmt, case_.prev, case_.prev_size, NULL));
        nthaka_format_reset(fmt);

        for (size_t j = 0; j < case_.size - 1; j++)
        {
            assert(nthaka_format_update(fmt, case_.buf[j]) == NTHAKA_BUFFER_PENDING);
        }
        assert(nthaka_format_update(fmt, case_.buf[case_.size - 1]) == NTHAKA_BUFFER_ACCEPTED);

        nthaka_gamepad_state_t actual;
        assert(nthaka_format_deserialize(fmt, case_.buf, case_.size, &actual));

        if (!nthaka_gamepad_state_equals(&case_.expected, &actual))
        {
            char str0[NTHAKA_GAMEPAD_STATE_STRING_LENGTH_MAX];
            char str1[NTHAKA_GAMEPAD_STATE_STRING_LENGTH_MAX];
            nthaka_gamepad_state_stringify(&(case_.expected), str0, NTHAKA_GAMEPAD_STATE_STRING_LENGTH_MAX);
            nthaka_gamepad_state_stringify(&actual, str1, NTHAKA_GAMEPAD_STATE_STRING_LENGTH_MAX);

            fprintf(stderr, "index: %d, expected: %s, actual: %s\n", i, str0, str1);
            ret++;
        }
    }

    return ret;
}

static int test_reset(void)
{
    int ret = 0;

    orca_format_t orca;
    assert(orca_format_init(&orca));
    nthaka_format_t *fmt = (nthaka_format_t *)&orca;

    assert(nthaka_format_update(fmt, 0x80) == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_update(fmt, 1) == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_update(fmt, 0) == NTHAKA_BUFFER_ACCEPTED);

    nthaka_format_reset(fmt);

    assert(nthaka_format_update(fmt, 0x80) == NTHAKA_BUFFER_PENDING);

    return ret;
}

int test_orca(void)
{
    return run_tests((test_t[]){TEST(test_init),
                                TEST(test_update),
                                TEST(test_deserialize),
                                TEST(test_hold_previous_state),
                                TEST(test_reset)},
                     5);
}