#include "nxamf.h"

#include <assert.h>

void _append(nxamf_buffer_interface_t *parent, uint8_t d)
{
    nxamf_multi_buffer_manager_t *buf = (nxamf_multi_buffer_manager_t *)parent;
    assert(buf != NULL);

    for (size_t i = 0; i < buf->len; i++)
    {
        buf->bufs[i]->append(buf->bufs[i], d);
    }
}

bool _deserialize(nxamf_buffer_interface_t *parent, nxamf_gamepad_state_t *out)
{
    nxamf_multi_buffer_manager_t *buf = (nxamf_multi_buffer_manager_t *)parent;
    assert(buf != NULL);

    for (size_t i = 0; i < buf->len; i++)
    {
        if (buf->bufs[i]->deserialize(buf->bufs[i], out))
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

void _clear(nxamf_buffer_interface_t *parent)
{
    nxamf_multi_buffer_manager_t *buf = (nxamf_multi_buffer_manager_t *)parent;
    assert(buf != NULL);

    for (size_t i = 0; i < buf->len; i++)
    {
        buf->bufs[i]->clear(buf->bufs[i]);
    }
}

void nxamf_multi_buffer_manager_init(nxamf_multi_buffer_manager_t *buf, nxamf_buffer_interface_t **bufs, size_t len)
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
    buf->parent.deserialize = _deserialize;
    buf->parent.clear = _clear;

    buf->bufs = bufs;
    buf->len = len;
    buf->last_deserialized_index = -1;
}

int nxamf_multi_buffer_manager_get_last_deserialized_index(nxamf_multi_buffer_manager_t *buf)
{
    assert(buf != NULL);
    return buf->last_deserialized_index;
}