#include "ublox_driver.h"

#include <stdbool.h>
#include <string.h>

#include "byte_utils.h"
#include "debug_info.h"
#include "io_utils.h"
#include "log.h"
#include "uart_drv.h"
#include "ubx_protocol.h"
#include "ubx_types.h"

#ifndef HAS_MCU
#include "clocks.h"
#endif /*HAS_MCU*/

#ifndef HAS_UART
#error "Ublox driver requires UART driver"
#endif

NavInfo_t NavInfo = {0};

bool ubx_driver_init(void) {
    memset(&NavInfo, 0x00, sizeof(NavInfo));
    bool res = set_log_level(UBX, LOG_LEVEL_NOTICE);
    return res;
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
        res = uart_send(UBX_UART_NUM, tx_array, tx_array_len, true);
        if(res) {
            UbloxProtocol.tx_pkt_cnt++;
        }
    }
    return res;
}

#define UBX_KEY_SIZE 4
static bool ubx_config_data_parse(uint8_t* keys) {
    uint16_t bytes = 0;
    bool res = false;
    ConfigurationKeyID_t keyId;
    memcpy(&keyId.word, keys, UBX_KEY_SIZE);
    bytes = ubx_key_len_2bytes(keyId.size);
    LOG_INFO(UBX, "Key: 0x%08x len %u", keyId.word, bytes);
    res = reverse_byte_order_array(&keys[4], bytes);
    res = print_mem(&keys[4], bytes, true, false, true, false);
    return res;
}

static bool ubx_cfg_valget_parse(uint8_t* payload) {
    bool res = false;
    CfgValGetHeader_t Header = {0};
    memcpy(&Header, payload, sizeof(CfgValGetHeader_t));
    if(0x01 == Header.version) {
        res = ubx_config_data_parse(&payload[4]);
    }
    return res;
}

static bool ubx_proc_cfg_frame(void) {
    bool res = false;
    switch(UbloxProtocol.fix_frame[UBX_INDEX_ID]) {
    case UBX_ID_CFG_GET_VAL:
        res = ubx_cfg_valget_parse(&UbloxProtocol.fix_frame[UBX_INDEX_PAYLOAD]);
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
        // io_printf("itow: %u ms" CRLF, data.iTOW);
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

static bool ubx_proc_nav_timeutc_frame(uint8_t* payload, uint16_t len) {
    bool res = false;
    if(20 <= len) {
        res = true;
        NavTimeUtc_t data = {0};
        memcpy(&data, payload, sizeof(NavTimeUtc_t));
        NavInfo.date_time.tm_hour = data.hour;
        NavInfo.date_time.tm_min = data.min;
        NavInfo.date_time.tm_sec = data.sec;

        NavInfo.date_time.tm_mday = data.day;
        NavInfo.date_time.tm_mon = data.month-1; /* see page 147*/
        NavInfo.date_time.tm_year = data.year;
        // io_printf("valid: %u" CRLF, data.ValidityFlags.byte);
    }
    return res;
}

static bool ubx_proc_nav_velned_frame(uint8_t* payload, uint16_t len) {
    bool res = false;
    if(36 <= len) {
        NavVelNed_t data = {0};
        memcpy(&data, payload, sizeof(NavVelNed_t));
        NavInfo.velocity.speed_ground = (double)data.speed_ground;
        NavInfo.velocity.velocity_north = (double)data.vel_north;
        NavInfo.velocity.velocity_east = (double)data.vel_east;
        NavInfo.velocity.velocity_down = (double)data.vel_down;
        NavInfo.velocity.heading = ((double)1e-5) * ((double)data.heading);
        NavInfo.velocity.speed = (double)data.speed;
        NavInfo.velocity.speed_ground = (double)data.speed_ground;
        NavInfo.velocity.accuracy_speed = (double)data.speed_acc;
        NavInfo.velocity.accuracy_course = ((double)1e-5) * ((double)data.course_acc);
        res = true;
    }
    return res;
}

static bool ubx_proc_nav_frame(uint8_t* frame, uint16_t len) {
    bool res = false;
    uint8_t id = frame[UBX_INDEX_ID];
    switch(id) {
    case UBX_ID_NAV_VELNED:
        res = ubx_proc_nav_velned_frame(frame + UBX_INDEX_PAYLOAD, len);
        break;

    case UBX_ID_NAV_TIMEUTC:
        res = ubx_proc_nav_timeutc_frame(frame + UBX_INDEX_PAYLOAD, len);
        break;

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
    switch(UbloxProtocol.fix_frame[UBX_INDEX_ID]) {
    case UBX_ACK_ACK:
        LOG_NOTICE(UBX, "Ack");
        UbloxProtocol.ack_cnt++;
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

bool ubx_proc_frame(UbloxProtocol_t* inst) {
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
    {UBX_CLA_NAV, UBX_ID_NAV_TIMEUTC}, {UBX_CLA_NAV, UBX_ID_NAV_VELNED},   {UBX_CLA_NAV, UBX_ID_NAV_POSLLH},
    {UBX_CLA_NAV, UBX_ID_NAV_ATT},     {UBX_CLA_NAV, UBX_ID_NAV_HPPOSLLH}, {UBX_CLA_SEC, UBX_ID_SEC_UNIQID},
};

bool ubx_cfg_set_val(uint32_t key_id, uint8_t* val, uint16_t val_len, uint8_t layers) {
    bool res = false;
    if(val && (0 < val_len)) {
        uint8_t payload[4 + 32];
        memset(payload, 0, sizeof(payload));
        uint16_t payload_len = 0;
        payload[0] = 0x00;   /*version*/
        payload[1] = layers; /*layers*/
        payload[2] = 0;
        payload[3] = 0;
        memcpy(&payload[4], &key_id, sizeof(key_id));
        // uint8_t key_size = ubx_keyid_2len(key_id);
        print_mem(val, val_len, true, false, true, true);
        memcpy(&payload[8], val, val_len);
        payload_len = 8 + val_len;
        res = ubx_send_message(UBX_CLA_CFG, UBX_ID_CFG_SET_VAL, payload, payload_len);
    }
    return res;
}

bool ubx_cfg_get_val(uint32_t key_id, uint8_t layers) {
    bool res = false;
    uint8_t payload[4 + 32];
    uint16_t payload_len = 0;
    payload[0] = 0x00;   /*version*/
    payload[1] = layers; /*layers*/
    payload[2] = 0;
    payload[3] = 0;
    memcpy(&payload[4], &key_id, sizeof(key_id));
    payload_len = 8;
    res = ubx_send_message(UBX_CLA_CFG, UBX_ID_CFG_GET_VAL, payload, payload_len);
    return res;
}

bool ubx_proc(void) {
    bool res = false;
    static uint16_t i = 0;
    res = ubx_send_message(pollLut[i].class, pollLut[i].id, NULL, 0);
    if(false == res) {
        LOG_ERROR(UBX, "Send Class:0x%02x ID:0x%02x Error", pollLut[i].class, pollLut[i].id);
    } else {
        LOG_DEBUG(UBX, "Send Class:0x%02x ID:0x%02x OK", pollLut[i].class, pollLut[i].id);
    }
    i++;
    if(ARRAY_SIZE(pollLut) < i) {
        i = 0;
    }
    uint32_t cur_time = get_time_ms32();
    uint32_t time_diff = 0;
    time_diff = cur_time - UbloxProtocol.rx_time_stamp;
    if(UBX_RX_TIME_OUT_MS < time_diff) {
        LOG_ERROR(UBX, "UBX proto link lost %f s", MS_2_S(time_diff));
        res = false;
    }
    return res;
}

bool ubx_reset_to_dflt(void) {
    bool res = false;
    UbxCfgCfg_t data = {0};
    /*UBX is a little-endian protocol*/
    data.clearMask = 0x0000FBFF;
    data.saveMask = 0x00000000;
    data.loadMask = 0x0000FFFF;
    data.deviceMask = 0x17;
    print_mem((uint8_t*)&data, sizeof(data), true, false, true, true);
    res = ubx_send_message(UBX_CLA_CFG, UBX_ID_CFG_CFG, (uint8_t*)&data, sizeof(data));
    if(res) {
        res = ubx_wait_ack(1000);
    }
    return res;
}

bool ubx_set_rate(uint16_t meas_rate_ms, uint16_t time_ref) {
    bool res = false;
    if(25 < meas_rate_ms) {
        UbxCfgRate_t data;
        data.meas_rate_ms = meas_rate_ms;
        data.navRate = 1;
        data.timeRef = time_ref;
        print_mem((uint8_t*)&data, sizeof(data), true, false, true, true);
        res = ubx_send_message(UBX_CLA_CFG, UBX_ID_CFG_RATE, (uint8_t*)&data, sizeof(data));
        if(res) {
            res = ubx_wait_ack(1000);
        }
    }
    return res;
}
