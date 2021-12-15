
#ifndef LORA_DRV_H
#define LORA_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"
#include "fifo_array.h"

#define LORA_TX_QUEUE_SIZE 60
#define LORA_MAX_FRAME_SIZE 256U
#define LORA_PERIOD_US S_2_US(15)

extern FifoArray_t FiFoLoRaTx;
/*TODO: rewrite to Interface array*/
typedef struct xLoRaIf_t {
    uint32_t err_cnt;
    uint32_t ovfl_err_cnt;
    uint32_t tx_ok_cnt;
    uint32_t tx_err_cnt;
    uint32_t tx_done_cnt;
    FifoArray_t FiFoLoRaTx;
    double max_distance;
} LoRaIf_t;

extern LoRaIf_t LoRaInterface;

bool lora_init(void);
bool lora_proc_payload(uint8_t* rx_payload, uint32_t rx_size);
bool lora_send_queue(uint8_t* const tx_payload, uint32_t len);
bool lora_process(void);

#endif /* LORA_DRV_H  */
