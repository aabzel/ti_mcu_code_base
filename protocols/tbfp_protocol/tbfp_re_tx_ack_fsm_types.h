#ifndef TBFP_RETX_ACK_FSM_TYPES_H
#define TBFP_RETX_ACK_FSM_TYPES_H

/*TBFP Trivial Binary Frame Protocol*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "system.h"
#include "timer_utils.h"

#define TBFP_RETX_TRY_MAX 3

#define TBFP_PERIOD_US  MSEC_2_US(15)

typedef enum eTbfpAck_t{
  ACK_NO_NEED= 0,
  ACK_NEED =1,
}TbfpAck_t;

typedef enum eTbfpReTxInput_t {
    TBFP_IN_NONE =0,
    TBFP_IN_RX_ACK = 1,
    TBFP_IN_RX_ACK_TIME_OUT = 2,
    TBFP_IN_TX_DONE_TIME_OUT = 3,
    TBFP_IN_TX_FRAME = 4,
    TBFP_IN_TX_DONE =5,
}TbfpReTxInput_t;

typedef enum eTbfpReTxState_t {
    TBFP_IDLE = 0,
    TBFP_WAIT_TX_DONE=1,
    TBFP_WAIT_ACK=2,
}TbfpReTxState_t;

typedef struct xTbfpReTxFsm_t{
    uint32_t tx_done_time_out_ms;
    uint32_t time_stamp_start_ms;
    uint32_t err_cnt;
    uint32_t err_tx_done;
    uint32_t ack_rx_cnt;
    uint32_t ack_tx_cnt;
    uint32_t silence_cnt;
    uint32_t silence_cnt_prev;
    uint32_t ack_time_out_cnt;
    uint32_t no_reply_cnt;
    uint32_t spin_cnt;
    uint8_t retx_cnt;
    uint16_t expected_ser_num;
    TbfpReTxInput_t input;
    TbfpReTxState_t state;
}TbfpReTxFsm_t;


#ifdef __cplusplus
}
#endif

#endif /* TBFP_RETX_ACK_FSM_TYPES_H */
