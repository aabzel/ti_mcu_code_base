#ifndef TBFP_RETX_ACK_FSM_H
#define TBFP_RETX_ACK_FSM_H

/*TBFP Trivial Binary Frame Protocol*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "tbfp_types.h"
#include "system.h"

bool is_tbfp_retx_idle(TbfpProtocol_t *instance);
bool tbfp_retx_start(TbfpProtocol_t *instance, uint8_t *array, uint32_t len);
bool tbfp_retx_tx_done(Interfaces_t interface);
bool tbfp_retx_ack(TbfpProtocol_t *instance, uint16_t sn);
bool tbfp_retx_proc(void);
bool tbfp_retx_init(TbfpProtocol_t *instance);
 
#ifdef __cplusplus
}
#endif

#endif /* TBFP_RETX_ACK_FSM_H */
