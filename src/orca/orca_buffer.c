#include "nthaka/orca.h"

#include <assert.h>

static void _clear(nthaka_buffer_interface_t *parent)
{
    orca_buffer_t *buf = (orca_buffer_t *)parent;
    assert(buf != NULL);

    buf->len = 0;
}

static const uint8_t _HEADER = 0x80;
static const uint8_t _SERVO = '@';
static const uint8_t _RELEASE_ALL = 0x80;
static const uint8_t _IGNORE = 0xFF;

static bool _is_completed(orca_buffer_t *buf)
{
    return (buf->len == 1 && buf->buf[0] == _SERVO) ||
           buf->len == ORCA_BUFFER_LENGTH;
}

static bool _append(nthaka_buffer_interface_t *parent, uint8_t d, nthaka_gamepad_state_t *out)
{
    orca_buffer_t *buf = (orca_buffer_t *)parent;
    assert(buf != NULL);

    buf->cached = false;

    if ((buf->len == 0 && d != _HEADER && d != _SERVO) ||
        d == _IGNORE)
    {
        return;
    }
    else if (_is_completed(buf))
    {
        parent->clear(parent);
        parent->append(parent, d, NULL);
        return;
    }

    buf->buf[buf->len] = d;
    buf->len++;

    if (_is_completed(buf))
    {
        // parent->deserialize(parent, &buf->prev);
        buf->cached = true;
    }

    return true;
}

static bool _deserialize(nthaka_buffer_interface_t *parent, nthaka_gamepad_state_t *out)
{
    orca_buffer_t *buf = (orca_buffer_t *)parent;
    assert(buf != NULL);
    assert(out != NULL);

    if (!_is_completed(buf))
    {
        return false;
    }

    nthaka_gamepad_state_copy(out, &buf->prev);
    if (buf->cached)
    {
        return true;
    }

    if (buf->len == 1)
    {
        assert(buf->buf[0] == _SERVO);

        out->home = NTHAKA_BUTTON_PRESSED;
    }
    else
    {
        out->home = NTHAKA_BUTTON_RELEASED;

        if (buf->buf[1] == _RELEASE_ALL)
        {
            out->a = NTHAKA_BUTTON_RELEASED;
            out->b = NTHAKA_BUTTON_RELEASED;
            out->x = NTHAKA_BUTTON_RELEASED;
            out->y = NTHAKA_BUTTON_RELEASED;
            out->l = NTHAKA_BUTTON_RELEASED;
            out->r = NTHAKA_BUTTON_RELEASED;
        }
        else
        {
            out->a = (nthaka_button_t)(buf->buf[1] & 1);
            out->b = (nthaka_button_t)((buf->buf[1] >> 1) & 1);
            out->x = (nthaka_button_t)((buf->buf[1] >> 2) & 1);
            out->y = (nthaka_button_t)((buf->buf[1] >> 3) & 1);
            out->l = (nthaka_button_t)((buf->buf[1] >> 4) & 1);
            out->r = (nthaka_button_t)((buf->buf[1] >> 5) & 1);
        }

        uint8_t dleft = 0;
        uint8_t dright = 0;
        uint8_t dup = 0;
        uint8_t ddown = 0;

        if (buf->buf[2] == _RELEASE_ALL)
        {
            out->zr = NTHAKA_BUTTON_RELEASED;
            out->plus = NTHAKA_BUTTON_RELEASED;
            dleft = 0;
            dright = 0;
            dup = 0;
            ddown = 0;
        }
        else
        {
            out->zr = (nthaka_button_t)(buf->buf[2] & 1);
            out->plus = (nthaka_button_t)((buf->buf[2] >> 1) & 1);
            dleft = (buf->buf[2] >> 2) & 1;
            dright = (buf->buf[2] >> 3) & 1;
            dup = (buf->buf[2] >> 4) & 1;
            ddown = (buf->buf[2] >> 5) & 1;
        }

        switch (dup << 3 | dright << 2 | ddown << 1 | dleft)
        {
        case 0b1000:
            out->hat = NTHAKA_HAT_UP;
            break;
        case 0b1100:
            out->hat = NTHAKA_HAT_UPRIGHT;
            break;
        case 0b0100:
            out->hat = NTHAKA_HAT_RIGHT;
            break;
        case 0b0110:
            out->hat = NTHAKA_HAT_DOWNRIGHT;
            break;
        case 0b0010:
            out->hat = NTHAKA_HAT_DOWN;
            break;
        case 0b0011:
            out->hat = NTHAKA_HAT_DOWNLEFT;
            break;
        case 0b0001:
            out->hat = NTHAKA_HAT_LEFT;
            break;
        case 0b1001:
            out->hat = NTHAKA_HAT_UPLEFT;
            break;
        default:
            out->hat = NTHAKA_HAT_NEUTRAL;
            break;
        }
    }

    nthaka_gamepad_state_copy(&buf->prev, out);

    return true;
}

void orca_buffer_init(orca_buffer_t *buf)
{
    assert(buf != NULL);

    buf->parent.append = _append;
    buf->parent.clear = _clear;

    buf->len = 0;
    nthaka_gamepad_state_copy(&(buf->prev), &NTHAKA_GAMEPAD_STATE_NEUTRAL);
    buf->cached = false;
}