#ifndef TEST_UTIL_H_
#define TEST_UTIL_H_

#include "nxamf.h"

typedef struct TestProtocolParams
{
    uint8_t *data;
    size_t length;
    NxamfGamepadState expected;
} TestProtocolParams;

void compare_states(NxamfGamepadState *s1, NxamfGamepadState *s2);
bool nxamf_gamepad_state_equals(NxamfGamepadState *s1, NxamfGamepadState *s2);
void test_protocol_params_run(NxamfBytesProtocolInterface *protocol, TestProtocolParams params[], size_t length);

#endif // TEST_UTIL_H_