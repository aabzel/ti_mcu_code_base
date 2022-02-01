#ifndef SX1262_CONFIG_H
#define SX1262_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lora_constants.h"

#define DFLT_TX_PAUSE_MS 50U /*TODO Find least possible value*/
#define DFLT_FREQ_MHZ 868950000
#define DFLT_LORA_BW LORA_BW_125
#define DFLT_SF SF7
#define DFLT_LORA_CR LORA_CR_4_8
#define DFLT_OUT_POWER 14 /*dBm*/

#define DFLT_PREAMBLE_LEN 8
#define DFLT_SYNC_WORD 0x1122334444332211
#define DFLT_LORA_SYNC_WORD 0x1424

#define RX_BASE_ADDRESS 0x00
#define TX_BASE_ADDRESS 0x00

#define BUSY_CNT_LIMIT 1000

#ifdef __cplusplus
}
#endif

#endif /* SX1262_CONFIG_H */
