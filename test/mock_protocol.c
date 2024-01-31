#include "mock_protocol.h"

#include <assert.h>

static nxamf_buffer_state_t _update(nxamf_protocol_handler_t *parent, uint8_t d)
{
    mock_protocol_t *ph = (mock_protocol_t *)parent;
    switch (ph->s)
    {
    case MOCK_STATE_INITIAL:
        if (d == 0)
        {
            ph->s = MOCK_STATE_0;
            return NXAMF_BUFFER_PENDING;
        }
        break;

    case MOCK_STATE_0:
        if (d == 1)
        {
            ph->s = MOCK_STATE_1;
            return NXAMF_BUFFER_PENDING;
        }
        break;

    case MOCK_STATE_1:
        if (d == 2)
        {
            ph->s = MOCK_STATE_FINAL;
            return NXAMF_BUFFER_ACCEPTED;
        }
        break;

    case MOCK_STATE_FINAL:
    default:
        break;
    }
    return NXAMF_BUFFER_REJECTED;
}
static bool _deserialize(nxamf_protocol_handler_t *parent, uint8_t buf[], size_t size, nxamf_gamepad_state_t *out)
{
    assert(buf[0] == 0);
    assert(buf[1] == 1);
    assert(buf[2] == 2);
    assert(size == 3);

    if (out != NULL)
    {
        nxamf_gamepad_state_copy(out, &NXAMF_GAMEPAD_STATE_NEUTRAL);
        out->extension[0] = buf[0];
        out->extension[1] = buf[1];
        out->extension[2] = buf[2];
    }
    return true;
}
static void _reset(nxamf_protocol_handler_t *parent)
{
    mock_protocol_t *ph = (mock_protocol_t *)parent;
    ph->s = MOCK_STATE_INITIAL;
}

void mock_protocol_init(mock_protocol_t *ph)
{
    ph->parent.update = _update;
    ph->parent.deserialize = _deserialize;
    ph->parent.reset = _reset;
    ph->s = MOCK_STATE_INITIAL;
}