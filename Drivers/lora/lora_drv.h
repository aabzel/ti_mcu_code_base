

#ifndef LORA_DRV_H
#define LORA_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include "fifo_array.h"

extern FifoArray_t FiFoLoRaTx;

typedef struct xLoRaIf_t {
    uint32_t err_cnt;
    FifoArray_t FiFoLoRaTx;
} LoRaIf_t;

extern LoRaIf_t LoRaInterface;

bool lora_init(void);
bool lora_proc_payload(uint8_t* rx_payload, uint8_t rx_size);

bool lora_send_queue(uint8_t* const tx_payload, uint32_t len);

#endif /* LORA_DRV_H  */
