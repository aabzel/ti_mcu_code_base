#include "tbfp_protocol.h"
/*FSM for parsing TBFP protocol from byte stream*/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef X86_64
#include <stdio.h>
#endif

#include "crc8_sae_j1850.h"
#include "data_utils.h"
#include "log.h"

static bool tbfp_parser_proc_wait_preamble(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    if(TBFP_PREAMBLE == rx_byte) {
        instance->parser.rx_frame[0] = rx_byte;
        instance->parser.rx_state = WAIT_LEN;
        instance->parser.load_len = 1;
#ifdef HAS_DEBUG
        instance->preamble_cnt++;
#ifdef HAS_MCU
        LOG_DEBUG(TBFP, "Preamble");
#endif
#endif
        res = true;
    } else {
        res = tbfp_parser_reset_rx(instance);
    }
    return res;
}

static bool tbfp_parser_proc_wait_len(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    if(1 == instance->parser.load_len) {
        instance->parser.exp_payload_len = rx_byte;
        instance->parser.rx_frame[1] = rx_byte;
        instance->parser.load_len = 2;
#ifdef HAS_DEBUG
#ifdef HAS_MCU
        LOG_DEBUG(TBFP, "Len:0x%02x %u", rx_byte, rx_byte);
#endif
        instance->max_len = max16u(instance->max_len, instance->parser.exp_payload_len);
        instance->min_len = min16u(instance->min_len, instance->parser.exp_payload_len);
#endif
        instance->parser.rx_state = WAIT_SERIAL_NUM;
        res = true;
    } else {
        res = tbfp_parser_reset_rx(instance);
    }
    return res;
}

static bool tbfp_parser_proc_wait_serial_num(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    if(2 == instance->parser.load_len) {
        instance->parser.rx_frame[2] = rx_byte;
        instance->parser.load_len = 3;
        instance->parser.rx_state = WAIT_SERIAL_NUM;
        res = true;
    } else if(3 == instance->parser.load_len) {
        instance->parser.rx_frame[3] = rx_byte;
        instance->parser.s_num = 0;
        memcpy(&instance->parser.s_num, &instance->parser.rx_frame[2], 2);
        instance->parser.load_len = 4;
        instance->parser.rx_state = WAIT_PAYLOAD;
#ifdef HAS_MCU
        LOG_DEBUG(TBFP, "SN:0x%04x %u", instance->parser.s_num, instance->parser.s_num);
#endif
        if((0 < instance->parser.exp_payload_len) && (instance->parser.exp_payload_len <= TBFP_MAX_PAYLOAD)) {
            instance->parser.rx_state = WAIT_PAYLOAD;
            res = true;
        } else if(0 == instance->parser.exp_payload_len) {
            instance->parser.rx_state = WAIT_CRC;
            res = true;
        } else {
            res = tbfp_parser_reset_rx(instance);
        }
    } else {
        res = tbfp_parser_reset_rx(instance);
    }
    return res;
}

static bool tbfp_parser_proc_wait_payload(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
#ifdef X86_64
    printf("\n%s(): rx: 0x%02x loadLen: %u", __FUNCTION__, rx_byte, instance->parser.load_len);
#endif
    if((TBFP_SIZE_HEADER + instance->parser.exp_payload_len) <= instance->parser.load_len) {
        res = tbfp_parser_reset_rx(instance);
    } else {
        instance->parser.rx_frame[instance->parser.load_len] = rx_byte;
        if(instance->parser.load_len == (TBFP_SIZE_HEADER + instance->parser.exp_payload_len - 1)) {
            instance->parser.rx_state = WAIT_CRC;
            res = true;
        } else if(instance->parser.load_len < (TBFP_SIZE_HEADER + instance->parser.exp_payload_len - 1)) {
            instance->parser.rx_state = WAIT_PAYLOAD;
            res = true;
        } else {
            /*Payload OverRun*/
            res = tbfp_parser_reset_rx(instance);
        }
        instance->parser.load_len++;
    }
    return res;
}

static bool tbfp_parser_proc_wait_crc8(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    uint16_t crc8_index = TBFP_SIZE_HEADER + instance->parser.exp_payload_len;
    if(crc8_index == instance->parser.load_len) {
        instance->parser.rx_frame[instance->parser.load_len] = rx_byte; /*read crc8*/
        instance->parser.read_crc8 = rx_byte;
        instance->parser.load_len = 0;
        uint16_t frame_len = TBFP_SIZE_HEADER + instance->parser.exp_payload_len;
        res = crc8_sae_j1850_check(&instance->parser.rx_frame[0], frame_len, instance->parser.read_crc8);
        if(res) {
#ifdef HAS_MCU
            LOG_DEBUG(TBFP, "SN:0x%04x %u crc OK", instance->parser.s_num, instance->parser.s_num);
#endif
            memcpy(instance->parser.fix_frame, instance->parser.rx_frame, TBFP_RX_FRAME_SIZE);
            instance->parser.rx_state = RX_DONE;
            instance->rx_pkt_cnt++;
            res = tbfp_proc_full(instance->parser.fix_frame, frame_len + TBFP_SIZE_CRC, instance->interface);
        } else {
#ifdef HAS_MCU
            LOG_DEBUG(TBFP, "SN:0x%04x %u crc err read:0x%02x", instance->parser.s_num, instance->parser.s_num,
                      instance->parser.read_crc8);
#endif
            instance->crc_err_cnt++;
            res = false; // errors
        }
        res = tbfp_parser_reset_rx(instance);
    } else {
        res = tbfp_parser_reset_rx(instance);
    }
    return res;
}

bool tbfp_proc_byte(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
#ifdef X86_64
    printf("\n%s(): rx: 0x%02x loadLen: %u", __FUNCTION__, rx_byte, instance->parser.load_len);
#endif
    switch(instance->parser.rx_state) {
    case WAIT_PREAMBLE:
        res = tbfp_parser_proc_wait_preamble(instance, rx_byte);
        break;
    case WAIT_LEN:
        res = tbfp_parser_proc_wait_len(instance, rx_byte);
        break;
    case WAIT_SERIAL_NUM:
        res = tbfp_parser_proc_wait_serial_num(instance, rx_byte);
        break;
    case WAIT_PAYLOAD:
        res = tbfp_parser_proc_wait_payload(instance, rx_byte);
        break;
    case WAIT_CRC:
        res = tbfp_parser_proc_wait_crc8(instance, rx_byte);
        break;
    default:
        res = tbfp_parser_reset_rx(instance);
        break;
    }
    return res;
}
