#ifndef UBX_PROTOCOL_H
#define UBX_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define UBX_SYN_0 0xB5 /* u*/
#define UBX_SYN_1 0x62 /* b*/

#define UBX_CLS_INDEX 2U

#define UBX_RX_FRAME_SIZE 100U
#define UBX_LEN_SIZE 2
#define UBX_CRC_SIZE 2
#define UBX_HEADER_SIZE (UBX_LEN_SIZE + 4)

/*UBX messages overview*/
#define UBX_CLA_NAV 0x01
#define UBX_CLA_RXM 0x02
#define UBX_CLA_INF 0x04
#define UBX_CLA_ACK 0x05
#define UBX_CLA_CFG 0x06
#define UBX_CLA_UPD 0x09
#define UBX_CLA_MON 0x0A
#define UBX_CLA_TIM 0x0D
#define UBX_CLA_ESF 0x10
#define UBX_CLA_MGA 0x13
#define UBX_CLA_SEC 0x27

#define UBX_CLA_CNT 15

typedef enum eUbxProtState_t {
    UBX_WAIT_SYC0 = 0U,
    UBX_WAIT_SYC1 = 1U,
    UBX_WAIT_CLASS = 2U,
    UBX_WAIT_ID = 3U,
    UBX_WAIT_LEN = 4U,
    UBX_WAIT_PAYLOAD = 5U,
    UBX_WAIT_CRC = 6U,
    UBX_RX_DONE = 7U
} UbxProtState_t;

typedef struct xUbloxPorotocol_t {
    uint32_t rx_pkt_cnt;
    uint32_t crc_err_cnt;
    uint32_t load_len;
    uint16_t exp_len;
    uint16_t read_crc;
    uint8_t rx_state;
    uint8_t rx_frame[UBX_RX_FRAME_SIZE];
    uint8_t fix_frame[UBX_RX_FRAME_SIZE];
    bool unproc_frame;
} UbloxPorotocol_t;

typedef struct xStatClass_t {
    uint8_t class_val;
    uint16_t cnt;
} StatClass_t;

extern UbloxPorotocol_t UbloxPorotocol;
extern StatClass_t tableRxClass[UBX_CLA_CNT];

bool ubx_reset_rx(void);
bool ublox_protocol_init(void);
uint16_t ubx_calc_crc16(uint8_t* array, uint16_t len);
bool ubx_proc_byte(uint8_t rx_byte);
bool ubx_update_stat(uint8_t val_class);

#ifdef __cplusplus
}
#endif

#endif /* UBX_PROTOCOL_H */
