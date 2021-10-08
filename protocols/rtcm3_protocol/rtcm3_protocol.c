
/*FSM for RTCM3 protocol*/
#include "rtcm3_protocol.h"

#include <string.h>

#include "byte_utils.h"
#include "crc24_q.h"
#include "data_utils.h"
#include "debug_info.h"
#include "io_utils.h"
#ifdef HAS_MCU
#include "log.h"
#endif

#ifdef X86_64
#include <stdio.h>
#endif

#ifdef HAS_LORA
#include "lora_drv.h"
#endif /*HAS_LORA*/

#ifdef HAS_UART
#include "uart_drv.h"
#endif /*HAS_UART*/

Rtcm3Porotocol_t Rtcm3Porotocol[RTCM_IF_CNT];

bool rtcm3_reset_rx(Rtcm3Porotocol_t* instance) {
    instance->load_len = 0;
    instance->exp_len.len16 = 0;
    instance->rx_state = RTCM3_WAIT_PREAMBLE;
    return true;
}

bool rtcm3_protocol_init(Rtcm3Porotocol_t* instance, uint8_t interface) {
    rtcm3_reset_rx(instance);
    memset(&Rtcm3Porotocol, 0x0, sizeof(Rtcm3Porotocol));
    instance->ack_cnt = 0;
    instance->interface = interface;
    memset(instance->fix_frame, 0x00, RTCM3_RX_FRAME_SIZE);
    memset(instance->rx_frame, 0x00, RTCM3_RX_FRAME_SIZE);
    return true;
}

static bool rtcm3_proc_wait_preamble(Rtcm3Porotocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    if(RTCM3_PREAMBLE == rx_byte) {
        instance->rx_state = RTCM3_WAIT_LEN;
        instance->rx_frame[0] = rx_byte;
        instance->load_len = 1;
        res = true;
    } else {
        rtcm3_reset_rx(instance);
    }
    return res;
}

static bool rtcm3_proc_wait_len(Rtcm3Porotocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    if(1 == instance->load_len) {
        instance->rx_frame[1] = rx_byte;
        instance->exp_len.len8[1] = rx_byte;
        instance->load_len = 2;
        instance->rx_state = RTCM3_WAIT_LEN;
        res = true;
    } else if(2 == instance->load_len) {
        instance->rx_frame[2] = rx_byte;
        instance->exp_len.len8[0] = rx_byte;
        instance->load_len = 3;
#ifdef X86_64
        printf("\n   exp len %u", instance->exp_len.field.len);
#endif
        instance->rx_state = RTCM3_WAIT_PAYLOAD;
        res = true;
#ifdef HAS_DEBUG
        instance->max_len = max16u(instance->max_len, instance->exp_len.field.len);
        instance->min_len = min16u(instance->min_len, instance->exp_len.field.len);
#endif
        if(RTCM3_RX_FRAME_SIZE < instance->exp_len.field.len) {
            res = false;
#ifdef HAS_DEBUG_RTCM3
            LOG_ERROR(SYS, "TooBig frame %u byte", instance->exp_len.field.len);
#endif
            rtcm3_reset_rx(instance);
        }
    } else {
        rtcm3_reset_rx(instance);
    }
    return res;
}

bool rtcm3_proc_wait_payload(Rtcm3Porotocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    if(instance->load_len < (RTCM3_HEADER_SIZE + instance->exp_len.field.len - 1)) {
        instance->rx_frame[instance->load_len] = rx_byte;
        instance->load_len++;
        instance->rx_state = RTCM3_WAIT_PAYLOAD;
        res = true;
    } else if(instance->load_len == (RTCM3_HEADER_SIZE + instance->exp_len.field.len - 1)) {
        instance->rx_frame[instance->load_len] = rx_byte;
        instance->load_len++;
        instance->rx_state = RTCM3_WAIT_CRC;
        res = true;
    } else {
        rtcm3_reset_rx(instance);
    }
    return res;
}

static bool rtcm3_proc_wait_crc24(Rtcm3Porotocol_t* instance, uint8_t rx_byte) {
#ifdef X86_64
    printf("\n %s() 0x%02x", __FUNCTION__, rx_byte);
#endif
    bool res = false;
    uint16_t crc24_index = RTCM3_HEADER_SIZE + instance->exp_len.field.len;
    if(crc24_index == instance->load_len) {
        instance->rx_frame[instance->load_len] = rx_byte;
        instance->load_len++;
        instance->rx_state = RTCM3_WAIT_CRC;
        res = true;
    } else if((crc24_index + 1) == instance->load_len) {
        instance->rx_frame[instance->load_len] = rx_byte;
        instance->load_len++;
        instance->rx_state = RTCM3_WAIT_CRC;
        res = true;
    } else if((crc24_index + 2) == instance->load_len) {
        instance->rx_frame[instance->load_len] = rx_byte;
        instance->load_len++;
        memcpy(&instance->read_crc, &instance->rx_frame[crc24_index], RTCM3_CRC24_SIZE);
        instance->read_crc = reverse_byte_order_uint24(instance->read_crc);
        uint16_t frame_length = instance->exp_len.field.len + RTCM3_HEADER_SIZE;
        res = crc24_q_check(&instance->rx_frame[0], frame_length, instance->read_crc);
        if(true == res) {
            res = true;
            instance->rx_state = RTCM3_RX_DONE;
            instance->rx_pkt_cnt++;
            memcpy(instance->fix_frame, instance->rx_frame, RTCM3_RX_FRAME_SIZE);
            /*Send RTCM3 frame to LoRa*/
#ifdef HAS_LORA
            if(RT_UART_ID == instance->interface) {
                res = lora_send_queue(instance->fix_frame, frame_length + RTCM3_CRC24_SIZE);
            }
#endif /*HAS_LORA*/
#ifdef HAS_UART1
            if(RT_LORA_ID == instance->interface) {
                res = uart_send(UART_NUM_ZED_F9P, instance->fix_frame, frame_length + RTCM3_CRC24_SIZE, true);
            }
#endif /*HAS_UART1*/
            rtcm3_reset_rx(instance);
        } else {
            instance->crc_err_cnt++;
            rtcm3_reset_rx(instance);
        }
    } else {
        rtcm3_reset_rx(instance);
    }
    return res;
}

bool rtcm3_proc_byte(Rtcm3Porotocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    switch(instance->rx_state) {
    case RTCM3_WAIT_PREAMBLE:
        res = rtcm3_proc_wait_preamble(instance, rx_byte);
        break;
    case RTCM3_WAIT_LEN:
        res = rtcm3_proc_wait_len(instance, rx_byte);
        break;
    case RTCM3_WAIT_PAYLOAD:
        res = rtcm3_proc_wait_payload(instance, rx_byte);
        break;
    case RTCM3_WAIT_CRC:
        res = rtcm3_proc_wait_crc24(instance, rx_byte);
        break;
    default:
        rtcm3_reset_rx(instance);
        break;
    }
    return res;
}
