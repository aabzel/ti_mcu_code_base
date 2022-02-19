#ifndef TBFP_TYPES_H
#define TBFP_TYPES_H

/*TBFP Trivial Binary Frame Protocol*/
#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "data_utils.h"
#include "gnss_types.h"
#include "tbfp_protocol_parser.h"
#include "tbfp_re_tx_ack_fsm_types.h"
#include "tbfp_constants.h"
#include "system.h"

typedef union uTbfpFlags_t{
    uint8_t byte;
     struct {
        uint8_t lifetime :4; /*bit 0-3*/
        uint8_t reserved:3;  /*bit 4-6*/
        uint8_t ack_need:1;  /*bit 7*/
    };
}TbfpFlags_t;

typedef struct xTbfHeader_t {
    uint8_t preamble;
    TbfpFlags_t flags; /*For ack, mesh feature (lifetime)*/
#ifdef HAS_TBFP_FLOW_CONTROL
    uint16_t snum; /* serial number of the frame. For flow controll.*/
#endif
    uint16_t len;
} __attribute__((packed)) TbfHeader_t;

typedef struct xTbfPingFrame_t {
    uint8_t id;
    uint64_t mac;
    time_t time_stamp;
    GnssCoordinate_t coordinate;
} __attribute__((packed)) TbfPingFrame_t;


typedef struct xTbfpProtocol_t {
    uint32_t rx_byte;
    uint32_t tx_byte;
    uint32_t rx_byte_prev;
    uint32_t tx_byte_prev;
    uint32_t rx_pkt_cnt;
    uint32_t tx_pkt_cnt;
    uint32_t len_err_cnt;
    uint32_t crc_err_cnt;
    uint32_t crc_err_cnt_prev;
    uint32_t err_cnt;
    uint32_t err_tx_cnt;
    uint32_t err_tx_cnt_prev;
    U32Value_t tx_rate;
    U32Value_t rx_rate;
    uint32_t lack_frame_in_data;
#ifdef HAS_DEBUG
    uint32_t preamble_cnt;
    uint16_t max_len;
    uint16_t min_len;
    bool debug;
#endif
#ifdef HAS_TBFP_FLOW_CONTROL
    uint32_t lost_rx_frames;
    uint16_t flow_torn_cnt;
    uint16_t flow_torn_cnt_prev;
    uint16_t prev_s_num;
    uint16_t s_num;
    uint32_t con_flow;
    uint32_t max_con_flow;
#endif
    TbfpReTxFsm_t ReTxFsm;
    Interfaces_t interface;
    TBFTparser_t parser;
    uint8_t tx_frame[TBFP_MAX_FRAME];/*TODO: Make dynamic memory alloc due to low memory shortage */
    uint32_t tx_frame_len;
} TbfpProtocol_t;

#ifdef __cplusplus
}
#endif

#endif /* TBFP_TYPES_H */
