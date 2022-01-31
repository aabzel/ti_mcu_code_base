#ifndef TBFP_PROTOCOL_H
#define TBFP_PROTOCOL_H

/*TBFP Trivial Binary Frame Protocol*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "tbfp_constants.h"
#include "gnss_utils.h"
#include "system.h"
#include "tbfp_protocol_parser.h"


typedef struct xTbfHeader_t {
    uint8_t preamble;
#ifdef HAS_TBFP_RETRANSMIT
    uint8_t lifetime; /*For mesh feature*/
#endif
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
    uint32_t rx_pkt_cnt;
    uint32_t tx_pkt_cnt;
    uint32_t crc_err_cnt;
    uint32_t err_cnt;
#ifdef HAS_DEBUG
    uint32_t preamble_cnt;
    uint16_t max_len;
    uint16_t min_len;
    bool debug;
#endif
#ifdef HAS_TBFP_FLOW_CONTROL
    uint16_t prev_s_num;
    uint16_t s_num;
    uint16_t con_flow;
    uint16_t max_con_flow;
#endif
    uint8_t tx_frame[TBFP_MAX_FRAME]; //
    Interfaces_t interface;
    TBFTparser_t parser;
} TbfpProtocol_t;

extern TbfpProtocol_t TbfpProtocol[IF_CNT]; /*RS232 LoRa*/

bool tbfp_check_flow_control(
                             Interfaces_t interface,
                             uint16_t snum,
                             uint16_t *const prev_s_num,
                             uint16_t *const con_flow,
                             uint16_t *const max_con_flow
                             ) ;
bool tbfp_generate_frame(uint8_t* array, uint32_t len);
bool tbfp_parser_reset_rx(TbfpProtocol_t* instance, RxState_t state);
bool tbfp_send(uint8_t* tx_array, uint32_t len, Interfaces_t interface, uint8_t lifetime);
bool tbfp_send_tunnel(uint8_t* tx_array, uint32_t len, Interfaces_t interface);
bool tbfp_send_cmd(uint8_t* tx_array, uint32_t len, Interfaces_t interface);
bool tbfp_send_chat(uint8_t* tx_array, uint32_t len, Interfaces_t interface, uint8_t lifetime);
bool tbfp_send_ping(uint8_t frame_id, Interfaces_t interface);
bool tbfp_protocol_init(TbfpProtocol_t* instance, Interfaces_t interface);
bool tbfp_proc(uint8_t* arr, uint16_t len, Interfaces_t interface, bool is_reset);
bool tbfp_proc_full(uint8_t* arr, uint16_t len, Interfaces_t interface);
bool tbfp_proc_byte(TbfpProtocol_t* instance, uint8_t rx_byte);
bool is_tbfp_protocol(uint8_t* arr, uint16_t len);
bool tbfp_compose_ping(uint8_t* out_frame, uint32_t* tx_frame_len, TbfPingFrame_t* pingFrame, Interfaces_t interface);

#ifdef __cplusplus
}
#endif

#endif /* TBFP_PROTOCOL_H */
