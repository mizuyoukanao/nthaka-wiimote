#include "buffer.h"

#include <assert.h>
#include <stdio.h>

#include "mock_state_machine.h"
#include "util.h"

static int test_init(void)
{
    int ret = 0;

    typedef struct test_case_t
    {
        nxamf_buffer_t *arg0;
        nxamf_state_machine_t *arg1;

        bool expected;
    } test_case_t;

    nxamf_buffer_t buf;
    nxamf_state_machine_t sm;

    test_case_t cases[] = {
        {.arg0 = NULL, .arg1 = NULL, .expected = false},
        {.arg0 = &buf, .arg1 = NULL, .expected = false},
        {.arg0 = NULL, .arg1 = &sm, .expected = false},
        {.arg0 = &buf, .arg1 = &sm, .expected = true},
    };
    const size_t len = sizeof(cases) / sizeof(test_case_t);

    for (size_t i = 0; i < len; i++)
    {
        test_case_t case_ = cases[i];

        bool actual = nxamf_buffer_init(case_.arg0, case_.arg1);
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
        size_t len;
        uint8_t d;

        nxamf_buffer_state_t expected_ret;
        nxamf_gamepad_state_t expected_out;
    } test_case_t;

    test_case_t cases[] = {{.prev = (uint8_t[]){}, .len = 0, .d = 0, .expected_ret = NXAMF_BUFFER_STATE_PENDING},
                           {.prev = (uint8_t[]){}, .len = 0, .d = 1, .expected_ret = NXAMF_BUFFER_STATE_REJECTED},
                           {.prev = (uint8_t[]){0}, .len = 1, .d = 1, .expected_ret = NXAMF_BUFFER_STATE_PENDING},
                           {.prev = (uint8_t[]){0}, .len = 1, .d = 2, .expected_ret = NXAMF_BUFFER_STATE_REJECTED},
                           {.prev = (uint8_t[]){0, 1}, .len = 2, .d = 2, .expected_ret = NXAMF_BUFFER_STATE_ACCEPTED, .expected_out = {.y = NXAMF_BUTTON_RELEASED, //
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
                                                                                                                                       .extension = {/**/ 0, 1, 2, /**/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
                           {.prev = (uint8_t[]){0, 1}, .len = 2, .d = 3, .expected_ret = NXAMF_BUFFER_STATE_REJECTED},
                           {.prev = (uint8_t[]){0, 1, 2}, .len = 3, .d = 0, .expected_ret = NXAMF_BUFFER_STATE_REJECTED}};
    const size_t len = sizeof(cases) / sizeof(test_case_t);

    for (size_t i = 0; i < len; i++)
    {
        test_case_t case_ = cases[i];

        mock_state_machine_t sm;
        mock_state_machine_init(&sm);
        nxamf_buffer_t buf;
        assert(nxamf_buffer_init(&buf, (nxamf_state_machine_t *)&sm));
        nxamf_gamepad_state_t actual_out;

        for (size_t j = 0; j < case_.len; j++)
        {
            nxamf_buffer_append(&buf, case_.prev[j], NULL);
        }

        nxamf_buffer_state_t actual_ret = nxamf_buffer_append(&buf, case_.d, &actual_out);
        if (actual_ret != case_.expected_ret)
        {
            fprintf(stderr, "index: %d, expected: %s, actual: %s\n", i, nxamf_buffer_state_t_(case_.expected_ret), nxamf_buffer_state_t_(actual_ret));
            ret++;
        }
        else if (actual_ret == NXAMF_BUFFER_STATE_ACCEPTED && !nxamf_gamepad_state_equals(&actual_out, &(case_.expected_out)))
        {
            char str0[NXAMF_GAMEPAD_STATE_STRING_LENGTH_MAX];
            char str1[NXAMF_GAMEPAD_STATE_STRING_LENGTH_MAX];
            nxamf_gamepad_state_stringify(&(case_.expected_out), str0, NXAMF_GAMEPAD_STATE_STRING_LENGTH_MAX);
            nxamf_gamepad_state_stringify(&actual_out, str1, NXAMF_GAMEPAD_STATE_STRING_LENGTH_MAX);

            fprintf(stderr, "index: %i, expected: %s, actual: %s\n", i, str0, str1);
            ret++;
        }
    }

    return ret;
}

static int test_clear(void)
{
    int ret = 0;

    mock_state_machine_t sm;
    mock_state_machine_init(&sm);
    nxamf_buffer_t buf;
    assert(nxamf_buffer_init(&buf, (nxamf_state_machine_t *)&sm));

    nxamf_buffer_append(&buf, 0, NULL);
    nxamf_buffer_append(&buf, 1, NULL);
    nxamf_buffer_append(&buf, 2, NULL);

    assert(sm.s == MOCK_STATE_FINAL);

    nxamf_buffer_clear(&buf);

    assert(sm.s == MOCK_STATE_INITIAL);

    nxamf_buffer_append(&buf, 0, NULL);
    nxamf_buffer_append(&buf, 1, NULL);
    nxamf_buffer_append(&buf, 2, NULL);

    assert(sm.s == MOCK_STATE_FINAL);

    return ret;
}

int test_buffer(void)
{
    printf("test_buffer:\n");
    int ret = 0;

    typedef struct test_t
    {
        char *name;
        int (*test)(void);
    } test_t;

    test_t tests[] = {{.name = "test_init", test_init},
                      {.name = "test_append", test_append},
                      {.name = "test_clear", test_clear}};
    const size_t len = sizeof(tests) / sizeof(test_t);

    for (size_t i = 0; i < len; i++)
    {
        if (tests[i].test() == 0)
        {
            printf("    %s: passed\n", tests[i].name);
        }
        else
        {
            printf("    %s: failed\n", tests[i].name);
            ret++;
        }
    }

    return ret;
}