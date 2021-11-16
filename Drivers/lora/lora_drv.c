#include "lora_drv.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "array.h"
#include "cli_manager.h"
#ifdef HAS_TBFP
#include "tbfp_protocol.h"
#endif
#include "core_driver.h"
#include "data_utils.h"
#include "fifo_array.h"
#include "flash_fs.h"
#include "log.h"
#include "none_blocking_pause.h"
#include "param_ids.h"
#ifdef HAS_RTCM3
#include "rtcm3_protocol.h"
#endif
#include "str_utils.h"
#ifdef HAS_SX1262
#include "sx1262_drv.h"
#endif

static Array_t ArrLoRaTxNode[LORA_TX_QUEUE_SIZE];
LoRaIf_t LoRaInterface = {0};

#ifdef HAS_RTCM3
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
#endif

bool lora_proc_payload(uint8_t* const rx_payload, uint8_t rx_size) {
    bool res = false;
#ifdef HAS_RTCM3
    res = rtcm3_lora_rx_proc(rx_payload, rx_size);
#endif /*HAS_RTCM3*/
#ifdef HAS_TBFP
    res = tbfp_proc(rx_payload, rx_size);
#endif /*HAS_TBFP*/
    return res;
}

bool lora_init(void) {
    bool res = false;
    uint16_t value_len = 0;
    res = mm_get(PAR_ID_LORA_MAX_LINK_DIST, (uint8_t*)&LoRaInterface.max_distance, 8, &value_len);
    if(false == res) {
        LoRaInterface.max_distance = 0.0;
        res = mm_set(PAR_ID_LORA_MAX_LINK_DIST, (uint8_t*)&LoRaInterface.max_distance, sizeof(double));
        if(false == res) {
            LOG_ERROR(PARAM, "SetDfltMaxLinkDistError");
        }
    }
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
    /*HeartBeat Lora Frame*/
    res = tbfp_send_ping(FRAME_ID_PONG);
    return res;
}
