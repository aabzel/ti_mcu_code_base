#ifndef U_BLOX_DRIVER_H
#define U_BLOX_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define SYNC_CHAR_1 0xB5 /*u*/
#define SYNC_CHAR_2 0x62 /*B*/
#define UBX_CRC_LEN 2U
#define UBX_HEADER_LEN 6U

bool ubx_send_message(uint8_t class_num, uint8_t id, uint8_t* payload,
                      uint16_t len);
#if 0
bool ubolx_proc_frame(T_ubx_msg* packet);
#endif

#ifdef __cplusplus
}
#endif

#endif /* U_BLOX_DRIVER_H */
