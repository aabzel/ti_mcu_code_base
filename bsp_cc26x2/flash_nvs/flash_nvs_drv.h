/*
 see page 79   1.3.2.2 Flash Memory
 see page 631 8.5 FLASH
 flash.h
 */
#ifndef FLASH_NVS_DRV_H
#define FLASH_NVS_DRV_H

#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/NVS.h>

#define CONFIG_NVSINTERNAL 0
#define CONFIG_TI_DRIVERS_NVS_COUNT 1
#define CONFIG_NVS_COUNT 1
#define NVS_BUFF_SIZE 64

#define FLASH_WR_TIME_MS 20

extern const uint_least8_t CONFIG_NVSINTERNAL_CONST;
extern uint8_t nvs_buffer[NVS_BUFF_SIZE];
extern NVS_Handle nvsHandle;
extern NVS_Attrs regionAttrs;

bool flash_nvs_init(void);
bool flash_nvs_erase(uint32_t addr, uint32_t array_len);
bool flash_nvs_write(uint32_t addr, uint8_t* array, uint32_t array_len);
/*
 * API
 erase 8kbyte flash block
 programm 64bit (8 byte) qword
 is block read only
 * */
#endif /* FLASH_NVS_DRV_H  */
