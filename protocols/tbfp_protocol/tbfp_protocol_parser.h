#ifndef TBFP_PROTOCOL_PARSER_H
#define TBFP_PROTOCOL_PARSER_H

/*TBFP Trivial Binary Frame Protocol*/
#ifdef __cplusplus
extern "C" {
#endif
 
#include "tbfp_protocol.h"
#include "protocol_common.h"

#include <stdbool.h>
#include <stdint.h>

//https://docs.google.com/spreadsheets/d/1VAT3Ak7AzcufgvuRHrRVoVfC3nxugFGJR5pyzxL4W7Q/edit#gid=0
#define TBFP_SIZE_CRC 1
#define TBFP_SIZE_LEN 2
#define TBFP_SIZE_SN 2
#define TBFP_SIZE_PREAMBLE 1
#define TBFP_SIZE_RE_TX_CNT 1

#define TBFP_SIZE_HEADER (TBFP_SIZE_PREAMBLE+TBFP_SIZE_LEN+TBFP_SIZE_SN+TBFP_SIZE_RE_TX_CNT)
#define TBFP_SIZE_OVERHEAD (TBFP_SIZE_HEADER+TBFP_SIZE_CRC)
#define TBFP_MAX_PAYLOAD (512+3+2+ 10)
#define TBFP_MAX_FRAME (TBFP_MAX_PAYLOAD+TBFP_SIZE_OVERHEAD)

typedef struct xTBFTparser_t{
    RxState_t rx_state;
    uint8_t read_crc8;
    uint16_t exp_payload_len;
    uint32_t load_len;
    uint16_t s_num;
    uint8_t rx_frame[TBFP_MAX_FRAME];
    uint8_t fix_frame[TBFP_MAX_FRAME];
}TBFTparser_t;


#ifdef __cplusplus
}
#endif

#endif /* TBFP_PROTOCOL_PARSER_H */
