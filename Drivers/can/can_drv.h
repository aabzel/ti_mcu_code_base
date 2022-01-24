

#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"

#define CAN_PERIOD_US S_2_US(3)

bool can_send(uint8_t* data, uint16_t len);
bool can_proc(void);
bool can_proc_payload(uint8_t* const rx_payload, uint32_t rx_size);

#endif /* CAN_DRIVER_H */
