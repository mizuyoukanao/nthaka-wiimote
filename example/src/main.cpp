#include <Arduino.h>

#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "nxamf.h"
#include "nxamf/pokecon.h"
#include "nxamf/nxmc2.h"

static const int SERIAL_INACTIVE_TIMEOUT = 100;
static int inactive_count = 0;

static PokeConProtocol *pokecon_protocol;
static NxamfBytesBuffer *pokecon;

static Nxmc2Protocol *nxmc2_protocol;
static NxamfBytesBuffer *nxmc2;

static bool is_pokecon = false;

static int64_t led_off(alarm_id_t id, void *user_data)
{
    digitalWrite(LED_BUILTIN, LOW);
    return false;
}

static void async_led_on_for_100ms()
{
    digitalWrite(LED_BUILTIN, HIGH);
    alarm_id_t alarm_id = add_alarm_in_ms(100, led_off, NULL, false);
}

static NxamfGamepadState *append_both(uint8_t packet)
{
    NxamfGamepadState *p = nxamf_bytes_buffer_append(pokecon, packet);
    NxamfGamepadState *n = nxamf_bytes_buffer_append(nxmc2, packet);

    if (p != NULL)
    {
        is_pokecon = true;
        return p;
    }
    else if (n != NULL)
    {
        is_pokecon = false;
        return n;
    }
    else
    {
        is_pokecon = false;
        return NULL;
    }
}

static void clear_both()
{
    nxamf_bytes_buffer_clear(pokecon);
    nxamf_bytes_buffer_clear(nxmc2);
}

static char buf[256];

static void print_button_state(const char *name, NxamfButtonState state)
{
    sprintf(buf, "%s\t%s", name, state == NXAMF_BUTTON_STATE_PRESSED ? "Pressed" : "Released");
    Serial1.println(buf);
}

static void print_hat_state(NxamfHatState state)
{
    const char *tmp;
    switch (state)
    {
    case NXAMF_HAT_STATE_UP:
        tmp = "Up";
        break;
    case NXAMF_HAT_STATE_UPRIGHT:
        tmp = "UpRight";
        break;
    case NXAMF_HAT_STATE_RIGHT:
        tmp = "Right";
        break;
    case NXAMF_HAT_STATE_DOWNRIGHT:
        tmp = "DownRight";
        break;
    case NXAMF_HAT_STATE_DOWN:
        tmp = "Down";
        break;
    case NXAMF_HAT_STATE_DOWNLEFT:
        tmp = "DownLeft";
        break;
    case NXAMF_HAT_STATE_LEFT:
        tmp = "Left";
        break;
    case NXAMF_HAT_STATE_UPLEFT:
        tmp = "UpLeft";
        break;
    case NXAMF_HAT_STATE_NEUTRAL:
        tmp = "Neurtal";
        break;
    default:
        tmp = "[Error]";
        break;
    }
    sprintf(buf, "Hat\t%s", tmp);
    Serial1.println(buf);
}

static void print_stick_state(const char *name, NxamfStickState *state)
{
    sprintf(buf, "%s\tx:%d,y:%d", name, state->x, state->y);
    Serial1.println(buf);
}

static void print_extension(uint8_t extension[])
{
    sprintf(buf, "Extension\t%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", extension[0], extension[1], extension[2], extension[3], extension[4], extension[5], extension[6], extension[7], extension[8], extension[9], extension[10], extension[11], extension[12], extension[13], extension[14], extension[15]);
    Serial1.println(buf);
}

static void reflect_state(NxamfGamepadState *state)
{
    if (state == NULL)
    {
        return;
    }

    async_led_on_for_100ms();

    Serial1.println("--------------------");
    sprintf(buf, "Mode\t%s", is_pokecon ? "PokeCon" : "NXMC2");
    Serial1.println(buf);

    print_button_state("Y", state->y);
    print_button_state("B", state->b);
    print_button_state("A", state->a);
    print_button_state("X", state->x);
    print_button_state("L", state->l);
    print_button_state("R", state->r);
    print_button_state("ZL", state->zl);
    print_button_state("ZR", state->zr);
    print_button_state("Minus", state->minus);
    print_button_state("Plus", state->plus);
    print_button_state("L Click", state->l_click);
    print_button_state("R Click", state->r_click);
    print_button_state("Home", state->home);
    print_button_state("Capture", state->capture);

    print_hat_state(state->hat);

    print_stick_state("L Stick", &state->l_stick);
    print_stick_state("R Stick", &state->r_stick);

    print_extension(state->extension);
}

void setup()
{
    Serial.begin(9600);

    Serial1.setTX(0);
    Serial1.setRX(1);
    Serial1.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pokecon_protocol = pokecon_protocol_new();
    if (pokecon_protocol == NULL)
    {
        abort();
    }
    pokecon = nxamf_bytes_buffer_new((NxamfBytesProtocolInterface *)pokecon_protocol);
    if (pokecon == NULL)
    {
        abort();
    }

    nxmc2_protocol = nxmc2_protocol_new();
    if (nxmc2_protocol == NULL)
    {
        abort();
    }
    nxmc2 = nxamf_bytes_buffer_new((NxamfBytesProtocolInterface *)nxmc2_protocol);
    if (nxmc2 == NULL)
    {
        abort();
    }
}

void loop()
{
    if (Serial.available() == 0)
    {
        inactive_count++;
        if (SERIAL_INACTIVE_TIMEOUT < inactive_count)
        {
            inactive_count = 0;
            clear_both();
        }
        return;
    }
    inactive_count = 0;

    uint8_t packet = Serial.read();
    NxamfGamepadState *state = append_both(packet);
    if (state == NULL)
    {
        return;
    }

    reflect_state(state);
}