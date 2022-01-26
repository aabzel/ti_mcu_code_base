#include "lora_drv.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "array.h"
#ifdef HAS_CLI
#include "log.h"
#include "cli_manager.h"
#endif
#ifdef HAS_TBFP
#include "tbfp_protocol.h"
#endif
#include "core_driver.h"
#include "data_utils.h"
#include "fifo_array.h"
#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#include "param_ids.h"
#endif
#include "none_blocking_pause.h"
#include "system.h"
#ifdef HAS_RTCM3
#include "rtcm3_protocol.h"
#endif
#include "str_utils.h"
#ifdef HAS_SX1262
#include "sx1262_drv.h"
#endif

static Array_t ArrLoRaTxNode[LORA_TX_QUEUE_SIZE];
LoRaIf_t LoRaInterface = {0};

bool lora_proc_payload(uint8_t* const rx_payload, uint32_t rx_size) {
    bool res = false;

#ifdef HAS_TBFP
    res = tbfp_proc(rx_payload, rx_size, IF_LORA, true);
    if(false == res) {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "LoRaProcErr");
#endif
    }
#endif /*HAS_TBFP*/
    return res;
}

bool lora_init(void) {
    bool res = false;
    LoRaInterface.max_distance = 0.0;
#ifdef HAS_FLASH_FS
    uint16_t value_len = 0;
    res = mm_get(PAR_ID_LORA_MAX_LINK_DIST, (uint8_t*)&LoRaInterface.max_distance, 8, &value_len);
    if(false == res) {
        LoRaInterface.max_distance = 0.0;
        res = mm_set(PAR_ID_LORA_MAX_LINK_DIST, (uint8_t*)&LoRaInterface.max_distance, sizeof(double));
        if(false == res) {
#ifdef HAS_LOG
            LOG_ERROR(PARAM, "SetDfltMaxLinkDistError");
#endif
        }
    }
#endif
    LoRaInterface.tx_ok_cnt = 0;
    LoRaInterface.tx_done_cnt = 0;
    res = fifo_arr_init(&LoRaInterface.FiFoLoRaTx, &ArrLoRaTxNode[0], ARRAY_SIZE(ArrLoRaTxNode));
    return res;
}

bool lora_send_queue(uint8_t* const tx_payload, uint32_t len) {
    bool res = false;
    if((NULL != tx_payload) && (len <= LORA_MAX_FRAME_SIZE)) {
        uint8_t* data = memdup((uint8_t*)tx_payload, len);
        if(data) {
            Array_t txNode = {.size = 0, .pArr = NULL};
            txNode.size = len;
            txNode.pArr = (uint8_t*)data;
            res = fifo_arr_push(&LoRaInterface.FiFoLoRaTx, txNode);
            if(false == res) {
                LoRaInterface.ovfl_err_cnt++;
            }
        } else {
            LoRaInterface.err_cnt++;
        }
    } else {
        LoRaInterface.err_cnt++;
    }
    return res;
}

bool lora_process(void) {
    bool res = false;
#ifdef HAS_TBFP
    /*HeartBeat Lora Frame*/
    res = tbfp_send_ping(FRAME_ID_PONG, IF_LORA);
#endif /*HAS_TBFT*/
    return res;
}
