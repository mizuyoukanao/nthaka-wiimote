#include "nthaka.h"
#include "internal.h"

#include <stdio.h>

void nthaka_gamepad_state_copy(nthaka_gamepad_state_t *dst, nthaka_gamepad_state_t *src)
{
    dst->y = src->y;
    dst->b = src->b;
    dst->a = src->a;
    dst->x = src->x;
    dst->l = src->l;
    dst->r = src->r;
    dst->zl = src->zl;
    dst->zr = src->zr;
    dst->minus = src->minus;
    dst->plus = src->plus;
    dst->l_click = src->l_click;
    dst->r_click = src->r_click;
    dst->home = src->home;
    dst->capture = src->capture;
    dst->hat = src->hat;
    dst->l_stick.x = src->l_stick.x;
    dst->l_stick.y = src->l_stick.y;
    dst->r_stick.x = src->r_stick.x;
    dst->r_stick.y = src->r_stick.y;

    for (size_t i = 0; i < nthaka_internal_size(dst->ext); i++)
    {
        dst->ext[i] = src->ext[i];
    }
}

static const char *_btn(nthaka_button_state_t btn)
{
    switch (btn)
    {
    case NTHAKA_BUTTON_STATE_RELEASED:
        return "NTHAKA_BUTTON_STATE_RELEASED";
    case NTHAKA_BUTTON_STATE_PRESSED:
        return "NTHAKA_BUTTON_STATE_PRESSED";
    default:
        return "UNKNOWN";
    }
}

static const char *_hat(nthaka_hat_state_t hat)
{
    switch (hat)
    {
    case NTHAKA_HAT_STATE_UP:
        return "NTHAKA_HAT_STATE_UP";
    case NTHAKA_HAT_STATE_UPRIGHT:
        return "NTHAKA_HAT_STATE_UPRIGHT";
    case NTHAKA_HAT_STATE_RIGHT:
        return "NTHAKA_HAT_STATE_RIGHT";
    case NTHAKA_HAT_STATE_DOWNRIGHT:
        return "NTHAKA_HAT_STATE_DOWNRIGHT";
    case NTHAKA_HAT_STATE_DOWN:
        return "NTHAKA_HAT_STATE_DOWN";
    case NTHAKA_HAT_STATE_DOWNLEFT:
        return "NTHAKA_HAT_STATE_DOWNLEFT";
    case NTHAKA_HAT_STATE_LEFT:
        return "NTHAKA_HAT_STATE_LEFT";
    case NTHAKA_HAT_STATE_UPLEFT:
        return "NTHAKA_HAT_STATE_UPLEFT";
    case NTHAKA_HAT_STATE_NEUTRAL:
        return "NTHAKA_HAT_STATE_NEUTRAL";
    default:
        return "UNKNOWN";
    }
}

void nthaka_gamepad_state_stringify(nthaka_gamepad_state_t *s, char *out, size_t size)
{
    snprintf(out, size, "{.y = %s, "
                        ".b = %s, "
                        ".a = %s, "
                        ".x = %s, "
                        ".l = %s, "
                        ".r = %s, "
                        ".zl = %s, "
                        ".zr = %s, "
                        ".minus = %s, "
                        ".plus = %s, "
                        ".l_click = %s, "
                        ".r_click = %s, "
                        ".home = %s, "
                        ".capture = %s, "
                        ".hat = %s, "
                        ".l_stick = {.x = %d, "
                        ".y = %d}, "
                        ".r_stick = {.x = %d, "
                        ".y = %d}, "
                        ".ext = {%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d}}",
             _btn(s->y),
             _btn(s->b),
             _btn(s->a),
             _btn(s->x),
             _btn(s->l),
             _btn(s->r),
             _btn(s->zl),
             _btn(s->zr),
             _btn(s->minus),
             _btn(s->plus),
             _btn(s->l_click),
             _btn(s->r_click),
             _btn(s->home),
             _btn(s->capture),
             _hat(s->hat),
             s->l_stick.x,
             s->l_stick.y,
             s->r_stick.x,
             s->r_stick.y,
             s->ext[0],
             s->ext[1],
             s->ext[2],
             s->ext[3],
             s->ext[4],
             s->ext[5],
             s->ext[6],
             s->ext[7],
             s->ext[8],
             s->ext[9],
             s->ext[10],
             s->ext[11],
             s->ext[12],
             s->ext[13],
             s->ext[14],
             s->ext[15]);
}