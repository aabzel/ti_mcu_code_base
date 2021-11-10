#ifndef FW_UPDATE_H
#define FW_UPDATE_H

#include <stdbool.h>
#include <stdint.h>

#include "memory_layout.h"

#define FLASH_SIZE (352 * K_BYTES)

#define MAX_BOOTLOADER_SIZE 0x1A000
#define MAX_GENERIC_SIZE (FLASH_SIZE - MAX_BOOTLOADER_SIZE - NVS_SIZE - K_BYTES)

#define FLASH_RD_TRY_CNT 8U
#define FLASH_RD_CHANK_SIZE 256U
#define DFLT_APP_END_ADDR (DFLT_APP_START_ADDR + MAX_GENERIC_SIZE)

bool restore_target(void);
bool loader_update_firmware(uint8_t* newFirmware, uint32_t new_fw_len);
bool parse_fw_version(void);
bool is_target_connected(void);
bool read_current_firmware(uint8_t** out_firmware, uint32_t* fw_len);
bool is_current_firmware_equal(uint8_t* newFirmware, uint32_t new_fw_len);

#endif /* FW_UPDATE_H */
