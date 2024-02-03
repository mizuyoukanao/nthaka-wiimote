#include "nthaka/pokecon.h"

#include <assert.h>
#include <stdio.h>

#include "util.h"

static int test_init(void)
{
    int ret = 0;

    typedef struct test_case_t
    {
        pokecon_format_handler_t *arg0;

        bool expected;
    } test_case_t;

    pokecon_format_handler_t pokecon;

    test_case_t cases[] = {
        {.arg0 = NULL, .expected = false},
        {.arg0 = &pokecon, .expected = true},
    };

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        bool actual = pokecon_format_handler_init(case_.arg0);
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

    test_case_t cases[] = {{.seq = "_", //
                            .size = 1,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_REJECTED}},
                           {.seq = "_0", //
                            .size = 2,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_REJECTED, NTHAKA_BUFFER_REJECTED}},
                           {.seq = "0 0\n", //
                            .size = 4,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED}},
                           {.seq = "0 0\n0", //
                            .size = 5,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED, NTHAKA_BUFFER_REJECTED}},
                           {.seq = "0x0004 8\r\n", //
                            .size = 10,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED}},
                           {.seq = "0x0002 8 26 da\r\n", //
                            .size = 16,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED}},
                           {.seq = "0x0001 8 26 26\r\n", //
                            .size = 16,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED}},
                           {.seq = "3 8 0x5a 0x6 80 80\r\n", //
                            .size = 20,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED}},
                           {.seq = "3 8 80 80 0xd 0x48\r\n", //
                            .size = 20,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED}},
                           {.seq = "3 8 80 80\r\n", //
                            .size = 11,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED}},
                           {.seq = "end\r\n", //
                            .size = 5,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED}}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        pokecon_format_handler_t pokecon;
        assert(pokecon_format_handler_init(&pokecon));
        nthaka_format_handler_t *fmt = (nthaka_format_handler_t *)&pokecon;

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
        {.buf = "0x0000 8\r\n", .size = 10, .expected = NTHAKA_GAMEPAD_NEUTRAL},
        {.buf = "0x0004 0\r\n", .size = 10, .expected = {.y = NTHAKA_BUTTON_PRESSED, //
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
        {.buf = (char[]){48, 120, 48, 48, 48, 48, 32, 48, 13, 10}, .size = 10, .expected = {.y = NTHAKA_BUTTON_RELEASED, //
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
        {.buf = "0x0002 8 26 da\r\n", .size = 16, .expected = {.y = NTHAKA_BUTTON_RELEASED, //
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
                                                               .l_stick = {.x = 0x26, .y = 0xda},
                                                               .r_stick = NTHAKA_STICK_NEUTRAL,
                                                               .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.buf = "0x0001 8 26 26\r\n", .size = 16, .expected = {.y = NTHAKA_BUTTON_RELEASED, //
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
                                                               .r_stick = {.x = 0x26, .y = 0x26},
                                                               .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.buf = "3 8 0x5a 0x6 80 80\r\n", .size = 20, .expected = {.y = NTHAKA_BUTTON_RELEASED, //
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
                                                                   .l_stick = {.x = 0x5a, .y = 0x6},
                                                                   .r_stick = {.x = 0x80, .y = 0x80},
                                                                   .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.buf = "3 8 80 80 0xd 0x48\r\n", .size = 20, .expected = {.y = NTHAKA_BUTTON_RELEASED, //
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
                                                                   .l_stick = {.x = 0x80, .y = 0x80},
                                                                   .r_stick = {.x = 0xd, .y = 0x48},
                                                                   .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
        {.buf = "3 8 80 80\r\n", .size = 11, .expected = NTHAKA_GAMEPAD_NEUTRAL},
        {.buf = "end\r\n", .size = 5, .expected = NTHAKA_GAMEPAD_NEUTRAL}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        pokecon_format_handler_t pokecon;
        assert(pokecon_format_handler_init(&pokecon));
        nthaka_format_handler_t *fmt = (nthaka_format_handler_t *)&pokecon;

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

    test_case_t cases[] = {{.prev = "0x0003 8 80 ff ff 80\r\n", //
                            .prev_size = 22,
                            .buf = "0x0004 8\r\n",
                            .size = 10,
                            .expected = {.y = NTHAKA_BUTTON_PRESSED, //
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
                                         .l_stick = {.x = 0x80, .y = 0xFF},
                                         .r_stick = {.x = 0xFF, .y = 0x80},
                                         .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
                           {.prev = "0x0001 8 80 ff\r\n", //
                            .prev_size = 16,
                            .buf = "0x0002 8 ff 80\r\n",
                            .size = 16,
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
                                         .hat = NTHAKA_HAT_NEUTRAL,
                                         .l_stick = {.x = 0xFF, .y = 0x80},
                                         .r_stick = {.x = 0x80, .y = 0xFF},
                                         .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
                           {.prev = "0x0003 8 80 ff ff 80\r\n", //
                            .prev_size = 22,
                            .buf = "3 8 80 80\r\n",
                            .size = 11,
                            .expected = NTHAKA_GAMEPAD_NEUTRAL}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        pokecon_format_handler_t pokecon;
        assert(pokecon_format_handler_init(&pokecon));
        nthaka_format_handler_t *fmt = (nthaka_format_handler_t *)&pokecon;

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

    pokecon_format_handler_t pokecon;
    assert(pokecon_format_handler_init(&pokecon));
    nthaka_format_handler_t *fmt = (nthaka_format_handler_t *)&pokecon;

    assert(nthaka_format_handler_update(fmt, '0') == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_handler_update(fmt, 'x') == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_handler_update(fmt, '0') == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_handler_update(fmt, '0') == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_handler_update(fmt, '0') == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_handler_update(fmt, '0') == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_handler_update(fmt, ' ') == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_handler_update(fmt, '8') == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_handler_update(fmt, '\r') == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_handler_update(fmt, '\n') == NTHAKA_BUFFER_ACCEPTED);

    nthaka_format_handler_reset(fmt);

    assert(nthaka_format_handler_update(fmt, '0') == NTHAKA_BUFFER_PENDING);

    return ret;
}

int test_pokecon(void)
{
    return run_tests((test_t[]){
                         TEST(test_init),
                         TEST(test_update),
                         TEST(test_deserialize),
                         TEST(test_hold_previous_state),
                         TEST(test_reset)},
                     5);
}