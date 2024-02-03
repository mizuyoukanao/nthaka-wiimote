#ifndef NTHAKA_H_
#define NTHAKA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

    typedef enum nthaka_button_state_t
    {
        NTHAKA_BUTTON_STATE_RELEASED,
        NTHAKA_BUTTON_STATE_PRESSED
    } nthaka_button_state_t;

#define nthaka_button_state_stringify(btn) ((btn) == NTHAKA_BUTTON_STATE_RELEASED  ? "RELEASED" \
                                            : (btn) == NTHAKA_BUTTON_STATE_PRESSED ? "PRESSED"  \
                                                                                   : "unknown")

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

#define nthaka_hat_state_stringify(hat) ((hat) == NTHAKA_HAT_STATE_UP          ? "UP"        \
                                         : (hat) == NTHAKA_HAT_STATE_UPRIGHT   ? "UPRIGHT"   \
                                         : (hat) == NTHAKA_HAT_STATE_RIGHT     ? "RIGHT"     \
                                         : (hat) == NTHAKA_HAT_STATE_DOWNRIGHT ? "DOWNRIGHT" \
                                         : (hat) == NTHAKA_HAT_STATE_DOWN      ? "DOWN"      \
                                         : (hat) == NTHAKA_HAT_STATE_DOWNLEFT  ? "DOWNLEFT"  \
                                         : (hat) == NTHAKA_HAT_STATE_LEFT      ? "LEFT"      \
                                         : (hat) == NTHAKA_HAT_STATE_UPLEFT    ? "UPLEFT"    \
                                         : (hat) == NTHAKA_HAT_STATE_NEUTRAL   ? "NEUTRAL"   \
                                                                               : "unknown")

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

#define nthaka_gamepad_state_copy(dst, src) ((dst != NULL && src != NULL) ? ((dst)->y = (src)->y,                 \
                                                                             (dst)->b = (src)->b,                 \
                                                                             (dst)->a = (src)->a,                 \
                                                                             (dst)->x = (src)->x,                 \
                                                                             (dst)->l = (src)->l,                 \
                                                                             (dst)->r = (src)->r,                 \
                                                                             (dst)->zl = (src)->zl,               \
                                                                             (dst)->zr = (src)->zr,               \
                                                                             (dst)->minus = (src)->minus,         \
                                                                             (dst)->plus = (src)->plus,           \
                                                                             (dst)->l_click = (src)->l_click,     \
                                                                             (dst)->r_click = (src)->r_click,     \
                                                                             (dst)->home = (src)->home,           \
                                                                             (dst)->capture = (src)->capture,     \
                                                                             (dst)->hat = (src)->hat,             \
                                                                             (dst)->l_stick.x = (src)->l_stick.x, \
                                                                             (dst)->l_stick.y = (src)->l_stick.y, \
                                                                             (dst)->r_stick.x = (src)->r_stick.x, \
                                                                             (dst)->r_stick.y = (src)->r_stick.y, \
                                                                             (dst)->ext[0] = (src)->ext[0],       \
                                                                             (dst)->ext[1] = (src)->ext[1],       \
                                                                             (dst)->ext[2] = (src)->ext[2],       \
                                                                             (dst)->ext[3] = (src)->ext[3],       \
                                                                             (dst)->ext[4] = (src)->ext[4],       \
                                                                             (dst)->ext[5] = (src)->ext[5],       \
                                                                             (dst)->ext[6] = (src)->ext[6],       \
                                                                             (dst)->ext[7] = (src)->ext[7],       \
                                                                             (dst)->ext[8] = (src)->ext[8],       \
                                                                             (dst)->ext[9] = (src)->ext[9],       \
                                                                             (dst)->ext[10] = (src)->ext[10],     \
                                                                             (dst)->ext[11] = (src)->ext[11],     \
                                                                             (dst)->ext[12] = (src)->ext[12],     \
                                                                             (dst)->ext[13] = (src)->ext[13],     \
                                                                             (dst)->ext[14] = (src)->ext[14],     \
                                                                             (dst)->ext[15] = (src)->ext[15],     \
                                                                             (void)0)                             \
                                                                          : (void)0)

#define nthaka_gamepad_state_stringify(s, out, size) ((s) != NULL ? (snprintf((out), (size), "{.y=%s,"                                                  \
                                                                                             ".b=%s,"                                                   \
                                                                                             ".a=%s,"                                                   \
                                                                                             ".x=%s,"                                                   \
                                                                                             ".l=%s,"                                                   \
                                                                                             ".r=%s,"                                                   \
                                                                                             ".zl=%s,"                                                  \
                                                                                             ".zr=%s,"                                                  \
                                                                                             ".minus=%s,"                                               \
                                                                                             ".plus=%s,"                                                \
                                                                                             ".l_click=%s,"                                             \
                                                                                             ".r_click=%s,"                                             \
                                                                                             ".home=%s,"                                                \
                                                                                             ".capture=%s,"                                             \
                                                                                             ".hat=%s,"                                                 \
                                                                                             ".l_stick={.x=%d,.y=%d},"                                  \
                                                                                             ".r_stick={.x=%d,.y=%d},"                                  \
                                                                                             ".ext={%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d}}", \
                                                                              nthaka_button_state_stringify((s)->y),                                    \
                                                                              nthaka_button_state_stringify((s)->b),                                    \
                                                                              nthaka_button_state_stringify((s)->a),                                    \
                                                                              nthaka_button_state_stringify((s)->x),                                    \
                                                                              nthaka_button_state_stringify((s)->l),                                    \
                                                                              nthaka_button_state_stringify((s)->r),                                    \
                                                                              nthaka_button_state_stringify((s)->zl),                                   \
                                                                              nthaka_button_state_stringify((s)->zr),                                   \
                                                                              nthaka_button_state_stringify((s)->minus),                                \
                                                                              nthaka_button_state_stringify((s)->plus),                                 \
                                                                              nthaka_button_state_stringify((s)->l_click),                              \
                                                                              nthaka_button_state_stringify((s)->r_click),                              \
                                                                              nthaka_button_state_stringify((s)->home),                                 \
                                                                              nthaka_button_state_stringify((s)->capture),                              \
                                                                              nthaka_hat_state_stringify((s)->hat),                                     \
                                                                              (s)->l_stick.x,                                                           \
                                                                              (s)->l_stick.y,                                                           \
                                                                              (s)->r_stick.x,                                                           \
                                                                              (s)->r_stick.y,                                                           \
                                                                              (s)->ext[0],                                                              \
                                                                              (s)->ext[1],                                                              \
                                                                              (s)->ext[2],                                                              \
                                                                              (s)->ext[3],                                                              \
                                                                              (s)->ext[4],                                                              \
                                                                              (s)->ext[5],                                                              \
                                                                              (s)->ext[6],                                                              \
                                                                              (s)->ext[7],                                                              \
                                                                              (s)->ext[8],                                                              \
                                                                              (s)->ext[9],                                                              \
                                                                              (s)->ext[10],                                                             \
                                                                              (s)->ext[11],                                                             \
                                                                              (s)->ext[12],                                                             \
                                                                              (s)->ext[13],                                                             \
                                                                              (s)->ext[14],                                                             \
                                                                              (s)->ext[15]),                                                            \
                                                                     (void)0)                                                                           \
                                                                  : (void)0)

    // char max[] = "{.y=RELEASED,.b=RELEASED,.a=RELEASED,.x=RELEASED,.l=RELEASED,.r=RELEASED,.zl=RELEASED,.zr=RELEASED,.minus=RELEASED,.plus=RELEASED,.l_click=RELEASED,.r_click=RELEASED,.home=RELEASED,.capture=RELEASED,.hat=DOWNRIGHT,.l_stick={.x=255,.y=255},.r_stick={.x=255,.y=255},.ext={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}}";
#define NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX 336

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