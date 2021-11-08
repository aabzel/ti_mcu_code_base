#include "lora_commands.h"

#include <stdio.h>
#include <string.h>

#include "convert.h"
#include "io_utils.h"
#include "log.h"
#include "lora_drv.h"

#ifdef HAS_SX1262
#include "sx1262_drv.h"
#endif



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
        io_printf("max_distance: %f m"CRLF, LoRaInterface.max_distance);

    } else {
        LOG_INFO(LORA,"ld");
    }
    return res;
}
