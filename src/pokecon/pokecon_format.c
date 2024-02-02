#include "nthaka/pokecon.h"
#include "../internal.h"

#include <assert.h>
#include <stdio.h>

pokecon_format_state_t pokecon_format_state_next(pokecon_format_state_t s, char c)
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

static const int _VALID_LENGTH_MIN = 4;
static const int _VALID_LENGTH_MAX = 30;

static nthaka_gamepad_state_t _;

static bool _deserialize(nthaka_format_t *parent, uint8_t *buf, size_t size, nthaka_gamepad_state_t *out)
{
    pokecon_format_t *fmt = (pokecon_format_t *)parent;
    if (fmt == NULL || buf == NULL || fmt->_s != POKECON_BUFFER_STATE_FINAL)
    {
        return false;
    }
    assert(_VALID_LENGTH_MIN <= size && size <= _VALID_LENGTH_MAX);

    if (out == NULL)
    {
        out = &_;
    }

    // Copy buf into a new char[]
    char str[size + 1];
    for (size_t i = 0; i < size; i++)
    {
        str[i] = (char)buf[i];
    }
    str[size] = '\0';

    uint16_t btns = 0x0000;
    uint8_t hat = NTHAKA_HAT_NEUTRAL;
    // sscanf doesn't care whether the "0x" prefix is present or not
    sscanf(str, "%hx %hhx", &btns, &hat);

    bool update_rs = nthaka_internal_bit(btns, 0) == 1;
    bool update_ls = nthaka_internal_bit(btns, 1) == 1;
    out->y = nthaka_internal_bit(btns, 2);
    out->b = nthaka_internal_bit(btns, 3);
    out->a = nthaka_internal_bit(btns, 4);
    out->x = nthaka_internal_bit(btns, 5);
    out->l = nthaka_internal_bit(btns, 6);
    out->r = nthaka_internal_bit(btns, 7);
    out->zl = nthaka_internal_bit(btns, 8);
    out->zr = nthaka_internal_bit(btns, 9);
    out->minus = nthaka_internal_bit(btns, 10);
    out->plus = nthaka_internal_bit(btns, 11);
    out->l_click = nthaka_internal_bit(btns, 12);
    out->r_click = nthaka_internal_bit(btns, 13);
    out->home = nthaka_internal_bit(btns, 14);
    out->capture = nthaka_internal_bit(btns, 15);

    switch (hat)
    {
    case 0:
        out->hat = NTHAKA_HAT_UP;
        break;
    case 1:
        out->hat = NTHAKA_HAT_UPRIGHT;
        break;
    case 2:
        out->hat = NTHAKA_HAT_RIGHT;
        break;
    case 3:
        out->hat = NTHAKA_HAT_DOWNRIGHT;
        break;
    case 4:
        out->hat = NTHAKA_HAT_DOWN;
        break;
    case 5:
        out->hat = NTHAKA_HAT_DOWNLEFT;
        break;
    case 6:
        out->hat = NTHAKA_HAT_LEFT;
        break;
    case 7:
        out->hat = NTHAKA_HAT_UPLEFT;
        break;
    case 8:
    default:
        out->hat = NTHAKA_HAT_NEUTRAL;
        break;
    }

    if (update_ls && update_rs)
    {
        uint8_t lx = NTHAKA_STICK_NEUTRAL;
        uint8_t ly = NTHAKA_STICK_NEUTRAL;
        uint8_t rx = NTHAKA_STICK_NEUTRAL;
        uint8_t ry = NTHAKA_STICK_NEUTRAL;
        sscanf(str, "%*hx %*hhx %hhx %hhx %hhx %hhx", &lx, &ly, &rx, &ry);

        out->l_stick.x = lx;
        out->l_stick.y = ly;
        fmt->_prev_l.x = out->l_stick.x;
        fmt->_prev_l.y = out->l_stick.y;

        out->r_stick.x = rx;
        out->r_stick.y = ry;
        fmt->_prev_r.x = out->r_stick.x;
        fmt->_prev_r.y = out->r_stick.y;
    }
    else if (update_ls || update_rs)
    {
        uint8_t x = NTHAKA_STICK_NEUTRAL;
        uint8_t y = NTHAKA_STICK_NEUTRAL;
        sscanf(str, "%*hx %*hhx %hhx %hhx", &x, &y);

        if (update_ls)
        {
            out->l_stick.x = x;
            out->l_stick.y = y;
            fmt->_prev_l.x = out->l_stick.x;
            fmt->_prev_l.y = out->l_stick.y;

            out->r_stick.x = fmt->_prev_r.x;
            out->r_stick.y = fmt->_prev_r.y;
        }
        else
        {
            out->l_stick.x = fmt->_prev_l.x;
            out->l_stick.y = fmt->_prev_l.y;

            out->r_stick.x = x;
            out->r_stick.y = y;
            fmt->_prev_r.x = out->r_stick.x;
            fmt->_prev_r.y = out->r_stick.y;
        }
    }
    else
    {
        out->l_stick.x = fmt->_prev_l.x;
        out->l_stick.y = fmt->_prev_l.y;

        out->r_stick.x = fmt->_prev_r.x;
        out->r_stick.y = fmt->_prev_r.y;
    }

    size_t length = sizeof(out->extension) / sizeof(uint8_t);
    for (size_t i = 0; i < length; i++)
    {
        out->extension[i] = 0;
    }

    return true;
}

static nthaka_buffer_state_t _update(nthaka_format_t *parent, uint8_t d)
{
    pokecon_format_t *fmt = (pokecon_format_t *)parent;
    if (fmt == NULL)
    {
        return NTHAKA_BUFFER_REJECTED;
    }

    fmt->_s = pokecon_format_state_next(fmt->_s, (char)d);
    return fmt->_s == POKECON_BUFFER_STATE_INITIAL ? NTHAKA_BUFFER_REJECTED
                                                   : (fmt->_s == POKECON_BUFFER_STATE_FINAL ? NTHAKA_BUFFER_ACCEPTED
                                                                                            : NTHAKA_BUFFER_PENDING);
}

static void _reset(nthaka_format_t *parent)
{
    pokecon_format_t *fmt = (pokecon_format_t *)parent;
    if (fmt == NULL)
    {
        return;
    }

    fmt->_s = POKECON_BUFFER_STATE_INITIAL;
}

bool pokecon_format_init(pokecon_format_t *fmt)
{
    if (fmt == NULL)
    {
        return false;
    }

    fmt->parent.deserialize = _deserialize;
    fmt->parent.reset = _reset;
    fmt->parent.update = _update;

    fmt->_s = POKECON_BUFFER_STATE_INITIAL;

    fmt->_prev_l.x = NTHAKA_STICK_NEUTRAL;
    fmt->_prev_l.y = NTHAKA_STICK_NEUTRAL;
    fmt->_prev_r.x = NTHAKA_STICK_NEUTRAL;
    fmt->_prev_r.y = NTHAKA_STICK_NEUTRAL;

    return true;
}