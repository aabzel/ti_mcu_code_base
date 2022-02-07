
#include "tbfp_retx_diag.h"

#include <string.h>
#include <inttypes.h>

#include "tbfp_re_tx_ack_fsm_types.h"

char* tbfp_retx_in2str(TbfpReTxInput_t in){
    char *name="?";
    switch(in) {
        case TBFP_IN_NONE :  name="None";break;
        case TBFP_IN_RX_ACK: name="RxAck";break;
        case TBFP_IN_RX_ACK_TIME_OUT :name="RxAckTineOut";break;
        case TBFP_IN_TX_DONE_TIME_OUT:name="TxDoneTimeOut";break;
        case TBFP_IN_TX_FRAME:name="TxFrame"; break;
        case TBFP_IN_TX_DONE: name="TxDone"; break;
        default: break;
    }
    return name;
}

char* tbfp_retx_state2str(TbfpReTxState_t state){
    char *name="?";
    switch(state) {
        case TBFP_IDLE :  name="Idle";break;
        case TBFP_WAIT_TX_DONE: name="WaitTx";break;
        case TBFP_WAIT_ACK :name="WaitAck";break;
        default: break;
    }
    return name;
}
