#include "tbfp_protocol.h"
/*FSM for parsing TBFP protocol from byte stream*/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef X86_64
#include <stdio.h>
#endif

#include "data_utils.h"
#include "crc8_sae_j1850.h"

static bool tbfp_parser_reset_rx(TbfpProtocol_t *instance){
    bool res = false;
    if (instance) {
        instance->parser.rx_state = WAIT_PREAMBLE;
        instance->parser.load_len = 0;
        res = true;
    }
    return res;
}

static bool tbfp_parser_proc_wait_preamble(TbfpProtocol_t *instance, uint8_t rx_byte){
    bool res = false;
    if (TBFP_PREAMBLE == rx_byte) {
        instance->parser.rx_state = WAIT_LEN;
        instance->parser.rx_frame[0] = rx_byte;
        instance->parser.load_len = 1;
#ifdef HAS_DEBUG
        instance->preamble_cnt++;
#endif
        res = true;
    }else{
        res = tbfp_parser_reset_rx(instance);
    }
    return res;
}

static bool tbfp_parser_proc_wait_len(TbfpProtocol_t *instance, uint8_t rx_byte)   {
    bool res = false;
    if(1==instance->parser.load_len){
        instance->parser.exp_payload_len = rx_byte;
        instance->parser.rx_frame[1] = rx_byte;
        instance->parser.load_len = 2;
#ifdef HAS_DEBUG
        instance->max_len = max16u(instance->max_len, instance->parser.exp_payload_len);
        instance->min_len = min16u(instance->min_len, instance->parser.exp_payload_len);
#endif
        if (0 < instance->parser.exp_payload_len) {
            instance->parser.rx_state = WAIT_PAYLOAD;
        } else {
            instance->parser.rx_state = WAIT_CRC;
        }
        res = true;
    } else {
        res = tbfp_parser_reset_rx(instance);
    }
    return res;
}

static bool tbfp_parser_proc_wait_payload(TbfpProtocol_t *instance, uint8_t rx_byte){
    bool res = false;
#ifdef X86_64
    printf("\n%s(): rx: 0x%02x loadLen: %u", __FUNCTION__, rx_byte,instance->parser.load_len);
#endif
    if((TBFP_HEADER_SIZE + instance->parser.exp_payload_len) <= instance->parser.load_len){
        res = tbfp_parser_reset_rx(instance);
    } else {
        instance->parser.rx_frame[instance->parser.load_len] = rx_byte;
        if(instance->parser.load_len  == (TBFP_HEADER_SIZE + instance->parser.exp_payload_len - 1)){
            instance->parser.rx_state = WAIT_CRC;
            res = true;
        }else if( instance->parser.load_len <(TBFP_HEADER_SIZE+ instance->parser.exp_payload_len )){
            instance->parser.rx_state = WAIT_PAYLOAD;
            res = true;
        }
        instance->parser.load_len++;
    }
    return res;
}

static bool tbfp_parser_proc_wait_crc8(TbfpProtocol_t *instance, uint8_t rx_byte)   {
    bool res = false;
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    uint16_t crc8_index = TBFP_HEADER_SIZE + instance->parser.exp_payload_len;
    if(crc8_index == instance->parser.load_len){
        instance->parser.rx_frame[instance->parser.load_len] = rx_byte;
        instance->parser.read_crc8 = rx_byte;
        instance->parser.load_len++;
        uint16_t frame_len = TBFP_HEADER_SIZE + instance->parser.exp_payload_len;
        res =  crc8_sae_j1850_check(&instance->parser.rx_frame[0], frame_len, instance->parser.read_crc8);
        if (res) {
            memcpy(instance->parser.fix_frame, instance->parser.rx_frame, TBFP_RX_FRAME_SIZE);
            instance->parser.rx_state = RX_DONE;
            instance->rx_pkt_cnt++;
            res = tbfp_proc(instance->parser.fix_frame,   frame_len+TBFP_CRC_SIZE, instance->interface);
        } else {
            res = false;
        }
        res = tbfp_parser_reset_rx(instance);
    } else {
        res = tbfp_parser_reset_rx(instance);
    }
    return res;
}

bool tbfp_proc_byte(TbfpProtocol_t *instance, uint8_t rx_byte){
    bool res = false;
#ifdef X86_64
    printf("\n%s(): rx: 0x%02x loadLen: %u", __FUNCTION__, rx_byte,instance->parser.load_len);
#endif
    switch(instance->parser.rx_state){
     case WAIT_PREAMBLE:
         res = tbfp_parser_proc_wait_preamble(instance, rx_byte);
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
    return res;
}
