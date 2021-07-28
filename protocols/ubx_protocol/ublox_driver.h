#ifndef U_BLOX_DRIVER_H
#define U_BLOX_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "data_utils.h"

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
    UBX_CH = 12
} UbxType_t;

#define UBX_KEY_CNT 1

typedef struct xkeyValItem_t {
    uint32_t key_id;
    AllTypeUnion_t u_value;
    uint8_t type;
} keyValItem_t;


/*
typedef struct xVariable_t{
 char name[];
 AllTypeUnion_t u_value
 uint8_t type;
}Variable_t;
**/

/*Variables that matter for the application*/
typedef struct xNavInfo_t{
    int32_t latitude;
    int32_t longitude;
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
}xNavInfo_t;

extern xNavInfo_t NavInfo;
extern keyValItem_t keyValTable[UBX_KEY_CNT];

bool ubx_send_message(uint8_t class_num, uint8_t id, uint8_t* payload, uint16_t len);
bool ubx_proc_frame(void);
bool ubx_driver_init(void);

#ifdef __cplusplus
}
#endif

#endif /* U_BLOX_DRIVER_H */
