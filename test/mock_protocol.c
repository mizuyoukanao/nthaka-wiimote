#include "mock_protocol.h"

#include <assert.h>

static nthaka_buffer_state_t _update(nthaka_format_t *parent, uint8_t d)
{
    mock_protocol_t *fmt = (mock_protocol_t *)parent;
    switch (fmt->s)
    {
    case MOCK_STATE_INITIAL:
        if (d == 0)
        {
            fmt->s = MOCK_STATE_0;
            return NTHAKA_BUFFER_PENDING;
        }
        break;

    case MOCK_STATE_0:
        if (d == 1)
        {
            fmt->s = MOCK_STATE_1;
            return NTHAKA_BUFFER_PENDING;
        }
        break;

    case MOCK_STATE_1:
        if (d == 2)
        {
            fmt->s = MOCK_STATE_FINAL;
            return NTHAKA_BUFFER_ACCEPTED;
        }
        break;

    case MOCK_STATE_FINAL:
    default:
        break;
    }
    return NTHAKA_BUFFER_REJECTED;
}
static bool _deserialize(nthaka_format_t *parent, uint8_t buf[], size_t size, nthaka_gamepad_state_t *out)
{
    assert(buf[0] == 0);
    assert(buf[1] == 1);
    assert(buf[2] == 2);
    assert(size == 3);

    if (out != NULL)
    {
        nthaka_gamepad_state_copy(out, &NTHAKA_GAMEPAD_STATE_NEUTRAL);
        out->extension[0] = buf[0];
        out->extension[1] = buf[1];
        out->extension[2] = buf[2];
    }
    return true;
}
static void _reset(nthaka_format_t *parent)
{
    mock_protocol_t *fmt = (mock_protocol_t *)parent;
    fmt->s = MOCK_STATE_INITIAL;
}

void mock_protocol_init(mock_protocol_t *fmt)
{
    fmt->parent.update = _update;
    fmt->parent.deserialize = _deserialize;
    fmt->parent.reset = _reset;
    fmt->s = MOCK_STATE_INITIAL;
}