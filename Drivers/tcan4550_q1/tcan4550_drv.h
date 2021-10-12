#ifndef TCAN4550_DRV_H
#define TCAN4550_DRV_H

#include <stdbool.h>
#include <stdint.h>
/*ADDRESS*/
#define DEVICE_ID_0 0
#define DEVICE_ID_1 4
//#define DEVICE_ID "TCAN4550"

// expected  5443414E 34353530
// read      4E414354 30353534
#define READ_OP_CODE 0x41

extern const uint64_t exp_dev_id;

bool is_tcan4550_connected(void);
bool tcan4550_init(void);
bool tcan4550_reset(void);
bool tcan4550_chip_select(bool state);
bool tcan4550_read(uint16_t address, uint8_t len, uint8_t* out_array, uint32_t size);
// get mode

#endif /* TCAN4550_DRV_H  */
