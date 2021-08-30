#include "lora_handler.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "cli_manager.h"
#include "log.h"
#include "none_blocking_pause.h"
#include "str_utils.h"
#include "sx1262_drv.h"

bool lora_proc_payload(uint8_t* rx_payload, uint8_t rx_size) {
    bool res = false;
    //int ret = 1;
    char   *substr=NULL;
    uint16_t offset = strlen(LORA_CMD_PREFIX);
    substr = strstr((const  char*)rx_payload, (const char*)LORA_CMD_PREFIX);
    if(NULL != substr) {
        char* cli_cmd = substr+ strlen(LORA_CMD_PREFIX);
        LOG_DEBUG(LORA, "cmd [%s]", cli_cmd);
        res = process_shell_cmd(cli_cmd);
    }

    substr = strstr((char*)rx_payload, PING_PREFIX);
    if(NULL != substr) {
        uint8_t tx_buf[TX_SIZE];
        strncpy((char*)tx_buf,"reply",sizeof(tx_buf));
       // wait_in_loop_ms(100) ;
        res = sx1262_start_tx(tx_buf, strlen((const char*)tx_buf), 0);
    }
    return res;
}
