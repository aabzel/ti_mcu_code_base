#ifndef TBFP_PROTOCOL_H
#define TBFP_PROTOCOL_H

/*TBFP Trivial Binary Frame �rotocol*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "gnss_utils.h"
#include "system.h"
#include "tbfp_protocol_parser.h"

typedef enum xFrameId_t {
    FRAME_ID_CMD = 0x44,   /*D*/
    FRAME_ID_CHAT = 0x43,  /*C*/
    FRAME_ID_PING = 0x50,  /*P*/
    FRAME_ID_PONG = 0x90,  /**/
    FRAME_ID_RTCM3 = 0xD3, /**/
} FrameId_t;

#define TBFP_PREAMBLE 0xA5

#define TBFP_INDEX_PREAMBLE 0
#define TBFP_INDEX_LEN 1
#define TBFP_INDEX_SER_NUM 2
#define TBFP_INDEX_PAYLOAD 4

#define TBFP_SIZE_ID 1
#define TBFP_OVERHEAD_SIZE (1+1+2+1)
//#define TBFP_MAX_PAYLOAD (256 - TBFP_SIZE_CRC - TBFP_SIZE_HEADER)

typedef struct xTbfHeader_t {
    uint8_t preamble;
    uint8_t len;
#ifdef HAS_TBFP_FLOW_CONTROL
    uint16_t snum; /* serial number of the frame. For flow controll.*/
#endif
} __attribute__((__packed__)) TbfHeader_t;

typedef struct xTbfPingFrame_t {
    uint8_t id;
    uint64_t mac;
    time_t time_stamp;
    GnssCoordinate_t coordinate;
} __attribute__((packed)) TbfPingFrame_t;

typedef struct xTbfpProtocol_t {
    uint32_t rx_pkt_cnt;
    uint32_t tx_pkt_cnt;
    uint32_t crc_err_cnt;
    uint32_t err_cnt;
#ifdef HAS_DEBUG
    uint32_t preamble_cnt;
    uint16_t max_len;
    uint16_t min_len;
#endif
#ifdef HAS_TBFP_FLOW_CONTROL
    uint16_t prev_s_num;
    uint16_t s_num;
    uint16_t con_flow;
    uint16_t max_con_flow;
#endif
    Interfaces_t interface;
    TBFTparser_t parser;
} TbfpProtocol_t;

extern TbfpProtocol_t TbfpProtocol[3]; /*RS232 LoRa*/

bool tbfp_parser_reset_rx(TbfpProtocol_t* instance);
bool tbfp_send(uint8_t* tx_array, uint32_t len, Interfaces_t interface);
bool tbfp_send_cmd(uint8_t* tx_array, uint32_t len, Interfaces_t interface);
bool tbfp_send_chat(uint8_t* tx_array, uint32_t len, Interfaces_t interface);
bool tbfp_send_ping(uint8_t frame_id, Interfaces_t interface);
bool tbfp_protocol_init(TbfpProtocol_t* instance, Interfaces_t interface);
bool tbfp_proc(uint8_t* arr, uint16_t len, Interfaces_t interface);
bool tbfp_proc_full(uint8_t* arr, uint16_t len, Interfaces_t interface);
bool tbfp_proc_byte(TbfpProtocol_t* instance, uint8_t rx_byte);
bool is_tbfp_protocol(uint8_t* arr, uint16_t len);
bool tbfp_compose_ping(uint8_t* out_frame, uint32_t* tx_frame_len, TbfPingFrame_t* pingFrame, Interfaces_t interface);

#ifdef __cplusplus
}
#endif

#endif /* TBFP_PROTOCOL_H */
