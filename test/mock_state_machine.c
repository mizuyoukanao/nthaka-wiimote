#include "mock_state_machine.h"

#include <assert.h>

static nxamf_buffer_state_t _next(nxamf_state_machine_t *parent, uint8_t d)
{
    mock_state_machine_t *sm = (mock_state_machine_t *)parent;
    switch (sm->s)
    {
    case MOCK_STATE_INITIAL:
        if (d == 0)
        {
            sm->s = MOCK_STATE_0;
            return NXAMF_BUFFER_STATE_PENDING;
        }
        break;

    case MOCK_STATE_0:
        if (d == 1)
        {
            sm->s = MOCK_STATE_1;
            return NXAMF_BUFFER_STATE_PENDING;
        }
        break;

    case MOCK_STATE_1:
        if (d == 2)
        {
            sm->s = MOCK_STATE_FINAL;
            return NXAMF_BUFFER_STATE_ACCEPTED;
        }
        break;

    case MOCK_STATE_FINAL:
    default:
        break;
    }
    return NXAMF_BUFFER_STATE_REJECTED;
}
static bool _deserialize(nxamf_state_machine_t *parent, uint8_t buf[], size_t size, nxamf_gamepad_state_t *out)
{
    assert(buf[0] == 0);
    assert(buf[1] == 1);
    assert(buf[2] == 2);
    assert(size == 3);

    if (out != NULL)
    {
        nxamf_gamepad_state_copy(&NXAMF_GAMEPAD_STATE_NEUTRAL, out);
        out->extension[0] = buf[0];
        out->extension[1] = buf[1];
        out->extension[2] = buf[2];
    }
    return true;
}
static void _reset(nxamf_state_machine_t *parent)
{
    mock_state_machine_t *sm = (mock_state_machine_t *)parent;
    sm->s = MOCK_STATE_INITIAL;
}

void mock_state_machine_init(mock_state_machine_t *sm)
{
    sm->parent.next = _next;
    sm->parent.deserialize = _deserialize;
    sm->parent.reset = _reset;
    sm->s = MOCK_STATE_INITIAL;
}