#include "util.h"

#include "nxamf.h"

#include <assert.h>
#include <memory.h>

void compare_states(NxamfGamepadState *s1, NxamfGamepadState *s2)
{
    assert(s1->y == s2->y);
    assert(s1->b == s2->b);
    assert(s1->a == s2->a);
    assert(s1->x == s2->x);
    assert(s1->l == s2->l);
    assert(s1->r == s2->r);
    assert(s1->zl == s2->zl);
    assert(s1->zr == s2->zr);
    assert(s1->minus == s2->minus);
    assert(s1->plus == s2->plus);
    assert(s1->l_click == s2->l_click);
    assert(s1->r_click == s2->r_click);
    assert(s1->home == s2->home);
    assert(s1->capture == s2->capture);
    assert(s1->hat == s2->hat);
    assert(s1->l_stick.x == s2->l_stick.x);
    assert(s1->l_stick.y == s2->l_stick.y);
    assert(s1->r_stick.x == s2->r_stick.x);
    assert(s1->r_stick.y == s2->r_stick.y);
    assert(memcmp(s1->extension, s2->extension, 16) == 0);
}

bool nxamf_gamepad_state_equals(NxamfGamepadState *s1, NxamfGamepadState *s2)
{
    return s1->y == s2->y &&
           s1->b == s2->b &&
           s1->a == s2->a &&
           s1->x == s2->x &&
           s1->l == s2->l &&
           s1->r == s2->r &&
           s1->zl == s2->zl &&
           s1->zr == s2->zr &&
           s1->minus == s2->minus &&
           s1->plus == s2->plus &&
           s1->l_click == s2->l_click &&
           s1->r_click == s2->r_click &&
           s1->home == s2->home &&
           s1->capture == s2->capture &&
           s1->hat == s2->hat &&
           s1->l_stick.x == s2->l_stick.x &&
           s1->l_stick.y == s2->l_stick.y &&
           s1->r_stick.x == s2->r_stick.x &&
           s1->r_stick.y == s2->r_stick.y &&
           s1->extension[0] == s2->extension[0] &&
           s1->extension[1] == s2->extension[1] &&
           s1->extension[2] == s2->extension[2] &&
           s1->extension[3] == s2->extension[3] &&
           s1->extension[4] == s2->extension[4] &&
           s1->extension[5] == s2->extension[5] &&
           s1->extension[6] == s2->extension[6] &&
           s1->extension[7] == s2->extension[7] &&
           s1->extension[8] == s2->extension[8] &&
           s1->extension[9] == s2->extension[9] &&
           s1->extension[10] == s2->extension[10] &&
           s1->extension[11] == s2->extension[11] &&
           s1->extension[12] == s2->extension[12] &&
           s1->extension[13] == s2->extension[13] &&
           s1->extension[14] == s2->extension[14] &&
           s1->extension[15] == s2->extension[15];
}

void test_protocol_params_run(NxamfBytesProtocolInterface *protocol, TestProtocolParams params[], size_t length)
{
    for (int i = 0; i < length; i++)
    {
        NxamfBytesBuffer *buffer = nxamf_bytes_buffer_new(protocol);
        assert(buffer != NULL);

        TestProtocolParams p = params[i];
        for (int j = 0; j < p.length - 1; j++)
        {
            assert(nxamf_bytes_buffer_append(buffer, p.data[j]) == NULL);
        }
        NxamfGamepadState *state = nxamf_bytes_buffer_append(buffer, p.data[p.length - 1]);
        assert(state != NULL);
        compare_states(state, &p.expected);

        nxamf_gamepad_state_delete(state);
        nxamf_bytes_buffer_delete(buffer);
    }
}