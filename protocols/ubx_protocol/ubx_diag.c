#include "ubx_diag.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "debug_info.h"
#include "io_utils.h"
#include "ubx_protocol.h"

char* class2str(uint8_t class_id) {
    char* name = "undef";
    switch(class_id) {
    case UBX_CLA_NAV:
        name = "NAV";
        break;
    case UBX_CLA_RXM:
        name = "RXM";
        break;
    case UBX_CLA_INF:
        name = "INF";
        break;
    case UBX_CLA_ACK:
        name = "ACK";
        break;
    case UBX_CLA_CFG:
        name = "CFG";
        break;
    case UBX_CLA_UPD:
        name = "UPD";
        break;
    case UBX_CLA_MON:
        name = "MON";
        break;
    case UBX_CLA_TIM:
        name = "TIM";
        break;
    case UBX_CLA_ESF:
        name = "ESF";
        break;
    case UBX_CLA_MGA:
        name = "MGA";
        break;
    case UBX_CLA_SEC:
        name = "SEC";
        break;
    }
    return name;
}

bool ubx_print_frame(uint8_t* frame) {
    bool res = false;
    io_printf(CRLF "%s ", class2str(frame[UBX_INDEX_CLS]));
    io_printf("0x%02x " CRLF, frame[UBX_INDEX_ID]);
    uint16_t* len = NULL;
    len = (uint16_t*)&frame[UBX_INDEX_LEN];
    print_mem(&frame[UBX_INDEX_PAYLOAD], *len, true, false, false, true);
    io_printf(CRLF);
    return res;
}
