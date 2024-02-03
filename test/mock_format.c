#include "mock_format.h"

#include <assert.h>

static nthaka_buffer_state_t _update(nthaka_format_handler_t *parent, uint8_t d)
{
    mock_format_t *fmt = (mock_format_t *)parent;
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
static bool _deserialize(nthaka_format_handler_t *parent, uint8_t buf[], size_t size, nthaka_gamepad_state_t *out)
{
    assert(buf[0] == 0);
    assert(buf[1] == 1);
    assert(buf[2] == 2);
    assert(size == 3);

    if (out != NULL)
    {
        nthaka_gamepad_state_copy(out, &NTHAKA_GAMEPAD_STATE_NEUTRAL);
        out->ext[0] = buf[0];
        out->ext[1] = buf[1];
        out->ext[2] = buf[2];
    }
    return true;
}
static void _reset(nthaka_format_handler_t *parent)
{
    mock_format_t *fmt = (mock_format_t *)parent;
    fmt->s = MOCK_STATE_INITIAL;
}

void mock_format_init(mock_format_t *fmt)
{
    fmt->parent.update = _update;
    fmt->parent.deserialize = _deserialize;
    fmt->parent.reset = _reset;
    fmt->s = MOCK_STATE_INITIAL;
}