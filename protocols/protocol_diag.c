#include "protocol_common.h"

char* RxState2Str(RxState_t rx_state) {
    char* name = "?";
    switch(rx_state) {
    case WAIT_PREAMBLE:
        name = "Pre";
        break;
    case WAIT_LEN:
        name = "Len";
        break;
    case WAIT_SERIAL_NUM:
        name = "Sn";
        break;
    case WAIT_RETX_CNT:
        name = "ReTx";
        break;
    case WAIT_PAYLOAD:
        name = "Pay";
        break;
    case WAIT_CRC:
        name = "Crc";
        break;
    case WAIT_UNDEF:
        name = "Und";
        break;
    case RX_DONE:
        name = "Done";
        break;
    case WAIT_INIT:
        name = "Init";
        break;
    default:
        name = "??";
        break;
    }
    return name;
}
