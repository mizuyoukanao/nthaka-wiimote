#include "nthaka/nxmc2.h"

#include <assert.h>
#include <stdio.h>

#include "util.h"

static int test_init(void)
{
    int ret = 0;

    typedef struct test_case_t
    {
        nxmc2_format_t *arg0;

        bool expected;
    } test_case_t;

    nxmc2_format_t nxmc2;

    test_case_t cases[] = {
        {.arg0 = NULL, .expected = false},
        {.arg0 = &nxmc2, .expected = true},
    };

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        bool actual = nxmc2_format_init(case_.arg0);
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
                           {.seq = (uint8_t[]){0, 0xAB}, //
                            .size = 2,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_REJECTED, NTHAKA_BUFFER_REJECTED}},
                           {.seq = (uint8_t[]){0xAB, 0, 0x40}, //
                            .size = 3,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_REJECTED}},
                           {.seq = (uint8_t[]){0xAB, 0, 0x3F}, //
                            .size = 3,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING}},
                           {.seq = (uint8_t[]){0xAB, 0, 0x3F, 9}, //
                            .size = 4,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_REJECTED}},
                           {.seq = (uint8_t[]){0xAB, 0, 0x3F, 8}, //
                            .size = 4,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING}},
                           {.seq = (uint8_t[]){0xAB, 0, 0x3F, 8, 0x80, 0x80, 0x80, 0x80, 0, 0, 0}, //
                            .size = 11,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED}},
                           {.seq = (uint8_t[]){0xAB, 0, 0x3F, 8, 0x80, 0x80, 0x80, 0x80, 0, 0, 0, 0}, //
                            .size = 12,
                            .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED, NTHAKA_BUFFER_REJECTED}}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        nxmc2_format_t nxmc2;
        assert(nxmc2_format_init(&nxmc2));
        nthaka_format_t *fmt = (nthaka_format_t *)&nxmc2;

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

        nthaka_gamepad_state_t expected;
    } test_case_t;

    test_case_t cases[] = {
        {.buf = (uint8_t[]){0xAB, 0, 0, 8, 128, 128, 128, 128, 0, 0, 0}, .expected = NTHAKA_GAMEPAD_STATE_NEUTRAL},
        {.buf = (uint8_t[]){0xAB, 1, 1, 8, 129, 127, 127, 129, 1, 2, 3}, .expected = {.y = NTHAKA_BUTTON_PRESSED, //
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
                                                                                      .l_stick = {.x = 129, .y = 127},
                                                                                      .r_stick = {.x = 127, .y = 129},
                                                                                      .extension = {1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        nxmc2_format_t nxmc2;
        assert(nxmc2_format_init(&nxmc2));
        nthaka_format_t *fmt = (nthaka_format_t *)&nxmc2;

        for (size_t j = 0; j < 10; j++)
        {
            assert(nthaka_format_update(fmt, case_.buf[j]) == NTHAKA_BUFFER_PENDING);
        }
        assert(nthaka_format_update(fmt, case_.buf[10]) == NTHAKA_BUFFER_ACCEPTED);

        nthaka_gamepad_state_t actual_out;
        assert(nthaka_format_deserialize(fmt, case_.buf, 11, &actual_out));
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

static int test_reset(void)
{
    int ret = 0;

    nxmc2_format_t nxmc2;
    assert(nxmc2_format_init(&nxmc2));
    nthaka_format_t *fmt = (nthaka_format_t *)&nxmc2;

    assert(nthaka_format_update(fmt, 0xAB) == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_update(fmt, 0) == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_update(fmt, 0x3F) == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_update(fmt, 8) == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_update(fmt, 0x80) == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_update(fmt, 0x80) == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_update(fmt, 0x80) == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_update(fmt, 0x80) == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_update(fmt, 0) == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_update(fmt, 1) == NTHAKA_BUFFER_PENDING);
    assert(nthaka_format_update(fmt, 2) == NTHAKA_BUFFER_ACCEPTED);

    nthaka_format_reset(fmt);

    assert(nthaka_format_update(fmt, 0xAB) == NTHAKA_BUFFER_PENDING);

    return ret;
}

int test_nxmc2(void)
{
    return run_tests((test_t[]){TEST(test_init),
                                TEST(test_update),
                                TEST(test_deserialize),
                                TEST(test_reset)},
                     4);
}