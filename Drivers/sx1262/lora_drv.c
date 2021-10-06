#include "lora_drv.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cli_manager.h"
#include "core_driver.h"
#include "data_utils.h"
#include "fifo_array.h"
#include "log.h"
#include "none_blocking_pause.h"
#include "str_utils.h"
#include "sx1262_drv.h"

#define LORA_TX_LEN 10

static Array_t ArrLoRaTxNode[LORA_TX_LEN];
FifoArray_t FiFoLoRaTx;

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
        uint64_t ble_mac = get_ble_mac();
        snprintf((char*)tx_buf, sizeof(tx_buf), "MAC:0x%" PRIx64, ble_mac);
        res = sx1262_start_tx(tx_buf, strlen((const char*)tx_buf) + 1, 0);
    }
    return res;
}

bool lora_init(void){
    bool res = false;
    res = fifo_arr_init(&FiFoLoRaTx, &ArrLoRaTxNode, ARRAY_SIZE(ArrLoRaTxNode));
    return res;
}
