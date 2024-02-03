#include "nthaka.h"
#include "nthaka/nxmc2.h"
#include "nthaka/orca.h"
#include "nthaka/pokecon.h"

#include <assert.h>
#include <stdio.h>

#include "util.h"

static int test_init(void)
{
    int ret = 0;

    typedef struct test_case_t
    {
        nthaka_multi_format_handler_t *arg0;
        nthaka_format_handler_t **arg1;
        size_t arg2;

        bool expected;
    } test_case_t;

    nthaka_multi_format_handler_t multi;
    nthaka_format_handler_t fmt_;
    nthaka_format_handler_t *fmt = &fmt_;

    test_case_t cases[] = {
        {.arg0 = NULL, .arg1 = NULL, .arg2 = 0, .expected = false},
        {.arg0 = NULL, .arg1 = (nthaka_format_handler_t *[]){}, .arg2 = 0, .expected = false},
        {.arg0 = NULL, .arg1 = (nthaka_format_handler_t *[]){NULL}, .arg2 = 1, .expected = false},
        {.arg0 = NULL, .arg1 = (nthaka_format_handler_t *[]){fmt}, .arg2 = 1, .expected = false},
        {.arg0 = &multi, .arg1 = NULL, .arg2 = 0, .expected = false},
        {.arg0 = &multi, .arg1 = (nthaka_format_handler_t *[]){}, .arg2 = 0, .expected = true},
        {.arg0 = &multi, .arg1 = (nthaka_format_handler_t *[]){NULL}, .arg2 = 1, .expected = false},
        {.arg0 = &multi, .arg1 = (nthaka_format_handler_t *[]){fmt}, .arg2 = 1, .expected = true}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        bool actual = nthaka_multi_format_handler_init(case_.arg0, case_.arg1, case_.arg2);
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

    test_case_t cases[] = {
        {.seq = (uint8_t[]){0x79},
         .size = 1,
         .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_REJECTED}},
        /* NXMC2 */
        {.seq = (uint8_t[]){0xAB, 0x00, 0x40},
         .size = 3,
         .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_REJECTED}},
        {.seq = (uint8_t[]){0xAB, 0x00, 0x00, 0x08, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, /**/ 0xAB},
         .size = 12,
         .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED, NTHAKA_BUFFER_REJECTED}},
        /* ORCA */
        {.seq = (uint8_t[]){0x80, 0x00, 0xFF, 0x00, /**/ 0x80},
         .size = 5,
         .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED, NTHAKA_BUFFER_REJECTED}},
        /* PokeCon */
        {.seq = "0x0000a",
         .size = 7,
         .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_REJECTED}},
        {.seq = "0x0000 8\r\n0",
         .size = 11,
         .expected = (nthaka_buffer_state_t[]){NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_PENDING, NTHAKA_BUFFER_ACCEPTED, NTHAKA_BUFFER_REJECTED}},
    };

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        nxmc2_format_handler_t nxmc2;
        assert(nxmc2_format_handler_init(&nxmc2));
        orca_format_handler_t orca;
        assert(orca_format_handler_init(&orca));
        pokecon_format_handler_t pokecon;
        assert(pokecon_format_handler_init(&pokecon));

        nthaka_multi_format_handler_t multi;
        assert(nthaka_multi_format_handler_init(&multi, (nthaka_format_handler_t *[]){(nthaka_format_handler_t *)&nxmc2, //
                                                                                      (nthaka_format_handler_t *)&orca,  //
                                                                                      (nthaka_format_handler_t *)&pokecon},
                                                3));
        nthaka_format_handler_t *fmt = (nthaka_format_handler_t *)&multi;

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
        {.buf = "0x0000 8\r\n", .size = 10, .expected = NTHAKA_GAMEPAD_STATE_NEUTRAL},
        {.buf = (uint8_t[]){0x80, 0, 0}, .size = 3, .expected = NTHAKA_GAMEPAD_STATE_NEUTRAL},
        {.buf = (uint8_t[]){0xAB, 0x00, 0x00, 0x08, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00}, .size = 11, .expected = NTHAKA_GAMEPAD_STATE_NEUTRAL}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        nxmc2_format_handler_t nxmc2;
        assert(nxmc2_format_handler_init(&nxmc2));
        orca_format_handler_t orca;
        assert(orca_format_handler_init(&orca));
        pokecon_format_handler_t pokecon;
        assert(pokecon_format_handler_init(&pokecon));

        nthaka_multi_format_handler_t multi;
        assert(nthaka_multi_format_handler_init(&multi, (nthaka_format_handler_t *[]){(nthaka_format_handler_t *)&nxmc2, //
                                                                                      (nthaka_format_handler_t *)&orca,  //
                                                                                      (nthaka_format_handler_t *)&pokecon},
                                                3));
        nthaka_format_handler_t *fmt = (nthaka_format_handler_t *)&multi;

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
                                         .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
                           {.prev = "0x0001 8 80 ff\r\n", //
                            .prev_size = 16,
                            .buf = "0x0002 8 ff 80\r\n",
                            .size = 16,
                            .expected = {.y = NTHAKA_BUTTON_STATE_RELEASED, //
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
                                         .home = NTHAKA_BUTTON_STATE_RELEASED,
                                         .capture = NTHAKA_BUTTON_STATE_RELEASED,
                                         .hat = NTHAKA_HAT_STATE_NEUTRAL,
                                         .l_stick = {.x = 0xFF, .y = 0x80},
                                         .r_stick = {.x = 0x80, .y = 0xFF},
                                         .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
                           {.prev = "0x0003 8 80 ff ff 80\r\n", //
                            .prev_size = 22,
                            .buf = "3 8 80 80\r\n",
                            .size = 11,
                            .expected = NTHAKA_GAMEPAD_STATE_NEUTRAL}};

    for (size_t i = 0; i < SIZE_OF(cases); i++)
    {
        test_case_t case_ = cases[i];

        nxmc2_format_handler_t nxmc2;
        assert(nxmc2_format_handler_init(&nxmc2));
        orca_format_handler_t orca;
        assert(orca_format_handler_init(&orca));
        pokecon_format_handler_t pokecon;
        assert(pokecon_format_handler_init(&pokecon));

        nthaka_multi_format_handler_t multi;
        assert(nthaka_multi_format_handler_init(&multi, (nthaka_format_handler_t *[]){(nthaka_format_handler_t *)&nxmc2, //
                                                                                      (nthaka_format_handler_t *)&orca,  //
                                                                                      (nthaka_format_handler_t *)&pokecon},
                                                3));
        nthaka_format_handler_t *fmt = (nthaka_format_handler_t *)&multi;

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

    nxmc2_format_handler_t nxmc2;
    assert(nxmc2_format_handler_init(&nxmc2));
    orca_format_handler_t orca;
    assert(orca_format_handler_init(&orca));
    pokecon_format_handler_t pokecon;
    assert(pokecon_format_handler_init(&pokecon));

    nthaka_multi_format_handler_t multi;
    assert(nthaka_multi_format_handler_init(&multi, (nthaka_format_handler_t *[]){(nthaka_format_handler_t *)&nxmc2, //
                                                                                  (nthaka_format_handler_t *)&orca,  //
                                                                                  (nthaka_format_handler_t *)&pokecon},
                                            3));
    nthaka_format_handler_t *fmt = (nthaka_format_handler_t *)&multi;

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

    assert(nthaka_format_handler_update(fmt, 0xAB) == NTHAKA_BUFFER_PENDING);

    return ret;
}

int test_multi_format(void)
{
    return run_tests((test_t[]){
                         TEST(test_init),
                         TEST(test_update),
                         TEST(test_deserialize),
                         TEST(test_hold_previous_state),
                         TEST(test_reset)},
                     5);
}