#ifndef NTHAKA_H_
#define NTHAKA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

    typedef enum nthaka_button_t
    {
        NTHAKA_BUTTON_RELEASED,
        NTHAKA_BUTTON_PRESSED
    } nthaka_button_t;

    typedef enum nthaka_hat_t
    {
        NTHAKA_HAT_UP,
        NTHAKA_HAT_UPRIGHT,
        NTHAKA_HAT_RIGHT,
        NTHAKA_HAT_DOWNRIGHT,
        NTHAKA_HAT_DOWN,
        NTHAKA_HAT_DOWNLEFT,
        NTHAKA_HAT_LEFT,
        NTHAKA_HAT_UPLEFT,
        NTHAKA_HAT_NEUTRAL
    } nthaka_hat_t;

#define NTHAKA_STICK_NEUTRAL 128U

    typedef struct nthaka_stick_t
    {
        uint8_t x;
        uint8_t y;
    } nthaka_stick_t;

    typedef struct nthaka_gamepad_state_t
    {
        nthaka_button_t y;
        nthaka_button_t b;
        nthaka_button_t a;
        nthaka_button_t x;
        nthaka_button_t l;
        nthaka_button_t r;
        nthaka_button_t zl;
        nthaka_button_t zr;
        nthaka_button_t minus;
        nthaka_button_t plus;
        nthaka_button_t l_click;
        nthaka_button_t r_click;
        nthaka_button_t home;
        nthaka_button_t capture;

        nthaka_hat_t hat;

        nthaka_stick_t l_stick;
        nthaka_stick_t r_stick;

        uint8_t extension[16];
    } nthaka_gamepad_state_t;

#define NTHAKA_GAMEPAD_STATE_NEUTRAL ((nthaka_gamepad_state_t){.y = NTHAKA_BUTTON_RELEASED, .b = NTHAKA_BUTTON_RELEASED, .a = NTHAKA_BUTTON_RELEASED, .x = NTHAKA_BUTTON_RELEASED, .l = NTHAKA_BUTTON_RELEASED, .r = NTHAKA_BUTTON_RELEASED, .zl = NTHAKA_BUTTON_RELEASED, .zr = NTHAKA_BUTTON_RELEASED, .minus = NTHAKA_BUTTON_RELEASED, .plus = NTHAKA_BUTTON_RELEASED, .l_click = NTHAKA_BUTTON_RELEASED, .r_click = NTHAKA_BUTTON_RELEASED, .home = NTHAKA_BUTTON_RELEASED, .capture = NTHAKA_BUTTON_RELEASED, .hat = NTHAKA_HAT_NEUTRAL, .l_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL}, .r_stick = {.x = NTHAKA_STICK_NEUTRAL, .y = NTHAKA_STICK_NEUTRAL}, .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}})
    void nthaka_gamepad_state_copy(nthaka_gamepad_state_t *src, nthaka_gamepad_state_t *dst);

    // char max[] = "{.y = NTHAKA_BUTTON_RELEASED, .b = NTHAKA_BUTTON_RELEASED, .a = NTHAKA_BUTTON_RELEASED, .x = NTHAKA_BUTTON_RELEASED, .l = NTHAKA_BUTTON_RELEASED, .r = NTHAKA_BUTTON_RELEASED, .zl = NTHAKA_BUTTON_RELEASED, .zr = NTHAKA_BUTTON_RELEASED, .minus = NTHAKA_BUTTON_RELEASED, .plus = NTHAKA_BUTTON_RELEASED, .l_click = NTHAKA_BUTTON_RELEASED, .r_click = NTHAKA_BUTTON_RELEASED, .home = NTHAKA_BUTTON_RELEASED, .capture = NTHAKA_BUTTON_RELEASED, .hat = NTHAKA_HAT_DOWNRIGHT, .l_stick = {.x = 255, .y = 255}, .r_stick = {.x = 255, .y = 255}, .extension = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}";
#define NTHAKA_GAMEPAD_STATE_STRING_LENGTH_MAX 612
    void nthaka_gamepad_state_stringify(nthaka_gamepad_state_t *s, char *out, size_t len);

    typedef struct nthaka_buffer_interface_t
    {
        bool (*append)(struct nthaka_buffer_interface_t *buf, uint8_t d, nthaka_gamepad_state_t *out);
        void (*clear)(struct nthaka_buffer_interface_t *buf);
    } nthaka_buffer_interface_t;

    typedef struct nthaka_multi_buffer_manager_t
    {
        nthaka_buffer_interface_t parent;

        nthaka_buffer_interface_t **bufs;
        size_t len;
        size_t last_deserialized_index;
    } nthaka_multi_buffer_manager_t;

    void nthaka_multi_buffer_manager_init(nthaka_multi_buffer_manager_t *buf, nthaka_buffer_interface_t **bufs, size_t len);
    size_t nthaka_multi_buffer_manager_get_last_deserialized_index(nthaka_multi_buffer_manager_t *buf);

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
#define NTHAKA_BUFFER_SIZE (size_t)(256)
#endif

    typedef struct nthaka_buffer_t
    {
        uint8_t _buf[NTHAKA_BUFFER_SIZE];
        size_t _size;
        nthaka_format_t *_fmt;

        nthaka_buffer_state_t _tmp;
    } nthaka_buffer_t;

#define nthaka_buffer_init(buf, fmt) ((buf) != NULL &&           \
                                      (fmt) != NULL &&           \
                                      ((buf)->_size = 0) == 0 && \
                                      ((buf)->_fmt = (fmt)) == (fmt))
#define nthaka_buffer_append(buf, d, out) ((buf) != NULL && ((buf)->_tmp = nthaka_format_update((buf)->_fmt, (d))) != NTHAKA_BUFFER_REJECTED ? (((buf)->_buf[(buf)->_size++] = (d)) == (d) && (buf)->_tmp == NTHAKA_BUFFER_ACCEPTED ? (nthaka_format_deserialize((buf)->_fmt, (buf)->_buf, (buf)->_size, (out)) ? NTHAKA_BUFFER_ACCEPTED  \
                                                                                                                                                                                                                                                                                                                : NTHAKA_BUFFER_REJECTED) \
                                                                                                                                                                                                                                    : NTHAKA_BUFFER_PENDING)                                                                              \
                                                                                                                                             : NTHAKA_BUFFER_REJECTED)
#define nthaka_buffer_clear(buf) ((buf) != NULL && ((buf)->_size = 0) == 0 ? nthaka_format_reset((buf)->_fmt) \
                                                                           : (void)0)

#ifdef __cplusplus
}
#endif

#endif // NTHAKA_H_