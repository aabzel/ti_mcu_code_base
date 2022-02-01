#ifndef SX1262_RETX_H
#define SX1262_RETX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "lora_constants.h"

#define RETX_TRY_CNT_DFLT 4 /*TODO Make Parameter*/

typedef struct xReTxFsm_t {
    uint8_t retx_cnt_max;
    uint8_t retx_cnt;
    uint8_t TxFrame[SX1262_MAX_FRAME_SIZE];
    uint16_t tx_frame_len;
}ReTxFsm_t;


bool is_sx1262_retx_idle(void);
bool sx1262_retx_init(void);
bool sx1262_start_retx(uint8_t* array, uint16_t len, uint8_t try_cnt);
bool sx1216_retx_proc(void);

#ifdef __cplusplus
}
#endif

#endif /* SX1262_RETX_H */
