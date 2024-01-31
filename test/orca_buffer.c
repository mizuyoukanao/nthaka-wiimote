// #include "orca_buffer.h"

// #include <stdio.h>

// #include "nthaka/orca.h"

// static bool _test_init(void)
// {
//     orca_buffer_t buf;
//     orca_buffer_init(&buf);

//     return true;
// }

// static bool _test_append(void)
// {
//     orca_buffer_t buf_;
//     orca_buffer_init(&buf_);
//     nthaka_buffer_interface_t *buf = (nthaka_buffer_interface_t *)&buf_;

//     buf->append(buf, 0xAB);

//     return true;
// }

// static bool _test_deserialize(void)
// {
//     typedef struct test_case_t
//     {
//         uint8_t *seq;
//         size_t len;
//         nthaka_gamepad_state_t expected;
//     } test_case_t;

//     test_case_t cases[] = {{.seq = (uint8_t[]){0x80, 0b00010101, 0b00010101},
//                             .len = 3,
//                             .expected = {.y = NTHAKA_BUTTON_RELEASED,
//                                          .b = NTHAKA_BUTTON_RELEASED,
//                                          .a = NTHAKA_BUTTON_PRESSED,
//                                          .x = NTHAKA_BUTTON_PRESSED,
//                                          .l = NTHAKA_BUTTON_PRESSED,
//                                          .r = NTHAKA_BUTTON_RELEASED,
//                                          .zl = NTHAKA_BUTTON_RELEASED,
//                                          .zr = NTHAKA_BUTTON_PRESSED,
//                                          .minus = NTHAKA_BUTTON_RELEASED,
//                                          .plus = NTHAKA_BUTTON_RELEASED,
//                                          .l_click = NTHAKA_BUTTON_RELEASED,
//                                          .r_click = NTHAKA_BUTTON_RELEASED,
//                                          .home = NTHAKA_BUTTON_RELEASED,
//                                          .capture = NTHAKA_BUTTON_RELEASED,
//                                          .hat = NTHAKA_HAT_UPLEFT,
//                                          .l_stick = {.x = NTHAKA_STICK_NEUTRAL,
//                                                      .y = NTHAKA_STICK_NEUTRAL},
//                                          .r_stick = {.x = NTHAKA_STICK_NEUTRAL,
//                                                      .y = NTHAKA_STICK_NEUTRAL},
//                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
//                            {.seq = (uint8_t[]){0xFF, 0x80, 0xFF, 0xFF, 0b00010101, 0xFF, 0b00010101},
//                             .len = 7,
//                             .expected = {.y = NTHAKA_BUTTON_RELEASED,
//                                          .b = NTHAKA_BUTTON_RELEASED,
//                                          .a = NTHAKA_BUTTON_PRESSED,
//                                          .x = NTHAKA_BUTTON_PRESSED,
//                                          .l = NTHAKA_BUTTON_PRESSED,
//                                          .r = NTHAKA_BUTTON_RELEASED,
//                                          .zl = NTHAKA_BUTTON_RELEASED,
//                                          .zr = NTHAKA_BUTTON_PRESSED,
//                                          .minus = NTHAKA_BUTTON_RELEASED,
//                                          .plus = NTHAKA_BUTTON_RELEASED,
//                                          .l_click = NTHAKA_BUTTON_RELEASED,
//                                          .r_click = NTHAKA_BUTTON_RELEASED,
//                                          .home = NTHAKA_BUTTON_RELEASED,
//                                          .capture = NTHAKA_BUTTON_RELEASED,
//                                          .hat = NTHAKA_HAT_UPLEFT,
//                                          .l_stick = {.x = NTHAKA_STICK_NEUTRAL,
//                                                      .y = NTHAKA_STICK_NEUTRAL},
//                                          .r_stick = {.x = NTHAKA_STICK_NEUTRAL,
//                                                      .y = NTHAKA_STICK_NEUTRAL},
//                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
//                            {.seq = (uint8_t[]){0x80, 0b00000001, 0, '@'},
//                             .len = 4,
//                             .expected = {
//                                 .y = NTHAKA_BUTTON_RELEASED,
//                                 .b = NTHAKA_BUTTON_RELEASED,
//                                 .a = NTHAKA_BUTTON_PRESSED,
//                                 .x = NTHAKA_BUTTON_RELEASED,
//                                 .l = NTHAKA_BUTTON_RELEASED,
//                                 .r = NTHAKA_BUTTON_RELEASED,
//                                 .zl = NTHAKA_BUTTON_RELEASED,
//                                 .zr = NTHAKA_BUTTON_RELEASED,
//                                 .minus = NTHAKA_BUTTON_RELEASED,
//                                 .plus = NTHAKA_BUTTON_RELEASED,
//                                 .l_click = NTHAKA_BUTTON_RELEASED,
//                                 .r_click = NTHAKA_BUTTON_RELEASED,
//                                 .home = NTHAKA_BUTTON_PRESSED,
//                                 .capture = NTHAKA_BUTTON_RELEASED,
//                                 .hat = NTHAKA_HAT_NEUTRAL,
//                                 .l_stick = {.x = NTHAKA_STICK_NEUTRAL,
//                                             .y = NTHAKA_STICK_NEUTRAL},
//                                 .r_stick = {.x = NTHAKA_STICK_NEUTRAL,
//                                             .y = NTHAKA_STICK_NEUTRAL},
//                                 .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
//                            {.seq = (uint8_t[]){0x80, 0b00000001, 0, '@', 0x80, 0, 0},
//                             .len = 7,
//                             .expected = NTHAKA_GAMEPAD_STATE_NEUTRAL}};
//     size_t length = sizeof(cases) / sizeof(test_case_t);

//     for (size_t i = 0; i < length; i++)
//     {
//         orca_buffer_t buf_;
//         orca_buffer_init(&buf_);
//         nthaka_buffer_interface_t *buf = (nthaka_buffer_interface_t *)&buf_;
//         nthaka_gamepad_state_t actual;

//         for (size_t j = 0; j < cases[i].len; j++)
//         {
//             buf->append(buf, (uint8_t)cases[i].seq[j]);
//         }
//         bool ret = buf->deserialize(buf, &actual);
//         if (!ret || !nthaka_gamepad_state_equals(&(cases[i].expected), &actual))
//         {
//             fprintf(stderr, "[_test_deserialize] Test failed at index %d.\n", i);

//             if (ret)
//             {
//                 char str[NTHAKA_GAMEPAD_STATE_STRING_LENGTH_MAX];
//                 size_t len = sizeof(str) / sizeof(char);
//                 nthaka_gamepad_state_stringify(&(cases[i].expected), str, len);
//                 fprintf(stderr, "expected: %s\n", str);

//                 nthaka_gamepad_state_stringify(&actual, str, len);
//                 fprintf(stderr, "actual  : %s\n", str);
//             }

//             return false;
//         }
//     }

//     return true;
// }

// static bool _test_clear(void)
// {
//     orca_buffer_t buf_;
//     orca_buffer_init(&buf_);
//     nthaka_buffer_interface_t *buf = (nthaka_buffer_interface_t *)&buf_;

//     nthaka_gamepad_state_t expected = {.y = NTHAKA_BUTTON_RELEASED,
//                                       .b = NTHAKA_BUTTON_RELEASED,
//                                       .a = NTHAKA_BUTTON_PRESSED,
//                                       .x = NTHAKA_BUTTON_RELEASED,
//                                       .l = NTHAKA_BUTTON_RELEASED,
//                                       .r = NTHAKA_BUTTON_RELEASED,
//                                       .zl = NTHAKA_BUTTON_RELEASED,
//                                       .zr = NTHAKA_BUTTON_RELEASED,
//                                       .minus = NTHAKA_BUTTON_RELEASED,
//                                       .plus = NTHAKA_BUTTON_RELEASED,
//                                       .l_click = NTHAKA_BUTTON_RELEASED,
//                                       .r_click = NTHAKA_BUTTON_RELEASED,
//                                       .home = NTHAKA_BUTTON_RELEASED,
//                                       .capture = NTHAKA_BUTTON_RELEASED,
//                                       .hat = NTHAKA_HAT_NEUTRAL,
//                                       .l_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                       .r_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                       .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
//     nthaka_gamepad_state_t actual;

//     buf->clear(buf);

//     buf->append(buf, 0x80);
//     buf->append(buf, 1);
//     buf->append(buf, 0);
//     if (!buf->deserialize(buf, &actual) || !nthaka_gamepad_state_equals(&expected, &actual))
//     {
//         fprintf(stderr, "[_test_clear] Test failed.\n");
//         return false;
//     }

//     return true;
// }

// bool test_orca_buffer(void)
// {
//     bool (*tests[])(void) = {
//         _test_init,
//         _test_append,
//         _test_deserialize,
//         _test_clear};
//     size_t length = sizeof(tests) / sizeof(bool (*)(void));
//     for (size_t i = 0; i < length; i++)
//     {
//         if (!tests[i]())
//         {
//             fprintf(stderr, "[test_orca_buffer] Test failed at index %d.\n", i);
//             return false;
//         }
//     }

//     return true;
// }