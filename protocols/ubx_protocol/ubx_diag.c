#include "ubx_diag.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "debug_info.h"
#include "io_utils.h"
#include "ubx_protocol.h"
#include "ubx_constants.h"

char* ReceiverMode2Str(uint8_t mode){
    char* name = "?";
    switch(mode){
        case RX_MODE_DISABLED: name = "Dis";break;
        case RX_MODE_SURVEY_IN: name = "SurveyIn";break;
        case RX_MODE_FIXED: name = "Fixed"; break;
        default : break;
    }
    return name;
}

char* FixType2Str(uint8_t mode){
    char* name = "?";
    switch(mode){
        case  FIX_TYPE_NO_FIX: name = "No";break;
        case  FIX_TYPE_DEAD_RECKONING_ONLY: name = "DeadRecOnly";break;
        case  FIX_TYPE_2D_FIX: name = "2D"; break;
        case  FIX_TYPE_3D_FIX: name = "3D"; break;
        case  FIX_TYPE_GNSS_DEAD_RECKONING_COMBINED: name = "GnssDrComb"; break;
        case  FIX_TYPE_TIME_ONLY_FIX: name = "TIME"; break;
        default : name = "??"; break;
    }
    return name;
}

char* class2str(uint8_t class_id) {
    char* name = "?";
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
#ifdef HAS_LOG
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
#endif
