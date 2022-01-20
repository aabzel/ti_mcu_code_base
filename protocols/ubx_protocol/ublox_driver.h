#ifndef U_BLOX_DRIVER_H
#define U_BLOX_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#ifdef HAS_CLOCK
#include "clocks.h"
#endif
#include "data_utils.h"
#include "gnss_utils.h"
#include "ubx_protocol.h"

#ifndef HAS_UART
#error "Ublox driver requires UART driver"
#endif


#define UBX_PERIOD_US S_2_US(3)
#ifdef HAS_UART1
#define UBX_SEND_TRY 15
#define UBX_SEND_TIME_OUT_MS 2000
#define UBX_RX_TIME_OUT_MS S_2_MS(15)

#ifdef UART_NUM_ZED_F9P
#define UBX_UART_NUM UART_NUM_ZED_F9P
#endif

#endif

typedef enum eReceiverMode_t {
    MODE_DISABLED = 0x00,
    MODE_SURVEY_IN = 0x01,
    MODE_FIXED = 0x02,
} ReceiverMode_t;

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

typedef enum eSatConst_t{
    SC_GPS=0,
    SC_GLONASS=1,
    SC_GALILEO=2,
    SC_BEI_DOU=3,
    SC_NONE=4,
}SatConst_t;


#if 0
#define UBX_KEY_CNT 1
#endif
typedef struct xkeyValItem_t {
    uint32_t key_id;
    Type64Union_t u_value;
    SatConst_t satellite_constellation;
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
bool ubx_send_message_ack(uint8_t class_num, uint8_t id, uint8_t* payload, uint16_t len);
bool ubx_send_message(uint8_t class_num, uint8_t id, uint8_t* payload, uint16_t len);
bool ubx_driver_init(void);
bool ubx_cfg_set_val(uint32_t key_id, uint8_t* val, uint16_t val_len, uint8_t layers);
bool ubx_cfg_get_val(uint32_t key_id, uint8_t layers);
bool ubx_proc_frame(UbloxProtocol_t* inst);
bool ubx_reset_to_dflt(void);
bool ubx_set_rate(uint16_t meas_rate_ms, uint16_t time_ref);
#ifdef __cplusplus
}
#endif

#endif /* U_BLOX_DRIVER_H */
