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

    bool nxamf_gamepad_state_equals(NxamfGamepadState *s1, NxamfGamepadState *s2);
    void nxamf_gamepad_state_copy(NxamfGamepadState *src, NxamfGamepadState *dst);

    typedef struct nxamf_buffer_interface_t
    {
        void (*append)(struct nxamf_buffer_interface_t *buf, uint8_t d);
        bool (*deserialize)(struct nxamf_buffer_interface_t *buf, NxamfGamepadState *out);
        void (*clear)(struct nxamf_buffer_interface_t *buf);
    } nxamf_buffer_interface_t;

    typedef struct nxamf_multi_buffer_manager_t
    {
        nxamf_buffer_interface_t parent;

        nxamf_buffer_interface_t **bufs;
        size_t len;
        int last_deserialized_index;
    } nxamf_multi_buffer_manager_t;

    void nxamf_multi_buffer_manager_init(nxamf_multi_buffer_manager_t *buf, nxamf_buffer_interface_t **bufs, size_t len);
    size_t nxamf_multi_buffer_manager_get_last_deserialized_index(nxamf_multi_buffer_manager_t *buf);

#ifdef __cplusplus
}
#endif

#endif // NXAMF_H_