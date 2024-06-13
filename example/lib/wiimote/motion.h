#ifndef MOTION_H
#define MOTION_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "wiimote.h"

void set_motion_state(struct wiimote_state * state, float pointer_x, float pointer_y);

#ifdef __cplusplus
}
#endif

#endif