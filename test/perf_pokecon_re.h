#ifndef TEST_PERF_POKECON_RE_H_
#define TEST_PERF_POKECON_RE_H_

#include "nxamf/pokecon.h"

#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <time.h>

#include "re.h"

static const size_t POKECON_PROTOCOL_ALLOWED_LENGTH = 18;
static const char *POKECON_PROTOCOL_ALLOWED[] = {
    // 使えなさそうなもの
    // - |
    // - ()

    // ボタンとスティックは2通りのフォーマットがあるらしい
    // 0-ffff（画面クリックでスティック操作時）
    // 0x0000-0xffff（その他）
    // 少し不正確だけど0?x?と、0xの後に続かないのを許容するパターンでお茶を濁す

    "^0x$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[0-9a-f] ?$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[0-9a-f] [0-8]$",

    // 下位2bitは0b00で確定
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[048c] [0-8]\r$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[048c] [0-8]\r?\n$",

    // 下位2bitのどちらかが0b1
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] $",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] 0x$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] 0?x?[1-9a-f]?[0-9a-f] ?$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0x$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f]$",

    // 下位2bitは0b10で確定 [26ae]
    // => しない。"Use LStick Mouse"、"Use RStick Mouse"を有効にすると、キャプチャ画面をクリックした際のbtnsの下位2ビットが、どちらかしか操作していない場合でも0b11になる。
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f]\r$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[1235679abdef] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f]\r?\n$",

    // 最下位bitが0b1
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[13579bdf] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] $",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[13579bdf] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0x$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[13579bdf] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] ?$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[13579bdf] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0x$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[13579bdf] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f]\r?$",
    "^0?x?[0-9a-f]?[0-9a-f]?[0-9a-f]?[13579bdf] [0-8] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f] 0?x?[1-9a-f]?[0-9a-f]\r?\n$"};

static void _append_re(nxamf_buffer_interface_t *parent, uint8_t d)
{
    pokecon_buffer_t *buf = (pokecon_buffer_t *)parent;
    assert(buf != NULL);

    buf->cached = false;
    buf->s = POKECON_BUFFER_STATE_INITIAL;

    char combined[buf->len + 2]; // buffer + d + '\0'
    memcpy(combined, buf->buf, buf->len);
    combined[buf->len] = (char)d;
    combined[buf->len + 1] = '\0';

    bool is_acceptable = false;
    int _;
    for (int i = 0; i < POKECON_PROTOCOL_ALLOWED_LENGTH; i++)
    {
        if (0 <= re_match(POKECON_PROTOCOL_ALLOWED[i], combined, &_))
        {
            is_acceptable = true;
            break;
        }
    }
    if (!is_acceptable)
    {
        // Rejected
        if (buf->len != 0)
        {
            parent->clear(parent);
            parent->append(parent, d);
        }
        return;
    }

    buf->buf[buf->len] = d;
    buf->len++;

    if (buf->buf[buf->len - 1] == '\n')
    {
        buf->s = POKECON_BUFFER_STATE_FINAL;
        parent->deserialize(parent, &buf->cache);
        buf->cached = true;
    }
}

void pokecon_buffer_re_init(pokecon_buffer_t *buf)
{
    pokecon_buffer_init(buf);
    buf->parent.append = _append_re;
}

static double _perf(int iter, nxamf_buffer_interface_t *impl, char seq[], size_t len, NxamfGamepadState *expected)
{
    NxamfGamepadState actual;
    clock_t start;
    clock_t end;

    start = clock();
    for (int i = 0; i < iter; i++)
    {
        for (size_t j = 0; j < len; j++)
        {
            impl->append(impl, (uint8_t)seq[j]);
        }
        assert(impl->deserialize(impl, &actual));
        assert(nxamf_gamepad_state_equals(expected, &actual));
        impl->clear(impl);
    }
    end = clock();

    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

bool test_perf_pokecon_re(void)
{
    const int ITERATIONS = 100000;

    pokecon_buffer_t sm_;
    pokecon_buffer_init(&sm_);
    nxamf_buffer_interface_t *sm = (nxamf_buffer_interface_t *)&sm_;

    pokecon_buffer_t re_;
    pokecon_buffer_re_init(&re_);
    nxamf_buffer_interface_t *re = (nxamf_buffer_interface_t *)&re_;

    char seq[] = "0x0000 8\r\n";
    size_t len = sizeof(seq) / sizeof(char) - 1;
    NxamfGamepadState expected = {.y = NXAMF_BUTTON_RELEASED,
                                  .b = NXAMF_BUTTON_RELEASED,
                                  .a = NXAMF_BUTTON_RELEASED,
                                  .x = NXAMF_BUTTON_RELEASED,
                                  .l = NXAMF_BUTTON_RELEASED,
                                  .r = NXAMF_BUTTON_RELEASED,
                                  .zl = NXAMF_BUTTON_RELEASED,
                                  .zr = NXAMF_BUTTON_RELEASED,
                                  .minus = NXAMF_BUTTON_RELEASED,
                                  .plus = NXAMF_BUTTON_RELEASED,
                                  .l_click = NXAMF_BUTTON_RELEASED,
                                  .r_click = NXAMF_BUTTON_RELEASED,
                                  .home = NXAMF_BUTTON_RELEASED,
                                  .capture = NXAMF_BUTTON_RELEASED,
                                  .hat = NXAMF_HAT_NEUTRAL,
                                  .l_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL},
                                  .r_stick = {.x = NXAMF_STICK_NEUTRAL, .y = NXAMF_STICK_NEUTRAL},
                                  .extension = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    printf("--------------------\n");
    printf("state machine: %fs\n", _perf(ITERATIONS, sm, seq, len, &expected));
    printf("regex: %fs\n", _perf(ITERATIONS, re, seq, len, &expected));
    printf("--------------------\n");

    return true;
}

#endif // TEST_PERF_POKECON_RE_H_