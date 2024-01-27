#ifndef NXAMF_H_
#define NXAMF_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

    typedef enum NxamfButton
    {
        NXAMF_BUTTON_RELEASED,
        NXAMF_BUTTON_PRESSED
    } NxamfButton;

    typedef enum NxamfHat
    {
        NXAMF_HAT_UP,
        NXAMF_HAT_UPRIGHT,
        NXAMF_HAT_RIGHT,
        NXAMF_HAT_DOWNRIGHT,
        NXAMF_HAT_DOWN,
        NXAMF_HAT_DOWNLEFT,
        NXAMF_HAT_LEFT,
        NXAMF_HAT_UPLEFT,
        NXAMF_HAT_NEUTRAL
    } NxamfHat;

#define NXAMF_STICK_NEUTRAL 128U

    typedef struct NxamfStick
    {
        uint8_t x;
        uint8_t y;
    } NxamfStick;

    typedef struct NxamfGamepadState
    {
        NxamfButton y;
        NxamfButton b;
        NxamfButton a;
        NxamfButton x;
        NxamfButton l;
        NxamfButton r;
        NxamfButton zl;
        NxamfButton zr;
        NxamfButton minus;
        NxamfButton plus;
        NxamfButton l_click;
        NxamfButton r_click;
        NxamfButton home;
        NxamfButton capture;

        NxamfHat hat;

        NxamfStick l_stick;
        NxamfStick r_stick;

        uint8_t extension[16];
    } NxamfGamepadState;

    typedef struct NxamfBytesProtocolInterface
    {
        bool (*is_acceptable)(struct NxamfBytesProtocolInterface *self, const uint8_t packet, const uint8_t buffer[], const size_t length);
        bool (*is_ready)(struct NxamfBytesProtocolInterface *self, const uint8_t buffer[], const size_t length);
        void (*convert)(struct NxamfBytesProtocolInterface *self, const uint8_t buffer[], const size_t length, NxamfGamepadState *state);
    } NxamfBytesProtocolInterface;

    typedef struct NxamfProtocolMultiplexer
    {
        NxamfBytesProtocolInterface parent;
        NxamfBytesProtocolInterface **protocols;
        size_t protocols_length;
        size_t ready_index;
    } NxamfProtocolMultiplexer;

    NxamfProtocolMultiplexer *nxamf_protocol_multiplexer_new(NxamfBytesProtocolInterface *protocols[], size_t length);
    void nxamf_protocol_multiplexer_delete(NxamfProtocolMultiplexer *self);

    typedef struct nxamf_buffer_interface_t
    {
        void (*append)(struct nxamf_buffer_interface_t *buf, uint8_t d);
        bool (*deserialize)(struct nxamf_buffer_interface_t *buf, NxamfGamepadState *out);
        void (*clear)(struct nxamf_buffer_interface_t *buf);
    } nxamf_buffer_interface_t;

#ifdef __cplusplus
}
#endif

#endif // NXAMF_H_