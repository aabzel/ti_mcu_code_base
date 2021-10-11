#ifndef TCAN4550_DRV_H
#define TCAN4550_DRV_H

#include <stdbool.h>
#include <stdint.h>


#define READ_OP_CODE 0x41

bool tcan4550_init(void);
bool tcan4550_reset(void);
bool tcan4550_chip_select(bool state);
bool tcan4550_read(uint16_t address, uint8_t len, uint8_t *out_array, uint32_t size);

#endif /* TCAN4550_DRV_H  */
