#include "ublox_driver.h"

#include <stdbool.h>
#include <string.h>

#include "io_utils.h"
#include "log.h"
#include "uart_drv.h"
#include "ubx_protocol.h"
#include "ubx_types.h"

#ifndef HAS_UART
#error "Ublox driver requires UART driver"
#endif

keyValItem_t keyValTable[UBX_KEY_CNT] = {{0x30210001, 0x00, UBX_U2}};

NavInfo_t NavInfo;

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
        uart_send(1, tx_array, tx_array_len, true);
    }
    return res;
}

#define UBX_KEY_SIZE 4
static bool ubx_config_data_parse(uint8_t* payload) {
    uint32_t read_key = 0;
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
    if(0x00 == frame[4]) {
        memcpy(&NavInfo.roll, &frame[8], 4);
        memcpy(&NavInfo.pitch, &frame[12], 4);
        memcpy(&NavInfo.heading, &frame[16], 4);

        memcpy(&NavInfo.acc_roll, &frame[20], 4);
        memcpy(&NavInfo.acc_pitch, &frame[24], 4);
        memcpy(&NavInfo.acc_heading, &frame[28], 4);
        res = true;
    }
    return res;
}

static bool ubx_proc_nav_posllh_frame(uint8_t* payload, uint16_t len) {
    bool res = false;
    if(28 <= len) {
        res = true;
        NavPosllh_t data = {0};
        memcpy(&data, payload, sizeof(NavPosllh_t));
        NavInfo.coordinate.latitude = ((double)data.lat) * ((double)1e-7);
        NavInfo.coordinate.longitude = ((double)data.lon) * ((double)1e-7);
        NavInfo.h_acc = data.hAcc;
        NavInfo.v_acc = data.vAcc;
        NavInfo.hmsl = data.hMSL;
        io_printf("itow: %u ms" CRLF, data.iTOW);
    }

    return res;
}

static bool ubx_proc_nav_hpposllh_frame(uint8_t* payload, uint16_t len) {
    bool res = false;
    if(36 <= len) {
        res = true;
        NavHpPosllh_t data;
        memcpy(&data, payload, sizeof(NavHpPosllh_t));
        NavInfo.coordinate.latitude = ((double)data.lat) * ((double)1e-7);
        NavInfo.coordinate.longitude = ((double)data.lon) * ((double)1e-7);
        NavInfo.h_acc = data.hAcc;
        NavInfo.v_acc = data.vAcc;
        NavInfo.hmsl = data.hMSL;
    }

    return res;
}

static bool ubx_proc_nav_frame(uint8_t* frame, uint16_t len) {
    bool res = false;
    uint8_t id = frame[UBX_INDEX_ID];
    switch(id) {
    case UBX_ID_NAV_POSLLH:
        res = ubx_proc_nav_posllh_frame(frame + UBX_INDEX_PAYLOAD, len);
        break;
    case UBX_ID_NAV_HPPOSLLH:
        res = ubx_proc_nav_hpposllh_frame(frame + UBX_INDEX_PAYLOAD, len);
        break;
    case UBX_ID_NAV_ATT:
        res = ubx_proc_nav_att_frame(frame + UBX_INDEX_PAYLOAD);
        break;
    default:
        LOG_ERROR(UBX, "Undef index 0x%x", id);
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

bool ubx_proc_sec_uniqid_frame(uint8_t* payload, uint16_t len) {
    bool res = false;
    if(9 <= len) {
        SecUniqId_t data = {0};
        memcpy(&data, payload, sizeof(SecUniqId_t));
        memcpy(NavInfo.id, data.UniqueChipId, 5);
        res = true;
    }
    return res;
}

static bool ubx_proc_sec_frame(uint8_t* frame, uint16_t len) {
    bool res = false;
    uint8_t id = frame[UBX_INDEX_ID];
    switch(id) {
    case UBX_ID_SEC_UNIQID:
        res = ubx_proc_sec_uniqid_frame(frame + UBX_INDEX_PAYLOAD, len);
        break;
    default:
        LOG_ERROR(UBX, "Undef SEC id 0x%x", id);
        break;
    }
    return res;
}

bool ubx_proc_frame(UbloxPorotocol_t* inst) {
    bool res = false;

    uint8_t in_class = inst->fix_frame[UBX_INDEX_CLS];
    if(inst->diag) {
        ubx_print_frame(inst->fix_frame);
    }
    ubx_update_stat(in_class);
    switch(in_class) {
    case UBX_CLA_NAV:
        res = ubx_proc_nav_frame(inst->fix_frame, inst->exp_len);
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
        res = ubx_proc_sec_frame(inst->fix_frame, inst->exp_len);
        break;
    default:
        LOG_ERROR(UBX, "Undef %u 0x%x", in_class, in_class);
        break;
    }
    memset(inst->fix_frame, 0x00, UBX_RX_FRAME_SIZE);
    ubx_reset_rx();

    return res;
}

static const UbxHeader_t pollLut[] = {
    {UBX_CLA_NAV, UBX_ID_NAV_POSLLH},
    {UBX_CLA_NAV, UBX_ID_NAV_ATT},
    {UBX_CLA_NAV, UBX_ID_NAV_HPPOSLLH},
    {UBX_CLA_SEC, UBX_ID_SEC_UNIQID},
};

bool ubx_proc(void) {
    bool res = false;
    static uint16_t i = 0;
    res = ubx_send_message(pollLut[i].class, pollLut[i].id, NULL, 0);
    if(false == res) {
        LOG_ERROR(UBX, "Send Class:0x%02x  ID:0x%02x Error ", pollLut[i].class, pollLut[i].id);
    }
    i++;
    if(ARRAY_SIZE(pollLut) < i) {
        i = 0;
    }
    return res;
}
