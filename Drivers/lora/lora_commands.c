#include "lora_commands.h"

#include <stdio.h>
#include <string.h>

#include "convert.h"
#include "io_utils.h"
#include "log.h"
#include "lora_drv.h"
#include "sx1262_drv.h"

bool lora_sent_command(int32_t argc, char* argv[]) {
    bool res = false;
    uint8_t tx_array[TX_SIZE] = CMD_PREFIX;
    uint16_t tx_array_len = 0;
    uint32_t timeout_s = 0;
    if(1 <= argc) {
        res = true;
        uint8_t a = 0;
        snprintf((char*)tx_array, sizeof(tx_array), "%s%s", tx_array, argv[0]);
        LOG_INFO(LORA, "arg: [%s]", (char*)tx_array);
        for(a = 1; a < argc; a++) {
            snprintf((char*)tx_array, sizeof(tx_array), "%s %s", tx_array, argv[a]);
        }
        LOG_INFO(LORA, "arg: [%s]", (char*)tx_array);
        tx_array_len = strlen((char*)tx_array) + 1;
    }

    if(0 == argc) {
        LOG_ERROR(LORA, "Usage: lc cmd arg1 arg2 [argN]");
        LOG_INFO(LORA, "cli_cmd command for remote target");
        res = false;
    }

    if(res) {
        res = sx1262_start_tx(tx_array, tx_array_len, timeout_s);
    }

    return res;
}

bool lora_sent_queue_command(int32_t argc, char* argv[]){
    bool res = false;
    uint8_t tx_array[TX_SIZE] = CMD_PREFIX;
    uint32_t tx_array_len = 0;
    if(1 <= argc) {
        res = try_str2array(argv[0], tx_array, sizeof(tx_array), &tx_array_len);
        if(false == res) {
            /*treat as plane text*/
            LOG_NOTICE(LORA, "Unable to extract hex array %s", argv[0]);
            strncpy((char*)tx_array, argv[0], sizeof(tx_array));
            tx_array_len = (uint16_t)strlen((char*)tx_array) + 1U;
            res = true;
        }
    }

    if (res) {
        res = lora_send_queue(tx_array, tx_array_len );
        if(false==res){
            LOG_ERROR(LORA,"LoRa Queue put error");
        }else{
            LOG_INFO(LORA,"OK");
        }
    }
    return res;
}


bool lora_diag_command(int32_t argc, char* argv[]){
    bool res = false;
    if(0==argc){
        io_printf("err cnt: %u"CRLF, LoRaInterface.err_cnt);
        io_printf("tx ok cnt: %u"CRLF, LoRaInterface.tx_ok_cnt);
        io_printf("tx done cnt: %u"CRLF, LoRaInterface.tx_done_cnt);
    } else {
        LOG_INFO(LORA,"ld");
    }
    return res;
}
