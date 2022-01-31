#ifndef TBFP_PROTOCOL_PARSER_H
#define TBFP_PROTOCOL_PARSER_H

/*TBFP Trivial Binary Frame Protocol*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "protocol_common.h"
#include "tbfp_constants.h"

//https://docs.google.com/spreadsheets/d/1VAT3Ak7AzcufgvuRHrRVoVfC3nxugFGJR5pyzxL4W7Q/edit#gid=0

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
