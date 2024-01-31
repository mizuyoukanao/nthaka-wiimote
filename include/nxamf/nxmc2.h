#ifndef NXAMF_NXMC2_H_
#define NXAMF_NXMC2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "../nxamf.h"

#define NXMC2_BUFFER_LENGTH 11

    typedef struct nxmc2_buffer_t
    {
        nxamf_buffer_interface_t parent;

        uint8_t buf[NXMC2_BUFFER_LENGTH];
        size_t len;
    } nxmc2_buffer_t;

    void nxmc2_buffer_init(nxmc2_buffer_t *buf);

    typedef enum nxmc2_protocol_state_t
    {
        NXMC2_PROTOCOL_INITIAL,
        NXMC2_PROTOCOL_0xAB,
        NXMC2_PROTOCOL_0xAB_0x00,
        NXMC2_PROTOCOL_0xAB_0x00_0x00,
        NXMC2_PROTOCOL_0xAB_0x00_0x00_0x08,
        NXMC2_PROTOCOL_0xAB_0x00_0x00_0x08_0x80,
        NXMC2_PROTOCOL_0xAB_0x00_0x00_0x08_0x80_0x80,
        NXMC2_PROTOCOL_0xAB_0x00_0x00_0x08_0x80_0x80_0x80,
        NXMC2_PROTOCOL_0xAB_0x00_0x00_0x08_0x80_0x80_0x80_0x80,
        NXMC2_PROTOCOL_0xAB_0x00_0x00_0x08_0x80_0x80_0x80_0x80_0x00,
        NXMC2_PROTOCOL_0xAB_0x00_0x00_0x08_0x80_0x80_0x80_0x80_0x00_0x00,
        NXMC2_PROTOCOL_FINAL
    } nxmc2_protocol_state_t;

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

    typedef struct nxmc2_protocol_t
    {
        nxamf_protocol_handler_t parent;

        nxmc2_protocol_state_t _s;
    } nxmc2_protocol_t;

    bool nxmc2_protocol_init(nxmc2_protocol_t *ph);

#ifdef __cplusplus
}
#endif

#endif // NXAMF_NXMC2_H_