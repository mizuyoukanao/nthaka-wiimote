#ifndef NXAMF_POKECON_H_
#define NXAMF_POKECON_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "../nxamf.h"

    typedef enum pokecon_buffer_state_t
    {
        POKECON_BUFFER_STATE_INITIAL,
        POKECON_BUFFER_STATE_0,
        POKECON_BUFFER_STATE_1,
        POKECON_BUFFER_STATE_10,
        POKECON_BUFFER_STATE_100,
        POKECON_BUFFER_STATE_1000,
        POKECON_BUFFER_STATE_1001,
        POKECON_BUFFER_STATE_1003,
        POKECON_BUFFER_STATE_1007,
        POKECON_BUFFER_STATE_101,
        POKECON_BUFFER_STATE_103,
        POKECON_BUFFER_STATE_107,
        POKECON_BUFFER_STATE_11,
        POKECON_BUFFER_STATE_13,
        POKECON_BUFFER_STATE_17,
        POKECON_BUFFER_STATE_3,
        POKECON_BUFFER_STATE_4,
        POKECON_BUFFER_STATE_7,
        POKECON_BUFFER_STATE_0x,
        POKECON_BUFFER_STATE_0x0,
        POKECON_BUFFER_STATE_0x00,
        POKECON_BUFFER_STATE_0x000,
        POKECON_BUFFER_STATE_0_,
        POKECON_BUFFER_STATE_0_0,
        POKECON_BUFFER_STATE_1_,
        POKECON_BUFFER_STATE_1_0,
        POKECON_BUFFER_STATE_3_,
        POKECON_BUFFER_STATE_3_0,
        POKECON_BUFFER_STATE_7_,
        POKECON_BUFFER_STATE_7_0,
        POKECON_BUFFER_STATE_1_0_,
        POKECON_BUFFER_STATE_1_0_0,
        POKECON_BUFFER_STATE_1_0_1,
        POKECON_BUFFER_STATE_1_0_10,
        POKECON_BUFFER_STATE_1_0_0x,
        POKECON_BUFFER_STATE_1_0_0_,
        POKECON_BUFFER_STATE_1_0_0_0,
        POKECON_BUFFER_STATE_1_0_0_1,
        POKECON_BUFFER_STATE_1_0_0_10,
        POKECON_BUFFER_STATE_1_0_0_0x,
        POKECON_BUFFER_STATE_7_0_,
        POKECON_BUFFER_STATE_7_0_0,
        POKECON_BUFFER_STATE_7_0_1,
        POKECON_BUFFER_STATE_7_0_10,
        POKECON_BUFFER_STATE_7_0_0x,
        POKECON_BUFFER_STATE_7_0_0_,
        POKECON_BUFFER_STATE_7_0_0_0,
        POKECON_BUFFER_STATE_7_0_0_1,
        POKECON_BUFFER_STATE_7_0_0_10,
        POKECON_BUFFER_STATE_7_0_0_0x,
        POKECON_BUFFER_STATE_3_0_,
        POKECON_BUFFER_STATE_3_0_0,
        POKECON_BUFFER_STATE_3_0_1,
        POKECON_BUFFER_STATE_3_0_10,
        POKECON_BUFFER_STATE_3_0_0x,
        POKECON_BUFFER_STATE_3_0_0_,
        POKECON_BUFFER_STATE_3_0_0_0,
        POKECON_BUFFER_STATE_3_0_0_1,
        POKECON_BUFFER_STATE_3_0_0_10,
        POKECON_BUFFER_STATE_3_0_0_0x,
        POKECON_BUFFER_STATE_0_0CR,
        POKECON_BUFFER_STATE_FINAL,
    } pokecon_buffer_state_t;

#define POKECON_BUFFER_MAX_LENGTH 30

    typedef struct pokecon_buffer_t
    {
        nxamf_buffer_interface_t parent;

        uint8_t buf[POKECON_BUFFER_MAX_LENGTH];
        size_t len;
        pokecon_buffer_state_t s;

        NxamfStick prev_l;
        NxamfStick prev_r;
    } pokecon_buffer_t;

    void pokecon_buffer_init(pokecon_buffer_t *buf);
    void pokecon_buffer_re_init(pokecon_buffer_t *buf);

#ifdef __cplusplus
}
#endif

#endif // NXAMF_POKECON_H_