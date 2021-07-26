
#include <stdbool.h>
#include <string.h>

#include "uart_drv.h"
#include "ublox_driver.h"

bool ubx_send_message(uint8_t class_num,
                      uint8_t id,
                      uint8_t* payload,
                      uint16_t len) {
    bool res = true;
    static uint8_t tx_array[256] = {0};
    uint16_t tx_array_len = 0U;
    uint16_t crc16 = 0U;
    tx_array[0] = SYNC_CHAR_1;
    tx_array[1] = SYNC_CHAR_2;
    tx_array[2] = class_num;
    tx_array[3] = id;
    memcpy(&tx_array[4], &len, 2);
    if ((0 < len) && (NULL != payload)) {
        memcpy(&tx_array[6], payload, len);
    }
    tx_array_len = len + UBX_HEADER_LEN + UBX_CRC_LEN;
    crc16 = ubx_calc_crc16(&tx_array[2], len + 4);
    memcpy(&tx_array[UBX_HEADER_LEN + len], &crc16, UBX_CRC_LEN);
    if (true == res) {
        uart_send(1,tx_array, tx_array_len);
    }
    return res;
}

#if 0
bool ubolx_proc_frame(T_ubx_msg* packet) {
    bool res = false;
    switch(packet->clas){
      case UBX_MSG_HNR: 
        break;
      case UBX_MSG_ACK: 
        break;
      case UBX_MSG_CFG: 
        break;
      case UBX_MSG_MON: 
        break;
      default:
      break;
    }
    return res;
}
#endif
