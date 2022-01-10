#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#include "cli_wrappers.h"
#include "log.h"
#include "array.h"
#include "convert.h"
#include "io_utils.h"
#include "macro_utils.h"
#include "scan_serial_port.h"
#include "utils_file.h"

#define VERSION 2

bool LoadConfigToTarget(char* file_name){
    LOG_DEBUG(SYS,"%s() %s:", __FUNCTION__,file_name);

    bool res = false;
    FILE *inFilePrt = NULL;
    inFilePrt = fopen(file_name, "r");
    if(inFilePrt ){
        res = true;
        uint32_t line = 0;
        char curFileStr[500];
        while (NULL != fgets(curFileStr, sizeof(curFileStr), inFilePrt)) {
            LOG_DEBUG(SYS,"[%s]", curFileStr);
            if(0<strlen(curFileStr)){
                res = cli_cmd_send(hComm, curFileStr, strlen(curFileStr));
                if(false==res){
                    LOG_ERROR(SYS,"CLI send error [%s]", curFileStr);
                }else{
                    LOG_INFO(SYS,"[*] CLI send OK");
                }
            }
            line++;
        }
        fclose (inFilePrt);
        res = true;
    }else{
        LOG_ERROR(SYS,"Unable to open file [%s]", file_name);
    }
    return res;
}


int main(int argc, char* argv[]) {
    int ret = 0;
    win_color_enable();
    set_log_level(SYS, LOG_LEVEL_DEBUG);
    set_log_level(COM, LOG_LEVEL_DEBUG);

    bool target_connected = false;
    bool res = false;
    LOG_DEBUG(SYS,"argc:%d",argc);
    LOG_INFO(SYS,"Firmware Configurator:");
    LOG_INFO(SYS,"Version: %u", VERSION);

    if(argc < 3) {
        LOG_ERROR(SYS,"Lack of config");
        ret = 1;
        res = false;
    } else {
        LOG_DEBUG(SYS,"config file [%s]",argv[3]);
        res = true;
    }

    uint32_t baud_rate = 0;
    if(res) {
        res = try_str2uint32(argv[2], &baud_rate);
        if(false == res) {
            LOG_ERROR(SYS,"Unable to extract baud_rate %s", argv[2]);
        }else{
            res = init_serial(argv[1], baud_rate);
        }
    }

    if(res) {
        target_connected = is_target_connected();
        if(target_connected) {
            LOG_INFO(SYS,"Spot target");
        } else {
            LOG_ERROR(SYS,"Target Disconnected");
        }
    }

    if(res) {
        res = cli_wrp_parse_fw_version();
    }

    if (3<=argc && target_connected) {
        res = LoadConfigToTarget(argv[3]);
        if(false==res){
            LOG_ERROR(SYS,"Config Load Error");
        } else {
            LOG_INFO(SYS,"Config Ok");
        }
    } else {
        LOG_ERROR(SYS,"Error");
    }

    if(target_connected){
        res=cli_wrp_restore_target();
    }

    CloseHandle(hComm);
    LOG_INFO(SYS,"End of program");
    return ret;
}
