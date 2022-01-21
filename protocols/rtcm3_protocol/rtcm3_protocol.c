/*FSM for RTCM3 protocol*/
#include "rtcm3_protocol.h"

#include <string.h>

#include "byte_utils.h"
#include "crc24_q.h"
#include "data_utils.h"
#include "debug_info.h"
#ifdef HAS_LED
#include "led_drv.h"
#endif
#ifdef HAS_LOG
#include "io_utils.h"
#include "log.h"
#endif

#ifdef X86_64
#include <stdio.h>
#include "log.h"
#endif

#ifdef HAS_LORA
#include "lora_drv.h"
#endif /*HAS_LORA*/

#ifdef HAS_TBFP
#include "tbfp_protocol.h"
#endif /*HAS_TBFP*/

#ifdef HAS_UART
#include "uart_drv.h"
#endif /*HAS_UART*/

Rtcm3Protocol_t Rtcm3Protocol[IF_CNT];

bool rtcm3_reset_rx(Rtcm3Protocol_t* instance) {
    instance->load_len = 0;
    instance->exp_len.len16 = 0;
    instance->rx_state = RTCM3_WAIT_PREAMBLE;
    return true;
}

bool rtcm3_protocol_init(Rtcm3Protocol_t* instance, Interfaces_t interface, bool lora_fwd) {
    rtcm3_reset_rx(instance);
    memset(instance, 0x0, sizeof(Rtcm3Protocol_t));
    memset(instance->fix_frame, 0x00, RTCM3_RX_FRAME_SIZE);
    memset(instance->rx_frame, 0x00, RTCM3_RX_FRAME_SIZE);
#ifdef HAS_DEBUG
    instance->forwarding[IF_LORA] = false;
#else
    instance->forwarding[IF_LORA] = lora_fwd;
#endif /*HAS_DEBUG*/
#ifdef HAS_LOG
    LOG_DEBUG(RTCM, "Init");
#endif
    instance->interface = interface;
    return true;
}

static bool rtcm3_proc_wait_preamble(Rtcm3Protocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    if(RTCM3_PREAMBLE == rx_byte) {
        instance->rx_state = RTCM3_WAIT_LEN;
        instance->rx_frame[0] = rx_byte;
        instance->load_len = 1;
#ifdef HAS_DEBUG
        instance->preamble_cnt++;
#endif
#ifdef HAS_LOG
        LOG_DEBUG(RTCM, "Preamble");
#endif
        res = true;
    } else {
        rtcm3_reset_rx(instance);
    }
    return res;
}

static bool rtcm3_proc_wait_len(Rtcm3Protocol_t* instance, uint8_t rx_byte) {
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
#ifdef HAS_LOG
        LOG_DEBUG(RTCM, "ExpLen %u", instance->exp_len.field.len);
#endif
#ifdef X86_64
        printf("\n   exp len %u", instance->exp_len.field.len);
#endif
        instance->rx_state = RTCM3_WAIT_PAYLOAD;
        res = true;
#ifdef HAS_DEBUG
        instance->max_len = max16u(instance->max_len, instance->exp_len.field.len);
        instance->min_len = min16u(instance->min_len, instance->exp_len.field.len);
#endif
        if(RTCM3_RX_FRAME_SIZE < (instance->exp_len.field.len+RTCM3_CRC24_SIZE)) {
            res = false;
            instance->err_cnt++;
            LOG_ERROR(SYS, "TooBigFrame %u byte. Max %u Byte", instance->exp_len.field.len+RTCM3_CRC24_SIZE, RTCM3_RX_FRAME_SIZE);
            rtcm3_reset_rx(instance);
        }
    } else {
        rtcm3_reset_rx(instance);
    }
    return res;
}

bool rtcm3_proc_wait_payload(Rtcm3Protocol_t* instance, uint8_t rx_byte) {
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

static bool rtcm3_proc_wait_crc24(Rtcm3Protocol_t* instance, uint8_t rx_byte) {
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
#ifdef HAS_LOG
            LOG_DEBUG(RTCM, "CRCok");
#endif
            instance->rx_state = RTCM3_RX_DONE;
            instance->rx_pkt_cnt++;
            memcpy(instance->fix_frame, instance->rx_frame, RTCM3_RX_FRAME_SIZE);
            /*Send RTCM3 frame to LoRa*/
#ifdef HAS_LED
            led_blink(&Led[LED_INDEX_RED], 10);
#endif
            if(IF_UART1 == instance->interface) {
                Interfaces_t interface = IF_NONE;
                for(interface = IF_LORA; interface <= IF_CNT; interface++){
                    if(instance->forwarding[interface]) {
#ifdef HAS_TBFP
                        /*Wrap to TBFP*/
                        res = tbfp_send(instance->fix_frame, frame_length + RTCM3_CRC24_SIZE, interface, 0);
                        if(false == res) {
                            instance->lost_pkt_cnt[interface]++;
                        }
#endif /*HAS_TBFP*/
                    }
                }
            }

            if((IF_LORA == instance->interface) || (IF_RS232 == instance->interface)) {
#ifdef HAS_UART1
                res = uart_send(UART_NUM_ZED_F9P, instance->fix_frame, frame_length + RTCM3_CRC24_SIZE, true);
                if(false == res) {
                    instance->uart_lost_pkt_cnt++;
                }
#endif /*HAS_UART1*/
            }

            rtcm3_reset_rx(instance);
        } else {
#ifdef HAS_LOG
            LOG_ERROR(RTCM, "%s CrcErr",interface2str(instance->interface));
#endif
            instance->crc_err_cnt++;
            rtcm3_reset_rx(instance);
        }
    } else {
        rtcm3_reset_rx(instance);
    }
    return res;
}

bool rtcm3_proc_byte(Rtcm3Protocol_t* instance, uint8_t rx_byte) {
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

bool is_rtcm3_frame(uint8_t* arr, uint16_t len) {
    bool res = true;
    Rtcm3Len_t ex_len;
    ex_len.len16 = 0;
    uint32_t read_crc24 = 0;
    uint16_t frame_length = 0;
    uint16_t crc24_index = 0;
    if((NULL == arr) || (0 == len)) {
        res = false;
    }

    if(res) {
        if(RTCM3_PREAMBLE != arr[0]) {
            res = false;
        }
    }
    if(res) {
        ex_len.len8[0] = arr[2];
        ex_len.len8[1] = arr[1];
        if(0 == ex_len.field.len) {
            res = false;
        } else {
            frame_length = ex_len.field.len + RTCM3_HEADER_SIZE;
        }
    }

    if(res) {
        crc24_index = RTCM3_HEADER_SIZE + ex_len.field.len;
        memcpy(&read_crc24, &arr[crc24_index], RTCM3_CRC24_SIZE);
        read_crc24 = reverse_byte_order_uint24(read_crc24);
        res = crc24_q_check(arr, frame_length, read_crc24);
    }

    return res;
}

bool rtcm3_proc_array(uint8_t* const payload, uint32_t size, Interfaces_t interface) {
    bool res = false;
#ifdef X86_64
    LOG_DEBUG(RTCM,"%s():", __FUNCTION__);
#endif
    if((NULL != payload) && (0 < size)) {
        uint32_t i = 0;
        rtcm3_reset_rx(&Rtcm3Protocol[interface]);
        uint32_t init_rx_pkt_cnt = Rtcm3Protocol[interface].rx_pkt_cnt;
        for(i = 0; i < size; i++) {
            res = rtcm3_proc_byte(&Rtcm3Protocol[interface], payload[i]);
        }
        if(init_rx_pkt_cnt < Rtcm3Protocol[interface].rx_pkt_cnt) {
            res = true;
        }
    }

    return res;
}
