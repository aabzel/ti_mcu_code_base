
/*FSM for RTCM3 protocol*/
#include "rtcm3_protocol.h"

#include <string.h>

#include "byte_utils.h"
#include "crc24_q.h"
#include "data_utils.h"
#include "debug_info.h"
#include "io_utils.h"

#ifdef X86_64
#include <stdio.h>
#endif

Rtcm3Porotocol_t Rtcm3Porotocol = {0};

bool rtcm3_reset_rx(void) {
    Rtcm3Porotocol.load_len = 0;
    Rtcm3Porotocol.exp_len.len16 = 0;
    Rtcm3Porotocol.rx_state = RTCM3_WAIT_PREAMBLE;
    return true;
}

bool rtcm3_protocol_init(void) {
    rtcm3_reset_rx();
    Rtcm3Porotocol.rx_pkt_cnt = 0;
    Rtcm3Porotocol.ack_cnt = 0;
    memset(Rtcm3Porotocol.fix_frame, 0x00, RTCM3_RX_FRAME_SIZE);
    memset(Rtcm3Porotocol.rx_frame, 0x00, RTCM3_RX_FRAME_SIZE);
    return true;
}

static bool rtcm3_proc_wait_preamble(uint8_t rx_byte) {
    bool res = false;
    if(RTCM3_PREAMBLE == rx_byte) {
        Rtcm3Porotocol.rx_state = RTCM3_WAIT_LEN;
        Rtcm3Porotocol.rx_frame[0] = rx_byte;
        Rtcm3Porotocol.load_len = 1;
        res = true;
    } else {
        rtcm3_reset_rx();
    }
    return res;
}

static bool rtcm3_proc_wait_len(uint8_t rx_byte) {
    bool res = false;
    if(1 == Rtcm3Porotocol.load_len) {
        Rtcm3Porotocol.rx_frame[1] = rx_byte;
        Rtcm3Porotocol.exp_len.len8[1] = rx_byte;
        Rtcm3Porotocol.load_len = 2;
        Rtcm3Porotocol.rx_state = RTCM3_WAIT_LEN;
        res = true;
    } else if(2 == Rtcm3Porotocol.load_len) {
        Rtcm3Porotocol.rx_frame[2] = rx_byte;
        Rtcm3Porotocol.exp_len.len8[0] = rx_byte;
        Rtcm3Porotocol.load_len = 3;
#ifdef X86_64
        printf("\n   exp len %u",Rtcm3Porotocol.exp_len.field.len);
#endif
        Rtcm3Porotocol.rx_state = RTCM3_WAIT_PAYLOAD;
        res = true;
    } else {
        rtcm3_reset_rx();
    }
    return res;
}

bool rtcm3_proc_wait_payload(uint8_t rx_byte) {
    bool res = false;
    if(Rtcm3Porotocol.load_len < (RTCM3_HEADER_SIZE + Rtcm3Porotocol.exp_len.field.len - 1)) {
        Rtcm3Porotocol.rx_frame[Rtcm3Porotocol.load_len] = rx_byte;
        Rtcm3Porotocol.load_len++;
        Rtcm3Porotocol.rx_state = RTCM3_WAIT_PAYLOAD;
        res = true;
    } else if(Rtcm3Porotocol.load_len == (RTCM3_HEADER_SIZE + Rtcm3Porotocol.exp_len.field.len - 1)) {
        Rtcm3Porotocol.rx_frame[Rtcm3Porotocol.load_len] = rx_byte;
        Rtcm3Porotocol.load_len++;
        Rtcm3Porotocol.rx_state = RTCM3_WAIT_CRC;
        res = true;
    } else {
        rtcm3_reset_rx();
    }
    return res;
}

static bool rtcm3_proc_wait_crc24(uint8_t rx_byte) {
#ifdef X86_64
    printf("\n %s() 0x%02x",__FUNCTION__,rx_byte);
#endif
    bool res = false;
    uint16_t crc24_index = RTCM3_HEADER_SIZE + Rtcm3Porotocol.exp_len.field.len;
    if(crc24_index == Rtcm3Porotocol.load_len) {
        Rtcm3Porotocol.rx_frame[Rtcm3Porotocol.load_len] = rx_byte;
        Rtcm3Porotocol.load_len++;
        Rtcm3Porotocol.rx_state = RTCM3_WAIT_CRC;
        res = true;
    } else if((crc24_index + 1) == Rtcm3Porotocol.load_len) {
        Rtcm3Porotocol.rx_frame[Rtcm3Porotocol.load_len] = rx_byte;
        Rtcm3Porotocol.load_len++;
        Rtcm3Porotocol.rx_state = RTCM3_WAIT_CRC;
        res = true;
    } else if((crc24_index + 2) == Rtcm3Porotocol.load_len) {
        Rtcm3Porotocol.rx_frame[Rtcm3Porotocol.load_len] = rx_byte;
        Rtcm3Porotocol.load_len++;
        memcpy(&Rtcm3Porotocol.read_crc, &Rtcm3Porotocol.rx_frame[crc24_index], RTCM3_CRC24_SIZE);
        Rtcm3Porotocol.read_crc = reverse_byte_order_uint24(Rtcm3Porotocol.read_crc);
        uint16_t frame_length = Rtcm3Porotocol.exp_len.field.len + RTCM3_HEADER_SIZE;
        res = crc24_q_check(&Rtcm3Porotocol.rx_frame[0], frame_length, Rtcm3Porotocol.read_crc);
        if(true == res) {
            Rtcm3Porotocol.rx_state = RTCM3_RX_DONE;
            Rtcm3Porotocol.rx_pkt_cnt++;
            memcpy(Rtcm3Porotocol.fix_frame, Rtcm3Porotocol.rx_frame, RTCM3_RX_FRAME_SIZE);
            /*Send RTCM3 frame to LoRa*/
            rtcm3_reset_rx();
            res = true;
        } else {
            Rtcm3Porotocol.crc_err_cnt++;
            rtcm3_reset_rx();
        }
    } else {
        rtcm3_reset_rx();
    }
    return res;
}

bool rtcm3_proc_byte(uint8_t rx_byte) {
    bool res = false;
    switch(Rtcm3Porotocol.rx_state) {
    case RTCM3_WAIT_PREAMBLE:
        res = rtcm3_proc_wait_preamble(rx_byte);
        break;
    case RTCM3_WAIT_LEN:
        res = rtcm3_proc_wait_len(rx_byte);
        break;
    case RTCM3_WAIT_PAYLOAD:
        res = rtcm3_proc_wait_payload(rx_byte);
        break;
    case RTCM3_WAIT_CRC:
        res = rtcm3_proc_wait_crc24(rx_byte);
        break;
    default:
        rtcm3_reset_rx();
        break;
    }
    return res;
}
