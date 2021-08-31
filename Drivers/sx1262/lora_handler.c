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
    char* substr = NULL;
    uint16_t offset = strlen(CMD_PREFIX);
    substr = strstr((const char*)rx_payload, (const char*)CMD_PREFIX);
    if(NULL != substr) {
        char* cli_cmd = substr + strlen(CMD_PREFIX);
        LOG_DEBUG(LORA, "cmd [%s]", cli_cmd);
        res = process_shell_cmd(cli_cmd);
    }

    substr = strstr((char*)rx_payload, PING_PREFIX);
    if(NULL != substr) {
        uint8_t tx_buf[TX_SIZE];
        strncpy((char*)tx_buf, "lora_reply", sizeof(tx_buf));
        res = sx1262_start_tx(tx_buf, strlen((const char*)tx_buf)+1, 0);
    }
    return res;
}
