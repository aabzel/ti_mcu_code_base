#ifndef PROTOCOL_GENERAL_H
#define PROTOCOL_GENERAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum eRxState_t{
    WAIT_PREAMBLE=0,
    WAIT_LEN=1,
    WAIT_SERIAL_NUM = 2,
    WAIT_PAYLOAD=3,
    WAIT_CRC=4,
    RX_DONE=5,
}RxState_t;

#ifdef __cplusplus
}
#endif

#endif /* PROTOCOL_GENERAL_H */
