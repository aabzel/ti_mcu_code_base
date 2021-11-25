#ifndef TBFP_PROTOCOL_PARSER_H
#define TBFP_PROTOCOL_PARSER_H

/*TBFP Trivial Binary Frame Ðrotocol*/
#ifdef __cplusplus
extern "C" {
#endif
 
#include "tbfp_protocol.h"
#include "protocol_common.h"

#include <stdbool.h>
#include <stdint.h>


#define TBFP_RX_FRAME_SIZE 256U
#define TBFP_SIZE_HEADER 2
#define TBFP_SIZE_CRC 1
#define TBFP_SIZE_OVERHEAD (TBFP_SIZE_HEADER+TBFP_SIZE_CRC)
#define TBFP_MAX_PAYLOAD (256-TBFP_SIZE_OVERHEAD)

typedef struct xTBFTparser_t{
    RxState_t rx_state;
    uint8_t read_crc8;
    uint8_t exp_payload_len;
    uint32_t load_len;
    uint8_t rx_frame[TBFP_RX_FRAME_SIZE];
    uint8_t fix_frame[TBFP_RX_FRAME_SIZE];
}TBFTparser_t;


#ifdef __cplusplus
}
#endif

#endif /* TBFP_PROTOCOL_PARSER_H */
