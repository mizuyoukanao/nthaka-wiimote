#include "nxamf.h"

#include <stdio.h>

void nxamf_gamepad_state_copy(nxamf_gamepad_state_t *dst, nxamf_gamepad_state_t *src)
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

    size_t length = sizeof(dst->extension) / sizeof(uint8_t);
    for (size_t i = 0; i < length; i++)
    {
        dst->extension[i] = src->extension[i];
    }
}

static const char *_btn(nxamf_button_t btn)
{
    switch (btn)
    {
    case NXAMF_BUTTON_RELEASED:
        return "NXAMF_BUTTON_RELEASED";
    case NXAMF_BUTTON_PRESSED:
        return "NXAMF_BUTTON_PRESSED";
    default:
        return "UNKNOWN";
    }
}

static const char *_hat(nxamf_hat_t hat)
{
    switch (hat)
    {
    case NXAMF_HAT_UP:
        return "NXAMF_HAT_UP";
    case NXAMF_HAT_UPRIGHT:
        return "NXAMF_HAT_UPRIGHT";
    case NXAMF_HAT_RIGHT:
        return "NXAMF_HAT_RIGHT";
    case NXAMF_HAT_DOWNRIGHT:
        return "NXAMF_HAT_DOWNRIGHT";
    case NXAMF_HAT_DOWN:
        return "NXAMF_HAT_DOWN";
    case NXAMF_HAT_DOWNLEFT:
        return "NXAMF_HAT_DOWNLEFT";
    case NXAMF_HAT_LEFT:
        return "NXAMF_HAT_LEFT";
    case NXAMF_HAT_UPLEFT:
        return "NXAMF_HAT_UPLEFT";
    case NXAMF_HAT_NEUTRAL:
        return "NXAMF_HAT_NEUTRAL";
    default:
        return "UNKNOWN";
    }
}

void nxamf_gamepad_state_stringify(nxamf_gamepad_state_t *s, char *out, size_t len)
{
    snprintf(out, len, "{.y = %s, "
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
                       ".extension = {%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d}}",
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
             s->extension[0],
             s->extension[1],
             s->extension[2],
             s->extension[3],
             s->extension[4],
             s->extension[5],
             s->extension[6],
             s->extension[7],
             s->extension[8],
             s->extension[9],
             s->extension[10],
             s->extension[11],
             s->extension[12],
             s->extension[13],
             s->extension[14],
             s->extension[15]);
}