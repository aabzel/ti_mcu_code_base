#include "ublox_driver.h"

#include <stdbool.h>
#include <string.h>

#include "io_utils.h"
#include "uart_drv.h"
#include "ubx_protocol.h"

#ifndef HAS_UART
#error "Ublox driver requires UART driver"
#endif

keyValItem_t keyValTable[UBX_KEY_CNT] = {{0x30210001, 0x00, UBX_U2}};

xNavInfo_t NavInfo;

bool ubx_driver_init(void) {
    memset(&NavInfo, 0x00, sizeof(NavInfo));
    return true;
}

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

#define UBX_KEY_SIZE 4
static bool ubx_config_data_parse(uint8_t* payload) {
    uint32_t read_key;
    uint16_t i = 0;
    bool res = false;
    memcpy(&read_key, payload, UBX_KEY_SIZE);
    for(i = 0; i < UBX_KEY_CNT; i++) {
        if(read_key == keyValTable[i].key_id) {
            if(UBX_U2 == keyValTable[i].type) {
                memcpy(&keyValTable[i].u_value.u16, &payload[4], 2);
                res = true;
            }
        }
    }
    return res;
}

static bool ubx_cfg_valget_parse(uint8_t* payload) {
    bool res = false;
    if(0x01 == payload[0]) {
        res = ubx_config_data_parse(&payload[4]);
    }
    return res;
}

static bool ubx_proc_cfg_frame(void) {
    bool res = false;
    switch(UbloxPorotocol.fix_frame[UBX_INDEX_ID]) {
    case 0x8b:
        res = ubx_cfg_valget_parse(&UbloxPorotocol.fix_frame[UBX_INDEX_PAYLOAD]);
        break;
    }
    return res;
}

static bool ubx_proc_nav_att_frame(uint8_t* frame) {
    bool res = false;
    if(0x00 == frame[UBX_INDEX_PAYLOAD + 4]) {
        memcpy(&NavInfo.roll, &frame[UBX_INDEX_PAYLOAD + 8], 4);
        memcpy(&NavInfo.pitch, &frame[UBX_INDEX_PAYLOAD + 12], 4);
        memcpy(&NavInfo.heading, &frame[UBX_INDEX_PAYLOAD + 16], 4);

        memcpy(&NavInfo.acc_roll, &frame[UBX_INDEX_PAYLOAD + 20], 4);
        memcpy(&NavInfo.acc_pitch, &frame[UBX_INDEX_PAYLOAD + 24], 4);
        memcpy(&NavInfo.acc_heading, &frame[UBX_INDEX_PAYLOAD + 28], 4);
        res = true;
    }
    return res;
}

static bool ubx_proc_nav_hpposllh_frame(uint8_t* frame) {
    bool res = false;
    if(0x00 == frame[UBX_INDEX_PAYLOAD]) {
        res = true;
        uint32_t itow = 0;
        memcpy(&itow, &frame[UBX_INDEX_PAYLOAD + 4], 4);
        memcpy(&NavInfo.longitude, &frame[UBX_INDEX_PAYLOAD + 8], 4);
        memcpy(&NavInfo.latitude, &frame[UBX_INDEX_PAYLOAD + 12], 4);
        memcpy(&NavInfo.hmsl, &frame[UBX_INDEX_PAYLOAD + 20], 4);
        memcpy(&NavInfo.h_acc, &frame[UBX_INDEX_PAYLOAD + 28], 4);
        memcpy(&NavInfo.v_acc, &frame[UBX_INDEX_PAYLOAD + 32], 4);
#ifdef HAS_UBX_DIAG
        io_printf("itow: %u ms" CRLF, itow);
        io_printf("hmsl: %u mm %u sm %u m" CRLF, NavInfo.hmsl, NavInfo.hmsl / 10, NavInfo.hmsl / 1000);
        io_printf(" %d %d " CRLF "%f %f" CRLF, NavInfo.latitude, NavInfo.longitude, 1e-7 * ((double)NavInfo.latitude),
                  1e-7 * ((double)NavInfo.longitude));
#endif /*HAS_UBX_DIAG*/
    }
    return res;
}

static bool ubx_proc_nav_frame(uint8_t* frame) {
    bool res = false;
    switch(frame[UBX_INDEX_ID]) {
    case UBX_NAV_HPPOSLLH:
        res = ubx_proc_nav_hpposllh_frame(frame);
        break;
    case UBX_NAV_ATT:
        res = ubx_proc_nav_att_frame(frame);
        break;
    default:
        break;
    }
    return res;
}

static bool ubx_proc_ack_frame(void) {
    bool res = false;
    switch(UbloxPorotocol.fix_frame[UBX_INDEX_ID]) {
    case UBX_ACK_ACK:
        UbloxPorotocol.ack_cnt++;
        res = true;
        break;
    case UBX_ACK_NAK:
        break;
    default:
        break;
    }
    return res;
}

bool ubx_proc_frame(void) {
    bool res = false;
    if(true == UbloxPorotocol.unproc_frame) {
        uint8_t in_class = UbloxPorotocol.fix_frame[UBX_INDEX_CLS];
#ifdef HAS_UBX_DIAG
        ubx_print_frame(UbloxPorotocol.fix_frame);
#endif /*HAS_UBX_DIAG*/
        ubx_update_stat(in_class);
        switch(in_class) {
        case UBX_CLA_NAV:
            res = ubx_proc_nav_frame(UbloxPorotocol.fix_frame);
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
