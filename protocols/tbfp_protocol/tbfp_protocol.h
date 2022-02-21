#ifndef TBFP_PROTOCOL_H
#define TBFP_PROTOCOL_H

/*TBFP Trivial Binary Frame Protocol*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "tbfp_constants.h"
#include "tbfp_types.h"
#ifdef HAS_GNSS
#include "gnss_utils.h"
#endif
#include "system.h"

extern TbfpProtocol_t TbfpProtocol[IF_CNT]; /*RS232 LoRa*/

bool tbfp_check_flow_control(Interfaces_t interface, uint16_t snum, uint16_t* const prev_s_num,
                             uint32_t* const con_flow, uint32_t* const max_con_flow);
bool tbfp_generate_frame(uint8_t* array, uint32_t len, Interfaces_t interface);
bool tbfp_parser_reset_rx(TbfpProtocol_t* instance, RxState_t state);
bool tbfp_send(uint8_t* tx_array, uint32_t len, Interfaces_t interface, uint8_t lifetime, TbfpAck_t ack);
bool tbfp_send_tunnel(uint8_t* tx_array, uint32_t len, Interfaces_t interface, TbfpAck_t ack);
bool tbfp_send_cmd(uint8_t* tx_array, uint32_t len, Interfaces_t interface);
bool tbfp_send_chat(uint8_t* tx_array, uint32_t len, Interfaces_t interface, uint8_t lifetime, TbfpAck_t ack);
bool tbfp_send_ping(uint8_t frame_id, Interfaces_t interface);
bool tbfp_protocol_init(TbfpProtocol_t* instance, Interfaces_t interface, uint8_t preamble_val);
bool tbfp_proc(uint8_t* arr, uint16_t len, Interfaces_t interface, bool is_reset);
bool tbfp_proc_full(uint8_t* arr, uint16_t len, Interfaces_t interface);
bool tbfp_proc_byte(TbfpProtocol_t* instance, uint8_t rx_byte);
bool is_tbfp_protocol(uint8_t* arr, uint16_t len, Interfaces_t interface);
bool tbfp_compose_ping(uint8_t* out_frame, uint32_t* tx_frame_len, TbfPingFrame_t* pingFrame, Interfaces_t interface);
bool tbfp_check(void);
bool tbfp_calc_byte_rate(void);

#ifdef __cplusplus
}
#endif

#endif /* TBFP_PROTOCOL_H */
