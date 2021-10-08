#include <string.h>

#include "data_utils.h"
#include "debug_info.h"
#include "io_utils.h"
#include "ubx_protocol.h"

UbloxPorotocol_t UbloxPorotocol = {0};
StatClass_t tableRxClass[UBX_CLA_CNT] = {0};

bool ubx_reset_rx(void) {
    UbloxPorotocol.load_len = 0;
    UbloxPorotocol.exp_len = 0;
    UbloxPorotocol.rx_state = UBX_WAIT_SYC0;
    return true;
}

bool ublox_protocol_init(void) {
    ubx_reset_rx();
    UbloxPorotocol.rx_pkt_cnt = 0;
    UbloxPorotocol.ack_cnt = 0;
    UbloxPorotocol.unproc_frame = false;
    memset(UbloxPorotocol.fix_frame, 0x00, UBX_RX_FRAME_SIZE);
    memset(tableRxClass, 0x00, sizeof(tableRxClass));
    return true;
}

uint16_t ubx_calc_crc16(uint8_t* const array, uint16_t len) {
    uint16_t crc16 = 0U;
    uint8_t crc_a = 0, crc_b = 0;
    uint16_t i = 0;
    for(i = 0; i < len; i++) {
        crc_a = crc_a + array[i];
        crc_b = crc_b + crc_a;
    }
    crc16 = (((uint16_t)crc_b) << 8) | ((uint16_t)crc_a);
    return crc16;
}

static bool proc_ublox_wait_sync0(uint8_t rx_byte) {
    bool res = false;
    if(UBX_SYN_0 == rx_byte) {
        UbloxPorotocol.rx_state = UBX_WAIT_SYC1;
        UbloxPorotocol.rx_frame[0] = rx_byte;
        UbloxPorotocol.load_len = 1;
        res = true;
    } else {
        ubx_reset_rx();
    }
    return res;
}

static bool proc_ublox_wait_sync1(uint8_t rx_byte) {
    bool res = false;
    if(UBX_SYN_1 == rx_byte) {
        UbloxPorotocol.rx_state = UBX_WAIT_CLASS;
        UbloxPorotocol.rx_frame[1] = rx_byte;
        UbloxPorotocol.load_len = 2;
        res = true;
    } else {
        ubx_reset_rx();
    }
    return res;
}

static bool proc_ublox_wait_calss(uint8_t rx_byte) {
    UbloxPorotocol.rx_frame[2] = rx_byte;
    UbloxPorotocol.load_len = 3;
    UbloxPorotocol.rx_state = UBX_WAIT_ID;
    return true;
}

static bool proc_ublox_wait_id(uint8_t rx_byte) {
    UbloxPorotocol.rx_frame[3] = rx_byte;
    UbloxPorotocol.load_len = 4;
    UbloxPorotocol.rx_state = UBX_WAIT_LEN;
    return true;
}

static bool proc_ublox_wait_len(uint8_t rx_byte) {
    bool res = false;
    if(4 == UbloxPorotocol.load_len) {
        UbloxPorotocol.rx_frame[4] = rx_byte;
        UbloxPorotocol.load_len = 5;
        UbloxPorotocol.rx_state = UBX_WAIT_LEN;
        res = true;
    } else if(5 == UbloxPorotocol.load_len) {
        UbloxPorotocol.rx_frame[5] = rx_byte;
        UbloxPorotocol.load_len = 6;
        memcpy(&UbloxPorotocol.exp_len, &UbloxPorotocol.rx_frame[4], UBX_LEN_SIZE);
        UbloxPorotocol.rx_state = UBX_WAIT_PAYLOAD;
        res = true;
        if(UBX_RX_FRAME_SIZE < UbloxPorotocol.exp_len) {
            ubx_reset_rx();
        }
    } else {
        ubx_reset_rx();
    }
    return res;
}

bool proc_ublox_wait_payload(uint8_t rx_byte) {
    bool res = false;
    if(UbloxPorotocol.load_len < (UBX_HEADER_SIZE + UbloxPorotocol.exp_len - 1)) {
        UbloxPorotocol.rx_frame[UbloxPorotocol.load_len] = rx_byte;
        UbloxPorotocol.load_len++;
        UbloxPorotocol.rx_state = UBX_WAIT_PAYLOAD;
        res = true;
    } else if(UbloxPorotocol.load_len == (UBX_HEADER_SIZE + UbloxPorotocol.exp_len - 1)) {
        UbloxPorotocol.rx_frame[UbloxPorotocol.load_len] = rx_byte;
        UbloxPorotocol.load_len++;
        UbloxPorotocol.rx_state = UBX_WAIT_CRC;
        res = true;
    } else {
        ubx_reset_rx();
    }
    return res;
}

static bool proc_ublox_wait_crc(uint8_t rx_byte) {
    bool res = false;
    uint16_t calc_crc = 0;
    uint16_t crc_index = UBX_HEADER_SIZE + UbloxPorotocol.exp_len;
    if(crc_index == UbloxPorotocol.load_len) {
        UbloxPorotocol.rx_frame[UbloxPorotocol.load_len] = rx_byte;
        UbloxPorotocol.load_len++;
        UbloxPorotocol.rx_state = UBX_WAIT_CRC;
        res = true;
    } else if((crc_index + 1) == UbloxPorotocol.load_len) {
        UbloxPorotocol.rx_frame[UbloxPorotocol.load_len] = rx_byte;
        UbloxPorotocol.load_len++;
        memcpy(&UbloxPorotocol.read_crc, &UbloxPorotocol.rx_frame[crc_index], UBX_LEN_SIZE);
        uint16_t len = UbloxPorotocol.exp_len + 4;
        calc_crc = ubx_calc_crc16(&UbloxPorotocol.rx_frame[2], len);
        if(calc_crc == UbloxPorotocol.read_crc) {
            UbloxPorotocol.rx_state = UBX_RX_DONE;
            UbloxPorotocol.rx_pkt_cnt++;
            memcpy(UbloxPorotocol.fix_frame, UbloxPorotocol.rx_frame, UBX_RX_FRAME_SIZE);
            UbloxPorotocol.unproc_frame = true;
            ubx_reset_rx();
            res = true;
        } else {
            UbloxPorotocol.crc_err_cnt++;
            ubx_reset_rx();
        }
    } else {
        ubx_reset_rx();
    }
    return res;
}

bool ubx_proc_byte(uint8_t rx_byte) {
    bool res = false;
    switch(UbloxPorotocol.rx_state) {
    case UBX_WAIT_SYC0:
        res = proc_ublox_wait_sync0(rx_byte);
        break;
    case UBX_WAIT_SYC1:
        res = proc_ublox_wait_sync1(rx_byte);
        break;
    case UBX_WAIT_CLASS:
        res = proc_ublox_wait_calss(rx_byte);
        break;
    case UBX_WAIT_ID:
        res = proc_ublox_wait_id(rx_byte);
        break;
    case UBX_WAIT_LEN:
        res = proc_ublox_wait_len(rx_byte);
        break;
    case UBX_WAIT_PAYLOAD:
        res = proc_ublox_wait_payload(rx_byte);
        break;
    case UBX_WAIT_CRC:
        res = proc_ublox_wait_crc(rx_byte);
        break;
    default:
        ubx_reset_rx();
        break;
    }
    return res;
}

bool ubx_update_stat(uint8_t val_class) {
    bool res = false;
    bool inc = false;
    uint8_t i;
    for(i = 0; i < ARRAY_SIZE(tableRxClass); i++) {
        if(val_class == tableRxClass[i].class_val) {
            tableRxClass[i].cnt++;
            res = true;
            inc = true;
            break;
        }
    }

    if(false == inc) {
        for(i = 0; i < ARRAY_SIZE(tableRxClass); i++) {
            if(0x00 == tableRxClass[i].class_val) {
                tableRxClass[i].class_val = val_class;
                tableRxClass[i].cnt = 1;
                res = true;
                inc = true;
                break;
            }
        }
    }

    return res;
}
