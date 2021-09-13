#ifndef SX1262_CONFIG_H
#define SX1262_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lora_defs.h"

#define DFLT_FREQ_MHZ 915000000UL
#define DFLT_LORA_BW_500 LORA_BW_500
#define DFLT_SF12 SF12
#define DFLT_LORA_CR_4_8 LORA_CR_4_8

#define SX1262_SPI_NUM 0
#define SYNC_WORD 0x1122334455667788

#define RX_BASE_ADDRESS (0x00 + (FIFO_SIZE / 2)) // 128
#define TX_BASE_ADDRESS 0x00

#define BUSY_CNT_LIMIT 1000

#ifdef __cplusplus
}
#endif

#endif /* SX1262_CONFIG_H */
