#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "array.h"
#include "convert.h"
#include "fw_update.h"
#include "io_utils.h"
#include "macro_utils.h"
#include "scan_serial_port.h"
#include "utils_file.h"

#define VERSION 1

int main(int argc, char* argv[]) {
    int ret = 0;
    bool target_connected = false;
    bool res = false;
    printf("\n[*] Firmware loader:");
    printf("\n[*] Version: %u", VERSION);

    if(argc < 3) {
        printf("\n[e] Lack of firmware");
        ret = 1;
        res = false;
    } else {
        res = true;
    }

    uint32_t baud_rate = 0;
    if(res) {
        res = try_str2uint32(argv[2], &baud_rate);
        if(false == res) {
            printf("\n[e] Unable to extract baud_rate %s", argv[2]);
        }
    }

    if(res) {
        res = init_serial(argv[1], baud_rate);
    }

    if(res) {
        target_connected = is_target_connected();
        if(target_connected) {
            printf("\n[*] Spot target");
        } else {
            printf("\n[e] Target Disconnected");
        }
    }

    if(res) {
        res = parse_fw_version();
    }

    uint32_t new_firmware_size = 0;
    uint8_t* NewFirmwareArray = NULL;
    if(res) {
        res = file_load_to_array(argv[3], &NewFirmwareArray, &new_firmware_size);
        if(res) {
            printf("\n[*] Load firmware from file to RAM ok len: %u byte", new_firmware_size);
            if(MAX_GENERIC_SIZE < new_firmware_size) {
                printf("\n[e] New firmware too big Max: %u byte", MAX_GENERIC_SIZE);
                res = false;
            }
        } else {
            printf("\n[e] Unable to Load firmware from file to RAM");
        }
    }

    if(NewFirmwareArray && (1 < new_firmware_size)) {
        res = is_current_firmware_equal(NewFirmwareArray, new_firmware_size);
        if(res) {
            printf("\n[i] firmware equal");
        } else {
            printf("\n[i] firmware diffetent");
            res = loader_update_firmware(NewFirmwareArray, new_firmware_size);
            if(res){
                printf("\n[i] firmware updared.");
            }
        }
    }

#ifdef HAS_READ_FW_FROM_FLASH
    uint8_t* firmwareFromFlash = NULL;
    uint32_t firmware_from_flash_len = 0;
    if(target_connected) {
        res = read_current_firmware(&firmwareFromFlash, &firmware_from_flash_len);
        if(res) {
            printf("\n[*] Load firmware from Flash to RAM ok len: %u", firmware_from_flash_len);
        } else {
            printf("\n[e] Unable to Load firmware from flash to RAM");
        }
    }
#endif

    if(target_connected) {
        res = restore_target();
        if(false == res) {
            printf("\n[e] Unable to restore target");
        }
    }

#ifdef HAS_READ_FW_FROM_FLASH
    if(NewFirmwareArray && firmwareFromFlash && (0 < new_firmware_size)) {
        uint32_t match = 0;
        uint32_t diff = 0;
        res = is_array_equal(NewFirmwareArray, firmwareFromFlash, new_firmware_size, &match, &diff);
        if(res) {
            printf("\n[i] No need for firmware update");
        } else {
            printf("\n[i] firmware different match: %u byte diff: %u byte", match, diff);
            float similarity = ((float)(100 * match)) / ((float)new_firmware_size);
            printf("\n[i] similarity: %f %%", similarity);
        }
    }
#endif

    CloseHandle(hComm);
    return ret;
}
