#include <Arduino.h>

#include "hardware/timer.h"

#include "nthaka.h"
#include "nthaka/nxmc2.h"
#include "nthaka/orca.h"
#include "nthaka/pokecon.h"

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

static void update()
{
    async_led_on(100);

    Serial1.println("--------------------");

    size_t *i = nthaka_multi_format_handler_get_last_deserialized_index(&fmt);
    sprintf(str, "index: %s", i == NULL ? "unknown" //
                              : *i == 0 ? "NXMC2"
                              : *i == 1 ? "ORCA"
                              : *i == 2 ? "PokeCon"
                                        : "unknown");
    Serial1.println(str);

    nthaka_gamepad_state_stringify(&out, str, NTHAKA_GAMEPAD_STATE_STRING_SIZE_MAX);
    Serial1.println(str);
}

void setup()
{
    Serial.setTimeout(100);
    Serial.begin(9600);

    Serial1.setTX(0);
    Serial1.setRX(1);
    Serial1.begin(115200);

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
    if (Serial.readBytes(&d, 1) != 1)
    {
        nthaka_buffer_clear(&buf);
        return;
    }

    switch (nthaka_buffer_append(&buf, d, &out))
    {
    case NTHAKA_BUFFER_ACCEPTED:
        break;
    case NTHAKA_BUFFER_REJECTED:
        nthaka_buffer_clear(&buf);
    case NTHAKA_BUFFER_PENDING:
    default:
        return;
    }

    update();
    
    nthaka_buffer_clear(&buf);
}