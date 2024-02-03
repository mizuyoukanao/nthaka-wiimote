#include "nthaka/dol.h"

#include <assert.h>
#include <stdio.h>

#include "util.h"

static int test_init(void)
{
    int ret = 0;

    typedef struct test_case_t
    {
        dol_format_handler_t *arg0;

        bool expected;
    } test_case_t;

    dol_format_handler_t dol;

    test_case_t cases[] = {
        {.arg0 = NULL, .expected = false},
        {.arg0 = &dol, .expected = true},
    };

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        bool actual = dol_format_handler_init(case_.arg0);
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
        char *seq;
        size_t size;
        nthaka_buffer_state_t *expected;
    } test_case_t;

    test_case_t cases[] = {{.seq = "@", //
                            .size = 1,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_ACCEPTED}},
                           {.seq = "aa", //
                            .size = 2,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_ACCEPTED, NTHAKA_BUFFER_REJECTED}},
                           {.seq = "?", //
                            .size = 1,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_REJECTED}}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        dol_format_handler_t dol;
        assert(dol_format_handler_init(&dol));
        nthaka_format_handler_t *fmt = (nthaka_format_handler_t *)&dol;

        for (size_t j = 0; j < case_.size; j++)
        {
            nthaka_buffer_state_t actual = nthaka_format_handler_update(fmt, case_.seq[j]);
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
        char *buf;
        size_t size;

        nthaka_gamepad_state_t expected;
    } test_case_t;

    test_case_t cases[] = {
        {.buf = "a", .size = 1, .expected = {.y = NTHAKA_BUTTON_RELEASED, //
                                             .b = NTHAKA_BUTTON_RELEASED,
                                             .a = NTHAKA_BUTTON_PRESSED,
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
                                             .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.buf = "y", .size = 1, .expected = {.y = NTHAKA_BUTTON_RELEASED, //
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
                                             .l_stick = {.x = 0, .y = NTHAKA_STICK_AXIS_NEUTRAL},
                                             .r_stick = NTHAKA_STICK_NEUTRAL,
                                             .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.buf = "l", .size = 1, .expected = {.y = NTHAKA_BUTTON_RELEASED, //
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
                                             .hat = NTHAKA_HAT_UP,
                                             .l_stick = NTHAKA_STICK_NEUTRAL,
                                             .r_stick = NTHAKA_STICK_NEUTRAL,
                                             .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.buf = "@", .size = 1, .expected = {.y = NTHAKA_BUTTON_RELEASED, //
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
                                             .home = NTHAKA_BUTTON_PRESSED,
                                             .capture = NTHAKA_BUTTON_RELEASED,
                                             .hat = NTHAKA_HAT_NEUTRAL,
                                             .l_stick = NTHAKA_STICK_NEUTRAL,
                                             .r_stick = NTHAKA_STICK_NEUTRAL,
                                             .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        dol_format_handler_t dol;
        assert(dol_format_handler_init(&dol));
        nthaka_format_handler_t *fmt = (nthaka_format_handler_t *)&dol;

        for (size_t j = 0; j < case_.size - 1; j++)
        {
            assert(nthaka_format_handler_update(fmt, case_.buf[j]) == NTHAKA_BUFFER_PENDING);
        }
        assert(nthaka_format_handler_update(fmt, case_.buf[case_.size - 1]) == NTHAKA_BUFFER_ACCEPTED);

        nthaka_gamepad_state_t actual_out;
        assert(nthaka_format_handler_deserialize(fmt, case_.buf, case_.size, &actual_out));
        if (!nthaka_gamepad_state_equals(&case_.expected, &actual_out))
        {
            char str0[NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX];
            char str1[NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX];
            nthaka_gamepad_state_stringify(&(case_.expected), str0, NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX);
            nthaka_gamepad_state_stringify(&actual_out, str1, NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX);

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
        char *prev;
        size_t prev_size;

        char *buf;
        size_t size;

        nthaka_gamepad_state_t expected;
    } test_case_t;

    test_case_t cases[] = {{.prev = "a", //
                            .prev_size = 1,
                            .buf = "@",
                            .size = 1,
                            .expected = {.y = NTHAKA_BUTTON_RELEASED, //
                                         .b = NTHAKA_BUTTON_RELEASED,
                                         .a = NTHAKA_BUTTON_PRESSED,
                                         .x = NTHAKA_BUTTON_RELEASED,
                                         .l = NTHAKA_BUTTON_RELEASED,
                                         .r = NTHAKA_BUTTON_RELEASED,
                                         .zl = NTHAKA_BUTTON_RELEASED,
                                         .zr = NTHAKA_BUTTON_RELEASED,
                                         .minus = NTHAKA_BUTTON_RELEASED,
                                         .plus = NTHAKA_BUTTON_RELEASED,
                                         .l_click = NTHAKA_BUTTON_RELEASED,
                                         .r_click = NTHAKA_BUTTON_RELEASED,
                                         .home = NTHAKA_BUTTON_PRESSED,
                                         .capture = NTHAKA_BUTTON_RELEASED,
                                         .hat = NTHAKA_HAT_NEUTRAL,
                                         .l_stick = NTHAKA_STICK_NEUTRAL,
                                         .r_stick = NTHAKA_STICK_NEUTRAL,
                                         .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
                           {.prev = "l", //
                            .prev_size = 1,
                            .buf = "j",
                            .size = 1,
                            .expected = {.y = NTHAKA_BUTTON_RELEASED, //
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
                                         .hat = NTHAKA_HAT_UPRIGHT,
                                         .l_stick = NTHAKA_STICK_NEUTRAL,
                                         .r_stick = NTHAKA_STICK_NEUTRAL,
                                         .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        dol_format_handler_t dol;
        assert(dol_format_handler_init(&dol));
        nthaka_format_handler_t *fmt = (nthaka_format_handler_t *)&dol;

        for (size_t j = 0; j < case_.prev_size - 1; j++)
        {
            assert(nthaka_format_handler_update(fmt, case_.prev[j]) == NTHAKA_BUFFER_PENDING);
        }
        assert(nthaka_format_handler_update(fmt, case_.prev[case_.prev_size - 1]) == NTHAKA_BUFFER_ACCEPTED);
        assert(nthaka_format_handler_deserialize(fmt, case_.prev, case_.prev_size, NULL));
        nthaka_format_handler_reset(fmt);

        for (size_t j = 0; j < case_.size - 1; j++)
        {
            assert(nthaka_format_handler_update(fmt, case_.buf[j]) == NTHAKA_BUFFER_PENDING);
        }
        assert(nthaka_format_handler_update(fmt, case_.buf[case_.size - 1]) == NTHAKA_BUFFER_ACCEPTED);

        nthaka_gamepad_state_t actual;
        assert(nthaka_format_handler_deserialize(fmt, case_.buf, case_.size, &actual));

        if (!nthaka_gamepad_state_equals(&case_.expected, &actual))
        {
            char str0[NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX];
            char str1[NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX];
            nthaka_gamepad_state_stringify(&(case_.expected), str0, NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX);
            nthaka_gamepad_state_stringify(&actual, str1, NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX);

            fprintf(stderr, "index: %d, expected: %s, actual: %s\n", i, str0, str1);
            ret++;
        }
    }

    return ret;
}

static int test_reset(void)
{
    int ret = 0;

    dol_format_handler_t dol;
    assert(dol_format_handler_init(&dol));
    nthaka_format_handler_t *fmt = (nthaka_format_handler_t *)&dol;

    assert(nthaka_format_handler_update(fmt, 'a') == NTHAKA_BUFFER_ACCEPTED);

    nthaka_format_handler_reset(fmt);

    assert(nthaka_format_handler_update(fmt, 'a') == NTHAKA_BUFFER_ACCEPTED);

    return ret;
}

int test_dol(void)
{
    return run_tests((test_t[]){TEST(test_init),
                                TEST(test_update),
                                TEST(test_deserialize),
                                TEST(test_hold_previous_state),
                                TEST(test_reset)},
                     5);
}