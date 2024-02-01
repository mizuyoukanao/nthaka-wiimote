#ifndef NTHAKA_ORCA_H_
#define NTHAKA_ORCA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "../nthaka.h"

#define ORCA_BUFFER_LENGTH 3

    typedef struct orca_buffer_t
    {
        nthaka_buffer_interface_t parent;

        uint8_t buf[ORCA_BUFFER_LENGTH];
        size_t len;
        nthaka_gamepad_state_t prev;
        bool cached;
    } orca_buffer_t;

    void orca_buffer_init(orca_buffer_t *buf);

    typedef enum orca_format_state_t
    {
        ORCA_FORMAT_INITIAL,
        ORCA_FORMAT_0x80,
        ORCA_FORMAT_0x80_0x00,
        ORCA_FORMAT_FINAL
    } orca_format_state_t;

    typedef struct orca_format_t
    {
        nthaka_format_t parent;

        orca_format_state_t _s;
        nthaka_gamepad_state_t _prev;
    } orca_format_t;

    bool orca_format_init(orca_format_t *fmt);

#ifdef __cplusplus
}
#endif

#endif // NTHAKA_ORCA_H_