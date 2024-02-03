#ifndef TEST_UTIL_H_
#define TEST_UTIL_H_

#include <stddef.h>
#include <stdio.h>

typedef struct test_t
{
    char *name;
    int (*test)(void);
} test_t;

#define TEST(t)                   \
    {                             \
        .name = (#t), .test = (t) \
    }

#define SIZE_OF(a) (sizeof((a)) / sizeof((a)[0]))

// Always returns true.
#define TEST_PASSED(name) (printf("\e[32m[  PASSED  ]\e[0m %s\n", (name)) != 0)
#define TEST_FAILED(name) (printf("\e[31m[  FAILED  ]\e[0m %s\n", (name)) != 0)

int run_tests(test_t tests[], size_t size);

#define bool_(b) ((b) ? "true" : "false")
#define nthaka_buffer_state_t_(s) ((s) == NTHAKA_BUFFER_PENDING    ? "NTHAKA_BUFFER_PENDING"  \
                                   : (s) == NTHAKA_BUFFER_REJECTED ? "NTHAKA_BUFFER_REJECTED" \
                                                                   : "NTHAKA_BUFFER_ACCEPTED")
#define nthaka_gamepad_state_equals(s1, s2) ((s1)->y == (s2)->y &&                 \
                                             (s1)->b == (s2)->b &&                 \
                                             (s1)->a == (s2)->a &&                 \
                                             (s1)->x == (s2)->x &&                 \
                                             (s1)->l == (s2)->l &&                 \
                                             (s1)->r == (s2)->r &&                 \
                                             (s1)->zl == (s2)->zl &&               \
                                             (s1)->zr == (s2)->zr &&               \
                                             (s1)->minus == (s2)->minus &&         \
                                             (s1)->plus == (s2)->plus &&           \
                                             (s1)->l_click == (s2)->l_click &&     \
                                             (s1)->r_click == (s2)->r_click &&     \
                                             (s1)->home == (s2)->home &&           \
                                             (s1)->capture == (s2)->capture &&     \
                                             (s1)->hat == (s2)->hat &&             \
                                             (s1)->l_stick.x == (s2)->l_stick.x && \
                                             (s1)->l_stick.y == (s2)->l_stick.y && \
                                             (s1)->r_stick.x == (s2)->r_stick.x && \
                                             (s1)->r_stick.y == (s2)->r_stick.y && \
                                             (s1)->ext[0] == (s2)->ext[0] &&       \
                                             (s1)->ext[1] == (s2)->ext[1] &&       \
                                             (s1)->ext[2] == (s2)->ext[2] &&       \
                                             (s1)->ext[3] == (s2)->ext[3] &&       \
                                             (s1)->ext[4] == (s2)->ext[4] &&       \
                                             (s1)->ext[5] == (s2)->ext[5] &&       \
                                             (s1)->ext[6] == (s2)->ext[6] &&       \
                                             (s1)->ext[7] == (s2)->ext[7] &&       \
                                             (s1)->ext[8] == (s2)->ext[8] &&       \
                                             (s1)->ext[9] == (s2)->ext[9] &&       \
                                             (s1)->ext[10] == (s2)->ext[10] &&     \
                                             (s1)->ext[11] == (s2)->ext[11] &&     \
                                             (s1)->ext[12] == (s2)->ext[12] &&     \
                                             (s1)->ext[13] == (s2)->ext[13] &&     \
                                             (s1)->ext[14] == (s2)->ext[14] &&     \
                                             (s1)->ext[15] == (s2)->ext[15])
#endif // TEST_UTIL_H_