#include "nxamf/pokecon.h"

#include <assert.h>
#include <stdio.h>

static pokecon_buffer_state_t _next(pokecon_buffer_state_t s, char c)
{
    switch (s)
    {
    case POKECON_BUFFER_STATE_INITIAL:
        switch (c)
        {
        case '0':
            return POKECON_BUFFER_STATE_0;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_1;
        case '3':
            return POKECON_BUFFER_STATE_3;
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_4;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_7;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_0:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_0_;
        case 'x':
            return POKECON_BUFFER_STATE_0x;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_1_;
        case '0':
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_10;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_11;
        case '3':
            return POKECON_BUFFER_STATE_13;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_17;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_10:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_0_;
        case '0':
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_100;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_101;
        case '3':
            return POKECON_BUFFER_STATE_103;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_107;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_100:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_0_;
        case '0':
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_1000;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_1001;
        case '3':
            return POKECON_BUFFER_STATE_1003;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_1007;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1000:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_0_;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1001:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_1_;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1003:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_3_;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1007:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_7_;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_101:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_1_;
        case '0':
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_1000;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_1001;
        case '3':
            return POKECON_BUFFER_STATE_1003;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_1007;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_103:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_3_;
        case '0':
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_1000;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_1001;
        case '3':
            return POKECON_BUFFER_STATE_1003;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_1007;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_107:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_7_;
        case '0':
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_1000;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_1001;
        case '3':
            return POKECON_BUFFER_STATE_1003;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_1007;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_11:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_1_;
        case '0':
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_100;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_101;
        case '3':
            return POKECON_BUFFER_STATE_103;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_107;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_13:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_3_;
        case '0':
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_100;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_101;
        case '3':
            return POKECON_BUFFER_STATE_103;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_107;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_17:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_7_;
        case '0':
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_100;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_101;
        case '3':
            return POKECON_BUFFER_STATE_103;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_107;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_3:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_3_;
        case '0':
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_10;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_11;
        case '3':
            return POKECON_BUFFER_STATE_13;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_17;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_4:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_0_;
        case '0':
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_10;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_11;
        case '3':
            return POKECON_BUFFER_STATE_13;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_17;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_7:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_7_;
        case '0':
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_10;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_11;
        case '3':
            return POKECON_BUFFER_STATE_13;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_17;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_0x:
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_0x0;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_0x0:
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_0x00;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_0x00:
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_0x000;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_0x000:
        switch (c)
        {
        case '0':
        case '4':
        case '8':
        case 'c':
            return POKECON_BUFFER_STATE_1000;
        case '1':
        case '2':
        case '5':
        case '6':
        case '9':
        case 'a':
        case 'd':
        case 'e':
            return POKECON_BUFFER_STATE_1001;
        case '3':
            return POKECON_BUFFER_STATE_1003;
        case '7':
        case 'b':
        case 'f':
            return POKECON_BUFFER_STATE_1007;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_0_:
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            return POKECON_BUFFER_STATE_0_0;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_0_0:
        switch (c)
        {
        case '\r':
            return POKECON_BUFFER_STATE_0_0CR;
        case '\n':
            return POKECON_BUFFER_STATE_FINAL;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1_:
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            return POKECON_BUFFER_STATE_1_0;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1_0:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_1_0_;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_3_:
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            return POKECON_BUFFER_STATE_3_0;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_3_0:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_3_0_;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_7_:
        switch (c)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            return POKECON_BUFFER_STATE_7_0;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_7_0:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_7_0_;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1_0_:
        switch (c)
        {
        case '0':
            return POKECON_BUFFER_STATE_1_0_0;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_1_0_1;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1_0_0:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_1_0_0_;
        case 'x':
            return POKECON_BUFFER_STATE_1_0_0x;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1_0_1:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_1_0_0_;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_1_0_10;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1_0_10:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_1_0_0_;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1_0_0x:
        switch (c)
        {
        case '0':
            return POKECON_BUFFER_STATE_1_0_10;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_1_0_1;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1_0_0_:
        switch (c)
        {
        case '0':
            return POKECON_BUFFER_STATE_1_0_0_0;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_1_0_0_1;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1_0_0_0:
        switch (c)
        {
        case '\r':
            return POKECON_BUFFER_STATE_0_0CR;
        case '\n':
            return POKECON_BUFFER_STATE_FINAL;
        case 'x':
            return POKECON_BUFFER_STATE_1_0_0_0x;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1_0_0_1:
        switch (c)
        {
        case '\r':
            return POKECON_BUFFER_STATE_0_0CR;
        case '\n':
            return POKECON_BUFFER_STATE_FINAL;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_1_0_0_10;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1_0_0_10:
        switch (c)
        {
        case '\r':
            return POKECON_BUFFER_STATE_0_0CR;
        case '\n':
            return POKECON_BUFFER_STATE_FINAL;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_1_0_0_0x:
        switch (c)
        {
        case '0':
            return POKECON_BUFFER_STATE_1_0_0_10;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_1_0_0_1;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_7_0_:
        switch (c)
        {
        case '0':
            return POKECON_BUFFER_STATE_7_0_0;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_7_0_1;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_7_0_0:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_7_0_0_;
        case 'x':
            return POKECON_BUFFER_STATE_7_0_0x;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_7_0_1:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_7_0_0_;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_7_0_10;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_7_0_10:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_7_0_0_;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_7_0_0x:
        switch (c)
        {
        case '0':
            return POKECON_BUFFER_STATE_7_0_10;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_7_0_1;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_7_0_0_:
        switch (c)
        {
        case '0':
            return POKECON_BUFFER_STATE_7_0_0_0;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_7_0_0_1;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_7_0_0_0:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_1_0_;
        case 'x':
            return POKECON_BUFFER_STATE_7_0_0_0x;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_7_0_0_1:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_1_0_;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_7_0_0_10;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_7_0_0_10:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_1_0_;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_7_0_0_0x:
        switch (c)
        {
        case '0':
            return POKECON_BUFFER_STATE_7_0_0_10;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_7_0_0_1;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_3_0_:
        switch (c)
        {
        case '0':
            return POKECON_BUFFER_STATE_3_0_0;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_3_0_1;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_3_0_0:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_3_0_0_;
        case 'x':
            return POKECON_BUFFER_STATE_3_0_0x;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_3_0_1:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_3_0_0_;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_3_0_10;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_3_0_10:
        switch (c)
        {
        case ' ':
            return POKECON_BUFFER_STATE_3_0_0_;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_3_0_0x:
        switch (c)
        {
        case '0':
            return POKECON_BUFFER_STATE_3_0_10;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_3_0_1;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_3_0_0_:
        switch (c)
        {
        case '0':
            return POKECON_BUFFER_STATE_3_0_0_0;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_3_0_0_1;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_3_0_0_0:
        switch (c)
        {
        case '\r':
            return POKECON_BUFFER_STATE_0_0CR;
        case '\n':
            return POKECON_BUFFER_STATE_FINAL;
        case ' ':
            return POKECON_BUFFER_STATE_1_0_;
        case 'x':
            return POKECON_BUFFER_STATE_3_0_0_0x;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_3_0_0_1:
        switch (c)
        {
        case '\r':
            return POKECON_BUFFER_STATE_0_0CR;
        case '\n':
            return POKECON_BUFFER_STATE_FINAL;
        case ' ':
            return POKECON_BUFFER_STATE_1_0_;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_3_0_0_10;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_3_0_0_10:
        switch (c)
        {
        case '\r':
            return POKECON_BUFFER_STATE_0_0CR;
        case '\n':
            return POKECON_BUFFER_STATE_FINAL;
        case ' ':
            return POKECON_BUFFER_STATE_1_0_;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_3_0_0_0x:
        switch (c)
        {
        case '0':
            return POKECON_BUFFER_STATE_3_0_0_10;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return POKECON_BUFFER_STATE_3_0_0_1;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_0_0CR:
        switch (c)
        {
        case '\n':
            return POKECON_BUFFER_STATE_FINAL;
        default:
            return POKECON_BUFFER_STATE_INITIAL;
        }
        break;
    case POKECON_BUFFER_STATE_FINAL:
    default:
        return POKECON_BUFFER_STATE_INITIAL;
    }
}

static void _clear(nxamf_buffer_interface_t *parent)
{
    pokecon_buffer_t *buf = (pokecon_buffer_t *)parent;
    assert(buf != NULL);

    buf->len = 0;
    buf->s = POKECON_BUFFER_STATE_INITIAL;
}

static const int _MIN_VALID_LENGTH = 4;
static const int _MAX_VALID_LENGTH = 30;

static bool _deserialize(nxamf_buffer_interface_t *parent, nxamf_gamepad_state_t *out)
{
    pokecon_buffer_t *buf = (pokecon_buffer_t *)parent;
    assert(buf != NULL);
    assert(out != NULL);

    if (buf->s != POKECON_BUFFER_STATE_FINAL)
    {
        return false;
    }
    assert(_MIN_VALID_LENGTH <= buf->len && buf->len <= _MAX_VALID_LENGTH);

    if (buf->cached)
    {
        nxamf_gamepad_state_copy(&buf->cache, out);
        return true;
    }

    // Copy buf into a new char[]
    char str[buf->len + 1];
    for (size_t i = 0; i < buf->len; i++)
    {
        str[i] = (char)buf->buf[i];
    }
    str[buf->len] = '\0';

    uint16_t btns = 0b0000000000000000U;
    uint8_t hat = NXAMF_HAT_NEUTRAL;
    // sscanf doesn't care whether the "0x" prefix is present or not
    sscanf(str, "%hx %hhx", &btns, &hat);

    out->y = (btns & 0b0000000000000100U) >> 2;
    out->b = (btns & 0b0000000000001000U) >> 3;
    out->a = (btns & 0b0000000000010000U) >> 4;
    out->x = (btns & 0b0000000000100000U) >> 5;
    out->l = (btns & 0b0000000001000000U) >> 6;
    out->r = (btns & 0b0000000010000000U) >> 7;
    out->zl = (btns & 0b0000000100000000U) >> 8;
    out->zr = (btns & 0b0000001000000000U) >> 9;
    out->minus = (btns & 0b0000010000000000U) >> 10;
    out->plus = (btns & 0b0000100000000000U) >> 11;
    out->l_click = (btns & 0b0001000000000000U) >> 12;
    out->r_click = (btns & 0b0010000000000000U) >> 13;
    out->home = (btns & 0b0100000000000000U) >> 14;
    out->capture = (btns & 0b1000000000000000U) >> 15;

    switch (hat)
    {
    case 0:
        out->hat = NXAMF_HAT_UP;
        break;
    case 1:
        out->hat = NXAMF_HAT_UPRIGHT;
        break;
    case 2:
        out->hat = NXAMF_HAT_RIGHT;
        break;
    case 3:
        out->hat = NXAMF_HAT_DOWNRIGHT;
        break;
    case 4:
        out->hat = NXAMF_HAT_DOWN;
        break;
    case 5:
        out->hat = NXAMF_HAT_DOWNLEFT;
        break;
    case 6:
        out->hat = NXAMF_HAT_LEFT;
        break;
    case 7:
        out->hat = NXAMF_HAT_UPLEFT;
        break;
    case 8:
    default:
        out->hat = NXAMF_HAT_NEUTRAL;
        break;
    }

    bool update_ls = ((btns & 0b0000000000000010U) >> 1) == 1;
    bool update_rs = (btns & 0b0000000000000001U) == 1;

    if (update_ls && update_rs)
    {
        uint8_t lx = NXAMF_STICK_NEUTRAL;
        uint8_t ly = NXAMF_STICK_NEUTRAL;
        uint8_t rx = NXAMF_STICK_NEUTRAL;
        uint8_t ry = NXAMF_STICK_NEUTRAL;
        sscanf(str, "%*hx %*hhx %hhx %hhx %hhx %hhx", &lx, &ly, &rx, &ry);

        out->l_stick.x = lx;
        out->l_stick.y = ly;
        buf->prev_l.x = out->l_stick.x;
        buf->prev_l.y = out->l_stick.y;

        out->r_stick.x = rx;
        out->r_stick.y = ry;
        buf->prev_r.x = out->r_stick.x;
        buf->prev_r.y = out->r_stick.y;
    }
    else if (update_ls || update_rs)
    {
        uint8_t x = NXAMF_STICK_NEUTRAL;
        uint8_t y = NXAMF_STICK_NEUTRAL;
        sscanf(str, "%*hx %*hhx %hhx %hhx", &x, &y);

        if (update_ls)
        {
            out->l_stick.x = x;
            out->l_stick.y = y;
            buf->prev_l.x = out->l_stick.x;
            buf->prev_l.y = out->l_stick.y;

            out->r_stick.x = buf->prev_r.x;
            out->r_stick.y = buf->prev_r.y;
        }
        else
        {
            out->l_stick.x = buf->prev_l.x;
            out->l_stick.y = buf->prev_l.y;

            out->r_stick.x = x;
            out->r_stick.y = y;
            buf->prev_r.x = out->r_stick.x;
            buf->prev_r.y = out->r_stick.y;
        }
    }
    else
    {
        out->l_stick.x = buf->prev_l.x;
        out->l_stick.y = buf->prev_l.y;

        out->r_stick.x = buf->prev_r.x;
        out->r_stick.y = buf->prev_r.y;
    }

    size_t length = sizeof(out->extension) / sizeof(uint8_t);
    for (size_t i = 0; i < length; i++)
    {
        out->extension[i] = 0;
    }

    return true;
}

static void _append(nxamf_buffer_interface_t *parent, uint8_t d)
{
    pokecon_buffer_t *buf = (pokecon_buffer_t *)parent;
    assert(buf != NULL);

    buf->cached = false;

    buf->s = _next(buf->s, (char)d);
    if (buf->s == POKECON_BUFFER_STATE_INITIAL)
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

    // 例："0x0002 8 81 81\r\n0x0000 8\r\n"
    // 使わなくてもデシリアライズして、0x81をprev_l.x, prev_l.yに入れる必要がある。
    if (buf->s == POKECON_BUFFER_STATE_FINAL)
    {
        parent->deserialize(parent, &buf->cache);
        buf->cached = true;
    }
}

void pokecon_buffer_init(pokecon_buffer_t *buf)
{
    assert(buf != NULL);

    buf->parent.append = _append;
    buf->parent.deserialize = _deserialize;
    buf->parent.clear = _clear;

    buf->len = 0;
    buf->s = POKECON_BUFFER_STATE_INITIAL;

    buf->prev_l.x = NXAMF_STICK_NEUTRAL;
    buf->prev_l.y = NXAMF_STICK_NEUTRAL;
    buf->prev_r.x = NXAMF_STICK_NEUTRAL;
    buf->prev_r.y = NXAMF_STICK_NEUTRAL;

    buf->cached = false;
}