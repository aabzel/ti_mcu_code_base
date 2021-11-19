#ifndef RTCM3_PROTOCOL_H
#define RTCM3_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define RTCM3_PREAMBLE 0xD3
#define RTCM3_RX_FRAME_SIZE 256U
#define RTCM3_HEADER_SIZE 3U
#define RTCM3_CRC24_SIZE 3U

typedef enum eRtcm3IfCmt_t {
    RT_UART_ID = 0,
    RT_LORA_ID = 1,
    RTCM_IF_CNT = 2,
    RT_BOTH_ID = 3,
} Rtcm3IfCmt_t;

typedef enum eRtcm3ProtState_t {
    RTCM3_WAIT_PREAMBLE = 0U,
    RTCM3_WAIT_LEN = 1U,
    RTCM3_WAIT_PAYLOAD = 2U,
    RTCM3_WAIT_CRC = 3U,
    RTCM3_RX_DONE = 4U
} Rtcm3ProtState_t;

typedef struct sRtcm3LenBit_t {
    uint16_t len : 10;
    uint16_t res : 6;
} Rtcm3LenBit_t;

typedef union uRtcm3Len_t {
    uint16_t len16;
    uint8_t len8[2];
    Rtcm3LenBit_t field;
} Rtcm3Len_t;

typedef struct xRtcm3Header_t {
    uint8_t preamble;
    Rtcm3Len_t ex_len;
} __attribute__((packed)) Rtcm3Header_t;

typedef struct xRtcm3Porotocol_t {
    uint32_t rx_pkt_cnt;
    uint32_t rx_rx_pkt_cnt;
    uint32_t lora_lost_pkt_cnt;
    uint32_t uart_lost_pkt_cnt;
    uint32_t crc_err_cnt;
    uint32_t load_len;
    uint32_t err_cnt;
#ifdef HAS_DEBUG
    uint32_t preamble_cnt;
    uint16_t max_len;
    uint16_t min_len;
#endif
    Rtcm3Len_t exp_len;
    uint32_t read_crc;
    uint16_t ack_cnt;
    Rtcm3ProtState_t rx_state;
    uint8_t rx_frame[RTCM3_RX_FRAME_SIZE];
    uint8_t fix_frame[RTCM3_RX_FRAME_SIZE];
    uint8_t interface;
    bool lora_fwd;
} Rtcm3Porotocol_t;

extern Rtcm3Porotocol_t Rtcm3Porotocol[RTCM_IF_CNT];

bool rtcm3_protocol_init(Rtcm3Porotocol_t* instance, uint8_t interface, bool lora_fwd);
bool rtcm3_proc_byte(Rtcm3Porotocol_t* instance, uint8_t rx_byte);
bool rtcm3_reset_rx(Rtcm3Porotocol_t* instance);
bool is_rtcm3_frame(uint8_t* arr, uint16_t len);
bool rtcm3_proc_array(uint8_t* const payload, uint32_t size, Rtcm3IfCmt_t interface);

#ifdef __cplusplus
}
#endif

#endif /* RTCM3_PROTOCOL_H */
