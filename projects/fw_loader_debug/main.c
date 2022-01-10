#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>

#include "array.h"
#include "cli_wrappers.h"
#include "log.h"
#include "convert.h"
#include "fw_update.h"
#include "io_utils.h"
#include "macro_utils.h"
#include "scan_serial_port.h"
#include "utils_file.h"
#include "win_utils.h"

#define VERSION 1

int main(int argc, char* argv[]) {
    int ret = 0;
    win_color_enable();
    set_log_level(SYS, LOG_LEVEL_DEBUG);
    set_log_level(COM, LOG_LEVEL_DEBUG);
    bool target_connected = false;
    bool res = false;
    LOG_DEBUG(SYS,"argc:%d",argc);
    LOG_INFO(SYS,"Firmware loader:");
    LOG_INFO(SYS,"Version: %u", VERSION);

    if(argc < 3) {
        LOG_ERROR(SYS,"Lack of firmware");
        ret = 1;
        res = false;
    } else {
        res = true;
    }

    uint32_t baud_rate = 0;
    if(res) {
        res = try_str2uint32(argv[2], &baud_rate);
        if(false == res) {
            LOG_ERROR(SYS,"Unable to extract baud_rate %s", argv[2]);
        }
    }

    if(res) {
        res = init_serial(argv[1], baud_rate);
    }

    if(res) {
        target_connected = is_target_connected();
        if(target_connected) {
            LOG_INFO(SYS,"Spot target");
        } else {
            LOG_ERROR(SYS,"Target Disconnected");
			res = false;
        }
    }

    if(res) {
        res = cli_wrp_parse_fw_version();
    }

    uint32_t new_firmware_size = 0;
    uint8_t* NewFirmwareArray = NULL;
    if(res) {
        res = file_load_to_array(argv[3], &NewFirmwareArray, &new_firmware_size);
        if(res) {
            LOG_INFO(SYS,"Load firmware from file to RAM ok len: %u byte", new_firmware_size);
            if(MAX_GENERIC_SIZE < new_firmware_size) {
                LOG_ERROR(SYS,"New firmware too big Max: %u byte", MAX_GENERIC_SIZE);
                res = false;
            }
        } else {
            LOG_ERROR(SYS,"Unable to Load firmware from file to RAM");
        }
    }

    if(NewFirmwareArray && (1 < new_firmware_size)) {
        res = is_current_firmware_equal(NewFirmwareArray, new_firmware_size);
        if(res) {
            LOG_INFO(SYS,"firmware equal");
        } else {
            LOG_INFO(SYS,"firmware different");
            res = loader_update_firmware(NewFirmwareArray, new_firmware_size);
            if(res) {
                LOG_INFO(SYS,"firmware updated!");
            } else {
                LOG_ERROR(SYS,"firmware update error!");
            }
        }
    }

#ifdef HAS_READ_FW_FROM_FLASH
    uint8_t* firmwareFromFlash = NULL;
    uint32_t firmware_from_flash_len = 0;
    if(target_connected) {
        res = read_current_firmware(&firmwareFromFlash, &firmware_from_flash_len);
        if(res) {
            LOG_INFO(SYS,"Load firmware from Flash to RAM ok len: %u", firmware_from_flash_len);
        } else {
            LOG_ERROR(SYS,"Unable to Load firmware from flash to RAM");
        }
    }
#endif

    if(target_connected) {
        res = cli_wrp_restore_target();
        if(false == res) {
            LOG_ERROR(SYS,"Unable to restore target");
        }
    }

#ifdef HAS_READ_FW_FROM_FLASH
    if(NewFirmwareArray && firmwareFromFlash && (0 < new_firmware_size)) {
        uint32_t match = 0;
        uint32_t diff = 0;
        res = is_array_equal(NewFirmwareArray, firmwareFromFlash, new_firmware_size, &match, &diff);
        if(res) {
            LOG_INFO(SYS,"No need for firmware update");
        } else {
            LOG_INFO(SYS,"firmware different match: %u byte diff: %u byte", match, diff);
            float similarity = ((float)(100 * match)) / ((float)new_firmware_size);
            LOG_INFO(SYS,"similarity: %f %%", similarity);
        }
    }
#endif

    CloseHandle(hComm);
    LOG_INFO(SYS,"End of program");
    return ret;
}
