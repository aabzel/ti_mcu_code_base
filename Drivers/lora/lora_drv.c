#include "lora_drv.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "array.h"
#ifdef HAS_CLI
#include "cli_manager.h"
#include "log.h"
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

char LoRaTxBuff[LORA_TX_BUFF_SIZE];
#if HAS_LORA_FIFO_ARRAYS
static Array_t ArrLoRaTxNode[LORA_TX_QUEUE_SIZE];
#endif
LoRaIf_t LoRaInterface = {0};

bool lora_proc_payload(uint8_t* const rx_payload, uint32_t rx_size) {
    bool res = false;

#ifdef HAS_TBFP
    res = tbfp_proc(rx_payload, rx_size, IF_SX1262, true);
    if(false == res) {
#ifdef HAS_LOG
        LOG_ERROR(LORA, "SX1262ProcErr");
#endif
        LoRaInterface.err_cnt++;
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

    res = fifo_init(&LoRaInterface.FiFoLoRaCharTx, &LoRaTxBuff[0], sizeof(LoRaTxBuff));
#if HAS_LORA_FIFO_ARRAYS
    LoRaInterface.tx_ok_cnt = 0;
    LoRaInterface.tx_done_cnt = 0;
    res = fifo_arr_init(&LoRaInterface.FiFoLoRaTx, &ArrLoRaTxNode[0], ARRAY_SIZE(ArrLoRaTxNode));
#endif
    return res;
}

#ifdef HAS_LORA_FIFO_ARRAYS
bool lora_send_array_queue(uint8_t* const tx_payload, uint32_t len) {
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
#endif

#ifdef HAS_TBFP
bool lora_transmit_from_queue(uint32_t cur_time_stamp_ms, uint32_t tx_done_time_stamp_ms) {
    bool res = false;
    uint32_t tx_time_diff_ms = 2 * DFLT_TX_PAUSE_MS;
    tx_time_diff_ms = cur_time_stamp_ms - tx_done_time_stamp_ms;
    uint32_t count = fifo_get_count(&LoRaInterface.FiFoLoRaCharTx);
    fifo_index_t tx_len = 0;
    uint8_t TxPayload[SX1262_MAX_PAYLOAD_SIZE-3] = {0};
    memset(TxPayload, 0, sizeof(TxPayload));
    if((DFLT_TX_PAUSE_MS < tx_time_diff_ms) &&
            (true == is_sx1262_retx_idle()) &&
            (sizeof(TxPayload) < count)) {

        res = fifo_pull_array(&LoRaInterface.FiFoLoRaCharTx, (char*)TxPayload, sizeof(TxPayload), &tx_len);
        if(res) {
            if(tx_len!=sizeof(TxPayload)){
#ifdef HAS_LOG
                LOG_ERROR(LORA, "FiFoPullLenErr Len:%u %u", tx_len,sizeof(TxPayload));
#endif
            }
#ifdef HAS_LOG
            LOG_DEBUG(LORA, "FiFoPull Len:%u", tx_len);
#endif
        }else{
#ifdef HAS_LOG
            LOG_ERROR(LORA, "FiFoPullErr Len:%u", tx_len);
#endif
        }


#ifdef HAS_LORA_FIFO_ARRAYS
        Array_t Node = {.size = 0, .pArr = NULL};
        res = fifo_arr_pull(&LoRaInterface.FiFoLoRaTx, &Node);
        if(res) {
            if(Node.pArr) {
                if(0 < Node.size) {
                    if(Node.size <= sizeof(tx_buf)) {

                        memcpy(tx_buf, Node.pArr, Node.size);
                        tx_len = Node.size;
                    }
                }
#ifdef HAS_MCU
                free(Node.pArr);
#endif /*HAS_MCU*/
            }
        }
#endif /*HAS_LORA_FIFO_ARRAYS*/
    } else {
        LOG_PARN(LORA, "FiFoCnt:%u", count);
        res = true;
    }
    if(res) {
        if((0 < tx_len) && (tx_len == sizeof(TxPayload))) {
#ifdef HAS_TBFP
                res = tbfp_send_tunnel(TxPayload, tx_len, IF_SX1262);
                if(res) {
                    LoRaInterface.tx_ok_cnt++;
                } else {
                    LoRaInterface.tx_err_cnt++;
                }
#endif /*HAS_TBFP*/

        }
    }
    return res;
}
#endif /*HAS_TBFP*/



bool lora_send_queue(uint8_t* tx_payload, uint32_t len) {
    bool res = false;
    if((NULL != tx_payload) && (0 < len)) {
        uint8_t debugSync[5] = {0x11, 0x22, 0x33, 0x44, 0x55};
        res = fifo_push_array(&LoRaInterface.FiFoLoRaCharTx, (char*)debugSync, (fifo_index_t)sizeof(debugSync));
        if(false == res) {
            LoRaInterface.ovfl_err_cnt++;
            LOG_ERROR(LORA, "TxQueueOverFlow");
        }
        if(res) {
            res = fifo_push_array(&LoRaInterface.FiFoLoRaCharTx, (char*)tx_payload, (fifo_index_t)len);
            if(false == res) {
                LoRaInterface.ovfl_err_cnt++;
                LOG_ERROR(LORA, "TxQueueOverFlow");
            }
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
