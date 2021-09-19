#ifndef EXT_RAM_SIM_DRV_H
#define EXT_RAM_SIM_DRV_H

#include <stdbool.h>
#include <stdint.h>

#define EXT_RAM_SIZE 1024U

extern uint8_t memory[EXT_RAM_SIZE];
extern volatile uint32_t ext_ram_busy_cnt;

bool ext_ram_erase(void);
bool ext_ram_init(void);
bool ext_ram_read(uint32_t address, uint8_t* read_data, uint32_t size);
bool ext_ram_write(uint32_t address, uint8_t* wr_data, uint32_t size);
bool ext_ram_init(void);

#endif /* EXT_RAM_SIM_DRV_H */
