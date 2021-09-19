#include "ext_ram_sim_commands.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_utils.h"
#include "convert.h"
#include "debug_info.h"
#include "ext_ram_sim_drv.h"
#include "io_utils.h"
#include "log.h"

bool ext_ram_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        float usage_pec = 0.0;
        uint32_t spare = 0;
        uint32_t busy = 0;
        res = mem_scan(&memory[0], sizeof(memory), &usage_pec, &spare, &busy);
        if(res) {
            io_printf("usage: %f %%" CRLF, usage_pec);
            io_printf("spare: %u Bytes %u kBytes" CRLF, spare, spare / 1024);
            io_printf("busy : %u Bytes %u kBytes" CRLF, busy, busy / 1024);
        } else {
        }
    }
    return res;
}

bool ext_ram_read_command(int32_t argc, char* argv[]) {
    (void)argv;
    bool res = false;
    uint32_t address = 0;
    uint8_t read_data[256] = {0};
    memset(read_data, 0xFF, sizeof(read_data));
    uint32_t size = 0;
    if(1 <= argc) {
        res = try_str2uint32(argv[0], &address);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to parse address", argv[0]);
        }
    }

    if(2 <= argc) {
        res = try_str2uint32(argv[1], &size);
        if(sizeof(read_data) < size) {
            LOG_ERROR(SYS, "Unable to read more %u byte",sizeof(read_data));
            res = false;
        }else{
            LOG_INFO(SYS, "size %u",size);
        }
    }

    if(2 < argc) {
        res = false;
        LOG_ERROR(SYS, "Usage: err addr size");
        LOG_INFO(SYS, " addr ");
        LOG_INFO(SYS, " size 0-256");
    }

    if(res) {
        res = ext_ram_read(address, read_data, size);
        if(res) {
            print_mem(read_data, size, true);
        } else {
            LOG_ERROR(SYS, "Read error");
        }
    }
    return res;
}

bool ext_ram_write_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint32_t address = 0;
    uint8_t tx_array[256];
    uint32_t array_len = 0;
    if(1 <= argc) {
        res = try_str2uint32(argv[0], &address);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to parse address", argv[0]);
        }
    }

    if(2 <= argc) {
        res = try_str2array(argv[1], tx_array, sizeof(tx_array), &array_len);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract tx array %s", argv[1]);
        }
    }
    if(res) {
        res = ext_ram_write(address, tx_array, array_len);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to write");
        } else {
            LOG_INFO(SYS, "OK!");
        }
    }

    return res;
}

bool ext_ram_erase_command(int32_t argc, char* argv[]) {
    (void)argv;
    bool res = false;
    if(0 == argc) {
        res = ext_ram_erase();
        if(true == res) {
            LOG_INFO(SYS, "RAM  erase done!");
        }
    } else {
        LOG_ERROR(SYS, "Usage: ere");
    }
    return res;
}
