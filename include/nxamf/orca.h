#ifndef NXAMF_ORCA_H_
#define NXAMF_ORCA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "../nxamf.h"

#define ORCA_BUFFER_LENGTH 3

    typedef struct orca_buffer_t
    {
        nxamf_buffer_interface_t parent;

        uint8_t buf[ORCA_BUFFER_LENGTH];
        size_t len;
        nxamf_gamepad_state_t prev;
        bool cached;
    } orca_buffer_t;

    void orca_buffer_init(orca_buffer_t *buf);

#ifdef __cplusplus
}
#endif

#endif // NXAMF_ORCA_H_