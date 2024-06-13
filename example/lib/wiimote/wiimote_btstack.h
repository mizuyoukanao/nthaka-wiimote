#ifndef _WIIMOTE_BTSTACK_H_
#define _WIIMOTE_BTSTACK_H_

#ifdef __cplusplus
extern "C"
{
#endif

void wiimote_emulator_set_led(void (*led_on)(), void (*led_off)());

void wiimote_emulator(void *report);

#ifdef __cplusplus
}
#endif

#endif // _WIIMOTE_BTSTACK_H_
