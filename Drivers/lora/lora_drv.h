
#ifndef LORA_DRV_H
#define LORA_DRV_H

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_CLOCK
#include "clocks.h"
#endif
#include "fifo_array.h"
#include "fifo_char.h"

#define LORA_MAX_FRAME_SIZE 256U
#define LORA_TX_BUFF_SIZE (LORA_MAX_FRAME_SIZE * 10)
#if HAS_LORA_FIFO_ARRAYS
#define LORA_TX_QUEUE_SIZE 60
#endif
#define LORA_PERIOD_US S_2_US(15)

extern FifoArray_t FiFoLoRaTx;
/*TODO: rewrite to Interface array*/
typedef struct xLoRaIf_t {
    uint32_t err_cnt;
    uint32_t ovfl_err_cnt;
    uint32_t tx_ok_cnt;
    uint32_t tx_err_cnt;
    uint32_t tx_done_cnt;
    Fifo_array_t FiFoLoRaCharTx;
#if HAS_LORA_FIFO_ARRAYS
    FifoArray_t FiFoLoRaTx;
#endif
    double max_distance;
} LoRaIf_t;

extern char LoRaTxBuff[LORA_TX_BUFF_SIZE];
extern LoRaIf_t LoRaInterface;

bool lora_transmit_from_queue(uint32_t cur_time_stamp_ms, uint32_t tx_done_time_stamp_ms) ;
bool lora_init(void);
bool lora_proc_payload(uint8_t* rx_payload, uint32_t rx_size);
bool lora_send_queue(uint8_t* const tx_payload, uint32_t len);
bool lora_process(void);

#endif /* LORA_DRV_H  */
