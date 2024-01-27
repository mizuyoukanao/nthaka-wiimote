#ifndef NXAMF_H_
#define NXAMF_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

    typedef enum nxamf_button_t
    {
        NXAMF_BUTTON_RELEASED,
        NXAMF_BUTTON_PRESSED
    } nxamf_button_t;

    typedef enum nxamf_hat_t
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
    } nxamf_hat_t;

#define NXAMF_STICK_NEUTRAL 128U

    typedef struct nxamf_stick_t
    {
        uint8_t x;
        uint8_t y;
    } nxamf_stick_t;

    typedef struct nxamf_gamepad_state_t
    {
        nxamf_button_t y;
        nxamf_button_t b;
        nxamf_button_t a;
        nxamf_button_t x;
        nxamf_button_t l;
        nxamf_button_t r;
        nxamf_button_t zl;
        nxamf_button_t zr;
        nxamf_button_t minus;
        nxamf_button_t plus;
        nxamf_button_t l_click;
        nxamf_button_t r_click;
        nxamf_button_t home;
        nxamf_button_t capture;

        nxamf_hat_t hat;

        nxamf_stick_t l_stick;
        nxamf_stick_t r_stick;

        uint8_t extension[16];
    } nxamf_gamepad_state_t;

    bool nxamf_gamepad_state_equals(nxamf_gamepad_state_t *s1, nxamf_gamepad_state_t *s2);
    void nxamf_gamepad_state_copy(nxamf_gamepad_state_t *src, nxamf_gamepad_state_t *dst);

    typedef struct nxamf_buffer_interface_t
    {
        void (*append)(struct nxamf_buffer_interface_t *buf, uint8_t d);
        bool (*deserialize)(struct nxamf_buffer_interface_t *buf, nxamf_gamepad_state_t *out);
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