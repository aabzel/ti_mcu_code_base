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
    LOG_PARN(TBFP, "ParsePre: %s 0x%02x", interface2str(instance->interface), rx_byte);
    if((instance->parser.preamble_val == rx_byte) && (0 == instance->parser.load_len)) {
        instance->parser.rx_frame[0] = rx_byte;
        instance->parser.rx_state = WAIT_RETX_CNT;
        instance->parser.load_len = 1;
#ifdef HAS_DEBUG
        instance->preamble_cnt++;
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "%s SpotPreamble 0x%x Flow:%u", interface2str(instance->interface), rx_byte,
                  instance->con_flow);
#endif
#endif
        res = true;
    } else {
#ifdef HAS_LOG
        LOG_PARN(TBFP, "ParsePreamErr 0x%x", rx_byte);
#endif
        res = tbfp_parser_reset_rx(instance, WAIT_PREAMBLE);
    }
    return res;
}

static bool tbfp_parser_proc_retransmit_cnt(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    LOG_PARN(TBFP, "ParseReTx: %u", rx_byte);
    if(TBFP_INDEX_RETX == instance->parser.load_len) {
        instance->parser.rx_frame[TBFP_INDEX_RETX] = rx_byte;
        instance->parser.load_len = TBFP_INDEX_RETX + 1;
        LOG_DEBUG(TBFP, "%s ReTxCnt:0x%x", interface2str(instance->interface), rx_byte);
        instance->parser.rx_state = WAIT_SERIAL_NUM;
        res = true;
    } else {
#ifdef HAS_LOG
        LOG_ERROR(TBFP, "ParseReTxErr");
#endif
        res = tbfp_parser_reset_rx(instance, WAIT_RETX_CNT);
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
        LOG_DEBUG(TBFP, "%s SN:%u=0x%04x", interface2str(instance->interface), instance->parser.s_num,
                  instance->parser.s_num);
#endif
        instance->parser.rx_state = WAIT_LEN;
        res = true;
    } else {
#ifdef HAS_LOG
        LOG_DEBUG(TBFP, "ParseSnErr");
#endif
        res = tbfp_parser_reset_rx(instance, WAIT_SERIAL_NUM);
    }
    return res;
}

#ifdef HAS_DEBUG
static bool tbfp_update_len_stat(TbfpProtocol_t* instance, uint16_t payload_len) {
    bool res = true;
    instance->max_len = max16u(instance->max_len, payload_len);
    instance->min_len = min16u(instance->min_len, payload_len);
    return res;
}
#endif /*HAS_DEBUG*/

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
        LOG_DEBUG(TBFP, "%s Len: %u=0x%04x", interface2str(instance->interface), instance->parser.exp_payload_len,
                  instance->parser.exp_payload_len);
#endif
        if(0 < instance->parser.exp_payload_len) {
            if(instance->parser.exp_payload_len <= TBFP_MAX_PAYLOAD) {
                instance->parser.rx_state = WAIT_PAYLOAD;
                res = true;
            } else {
                instance->len_err_cnt++;
#ifdef HAS_LOG
                LOG_DEBUG(TBFP, "%s TooBigData %u Byte", interface2str(instance->interface),
                          instance->parser.exp_payload_len);
#endif
                res = tbfp_parser_reset_rx(instance, WAIT_LEN);
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
        res = tbfp_parser_reset_rx(instance, WAIT_LEN);
    }
    return res;
}

static bool tbfp_parser_proc_wait_payload(TbfpProtocol_t* instance, uint8_t rx_byte) {
    bool res = false;
    LOG_PARN(TBFP, "%s ParsePayLoad Data[%u]=%u=0x%02x", interface2str(instance->interface),
             instance->parser.load_len - TBFP_SIZE_HEADER, rx_byte, rx_byte);
    if((TBFP_SIZE_HEADER + instance->parser.exp_payload_len) <= instance->parser.load_len) {
        res = tbfp_parser_reset_rx(instance, WAIT_PAYLOAD);
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
            res = tbfp_parser_reset_rx(instance, WAIT_PAYLOAD);
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
    LOG_PARN(TBFP, "ProcCrc8");
    uint16_t crc8_index = TBFP_SIZE_HEADER + instance->parser.exp_payload_len;
    if(crc8_index == instance->parser.load_len) {
        instance->parser.rx_frame[instance->parser.load_len] = rx_byte; /*read crc8*/
        instance->parser.read_crc8 = rx_byte;
        instance->parser.load_len = 0;
        uint16_t frame_len = TBFP_SIZE_HEADER + instance->parser.exp_payload_len;
        res = crc8_sae_j1850_check(&instance->parser.rx_frame[0], frame_len, instance->parser.read_crc8);
        if(res) {
#ifdef HAS_LOG
            LOG_DEBUG(TBFP, "%s SN:%u=0x%04x Crc8=0x02x Ok! Flow %u", interface2str(instance->interface),
                      instance->parser.s_num, instance->parser.s_num, instance->parser.read_crc8, instance->con_flow);
            // led_blink(&Led[LED_INDEX_RED], 10);
#endif
#ifdef HAS_DEBUG
            tbfp_update_len_stat(instance, instance->parser.exp_payload_len);
#endif

            memcpy(instance->parser.fix_frame, instance->parser.rx_frame, frame_len + TBFP_SIZE_CRC);
            instance->parser.rx_state = RX_DONE;
            instance->rx_pkt_cnt++;
            res = tbfp_parser_reset_rx(instance, WAIT_CRC);
            res = tbfp_proc_full(instance->parser.fix_frame, frame_len + TBFP_SIZE_CRC, instance->interface);
        } else {
            instance->crc_err_cnt++;
#ifdef HAS_LOG
            LOG_DEBUG(TBFP, "%s SN:%u=0x%04x Crc8Err read:0x%02x RxPayLen:%u", interface2str(instance->interface),
                      instance->parser.s_num, instance->parser.s_num, instance->parser.read_crc8,
                      instance->parser.exp_payload_len);
#endif
            res = false; // errors
        }
        res = tbfp_parser_reset_rx(instance, WAIT_CRC);
    } else {
#ifdef HAS_LOG
        LOG_ERROR(TBFP, "ParseCrc8Err");
#endif
        res = tbfp_parser_reset_rx(instance, WAIT_CRC);
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
        res = tbfp_parser_reset_rx(instance, WAIT_UNDEF);
        break;
    }
    if(false == res) {
        LOG_ERROR(TBFP, "ParseByteErr %u", rx_byte);
    }
    return res;
}
