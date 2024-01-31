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

#define NXAMF_GAMEPAD_STATE_NEUTRAL ((nxamf_gamepad_state_t){.y = NXAMF_BUTTON_RELEASED, .b = NXAMF_BUTTON_RELEASED, .a = NXAMF_BUTTON_RELEASED, .x = NXAMF_BUTTON_RELEASED, .l = NXAMF_BUTTON_RELEASED, .r = NXAMF_BUTTON_RELEASED, .zl = NXAMF_BUTTON_RELEASED, .zr = NXAMF_BUTTON_RELEASED, .minus = NXAMF_BUTTON_RELEASED, .plus = NXAMF_BUTTON_RELEASED, .l_click = NXAMF_BUTTON_RELEASED, .r_click = NXAMF_BUTTON_RELEASED, .home = NXAMF_BUTTON_RELEASED, .capture = NXAMF_BUTTON_RELEASED, .hat = NXAMF_HAT_NEUTRAL, .l_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .r_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL}, .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}})
    void nxamf_gamepad_state_copy(nxamf_gamepad_state_t *src, nxamf_gamepad_state_t *dst);

    // char max[] = "{.y = NXAMF_BUTTON_RELEASED, .b = NXAMF_BUTTON_RELEASED, .a = NXAMF_BUTTON_RELEASED, .x = NXAMF_BUTTON_RELEASED, .l = NXAMF_BUTTON_RELEASED, .r = NXAMF_BUTTON_RELEASED, .zl = NXAMF_BUTTON_RELEASED, .zr = NXAMF_BUTTON_RELEASED, .minus = NXAMF_BUTTON_RELEASED, .plus = NXAMF_BUTTON_RELEASED, .l_click = NXAMF_BUTTON_RELEASED, .r_click = NXAMF_BUTTON_RELEASED, .home = NXAMF_BUTTON_RELEASED, .capture = NXAMF_BUTTON_RELEASED, .hat = NXAMF_HAT_DOWNRIGHT, .l_stick = {.x = 255, .y = 255}, .r_stick = {.x = 255, .y = 255}, .extension = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}";
#define NXAMF_GAMEPAD_STATE_STRING_LENGTH_MAX 612
    void nxamf_gamepad_state_stringify(nxamf_gamepad_state_t *s, char *out, size_t len);

    typedef struct nxamf_buffer_interface_t
    {
        bool (*append)(struct nxamf_buffer_interface_t *buf, uint8_t d, nxamf_gamepad_state_t *out);
        void (*clear)(struct nxamf_buffer_interface_t *buf);
    } nxamf_buffer_interface_t;

    typedef struct nxamf_multi_buffer_manager_t
    {
        nxamf_buffer_interface_t parent;

        nxamf_buffer_interface_t **bufs;
        size_t len;
        size_t last_deserialized_index;
    } nxamf_multi_buffer_manager_t;

    void nxamf_multi_buffer_manager_init(nxamf_multi_buffer_manager_t *buf, nxamf_buffer_interface_t **bufs, size_t len);
    size_t nxamf_multi_buffer_manager_get_last_deserialized_index(nxamf_multi_buffer_manager_t *buf);

    typedef enum nxamf_buffer_state_t
    {
        NXAMF_BUFFER_PENDING,
        NXAMF_BUFFER_REJECTED,
        NXAMF_BUFFER_ACCEPTED
    } nxamf_buffer_state_t;

    typedef struct nxamf_protocol_handler_t
    {
        nxamf_buffer_state_t (*update)(struct nxamf_protocol_handler_t *ph, uint8_t d);
        bool (*deserialize)(struct nxamf_protocol_handler_t *ph, uint8_t buf[], size_t size, nxamf_gamepad_state_t *out);
        void (*reset)(struct nxamf_protocol_handler_t *ph);
    } nxamf_protocol_handler_t;

#define nxamf_protocol_handler_update(ph, d) ((ph) != NULL ? (ph)->update((ph), (d)) \
                                                           : NXAMF_BUFFER_REJECTED)
#define nxamf_protocol_handler_deserialize(ph, buf, size, out) ((ph) != NULL &&  \
                                                                (buf) != NULL && \
                                                                (ph)->deserialize((ph), (buf), (size), (out)))
#define nxamf_protocol_handler_reset(ph) ((ph) != NULL ? (ph)->reset((ph)) \
                                                       : (void)0)

#ifndef NXAMF_BUFFER_SIZE
#define NXAMF_BUFFER_SIZE (size_t)(256)
#endif

    typedef struct nxamf_buffer_t
    {
        uint8_t _buf[NXAMF_BUFFER_SIZE];
        size_t _size;
        nxamf_protocol_handler_t *_ph;

        nxamf_buffer_state_t _tmp;
    } nxamf_buffer_t;

#define nxamf_buffer_init(buf, ph) ((buf) != NULL &&           \
                                    (ph) != NULL &&            \
                                    ((buf)->_size = 0) == 0 && \
                                    ((buf)->_ph = (ph)) == (ph))
#define nxamf_buffer_append(buf, d, out) ((buf) != NULL && ((buf)->_tmp = nxamf_protocol_handler_update((buf)->_ph, (d))) != NXAMF_BUFFER_REJECTED ? (((buf)->_buf[(buf)->_size++] = (d)) == (d) && (buf)->_tmp == NXAMF_BUFFER_ACCEPTED ? (nxamf_protocol_handler_deserialize((buf)->_ph, (buf)->_buf, (buf)->_size, (out)) ? NXAMF_BUFFER_ACCEPTED  \
                                                                                                                                                                                                                                                                                                                             : NXAMF_BUFFER_REJECTED) \
                                                                                                                                                                                                                                         : NXAMF_BUFFER_PENDING)                                                                                      \
                                                                                                                                                   : NXAMF_BUFFER_REJECTED)
#define nxamf_buffer_clear(buf) ((buf) != NULL && ((buf)->_size = 0) == 0 ? nxamf_protocol_handler_reset((buf)->_ph) \
                                                                          : (void)0)

#ifdef __cplusplus
}
#endif

#endif // NXAMF_H_