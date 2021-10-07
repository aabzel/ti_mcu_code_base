#include "lora_drv.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "array.h"
#include "cli_manager.h"
#include "core_driver.h"
#include "data_utils.h"
#include "fifo_array.h"
#include "log.h"
#include "none_blocking_pause.h"
#include "rtcm3_protocol.h"
#include "str_utils.h"
#include "sx1262_drv.h"

#define LORA_TX_LEN 10

static Array_t ArrLoRaTxNode[LORA_TX_LEN];
LoRaIf_t LoRaInterface;

bool rtcm3_lora_rx_proc(uint8_t* const payload, uint32_t size) {
    bool res = false;
    if((NULL != payload) && (0 < size)) {
        uint32_t i = 0;
        rtcm3_reset_rx(&Rtcm3Porotocol[RT_LORA_ID]);
        uint32_t init_rx_pkt_cnt = Rtcm3Porotocol[RT_LORA_ID].rx_pkt_cnt;
        for(i = 0; i < size; i++) {
            res = rtcm3_proc_byte(&Rtcm3Porotocol[RT_LORA_ID], payload[i]);
        }
        if(init_rx_pkt_cnt < Rtcm3Porotocol[RT_LORA_ID].rx_pkt_cnt) {
            res = true;
        }
    }

    return res;
}

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
        uint8_t tx_buf[TX_SIZE] = {0};
        uint64_t ble_mac = get_ble_mac();
        snprintf((char*)tx_buf, sizeof(tx_buf), "MAC:0x%" PRIx64, ble_mac);
        res = sx1262_start_tx(tx_buf, strlen((const char*)tx_buf) + 1, 0);
    }
    res = rtcm3_lora_rx_proc(rx_payload, rx_size);
    return res;
}

bool lora_init(void) {
    bool res = false;
    res = fifo_arr_init(&LoRaInterface.FiFoLoRaTx, &ArrLoRaTxNode[0], ARRAY_SIZE(ArrLoRaTxNode));
    return res;
}

bool lora_send_queue(uint8_t* const tx_payload, uint32_t len) {
    bool res = false;
    if((NULL!=tx_payload) && (len<=RTCM3_RX_FRAME_SIZE)) {
        uint8_t* data = memdup((uint8_t*)tx_payload, len);
        if(data) {
            Array_t txNode = {.size = 0, .pArr = NULL};
            txNode.size = len;
            txNode.pArr = (uint8_t*)data;
            res = fifo_arr_push(&LoRaInterface.FiFoLoRaTx, txNode);
            if(false == res) {
                LoRaInterface.err_cnt++;
            }
        }else{
            LoRaInterface.err_cnt++;
        }
    }else{
        LoRaInterface.err_cnt++;
    }
    return res;
}
