// #include "multi_buffer_manager.h"

// #include <stdio.h>

// #include "nthaka.h"
// #include "nthaka/nxmc2.h"
// #include "nthaka/orca.h"
// #include "nthaka/pokecon.h"

// static bool _test_init(void)
// {
//     nthaka_buffer_interface_t *bufs[] = {};
//     nthaka_multi_buffer_manager_t buf;
//     nthaka_multi_buffer_manager_init(&buf, bufs, 0);

//     return true;
// }

// static bool _test_append(void)
// {
//     nthaka_buffer_interface_t *bufs[] = {};
//     nthaka_multi_buffer_manager_t buf_;
//     nthaka_multi_buffer_manager_init(&buf_, bufs, 0);
//     nthaka_buffer_interface_t *buf = (nthaka_buffer_interface_t *)&buf_;

//     buf->append(buf, 0);

//     return true;
// }

// static bool _test_deserialize(void)
// {
//     typedef struct test_case_t
//     {
//         uint8_t *seq;
//         size_t len;
//         size_t from;
//         nthaka_gamepad_state_t expected;
//     } test_case_t;

//     test_case_t cases[] = {{.seq = (uint8_t[]){0xABU, 0x01U, 0x20U, 0x08U, 0x80U, 0x80U, 0x80U, 0x80U, 0x00U, 0x00U, 0x00U},
//                             .len = 11,
//                             .from = 0,
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
//                            {.seq = (uint8_t[]){0x80, 0b00000001, 0, '@'},
//                             .len = 4,
//                             .from = 1,
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
//                            {.seq = "0x0000 8\r\n", //
//                             .len = 10,
//                             .from = 2,
//                             .expected = {.y = NTHAKA_BUTTON_RELEASED, //
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
//                                          .l_stick = {.x = NTHAKA_STICK_NEUTRAL, //
//                                                      .y = NTHAKA_STICK_NEUTRAL},
//                                          .r_stick = {.x = NTHAKA_STICK_NEUTRAL, //
//                                                      .y = NTHAKA_STICK_NEUTRAL},
//                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
//                            {.seq = "0x0002 8 80 80\r\n0x0002 8 ff 80\r\n0x0002 8 80 80\r\n", //
//                             .len = 48,
//                             .from = 2,
//                             .expected = {.y = NTHAKA_BUTTON_RELEASED, //
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
//                                          .l_stick = {.x = NTHAKA_STICK_NEUTRAL, //
//                                                      .y = NTHAKA_STICK_NEUTRAL},
//                                          .r_stick = {.x = NTHAKA_STICK_NEUTRAL, //
//                                                      .y = NTHAKA_STICK_NEUTRAL},
//                                          .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}}};
//     size_t length = sizeof(cases) / sizeof(test_case_t);

//     for (size_t i = 0; i < length; i++)
//     {
//         nxmc2_buffer_t nxmc2;
//         nxmc2_buffer_init(&nxmc2);
//         orca_buffer_t orca;
//         orca_buffer_init(&orca);
//         pokecon_buffer_t pokecon;
//         pokecon_buffer_init(&pokecon);
//         nthaka_buffer_interface_t *bufs[] = {(nthaka_buffer_interface_t *)&nxmc2,
//                                             (nthaka_buffer_interface_t *)&orca,
//                                             (nthaka_buffer_interface_t *)&pokecon};

//         nthaka_multi_buffer_manager_t buf_;
//         nthaka_multi_buffer_manager_init(&buf_, bufs, sizeof(bufs) / sizeof(nthaka_buffer_interface_t *));
//         nthaka_buffer_interface_t *buf = (nthaka_buffer_interface_t *)&buf_;
//         nthaka_gamepad_state_t actual;

//         for (size_t j = 0; j < cases[i].len; j++)
//         {
//             buf->append(buf, cases[i].seq[j]);
//         }
//         bool ret = buf->deserialize(buf, &actual);
//         size_t from = nthaka_multi_buffer_manager_get_last_deserialized_index(&buf_);
//         if (!ret || from != cases[i].from || !nthaka_gamepad_state_equals(&(cases[i].expected), &actual))
//         {
//             fprintf(stderr, "[_test_deserialize] Test failed at index %d.\n", i);

//             if (ret)
//             {
//                 char str[NTHAKA_GAMEPAD_STATE_STRING_LENGTH_MAX];
//                 size_t len = sizeof(str) / sizeof(char);
//                 nthaka_gamepad_state_stringify(&(cases[i].expected), str, len);
//                 fprintf(stderr, "from: %d\n", cases[i].from);
//                 fprintf(stderr, "expected: %s\n", str);

//                 nthaka_gamepad_state_stringify(&actual, str, len);
//                 fprintf(stderr, "from: %d\n", from);
//                 fprintf(stderr, "actual  : %s\n", str);
//             }

//             return false;
//         }
//     }

//     return true;
// }

// static bool _test_clear(void)
// {
//     nxmc2_buffer_t nxmc2;
//     nxmc2_buffer_init(&nxmc2);
//     nthaka_buffer_interface_t *bufs[] = {(nthaka_buffer_interface_t *)&nxmc2};

//     nthaka_multi_buffer_manager_t buf_;
//     nthaka_multi_buffer_manager_init(&buf_, bufs, 1);
//     nthaka_buffer_interface_t *buf = (nthaka_buffer_interface_t *)&buf_;

//     nthaka_gamepad_state_t expected = NTHAKA_GAMEPAD_STATE_NEUTRAL;
//     nthaka_gamepad_state_t actual;

//     buf->clear(buf);

//     buf->append(buf, 0xAB);
//     buf->append(buf, 0);
//     buf->append(buf, 0);
//     buf->append(buf, 8);
//     buf->append(buf, 128);
//     buf->append(buf, 128);
//     buf->append(buf, 128);
//     buf->append(buf, 128);
//     buf->append(buf, 0);
//     buf->append(buf, 0);
//     buf->append(buf, 0);

//     bool ret = buf->deserialize(buf, &actual);
//     if (!ret || !nthaka_gamepad_state_equals(&expected, &actual))
//     {
//         fprintf(stderr, "[_test_clear] Test failed.\n");
//         return false;
//     }

//     return true;
// }

// bool test_multi_buffer_manager(void)
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
//             fprintf(stderr, "[test_multi_buffer_manager] Test failed at index %d.\n", i);
//             return false;
//         }
//     }

//     return true;
// }