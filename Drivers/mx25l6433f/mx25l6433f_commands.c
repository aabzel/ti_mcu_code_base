#include "mx25l6433f_commands.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


bool cmd_mx25_diag(int32_t argc, char* argv[]){
	bool res;
	return res;
}


bool mx25_read_command(int32_t argc, char* argv[]){
    (void)argv;
    bool res = false;
    if (3 == argc) {
        res = true;
        uint16_t sector = 0;
        if (true == res) {
            res = try_str2uint16(argv[0], &sector);
        }

        uint16_t address = 0;
        if (true == res) {
            res = try_str2uint16(argv[1], &address);
        }
        uint8_t read_data[256] = {0};
        memset(read_data, 0xFF, sizeof(read_data));
        uint16_t size = 0;
        if (true == res) {
            res = try_str2uint16(argv[2], &size);
            if (sizeof(read_data) < size) {
                res = false;
            }
        }
        if (true == res) {
            MX25_memory.Read_data(sector, address, read_data, size);
            print_mem(read_data, size);
        }
    } else {
        LOG_ERROR(SYS, "Usage: mxr sector addr size");
        LOG_INFO(SYS, " sector");
        LOG_INFO(SYS, " addr ");
        LOG_INFO(SYS, " size 0-256");
    }
    return res;
}
bool mx25_write_command(int32_t argc, char* argv[]){}

bool mx25_erase_command(int32_t argc, char* argv[]){
      (void)argv;
    bool res = false;
    if (0 == argc) {
        res = true;
        res = mx25_erase_chip();
        if (true == res) {
            LOG_INFO(SYS, "MX25L6433F  erase done!");
        }
    } else {
        LOG_ERROR(SYS, "Usage: mxe");
    }
    return res;
}