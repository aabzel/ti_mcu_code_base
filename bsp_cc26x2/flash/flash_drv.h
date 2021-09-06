/*
 see page 79   1.3.2.2 Flash Memory
 see page 631 8.5 FLASH
 flash.h
 */
#ifndef FLASH_DRV_H
#define FLASH_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include <ti/drivers/NVS.h>

#define K_BYTES 1024
#define NOR_FLASH_BASE 0x00000000

#define NOR_FLASH_SIZE (352 * K_BYTES)
#define NVS_SIZE (32 * K_BYTES)

#define NVS_FLASH_START 0x00010000

#define CONFIG_NVSINTERNAL 0
#define CONFIG_TI_DRIVERS_NVS_COUNT 1
#define CONFIG_NVS_COUNT 1
#define NVS_BUFF_SIZE 64

extern const uint_least8_t CONFIG_NVSINTERNAL_CONST;
extern uint8_t nvs_buffer[NVS_BUFF_SIZE];
extern NVS_Handle nvsHandle;
extern NVS_Attrs regionAttrs;

bool flash_init(void);
bool flash_write(uint32_t addr, uint8_t* array, uint32_t array_len);
bool flash_read(uint32_t* addr, uint8_t* rx_array, uint32_t array_len);
bool flash_erase(uint32_t addr, uint32_t array_len);
bool is_errased(uint32_t addr, uint32_t size);
/*
 * API
 erase 8kbyte flash block
 programm 64bit (8 byte) qword
 is block read only
 * */

bool flash_scan(uint8_t* base, uint32_t size, float* usage_pec, uint32_t* spare, uint32_t* busy);

#endif /* FLASH_DRV_H  */
