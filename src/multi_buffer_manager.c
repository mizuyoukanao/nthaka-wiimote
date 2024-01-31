#include "nthaka.h"

#include <assert.h>

static bool _append(nthaka_buffer_interface_t *parent, uint8_t d, nthaka_gamepad_state_t *out)
{
    nthaka_multi_buffer_manager_t *buf = (nthaka_multi_buffer_manager_t *)parent;
    assert(buf != NULL);

    for (size_t i = 0; i < buf->len; i++)
    {
        buf->bufs[i]->append(buf->bufs[i], d, NULL);
    }

    return true;
}

static bool _deserialize(nthaka_buffer_interface_t *parent, nthaka_gamepad_state_t *out)
{
    nthaka_multi_buffer_manager_t *buf = (nthaka_multi_buffer_manager_t *)parent;
    assert(buf != NULL);

    for (size_t i = 0; i < buf->len; i++)
    {
        if (false /*buf->bufs[i]->deserialize(buf->bufs[i], out)*/)
        {
            // Clear other buffers
            for (size_t j = 0; j < buf->len; j++)
            {
                if (j == i)
                {
                    continue;
                }
                buf->bufs[j]->clear(buf->bufs[j]);
            }

            buf->last_deserialized_index = i;
            return true;
        }
    }

    return false;
}

static void _clear(nthaka_buffer_interface_t *parent)
{
    nthaka_multi_buffer_manager_t *buf = (nthaka_multi_buffer_manager_t *)parent;
    assert(buf != NULL);

    for (size_t i = 0; i < buf->len; i++)
    {
        buf->bufs[i]->clear(buf->bufs[i]);
    }
}

void nthaka_multi_buffer_manager_init(nthaka_multi_buffer_manager_t *buf, nthaka_buffer_interface_t **bufs, size_t len)
{
    assert(buf != NULL);
    assert(bufs != NULL);
#ifndef NDEBUG
    for (size_t i = 0; i < len; i++)
    {
        assert(bufs[i] != NULL);
    }
#endif

    buf->parent.append = _append;
    buf->parent.clear = _clear;

    buf->bufs = bufs;
    buf->len = len;
    buf->last_deserialized_index = SIZE_MAX;
}

size_t nthaka_multi_buffer_manager_get_last_deserialized_index(nthaka_multi_buffer_manager_t *buf)
{
    assert(buf != NULL);
    return buf->last_deserialized_index;
}