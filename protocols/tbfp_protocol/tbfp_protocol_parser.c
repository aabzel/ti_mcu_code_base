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
#ifdef HAS_LOG
#include "log.h"
#endif

static bool tbfp_parser_proc_wait_preamble(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    if((TBFP_PREAMBLE == rx_byte) && (0==instance->parser.load_len)) {
        instance->parser.rx_frame[0] = rx_byte;
        instance->parser.rx_state = WAIT_RETX_CNT;
        instance->parser.load_len = 1;
#ifdef HAS_DEBUG
        instance->preamble_cnt++;
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "Preamble");
#endif
#endif
        res = true;
    } else {
#ifdef HAS_LOG
        LOG_PARN(TBFP, "ParsePreamErr 0x%x", rx_byte);
#endif
        res = tbfp_parser_reset_rx(instance);
    }
    return res;
}
static bool tbfp_parser_proc_retransmit_cnt(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
#ifdef X86_64
    LOG_PARN(TBFP, "ParseReTx: %u", rx_byte);
#endif
    if(TBFP_INDEX_RETX == instance->parser.load_len) {
        instance->parser.rx_frame[TBFP_INDEX_RETX] = rx_byte;
        instance->parser.load_len = TBFP_INDEX_RETX + 1;
        instance->parser.rx_state = WAIT_SERIAL_NUM;
        res = true;
    } else {
#ifdef HAS_LOG
        LOG_ERROR(TBFP, "ParseReTxErr");
#endif
        res = tbfp_parser_reset_rx(instance);
    }
    return res;
}

static bool tbfp_parser_proc_wait_serial_num(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    LOG_PARN(TBFP, "ParseSn [%u]=0x%02x", instance->parser.load_len, rx_byte);
    if(TBFP_INDEX_SER_NUM == instance->parser.load_len) {
        instance->parser.rx_frame[TBFP_INDEX_SER_NUM] = rx_byte;
        instance->parser.load_len = TBFP_INDEX_SER_NUM + 1;
        instance->parser.rx_state = WAIT_SERIAL_NUM;
        res = true;
    } else if((TBFP_INDEX_SER_NUM + 1) == instance->parser.load_len) {
        instance->parser.rx_frame[TBFP_INDEX_SER_NUM + 1] = rx_byte;
        instance->parser.s_num = 0;
        memcpy(&instance->parser.s_num, &instance->parser.rx_frame[TBFP_INDEX_SER_NUM], TBFP_SIZE_SN);
        instance->parser.load_len = TBFP_INDEX_SER_NUM + 2;
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "SN:0x%04x %u", instance->parser.s_num, instance->parser.s_num);
#endif
        instance->parser.rx_state = WAIT_LEN;
        res = true;
    } else {
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "ParseSnErr");
#endif
        res = tbfp_parser_reset_rx(instance);
    }
    return res;
}

static bool tbfp_parser_proc_wait_len(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    if(TBFP_INDEX_LEN == instance->parser.load_len) {
        instance->parser.rx_frame[TBFP_INDEX_LEN] = rx_byte;
        instance->parser.load_len = TBFP_INDEX_LEN + 1;
        instance->parser.rx_state = WAIT_LEN;
        res = true;
    } else if((TBFP_INDEX_LEN + 1) == instance->parser.load_len) {
        instance->parser.rx_frame[TBFP_INDEX_LEN + 1] = rx_byte;
        instance->parser.load_len = TBFP_INDEX_LEN + TBFP_SIZE_LEN;
        memcpy(&(instance->parser.exp_payload_len), &(instance->parser.rx_frame[TBFP_INDEX_LEN]), TBFP_SIZE_LEN);
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "Len:0x%04x %u", instance->parser.exp_payload_len, instance->parser.exp_payload_len);
#endif
#ifdef HAS_DEBUG
        instance->max_len = max16u(instance->max_len, instance->parser.exp_payload_len);
        instance->min_len = min16u(instance->min_len, instance->parser.exp_payload_len);
#endif
        if(0 < instance->parser.exp_payload_len) {
            if(instance->parser.exp_payload_len <= TBFP_MAX_PAYLOAD) {
                instance->parser.rx_state = WAIT_PAYLOAD;
                res = true;
            } else {
                instance->err_cnt++;
#ifdef HAS_LOG
                LOG_ERROR(TBFP, "TooBigData %u Byte", instance->parser.exp_payload_len);
#endif
                res = tbfp_parser_reset_rx(instance);
            }

        } else {
            // 0 == instance->parser.exp_payload_len
            instance->parser.rx_state = WAIT_CRC;
            res = true;
        }
    } else {
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "ParseLenErr");
#endif
        res = tbfp_parser_reset_rx(instance);
    }
    return res;
}

static bool tbfp_parser_proc_wait_payload(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    LOG_PARN(TBFP, "ParsePayLoad Data[%u]=0x%02x", instance->parser.load_len - TBFP_SIZE_HEADER, rx_byte);
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
#ifdef HAS_LOG
            LOG_ERROR(TBFP, "PayLoadOverRun");
#endif
            res = tbfp_parser_reset_rx(instance);
        }
        instance->parser.load_len++;
    }
    if(false == res) {
        LOG_ERROR(TBFP, "ParsePayLoad Data[%u]=0x%02x", instance->parser.load_len - TBFP_SIZE_HEADER, rx_byte);
    }
    return res;
}

static bool tbfp_parser_proc_wait_crc8(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
#ifdef X86_64
    LOG_PARN("\n%s():", __FUNCTION__);
#endif
    uint16_t crc8_index = TBFP_SIZE_HEADER + instance->parser.exp_payload_len;
    if(crc8_index == instance->parser.load_len) {
        instance->parser.rx_frame[instance->parser.load_len] = rx_byte; /*read crc8*/
        instance->parser.read_crc8 = rx_byte;
        instance->parser.load_len = 0;
        uint16_t frame_len = TBFP_SIZE_HEADER + instance->parser.exp_payload_len;
        res = crc8_sae_j1850_check(&instance->parser.rx_frame[0], frame_len, instance->parser.read_crc8);
        if(res) {
#ifdef HAS_LOG
            LOG_DEBUG(TBFP, "SN:0x%04x %u Crc8 Ok!", instance->parser.s_num, instance->parser.s_num);
            // led_blink(&Led[LED_INDEX_RED], 10);
#endif
            memcpy(instance->parser.fix_frame, instance->parser.rx_frame, TBFP_MAX_FRAME);
            instance->parser.rx_state = RX_DONE;
            instance->rx_pkt_cnt++;
            res = tbfp_proc_full(instance->parser.fix_frame, frame_len + TBFP_SIZE_CRC, instance->interface);
        } else {
#ifdef HAS_LOG
            LOG_ERROR(TBFP, "SN:0x%04x %u crc err read:0x%02x", instance->parser.s_num, instance->parser.s_num,
                      instance->parser.read_crc8);
#endif
            instance->crc_err_cnt++;
            res = false; // errors
        }
        res = tbfp_parser_reset_rx(instance);
    } else {
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "ParseCrc8Err");
#endif
        res = tbfp_parser_reset_rx(instance);
    }
    return res;
}

bool tbfp_proc_byte(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    LOG_PARN(TBFP, "ProcByte: [%u]=0x%02x", instance->parser.load_len, rx_byte);
    switch(instance->parser.rx_state) {
    case WAIT_PREAMBLE:
        res = tbfp_parser_proc_wait_preamble(instance, rx_byte);
        break;
    case WAIT_RETX_CNT:
        res = tbfp_parser_proc_retransmit_cnt(instance, rx_byte);
        break;
    case WAIT_SERIAL_NUM:
        res = tbfp_parser_proc_wait_serial_num(instance, rx_byte);
        break;
    case WAIT_LEN:
        res = tbfp_parser_proc_wait_len(instance, rx_byte);
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
    if(false == res) {
        LOG_ERROR(TBFP, "ParseByteErr %u", rx_byte);
    }
    return res;
}
