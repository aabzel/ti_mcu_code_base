#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "log.h"
#include "array.h"
#include "cli_wrappers.h"
#include "convert.h"
#include "io_utils.h"
#include "macro_utils.h"
#include "scan_serial_port.h"
#include "utils_file.h"

#define VERSION 2

bool LoadConfigToTarget(char* file_name){
    printf("\n[d]%s() %s:", __FUNCTION__,file_name);
#ifdef HAS_DEBUG
#endif
    bool res = false;
    FILE *inFilePrt = NULL;
    inFilePrt = fopen(file_name, "r");
    if(inFilePrt ){
        res = true;
        uint32_t line = 0;
        char curFileStr[500];
        while (NULL != fgets(curFileStr, sizeof(curFileStr), inFilePrt)) {
            printf ("\n>[%s]", curFileStr);
#ifdef DEBUG_IN_FILE_STR
#endif
            if(0<strlen(curFileStr)){
                res = cli_cmd_send(hComm, curFileStr, strlen(curFileStr));
                if(false==res){
                    printf ("\n[e] CLI send error [%s]", curFileStr);
                }else{
                    printf ("\n[*] CLI send OK");
                }
            }
            line++;
        }
        fclose (inFilePrt);
        res = true;
    }else{
        printf ("\n[e] Unable to open file [%s]", file_name);
    }
    return res;
}

int main(int argc, char* argv[]) {
    int ret = 0;
    bool target_connected = false;
    bool res = false;
    printf("\n[d] argc:%d",argc);
    printf("\n[*] Firmware Configurator:");
    printf("\n[*] Version: %u", VERSION);

    if(argc < 3) {
        printf("\n[e] Lack of config");
        ret = 1;
        res = false;
    } else {
        printf("\n[d] config file [%s]",argv[3]);
        res = true;
    }

    uint32_t baud_rate = 0;
    if(res) {
        res = try_str2uint32(argv[2], &baud_rate);
        if(false == res) {
            printf("\n[e] Unable to extract baud_rate %s", argv[2]);
        }else{
            res = init_serial(argv[1], baud_rate);
        }
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
        res = cli_wrp_parse_fw_version();
    }

    if (3<=argc) {
        res = LoadConfigToTarget(argv[3]);
        if(false==res){
            printf("\n[e] Config Load Error");
        } else {
            printf("\n[*] Config Ok");
        }
    } else {
        printf("\n[e] Error");
    }

    CloseHandle(hComm);
    LOG_INFO(SYS,"\n[*] End of program");
    return ret;
}
