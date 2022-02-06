#include "tbfp_re_tx_ack_fsm.h"

#include <string.h>

#include "clocks.h"
#include "log.h"
#include "data_utils.h"
#include "tbfp_protocol.h"
#include "sx1262_constants.h"
#include "sx1262_drv.h"
#include "system.h"

bool is_tbfp_retx_idle(TbfpProtocol_t *instance){
    bool res = false;
    if( (TBFP_IDLE==instance->ReTxFsm.CurState) &&
        (0==instance->ReTxFsm.retx_cnt)) {
        res = true;
    }
    return res;
}

bool tbfp_retx_start(TbfpProtocol_t *instance, uint8_t *array, uint32_t len){
    bool res = false;
    if (instance && array && (0<len)) {
        memcpy(instance->tx_frame, array, len);
        instance->tx_frame_len = len;
        uint32_t time_stamp = get_time_ms32();
        if (IF_SX1262 == instance->interface) {
           instance->ReTxFsm.retx_cnt = TBFP_RETX_TRY_MAX;
           res = sx1262_start_tx(instance->tx_frame,
                                instance->tx_frame_len, TX_SINGLE_MODE);
           if(res) {
               instance->ReTxFsm.time_stamp_start_ms = time_stamp;
               instance->ReTxFsm.CurState = TBFP_WAIT_TX_DONE;
               LOG_DEBUG(RETX,"%s Send Try:%u",
                         interface2str(instance->interface),
                         TBFP_RETX_TRY_MAX - instance->ReTxFsm.retx_cnt+1);
           } else {
               instance->ReTxFsm.CurState = TBFP_IDLE;
           }
        } else {
            LOG_ERROR(RETX, "ReTxOnlyForSX1262_If");
        }
    }
    return res;
}

bool tbfp_retx_init(TbfpProtocol_t *instance){
    bool res = false;
    if (instance) {
        instance->ReTxFsm.CurState = TBFP_IDLE;
        instance->ReTxFsm.retx_cnt = 0;
        res = true;
    }
    return res;
}

static bool tbfp_proc_retx_wait_tx_done(TbfpProtocol_t *instance,
                                 uint32_t time_stamp_cur){
    bool res = false;
    uint32_t time_stamp_diff = 0;
    time_stamp_diff = time_stamp_cur-instance->ReTxFsm.time_stamp_start_ms;

    switch(instance->ReTxFsm.input){
        case TBFP_IN_TX_DONE:
            instance->ReTxFsm.CurState= TBFP_WAIT_ACK;
            instance->ReTxFsm.time_stamp_start_ms = time_stamp_cur;
            LOG_DEBUG(RETX,"%s TxDoneOk", interface2str(instance->interface));
            res = true;
            break;
        case TBFP_IN_TX_DONE_TIME_OUT:
            instance->ReTxFsm.CurState=TBFP_IDLE;
            instance->ReTxFsm.err_cnt++;
            LOG_ERROR(RETX,"%s LackTxDone", interface2str(instance->interface));
            break;
        case TBFP_IN_TX_FRAME:
        case TBFP_IN_RX_ACK:
        case TBFP_IN_NONE:
        case TBFP_IN_RX_ACK_TIME_OUT:
            res = true;
            break;
        default:
            res = false;
            break;
    }

    if(WAIT_TX_DONE_TIME_OUT_MS < time_stamp_diff){
        instance->ReTxFsm.input=TBFP_IN_TX_DONE_TIME_OUT;
        res = true;
    }
    return res;
}

static bool tbfp_proc_retx_wait_ack(TbfpProtocol_t *instance, uint32_t time_stamp_cur){
    bool res = false;
    uint32_t time_stamp_diff = 0;
    time_stamp_diff = time_stamp_cur- instance->ReTxFsm.time_stamp_start_ms;

    switch(instance->ReTxFsm.input){
        case TBFP_IN_RX_ACK:
            instance->ReTxFsm.CurState = TBFP_IDLE;
            instance->ReTxFsm.spin_cnt++;
            LOG_DEBUG(RETX,"%s AckOk", interface2str(instance->interface));
            res = true;
            break;
        case TBFP_IN_RX_ACK_TIME_OUT:
            instance->ReTxFsm.CurState=TBFP_IDLE;
            instance->ReTxFsm.err_cnt++;
            LOG_ERROR(RETX,"%s LackAck", interface2str(instance->interface));
            res = true;
            break;
        case TBFP_IN_NONE:
        case TBFP_IN_TX_DONE_TIME_OUT:
        case TBFP_IN_TX_FRAME:
        case TBFP_IN_TX_DONE:
            res = true;
            break;
        default:
            res = false;
            break;
    }

    if(WAIT_ACK_TIME_OUT_MS < time_stamp_diff){
        instance->ReTxFsm.input  = TBFP_IN_RX_ACK_TIME_OUT;
    }
    return res;
}

static bool tbfp_proc_retx_idle(TbfpProtocol_t *instance){
    bool res = false;
    if(0<instance->ReTxFsm.retx_cnt){
        instance->ReTxFsm.retx_cnt--;
        if (IF_SX1262 == instance->interface) {
           res = sx1262_start_tx(instance->tx_frame,
                                instance->tx_frame_len, TX_SINGLE_MODE);
           if(res) {
               LOG_DEBUG(RETX,"%s Send Try:%u",
                         interface2str(instance->interface),
                         TBFP_RETX_TRY_MAX- instance->ReTxFsm.retx_cnt+1);
               instance->ReTxFsm.CurState= TBFP_WAIT_TX_DONE;
           } else {
               instance->ReTxFsm.CurState= TBFP_IDLE;
           }
        }else{
            LOG_ERROR(RETX, "ReTxOnlyForSX1262_If");
        }
    }
    return res;
}

static bool tbfp_proc_retx_one(TbfpProtocol_t *instance){
    bool res = false;
    uint32_t time_stamp = get_time_ms32();
    if(instance){
        switch(instance->ReTxFsm.CurState){
        case TBFP_IDLE:
            res= tbfp_proc_retx_idle(instance);
        break;
        case TBFP_WAIT_TX_DONE:
            res= tbfp_proc_retx_wait_tx_done(instance, time_stamp);
            break;
        case TBFP_WAIT_ACK:
            res= tbfp_proc_retx_wait_ack(instance, time_stamp);
            break;
        default:
            res = false;
            break;
        }
    }
    return res;
}

bool tbfp_retx_proc(void){
    bool res = false;
    bool out_res = true;
    uint8_t i = 0;
    for(i=0; i<ARRAY_SIZE(TbfpProtocol); i++){
        res = tbfp_proc_retx_one(&TbfpProtocol[i]);
        if(false == res) {
            out_res = false;
        }
    }
    return out_res;
}

bool tbfp_retx_ack(TbfpProtocol_t *instance, uint16_t ser_num){
    bool res = false;
    if (instance) {
        if(instance->ReTxFsm.expected_ser_num == ser_num){
            instance->ReTxFsm.input=TBFP_IN_RX_ACK;
            res = true;
        }
    }
    return res;
}

bool tbfp_retx_tx_done(Interfaces_t interface){
    bool res = false;
    uint8_t i = 0;
    for(i=0; i<ARRAY_SIZE(TbfpProtocol); i++){
        if(TbfpProtocol[i].interface==interface){
            TbfpProtocol[i].ReTxFsm.input=TBFP_IN_TX_DONE;
            res = true;
        }
    }

    return res;
}
