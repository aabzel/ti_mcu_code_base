#ifndef PROTOCOL_GENERAL_H
#define PROTOCOL_GENERAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum eRxState_t {
    WAIT_PREAMBLE = 0,
    WAIT_LEN = 1,
    WAIT_SERIAL_NUM = 2,
    WAIT_RETX_CNT = 3,
    WAIT_PAYLOAD = 4,
    WAIT_CRC = 5,
    RX_DONE = 6,
} RxState_t;

#ifdef __cplusplus
}
#endif

#endif /* PROTOCOL_GENERAL_H */
