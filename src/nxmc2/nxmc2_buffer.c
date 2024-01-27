#include "nxamf/nxmc2.h"

#include <assert.h>

static const uint8_t _HEADER = 0xAB;
static const uint8_t _MAX_HAT_VALUE = 8;

typedef enum nxmc2_buffer_index_t
{
    NXMC2_BUFFER_INDEX_HEADER,
    NXMC2_BUFFER_INDEX_BUTTONS_LSB,
    NXMC2_BUFFER_INDEX_BUTTONS_MSB,
    NXMC2_BUFFER_INDEX_HAT,
    NXMC2_BUFFER_INDEX_LX,
    NXMC2_BUFFER_INDEX_LY,
    NXMC2_BUFFER_INDEX_RX,
    NXMC2_BUFFER_INDEX_RY,
    NXMC2_BUFFER_INDEX_EXTENSION_0,
    NXMC2_BUFFER_INDEX_EXTENSION_1,
    NXMC2_BUFFER_INDEX_EXTENSION_2,
} nxmc2_buffer_index_t;

static void _clear(nxamf_buffer_interface_t *parent)
{
    nxmc2_buffer_t *buf = (nxmc2_buffer_t *)parent;
    assert(buf != NULL);

    buf->len = 0;
}

static void _append(nxamf_buffer_interface_t *parent, uint8_t d)
{
    nxmc2_buffer_t *buf = (nxmc2_buffer_t *)parent;
    assert(buf != NULL);

    if (buf->len == NXMC2_BUFFER_INDEX_HEADER && d != _HEADER)
    {
        // Rejected
        return;
    }
    else if ((buf->len == NXMC2_BUFFER_INDEX_HAT && _MAX_HAT_VALUE < d) ||
             NXMC2_BUFFER_LENGTH <= buf->len)
    {
        parent->clear(parent);
        parent->append(parent, d);
        return;
    }

    buf->buf[buf->len] = d;
    buf->len++;
}

static bool _deserialize(nxamf_buffer_interface_t *parent, nxamf_gamepad_state_t *out)
{
    nxmc2_buffer_t *buf = (nxmc2_buffer_t *)parent;
    assert(buf != NULL);
    assert(out != NULL);

    if (buf->len != NXMC2_BUFFER_LENGTH)
    {
        return false;
    }
    assert(buf->buf[NXMC2_BUFFER_INDEX_HEADER] == _HEADER);
    assert(buf->buf[NXMC2_BUFFER_INDEX_HAT] <= _MAX_HAT_VALUE);

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

void nxmc2_buffer_init(nxmc2_buffer_t *buf)
{
    assert(buf != NULL);

    buf->parent.append = _append;
    buf->parent.deserialize = _deserialize;
    buf->parent.clear = _clear;

    buf->len = 0;
}