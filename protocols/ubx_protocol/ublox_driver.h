#ifndef U_BLOX_DRIVER_H
#define U_BLOX_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#ifdef HAS_MCU
#include "clocks.h"
#endif
#include "data_utils.h"
#include "gnss_utils.h"
#include "ubx_protocol.h"

#ifdef HAS_MCU
#define UBX_UART_NUM UART_NUM_ZED_F9P
#define UBX_PERIOD_US S_2_US(3)
#define UBX_RX_TIME_OUT_MS S_2_MS(15)
#endif

typedef enum eReceiverMode_t{
  MODE_DISABLED = 0x00,
  MODE_SURVEY_IN=0x01,
  MODE_FIXED = 0x02,
}ReceiverMode_t;

/*UBX data types*/
typedef enum eUbxType_t {
    UBX_U1 = 1,
    UBX_U2 = 2,
    UBX_U4 = 3,
    UBX_X1 = 4,
    UBX_X2 = 5,
    UBX_X4 = 6,
    UBX_I1 = 7,
    UBX_I2 = 8,
    UBX_I4 = 9,
    UBX_R4 = 10,
    UBX_R8 = 11,
    UBX_CH = 12,
    UBX_L = 13,
} UbxType_t;

#if 0
#define UBX_KEY_CNT 1
#endif
typedef struct xkeyValItem_t {
    uint32_t key_id;
    Type64Union_t u_value;
#if 0
    uint8_t type;
#endif
} keyValItem_t;
/*
typedef struct xVariable_t{
 char name[];
 AllTypeUnion_t u_value
 uint8_t type;
}Variable_t;
**/

/*Variables that matter for the application*/
typedef struct xNavInfo_t {
    struct tm date_time;
    GnssCoordinate_t coordinate;
    GnssVelocity_t velocity;
    uint8_t id[5];
    int32_t hmsl;
    uint32_t h_acc;
    uint32_t v_acc;
    int32_t roll;
    int32_t pitch;
    int32_t heading;
    uint32_t acc_roll;
    uint32_t acc_pitch;
    uint32_t acc_heading;

    /*velocity*/
} NavInfo_t;

extern NavInfo_t NavInfo;

bool ubx_proc(void);
bool ubx_send_message(uint8_t class_num, uint8_t id, uint8_t* payload, uint16_t len);
bool ubx_driver_init(void);
bool ubx_cfg_set_val(uint32_t key_id, uint8_t* val, uint16_t val_len, uint8_t layers);
bool ubx_cfg_get_val(uint32_t key_id, uint8_t layers);
bool ubx_proc_frame(UbloxPorotocol_t* inst);
#ifdef __cplusplus
}
#endif

#endif /* U_BLOX_DRIVER_H */
