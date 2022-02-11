#ifndef TBFP_RETX_DIAG_H
#define TBFP_RETX_DIAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "tbfp_re_tx_ack_fsm_types.h"

char* tbfp_retx_in2str(TbfpReTxInput_t in);
char* tbfp_retx_state2str(TbfpReTxState_t state);

#ifdef __cplusplus
}
#endif

#endif /* TBFP_RETX_DIAG_H */
