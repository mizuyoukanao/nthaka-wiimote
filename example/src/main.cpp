#include <Arduino.h>

#include "hardware/timer.h"

#include "nthaka.h"
#include "nthaka/nxmc2.h"
#include "nthaka/orca.h"
#include "nthaka/pokecon.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "wiimote.h"
#include "wiimote_btstack.h"

#ifdef __cplusplus
}
#endif

static nxmc2_format_handler_t nxmc2;
static orca_format_handler_t orca;
static pokecon_format_handler_t pokecon;
static nthaka_format_handler_t *fmts[] = {(nthaka_format_handler_t *)&nxmc2, //
                                          (nthaka_format_handler_t *)&orca,  //
                                          (nthaka_format_handler_t *)&pokecon};
static nthaka_multi_format_handler_t fmt;
static nthaka_buffer_t buf;

static nthaka_gamepad_state_t out;
static char str[NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX];

static WiimoteReport wiimote_report = {
        .wiimote = {0},
        .nunchuk = {0},
        .classic = {0},
        .switch_mode = 0,
        .mode = NO_EXTENSION,
        .fake_motion = 0,
        .center_accel = 0
};

static int64_t _led_off(alarm_id_t id, void *user_data)
{
    digitalWriteFast(LED_BUILTIN, LOW);
    return 0;
}

static inline void async_led_on(uint32_t dur_ms)
{
    digitalWriteFast(LED_BUILTIN, HIGH);
    add_alarm_in_ms(dur_ms, _led_off, NULL, false);
}

void led_on() {
    digitalWrite(LED_BUILTIN, HIGH);
}

void led_off() {
    digitalWrite(LED_BUILTIN, LOW);
}

static void update()
{
    async_led_on(100);

    //wiimote or classic
    switch (wiimote_report.mode) {
        case NO_EXTENSION:
            /*new data*/
            wiimote_report.wiimote.a = out.a;
            wiimote_report.wiimote.b = out.b;
            wiimote_report.wiimote.minus = out.minus;
            wiimote_report.wiimote.plus = out.plus;
            wiimote_report.wiimote.home = out.home;
            wiimote_report.wiimote.one = out.l_click;
            wiimote_report.wiimote.two = out.r_click;
            switch (out.hat) {
                case NTHAKA_HAT_UP:
                    wiimote_report.wiimote.up = 1;
                    wiimote_report.wiimote.down = 0;
                    wiimote_report.wiimote.left = 0;
                    wiimote_report.wiimote.right = 0;
                    break;
                case NTHAKA_HAT_UPRIGHT:
                    wiimote_report.wiimote.up = 1;
                    wiimote_report.wiimote.down = 0;
                    wiimote_report.wiimote.left = 0;
                    wiimote_report.wiimote.right = 1;
                    break;
                case NTHAKA_HAT_RIGHT:
                    wiimote_report.wiimote.up = 0;
                    wiimote_report.wiimote.down = 0;
                    wiimote_report.wiimote.left = 0;
                    wiimote_report.wiimote.right = 1;
                    break;
                case NTHAKA_HAT_DOWNRIGHT:
                    wiimote_report.wiimote.up = 0;
                    wiimote_report.wiimote.down = 1;
                    wiimote_report.wiimote.left = 0;
                    wiimote_report.wiimote.right = 1;
                    break;
                case NTHAKA_HAT_DOWN:
                    wiimote_report.wiimote.up = 0;
                    wiimote_report.wiimote.down = 1;
                    wiimote_report.wiimote.left = 0;
                    wiimote_report.wiimote.right = 0;
                    break;
                case NTHAKA_HAT_DOWNLEFT:
                    wiimote_report.wiimote.up = 0;
                    wiimote_report.wiimote.down = 1;
                    wiimote_report.wiimote.left = 1;
                    wiimote_report.wiimote.right = 0;
                    break;
                case NTHAKA_HAT_LEFT:
                    wiimote_report.wiimote.up = 0;
                    wiimote_report.wiimote.down = 0;
                    wiimote_report.wiimote.left = 1;
                    wiimote_report.wiimote.right = 0;
                    break;
                case NTHAKA_HAT_UPLEFT:
                    wiimote_report.wiimote.up = 1;
                    wiimote_report.wiimote.down = 0;
                    wiimote_report.wiimote.left = 1;
                    wiimote_report.wiimote.right = 0;
                    break;
                default:
                case NTHAKA_HAT_NEUTRAL:
                    wiimote_report.wiimote.up = 0;
                    wiimote_report.wiimote.down = 0;
                    wiimote_report.wiimote.left = 0;
                    wiimote_report.wiimote.right = 0;
                    break;
            }
            wiimote_report.wiimote.ir_x = (int8_t)map(out.l_stick.x, 0, 255, -128, 127);
            wiimote_report.wiimote.ir_y = (int8_t)map(out.l_stick.y, 0, 255, 127, -128);

            /*check if it wanna change the mode*/
            if (wiimote_report.wiimote.one & wiimote_report.wiimote.two) {
                wiimote_report.switch_mode = 1;
            }
                /*wait to change to zero*/
            else {
                if (wiimote_report.switch_mode) {
                    wiimote_report.mode = WIIMOTE_AND_NUNCHUCK;
                    wiimote_report.switch_mode = 0;
                    wiimote_report.reset_ir = 1;
                }
            }
            break;
        case WIIMOTE_AND_NUNCHUCK:
            /*new data*/
            /*wiimote*/
            wiimote_report.wiimote.a = out.a;
            wiimote_report.wiimote.b = out.b;
            wiimote_report.wiimote.minus = out.minus;
            wiimote_report.wiimote.plus = out.plus;
            wiimote_report.wiimote.home = out.home;
            wiimote_report.wiimote.one = out.l_click;
            wiimote_report.wiimote.two = out.r_click;
            switch (out.hat) {
                case NTHAKA_HAT_UP:
                    wiimote_report.wiimote.up = 1;
                    wiimote_report.wiimote.down = 0;
                    wiimote_report.wiimote.left = 0;
                    wiimote_report.wiimote.right = 0;
                    break;
                case NTHAKA_HAT_UPRIGHT:
                    wiimote_report.wiimote.up = 1;
                    wiimote_report.wiimote.down = 0;
                    wiimote_report.wiimote.left = 0;
                    wiimote_report.wiimote.right = 1;
                    break;
                case NTHAKA_HAT_RIGHT:
                    wiimote_report.wiimote.up = 0;
                    wiimote_report.wiimote.down = 0;
                    wiimote_report.wiimote.left = 0;
                    wiimote_report.wiimote.right = 1;
                    break;
                case NTHAKA_HAT_DOWNRIGHT:
                    wiimote_report.wiimote.up = 0;
                    wiimote_report.wiimote.down = 1;
                    wiimote_report.wiimote.left = 0;
                    wiimote_report.wiimote.right = 1;
                    break;
                case NTHAKA_HAT_DOWN:
                    wiimote_report.wiimote.up = 0;
                    wiimote_report.wiimote.down = 1;
                    wiimote_report.wiimote.left = 0;
                    wiimote_report.wiimote.right = 0;
                    break;
                case NTHAKA_HAT_DOWNLEFT:
                    wiimote_report.wiimote.up = 0;
                    wiimote_report.wiimote.down = 1;
                    wiimote_report.wiimote.left = 1;
                    wiimote_report.wiimote.right = 0;
                    break;
                case NTHAKA_HAT_LEFT:
                    wiimote_report.wiimote.up = 0;
                    wiimote_report.wiimote.down = 0;
                    wiimote_report.wiimote.left = 1;
                    wiimote_report.wiimote.right = 0;
                    break;
                case NTHAKA_HAT_UPLEFT:
                    wiimote_report.wiimote.up = 1;
                    wiimote_report.wiimote.down = 0;
                    wiimote_report.wiimote.left = 1;
                    wiimote_report.wiimote.right = 0;
                    break;
                default:
                case NTHAKA_HAT_NEUTRAL:
                    wiimote_report.wiimote.up = 0;
                    wiimote_report.wiimote.down = 0;
                    wiimote_report.wiimote.left = 0;
                    wiimote_report.wiimote.right = 0;
                    break;
            }
            wiimote_report.wiimote.ir_x = (int8_t)map(out.r_stick.x, 0, 255, -128, 127);
            wiimote_report.wiimote.ir_y = (int8_t)map(out.r_stick.y, 0, 255, 127, -128);

            /*nunchuk*/
            wiimote_report.nunchuk.x = out.l_stick.x;
            wiimote_report.nunchuk.y = map(out.l_stick.y, 0, 255, 255, 0);
            wiimote_report.nunchuk.c = out.l;
            wiimote_report.nunchuk.z = out.zl;

            /*check if it wanna change the mode*/
            if (wiimote_report.wiimote.one & wiimote_report.wiimote.two) {
                wiimote_report.switch_mode = 1;
            }
                /*wait to change to zero*/
            else {
                if (wiimote_report.switch_mode) {
                    wiimote_report.mode = CLASSIC_CONTROLLER;
                    wiimote_report.switch_mode = 0;
                    wiimote_report.reset_ir = 1;
                }
            }

            // Fake motion
            if (out.r) {

                wiimote_report.fake_motion = 1;

                int8_t step = 48;

                if(wiimote_report.wiimote.accel_x > 1000 || wiimote_report.wiimote.accel_x < 10){
                    step = -step;
                }

                // Start moving all directions

                wiimote_report.wiimote.accel_x = step + (wiimote_report.wiimote.accel_x & 0x3FF);
                wiimote_report.nunchuk.accel_x = step + (wiimote_report.nunchuk.accel_x & 0x3FF);

                wiimote_report.wiimote.accel_y = step + (wiimote_report.wiimote.accel_y & 0x3FF);
                wiimote_report.nunchuk.accel_y = step + (wiimote_report.nunchuk.accel_y & 0x3FF);

                wiimote_report.wiimote.accel_z = step + (wiimote_report.wiimote.accel_z & 0x3FF);
                wiimote_report.nunchuk.accel_z = step + (wiimote_report.nunchuk.accel_z & 0x3FF);
            } else {
                if (wiimote_report.fake_motion) {

                    // Set center values
                    wiimote_report.wiimote.accel_x += 0x82 << 2;
                    wiimote_report.nunchuk.accel_x += 0x82 << 2;

                    wiimote_report.wiimote.accel_y += 0x82 << 2;
                    wiimote_report.nunchuk.accel_y += 0x82 << 2;

                    wiimote_report.wiimote.accel_z += 0x9f << 2;
                    wiimote_report.nunchuk.accel_z += 0x9f << 2;

                    wiimote_report.fake_motion = 0;
                    wiimote_report.center_accel = 1;
                }
            }

            break;
        case CLASSIC_CONTROLLER:
            /*new data*/
            wiimote_report.classic.a = out.a;
            wiimote_report.classic.b = out.b;
            wiimote_report.classic.x = out.x;
            wiimote_report.classic.y = out.y;
            wiimote_report.classic.home = out.home;
            wiimote_report.classic.minus = out.minus;
            wiimote_report.classic.plus = out.plus;
            switch (out.hat) {
                case NTHAKA_HAT_UP:
                    wiimote_report.classic.up = 1;
                    wiimote_report.classic.down = 0;
                    wiimote_report.classic.left = 0;
                    wiimote_report.classic.right = 0;
                    break;
                case NTHAKA_HAT_UPRIGHT:
                    wiimote_report.classic.up = 1;
                    wiimote_report.classic.down = 0;
                    wiimote_report.classic.left = 0;
                    wiimote_report.classic.right = 1;
                    break;
                case NTHAKA_HAT_RIGHT:
                    wiimote_report.classic.up = 0;
                    wiimote_report.classic.down = 0;
                    wiimote_report.classic.left = 0;
                    wiimote_report.classic.right = 1;
                    break;
                case NTHAKA_HAT_DOWNRIGHT:
                    wiimote_report.classic.up = 0;
                    wiimote_report.classic.down = 1;
                    wiimote_report.classic.left = 0;
                    wiimote_report.classic.right = 1;
                    break;
                case NTHAKA_HAT_DOWN:
                    wiimote_report.classic.up = 0;
                    wiimote_report.classic.down = 1;
                    wiimote_report.classic.left = 0;
                    wiimote_report.classic.right = 0;
                    break;
                case NTHAKA_HAT_DOWNLEFT:
                    wiimote_report.classic.up = 0;
                    wiimote_report.classic.down = 1;
                    wiimote_report.classic.left = 1;
                    wiimote_report.classic.right = 0;
                    break;
                case NTHAKA_HAT_LEFT:
                    wiimote_report.classic.up = 0;
                    wiimote_report.classic.down = 0;
                    wiimote_report.classic.left = 1;
                    wiimote_report.classic.right = 0;
                    break;
                case NTHAKA_HAT_UPLEFT:
                    wiimote_report.classic.up = 1;
                    wiimote_report.classic.down = 0;
                    wiimote_report.classic.left = 1;
                    wiimote_report.classic.right = 0;
                    break;
                default:
                case NTHAKA_HAT_NEUTRAL:
                    wiimote_report.classic.up = 0;
                    wiimote_report.classic.down = 0;
                    wiimote_report.classic.left = 0;
                    wiimote_report.classic.right = 0;
                    break;
            }
            wiimote_report.classic.lz = out.zl;
            wiimote_report.classic.rz = out.zr;
            wiimote_report.classic.lt = out.l ? 255 : 0;
            wiimote_report.classic.rt = out.r ? 255 : 0;
            wiimote_report.classic.ltrigger = out.l;    //digital button of lt
            wiimote_report.classic.rtrigger = out.r;  //digital button of rt
            wiimote_report.classic.ls_x = out.l_stick.x;
            wiimote_report.classic.ls_y = map(out.l_stick.y, 0, 255, 255, 0);
            wiimote_report.classic.rs_x = out.r_stick.x;
            wiimote_report.classic.rs_y = map(out.r_stick.y, 0, 255, 255, 0);

            /*check if it wanna change the mode*/
            if (out.l_click & out.r_click) {
                wiimote_report.switch_mode = 1;
            }
                /*wait to change to zero*/
            else {
                if (wiimote_report.switch_mode) {
                    wiimote_report.mode = NO_EXTENSION;
                    wiimote_report.switch_mode = 0;
                    wiimote_report.reset_ir = 1;
                }
            }
    }

    //Serial1.println("--------------------");

    //size_t *i = nthaka_multi_format_handler_get_last_deserialized_index(&fmt);
    //sprintf(str, "index: %s", i == NULL ? "unknown" //
    //                          : *i == 0 ? "NXMC2"
    //                          : *i == 1 ? "ORCA"
    //                          : *i == 2 ? "PokeCon"
    //                                    : "unknown");
    //Serial1.println(str);

    //nthaka_gamepad_state_stringify(&out, str, NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX);
    //Serial1.println(str);
}

void setup()
{
    Serial.setTimeout(100);
    Serial.begin(9600);

    //Serial1.setTX(0);
    //Serial1.setRX(1);
    //Serial1.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    if (!(nxmc2_format_handler_init(&nxmc2) &&
          orca_format_handler_init(&orca) &&
          pokecon_format_handler_init(&pokecon) &&
          nthaka_multi_format_handler_init(&fmt, fmts, 3) &&
          nthaka_buffer_init(&buf, (nthaka_format_handler_t *)&fmt)))
    {
        digitalWrite(LED_BUILTIN, HIGH);
        while (true)
            ;
    }
}

void loop()
{
    uint8_t d;
    nthaka_buffer_state_t s;
    if (Serial.readBytes(&d, 1) != 1 ||
        (s = nthaka_buffer_append(&buf, d, &out)) == NTHAKA_BUFFER_REJECTED)
    {
        nthaka_buffer_clear(&buf);
        return;
    }
    else if (s == NTHAKA_BUFFER_PENDING)
    {
        return;
    }

    update();
    nthaka_buffer_clear(&buf);
}

void setup1()
{
    wiimote_emulator_set_led(led_on, led_off);
    wiimote_emulator(&wiimote_report);
}

void loop1()
{

}
