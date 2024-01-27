#ifndef NXAMF_NXMC2_H_
#define NXAMF_NXMC2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "../nxamf.h"

    typedef struct Nxmc2Protocol
    {
        NxamfBytesProtocolInterface parent;
    } Nxmc2Protocol;

    Nxmc2Protocol *nxmc2_protocol_new(void);
    void nxmc2_protocol_delete(Nxmc2Protocol *self);

#define NXMC2_BUFFER_LENGTH 11

    typedef struct nxmc2_buffer_t
    {
        nxamf_buffer_interface_t parent;

        uint8_t buf[NXMC2_BUFFER_LENGTH];
        size_t len;
    } nxmc2_buffer_t;

    void nxmc2_buffer_init(nxmc2_buffer_t *buf);

#ifdef __cplusplus
}
#endif

#endif // NXAMF_NXMC2_H_