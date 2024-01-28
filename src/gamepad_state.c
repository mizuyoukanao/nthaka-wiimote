#include "nxamf.h"

#include <stdio.h>

bool nxamf_gamepad_state_equals(nxamf_gamepad_state_t *s1, nxamf_gamepad_state_t *s2)
{
    return s1->y == s2->y &&
           s1->b == s2->b &&
           s1->a == s2->a &&
           s1->x == s2->x &&
           s1->l == s2->l &&
           s1->r == s2->r &&
           s1->zl == s2->zl &&
           s1->zr == s2->zr &&
           s1->minus == s2->minus &&
           s1->plus == s2->plus &&
           s1->l_click == s2->l_click &&
           s1->r_click == s2->r_click &&
           s1->home == s2->home &&
           s1->capture == s2->capture &&
           s1->hat == s2->hat &&
           s1->l_stick.x == s2->l_stick.x &&
           s1->l_stick.y == s2->l_stick.y &&
           s1->r_stick.x == s2->r_stick.x &&
           s1->r_stick.y == s2->r_stick.y &&
           s1->extension[0] == s2->extension[0] &&
           s1->extension[1] == s2->extension[1] &&
           s1->extension[2] == s2->extension[2] &&
           s1->extension[3] == s2->extension[3] &&
           s1->extension[4] == s2->extension[4] &&
           s1->extension[5] == s2->extension[5] &&
           s1->extension[6] == s2->extension[6] &&
           s1->extension[7] == s2->extension[7] &&
           s1->extension[8] == s2->extension[8] &&
           s1->extension[9] == s2->extension[9] &&
           s1->extension[10] == s2->extension[10] &&
           s1->extension[11] == s2->extension[11] &&
           s1->extension[12] == s2->extension[12] &&
           s1->extension[13] == s2->extension[13] &&
           s1->extension[14] == s2->extension[14] &&
           s1->extension[15] == s2->extension[15];
}

void nxamf_gamepad_state_copy(nxamf_gamepad_state_t *src, nxamf_gamepad_state_t *dst)
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