#include <Arduino.h>

#include "hardware/timer.h"

#include "nxamf.h"
#include "nxamf/nxmc2.h"
#include "nxamf/orca.h"
#include "nxamf/pokecon.h"

static nxmc2_buffer_t nxmc2;
static orca_buffer_t orca;
static pokecon_buffer_t pokecon;
static nxamf_buffer_interface_t *bufs[] = {(nxamf_buffer_interface_t *)&nxmc2,
                                           (nxamf_buffer_interface_t *)&orca,
                                           (nxamf_buffer_interface_t *)&pokecon};
static nxamf_multi_buffer_manager_t mgr;
static nxamf_buffer_interface_t *buf = (nxamf_buffer_interface_t *)&mgr;
static nxamf_gamepad_state_t out;

static char _[NXAMF_GAMEPAD_STATE_STRING_LENGTH_MAX];

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

static void reflect()
{
    async_led_on(100);

    Serial1.println("--------------------");

    size_t i = nxamf_multi_buffer_manager_get_last_deserialized_index(&mgr);
    sprintf(_, "index: %s", i == 0   ? "NXMC2" //
                            : i == 1 ? "ORCA"
                            : i == 2 ? "PokeCon"
                                     : "UNKNOWN");
    Serial1.println(_);

    nxamf_gamepad_state_stringify(&out, _, NXAMF_GAMEPAD_STATE_STRING_LENGTH_MAX);
    Serial1.println(_);
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

    nxmc2_buffer_init(&nxmc2);
    orca_buffer_init(&orca);
    pokecon_buffer_init(&pokecon);
    nxamf_multi_buffer_manager_init(&mgr, bufs, 3);
}

void loop()
{
    uint8_t d;
    if (Serial.readBytes(&d, 1) != 1)
    {
        buf->clear(buf);
        return;
    }

    buf->append(buf, d);
    if (!buf->deserialize(buf, &out))
    {
        return;
    }

    reflect();
}