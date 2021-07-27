#include "ublox_driver.h"

#include <stdbool.h>
#include <string.h>

#include "uart_drv.h"
#include "ubx_protocol.h"

bool ubx_send_message(uint8_t class_num, uint8_t id, uint8_t* payload, uint16_t len) {
    bool res = true;
    static uint8_t tx_array[256] = {0};
    uint16_t tx_array_len = 0U;
    uint16_t crc16 = 0U;
    tx_array[0] = UBX_SYN_0;
    tx_array[1] = UBX_SYN_1;
    tx_array[2] = class_num;
    tx_array[3] = id;
    memcpy(&tx_array[4], &len, 2);
    if((0 < len) && (NULL != payload)) {
        memcpy(&tx_array[6], payload, len);
    }
    tx_array_len = len + UBX_HEADER_SIZE + UBX_CRC_SIZE;
    crc16 = ubx_calc_crc16(&tx_array[2], len + 4);
    memcpy(&tx_array[UBX_HEADER_SIZE + len], &crc16, UBX_CRC_SIZE);
    if(true == res) {
        uart_send(1, tx_array, tx_array_len);
    }
    return res;
}

static bool ubx_proc_cfg_frame(void) {
    bool res = false;
    return res;
}

static bool ubx_proc_ack_frame(void) {
    bool res = false;
    switch (UbloxPorotocol.fix_frame[UBX_INDEX_ID]){
    case UBX_ACK_ACK:
        UbloxPorotocol.ack_cnt++;
        res = true;
        break;
    case UBX_ACK_NAK: break;
    default: break;
    }
    return res;
}


bool ubx_proc_frame(void) {
    bool res = false;
    if(true == UbloxPorotocol.unproc_frame) {
        uint8_t in_class=UbloxPorotocol.fix_frame[UBX_INDEX_CLS];
#ifdef HAS_UBX_DIAG
        ubx_print_frame(UbloxPorotocol.fix_frame);
#endif /*HAS_UBX_DIAG*/
        ubx_update_stat(in_class);
        switch(in_class) {
        case UBX_CLA_NAV:
            break;
        case UBX_CLA_RXM:
            break;
        case UBX_CLA_INF:
            break;
        case UBX_CLA_ACK:
            res = ubx_proc_ack_frame();
            break;
        case UBX_CLA_CFG:
            res = ubx_proc_cfg_frame();
            break;
        case UBX_CLA_UPD:
            break;
        case UBX_CLA_MON:
            break;
        case UBX_CLA_TIM:
            break;
        case UBX_CLA_ESF:
            break;
        case UBX_CLA_MGA:
            break;
        case UBX_CLA_SEC:
            break;
        default:
            break;
        }
        memset(UbloxPorotocol.fix_frame, 0x00, UBX_RX_FRAME_SIZE);
        UbloxPorotocol.unproc_frame = false;
        ubx_reset_rx();
    }
    return res;
}
