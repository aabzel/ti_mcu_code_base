#ifndef U_BLOX_DRIVER_H
#define U_BLOX_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

bool ubx_send_message(uint8_t class_num, uint8_t id, uint8_t* payload, uint16_t len);
bool ubx_proc_frame(void);

#ifdef __cplusplus
}
#endif

#endif /* U_BLOX_DRIVER_H */
