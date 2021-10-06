

#ifndef LORA_DRV_H
#define LORA_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include "fifo_array.h"

extern FifoArray_t FiFoLoRaTx;

bool lora_init(void);
bool lora_proc_payload(uint8_t* rx_payload, uint8_t rx_size);

#endif /* LORA_DRV_H  */
