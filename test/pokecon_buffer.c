// #include "pokecon_buffer.h"

// #include <stdio.h>

// #include "nthaka/pokecon.h"

// static bool _test_init(void)
// {
//     pokecon_buffer_t buf;
//     pokecon_buffer_init(&buf);

//     return true;
// }

// static bool _test_append(void)
// {
//     pokecon_buffer_t buf_;
//     pokecon_buffer_init(&buf_);
//     nthaka_buffer_interface_t *buf = (nthaka_buffer_interface_t *)&buf_;

//     buf->append(buf, '0');

//     return true;
// }

// static bool _test_deserialize(void)
// {
//     typedef struct test_case_t
//     {
//         char *seq;
//         size_t len;
//         nthaka_gamepad_state_t expected;
//     } test_case_t;

//     test_case_t cases[] = {{.seq = "0 0\n",
//                             .len = 4,
//                             .expected = {.y = NTHAKA_BUTTON_RELEASED,
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
//                                          .capture = NTHAKA_BUTTON_RELEASED,
//                                          .hat = NTHAKA_HAT_UP,
//                                          .l_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                          .r_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
//                            {.seq = "0x0004 8\r\n",
//                             .len = 10,
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
//                                          .capture = NTHAKA_BUTTON_RELEASED,
//                                          .hat = NTHAKA_HAT_NEUTRAL,
//                                          .l_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                          .r_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
//                            {.seq = "0x0002 8 26 da\r\n",
//                             .len = 16,
//                             .expected = {.y = NTHAKA_BUTTON_RELEASED,
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
//                                          .capture = NTHAKA_BUTTON_RELEASED,
//                                          .hat = NTHAKA_HAT_NEUTRAL,
//                                          .l_stick = {.x = 0x26, .y = 0xda},
//                                          .r_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
//                            {.seq = "0x0001 8 26 26\r\n",
//                             .len = 16,
//                             .expected = {.y = NTHAKA_BUTTON_RELEASED,
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
//                                          .capture = NTHAKA_BUTTON_RELEASED,
//                                          .hat = NTHAKA_HAT_NEUTRAL,
//                                          .l_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                          .r_stick = {.x = 0x26, .y = 0x26},
//                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
//                            {.seq = "3 8 0x5a 0x6 80 80\r\n",
//                             .len = 20,
//                             .expected = {.y = NTHAKA_BUTTON_RELEASED,
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
//                                          .capture = NTHAKA_BUTTON_RELEASED,
//                                          .hat = NTHAKA_HAT_NEUTRAL,
//                                          .l_stick = {.x = 0x5a, .y = 0x6},
//                                          .r_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
//                            {.seq = "3 8 80 80 0xd 0x48\r\n",
//                             .len = 20,
//                             .expected = {.y = NTHAKA_BUTTON_RELEASED,
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
//                                          .capture = NTHAKA_BUTTON_RELEASED,
//                                          .hat = NTHAKA_HAT_NEUTRAL,
//                                          .l_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                          .r_stick = {.x = 0xd, .y = 0x48},
//                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
//                            {.seq = "3 8 80 80\r\n",
//                             .len = 11,
//                             .expected = NTHAKA_GAMEPAD_STATE_NEUTRAL},
//                            {.seq = "0x0002 8 81 81\r\n0x0000 8\r\n",
//                             .len = 26,
//                             .expected = {.y = NTHAKA_BUTTON_RELEASED,
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
//                                          .capture = NTHAKA_BUTTON_RELEASED,
//                                          .hat = NTHAKA_HAT_NEUTRAL,
//                                          .l_stick = {.x = 0x81, .y = 0x81},
//                                          .r_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
//                            {.seq = "0x0002 8 80 80\r\n0x0002 8 ff 80\r\n0x0002 8 80 80\r\n",
//                             .len = 48,
//                             .expected = {.y = NTHAKA_BUTTON_RELEASED,
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
//                                          .capture = NTHAKA_BUTTON_RELEASED,
//                                          .hat = NTHAKA_HAT_NEUTRAL,
//                                          .l_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                          .r_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}}};
//     size_t length = sizeof(cases) / sizeof(test_case_t);

//     for (size_t i = 0; i < length; i++)
//     {
//         pokecon_buffer_t buf_;
//         pokecon_buffer_init(&buf_);
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

// static bool _test_clear(void)
// {
//     pokecon_buffer_t buf_;
//     pokecon_buffer_init(&buf_);
//     nthaka_buffer_interface_t *buf = (nthaka_buffer_interface_t *)&buf_;

//     nthaka_gamepad_state_t expected = {.y = NTHAKA_BUTTON_RELEASED,
//                                       .b = NTHAKA_BUTTON_RELEASED,
//                                       .a = NTHAKA_BUTTON_RELEASED,
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
//                                       .hat = NTHAKA_HAT_UP,
//                                       .l_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                       .r_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL},
//                                       .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
//     nthaka_gamepad_state_t actual;

//     buf->clear(buf);

//     buf->append(buf, '0');
//     buf->append(buf, ' ');
//     buf->append(buf, '0');
//     buf->append(buf, '\n');

//     if (!buf->deserialize(buf, &actual) || !nthaka_gamepad_state_equals(&expected, &actual))
//     {
//         fprintf(stderr, "[_test_clear] Test failed.\n");
//         return false;
//     }

//     return true;
// }

// bool test_pokecon_buffer(void)
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
//             fprintf(stderr, "[test_pokecon_buffer] Test failed at index %d.\n", i);
//             return false;
//         }
//     }

//     return true;
// }