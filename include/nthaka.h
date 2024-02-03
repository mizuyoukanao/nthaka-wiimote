#ifndef NTHAKA_H_
#define NTHAKA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

    typedef enum nthaka_button_state_t
    {
        NTHAKA_BUTTON_STATE_RELEASED,
        NTHAKA_BUTTON_STATE_PRESSED
    } nthaka_button_state_t;

    typedef enum nthaka_hat_state_t
    {
        NTHAKA_HAT_STATE_UP,
        NTHAKA_HAT_STATE_UPRIGHT,
        NTHAKA_HAT_STATE_RIGHT,
        NTHAKA_HAT_STATE_DOWNRIGHT,
        NTHAKA_HAT_STATE_DOWN,
        NTHAKA_HAT_STATE_DOWNLEFT,
        NTHAKA_HAT_STATE_LEFT,
        NTHAKA_HAT_STATE_UPLEFT,
        NTHAKA_HAT_STATE_NEUTRAL
    } nthaka_hat_state_t;

    typedef struct nthaka_stick_state_t
    {
        uint8_t x;
        uint8_t y;
    } nthaka_stick_state_t;

#define NTHAKA_STICK_STATE_AXIS_NEUTRAL 128U
#define NTHAKA_STICK_STATE_NEUTRAL ((nthaka_stick_state_t){.x = NTHAKA_STICK_STATE_AXIS_NEUTRAL, \
                                                           .y = NTHAKA_STICK_STATE_AXIS_NEUTRAL})

    typedef struct nthaka_gamepad_state_t
    {
        nthaka_button_state_t y;
        nthaka_button_state_t b;
        nthaka_button_state_t a;
        nthaka_button_state_t x;
        nthaka_button_state_t l;
        nthaka_button_state_t r;
        nthaka_button_state_t zl;
        nthaka_button_state_t zr;
        nthaka_button_state_t minus;
        nthaka_button_state_t plus;
        nthaka_button_state_t l_click;
        nthaka_button_state_t r_click;
        nthaka_button_state_t home;
        nthaka_button_state_t capture;

        nthaka_hat_state_t hat;

        nthaka_stick_state_t l_stick;
        nthaka_stick_state_t r_stick;

        uint8_t ext[16];
    } nthaka_gamepad_state_t;

#define NTHAKA_GAMEPAD_STATE_NEUTRAL ((nthaka_gamepad_state_t){.y = NTHAKA_BUTTON_STATE_RELEASED,       \
                                                               .b = NTHAKA_BUTTON_STATE_RELEASED,       \
                                                               .a = NTHAKA_BUTTON_STATE_RELEASED,       \
                                                               .x = NTHAKA_BUTTON_STATE_RELEASED,       \
                                                               .l = NTHAKA_BUTTON_STATE_RELEASED,       \
                                                               .r = NTHAKA_BUTTON_STATE_RELEASED,       \
                                                               .zl = NTHAKA_BUTTON_STATE_RELEASED,      \
                                                               .zr = NTHAKA_BUTTON_STATE_RELEASED,      \
                                                               .minus = NTHAKA_BUTTON_STATE_RELEASED,   \
                                                               .plus = NTHAKA_BUTTON_STATE_RELEASED,    \
                                                               .l_click = NTHAKA_BUTTON_STATE_RELEASED, \
                                                               .r_click = NTHAKA_BUTTON_STATE_RELEASED, \
                                                               .home = NTHAKA_BUTTON_STATE_RELEASED,    \
                                                               .capture = NTHAKA_BUTTON_STATE_RELEASED, \
                                                               .hat = NTHAKA_HAT_STATE_NEUTRAL,         \
                                                               .l_stick = NTHAKA_STICK_STATE_NEUTRAL,   \
                                                               .r_stick = NTHAKA_STICK_STATE_NEUTRAL,   \
                                                               .ext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}})
    void nthaka_gamepad_state_copy(nthaka_gamepad_state_t *dst, nthaka_gamepad_state_t *src);

    // char max[] = "{.y = NTHAKA_BUTTON_STATE_RELEASED, .b = NTHAKA_BUTTON_STATE_RELEASED, .a = NTHAKA_BUTTON_STATE_RELEASED, .x = NTHAKA_BUTTON_STATE_RELEASED, .l = NTHAKA_BUTTON_STATE_RELEASED, .r = NTHAKA_BUTTON_STATE_RELEASED, .zl = NTHAKA_BUTTON_STATE_RELEASED, .zr = NTHAKA_BUTTON_STATE_RELEASED, .minus = NTHAKA_BUTTON_STATE_RELEASED, .plus = NTHAKA_BUTTON_STATE_RELEASED, .l_click = NTHAKA_BUTTON_STATE_RELEASED, .r_click = NTHAKA_BUTTON_STATE_RELEASED, .home = NTHAKA_BUTTON_STATE_RELEASED, .capture = NTHAKA_BUTTON_STATE_RELEASED, .hat = NTHAKA_HAT_STATE_DOWNRIGHT, .l_stick = {.x = 255, .y = 255}, .r_stick = {.x = 255, .y = 255}, .ext = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}";
#define NTHAKA_GAMEPAD_STATE_STRING_LENGTH_MAX 612
    void nthaka_gamepad_state_stringify(nthaka_gamepad_state_t *s, char *out, size_t size);

    typedef enum nthaka_buffer_state_t
    {
        NTHAKA_BUFFER_PENDING,
        NTHAKA_BUFFER_REJECTED,
        NTHAKA_BUFFER_ACCEPTED
    } nthaka_buffer_state_t;

    typedef struct nthaka_format_t
    {
        nthaka_buffer_state_t (*update)(struct nthaka_format_t *fmt, uint8_t d);
        bool (*deserialize)(struct nthaka_format_t *fmt, uint8_t buf[], size_t size, nthaka_gamepad_state_t *out);
        void (*reset)(struct nthaka_format_t *fmt);
    } nthaka_format_t;

#define nthaka_format_update(fmt, d) ((fmt) != NULL ? (fmt)->update((fmt), (d)) \
                                                    : NTHAKA_BUFFER_REJECTED)
#define nthaka_format_deserialize(fmt, buf, size, out) ((fmt) != NULL && \
                                                        (buf) != NULL && \
                                                        (fmt)->deserialize((fmt), (buf), (size), (out)))
#define nthaka_format_reset(fmt) ((fmt) != NULL ? (fmt)->reset((fmt)) \
                                                : (void)0)

#ifndef NTHAKA_BUFFER_SIZE
#define NTHAKA_BUFFER_SIZE (size_t)(64)
#endif

    typedef struct nthaka_buffer_t
    {
        uint8_t _buf[NTHAKA_BUFFER_SIZE];
        size_t _size;
        nthaka_format_t *_fmt;

        nthaka_buffer_state_t _;
    } nthaka_buffer_t;

#define nthaka_buffer_init(buf, fmt) ((buf) != NULL && \
                                      (fmt) != NULL && \
                                      ((buf)->_size = 0, (buf)->_fmt = (fmt), true))
#define nthaka_buffer_append(buf, d, out) ((buf) != NULL && ((buf)->_ = nthaka_format_update((buf)->_fmt, (d))) != NTHAKA_BUFFER_REJECTED ? (((buf)->_buf[(buf)->_size] = (d), (buf)->_size++, (buf)->_ == NTHAKA_BUFFER_ACCEPTED) ? (nthaka_format_deserialize((buf)->_fmt, (buf)->_buf, (buf)->_size, (out)) ? NTHAKA_BUFFER_ACCEPTED  \
                                                                                                                                                                                                                                                                                                               : NTHAKA_BUFFER_REJECTED) \
                                                                                                                                                                                                                                   : NTHAKA_BUFFER_PENDING)                                                                              \
                                                                                                                                          : NTHAKA_BUFFER_REJECTED)
#define nthaka_buffer_clear(buf) ((buf) != NULL ? ((buf)->_size = 0, nthaka_format_reset((buf)->_fmt)) \
                                                : (void)0)

#ifdef __cplusplus
}
#endif

#endif // NTHAKA_H_