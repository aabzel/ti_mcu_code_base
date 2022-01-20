

#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

bool can_send(uint8_t* data, uint16_t len);


#endif /* CAN_DRIVER_H */
