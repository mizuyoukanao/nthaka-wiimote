#include "nthaka/nxmc2.h"

#include <assert.h>

static const uint8_t _HEADER = 0xAB;
static const uint8_t _HAT_VALUE_MAX = 8;

static nthaka_gamepad_state_t _;

static bool _deserialize(nxmc2_buffer_t *buf, nthaka_gamepad_state_t *out)
{
    assert(buf != NULL);

    if (buf->len != NXMC2_BUFFER_LENGTH)
    {
        return false;
    }
    assert(buf->buf[NXMC2_BUFFER_INDEX_HEADER] == _HEADER);
    assert(buf->buf[NXMC2_BUFFER_INDEX_HAT] <= _HAT_VALUE_MAX);

    if (out == NULL)
    {
        // Since there are no side effects.
        return true;
    }

    uint8_t btns_lsb = buf->buf[NXMC2_BUFFER_INDEX_BUTTONS_LSB];
    out->y = (btns_lsb & 0b00000001U);
    out->b = (btns_lsb & 0b00000010U) >> 1;
    out->a = (btns_lsb & 0b00000100U) >> 2;
    out->x = (btns_lsb & 0b00001000U) >> 3;
    out->l = (btns_lsb & 0b00010000U) >> 4;
    out->r = (btns_lsb & 0b00100000U) >> 5;
    out->zl = (btns_lsb & 0b01000000U) >> 6;
    out->zr = (btns_lsb & 0b10000000U) >> 7;

    uint8_t btns_msb = buf->buf[NXMC2_BUFFER_INDEX_BUTTONS_MSB];
    out->minus = (btns_msb & 0b00000001U);
    out->plus = (btns_msb & 0b00000010U) >> 1;
    out->l_click = (btns_msb & 0b00000100U) >> 2;
    out->r_click = (btns_msb & 0b00001000U) >> 3;
    out->home = (btns_msb & 0b00010000U) >> 4;
    out->capture = (btns_msb & 0b00100000U) >> 5;
    // out->_ = (btns_msb & 0b11000000U) >> 6;

    out->hat = buf->buf[NXMC2_BUFFER_INDEX_HAT];

    out->l_stick.x = buf->buf[NXMC2_BUFFER_INDEX_LX];
    out->l_stick.y = buf->buf[NXMC2_BUFFER_INDEX_LY];
    out->r_stick.x = buf->buf[NXMC2_BUFFER_INDEX_RX];
    out->r_stick.y = buf->buf[NXMC2_BUFFER_INDEX_RY];

    out->extension[0] = buf->buf[NXMC2_BUFFER_INDEX_EXTENSION_0];
    out->extension[1] = buf->buf[NXMC2_BUFFER_INDEX_EXTENSION_1];
    out->extension[2] = buf->buf[NXMC2_BUFFER_INDEX_EXTENSION_2];

    size_t length = sizeof(out->extension) / sizeof(uint8_t);
    for (size_t i = 3; i < length; i++)
    {
        out->extension[i] = 0;
    }

    return true;
}

static bool _append(nthaka_buffer_interface_t *parent, uint8_t d, nthaka_gamepad_state_t *out)
{
    nxmc2_buffer_t *buf = (nxmc2_buffer_t *)parent;

    if (buf == NULL ||
        NXMC2_BUFFER_LENGTH <= buf->len ||
        (buf->len == NXMC2_BUFFER_INDEX_HEADER && d != _HEADER) ||
        (buf->len == NXMC2_BUFFER_INDEX_HAT && _HAT_VALUE_MAX < d))
    {
        return false;
    }

    buf->buf[buf->len] = d;
    buf->len++;

    return _deserialize(buf, out);
}

static void _clear(nthaka_buffer_interface_t *parent)
{
    nxmc2_buffer_t *buf = (nxmc2_buffer_t *)parent;
    assert(buf != NULL);

    buf->len = 0;
}

void nxmc2_buffer_init(nxmc2_buffer_t *buf)
{
    assert(buf != NULL);

    buf->parent.append = _append;
    buf->parent.clear = _clear;

    buf->len = 0;
}