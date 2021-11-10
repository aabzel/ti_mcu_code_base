#ifndef TBFP_PROTOCOL_H
#define TBFP_PROTOCOL_H

/*TBFP Trivial Binary Frame Ðrotocol*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "gnss_utils.h"

typedef enum xFrameId_t {
    FRAME_ID_CMD = 0x44,  /*D*/
    FRAME_ID_CHAT = 0x43, /*C*/
    FRAME_ID_PING = 0x50, /*P*/
    FRAME_ID_PONG = 0x90, /**/
} FrameId_t;

#define TBFP_PREAMBLE 0xA5

#define TBFP_INDEX_PREAMBLE 0
#define TBFP_INDEX_LEN 1
#define TBFP_INDEX_PAYLOAD 2

#define TBFP_SIZE_HEADER 2
#define TBFP_SIZE_ID 1
#define TBFP_SIZE_CRC 1

#define TBFP_MAX_PAYLOAD (255 - TBFP_SIZE_CRC - TBFP_SIZE_HEADER)

typedef struct xTbfHeader_t {
    uint8_t preamble;
    uint8_t len;
} __attribute__((packed)) TbfHeader_t;

typedef struct xTbfPingFrame_t {
    uint8_t id;
    uint64_t mac;
    time_t time_stamp;
    GnssCoordinate_t coordinate;
} __attribute__((packed)) TbfPingFrame_t;

typedef struct xTbfpPorotocol_t {
    uint32_t rx_pkt_cnt;
    uint32_t crc_err_cnt;
    uint32_t err_cnt;
#ifdef HAS_DEBUG
    uint32_t preamble_cnt;
    uint16_t max_len;
    uint16_t min_len;
#endif
} TbfpPorotocol_t;

extern TbfpPorotocol_t TbfpPorotocol;

bool tbfp_send_cmd(uint8_t* tx_array, uint32_t len);
bool tbfp_send_chat(uint8_t* tx_array, uint32_t len);
bool tbfp_send_ping(uint8_t frame_id);
bool tbfp_protocol_init(TbfpPorotocol_t* instance);
bool tbfp_proc(uint8_t* arr, uint16_t len);
bool is_tbfp_protocol(uint8_t* arr, uint16_t len);
bool tbfp_compose_ping(uint8_t* out_frame, uint32_t* tx_frame_len, TbfPingFrame_t* pingFrame);

#ifdef __cplusplus
}
#endif

#endif /* TBFP_PROTOCOL_H */
