#ifndef TBFP_CONSTANTS_H
#define TBFP_CONSTANTS_H

/*TBFP Trivial Binary Frame Protocol*/
#ifdef __cplusplus
extern "C" {
#endif


#define TBFP_SIZE_CRC 1
#define TBFP_SIZE_LEN 2
#define TBFP_SIZE_SN 2
#define TBFP_SIZE_PREAMBLE 1
#define TBFP_SIZE_RE_TX_CNT 1

#define TBFP_SIZE_HEADER (TBFP_SIZE_PREAMBLE+TBFP_SIZE_LEN+TBFP_SIZE_SN+TBFP_SIZE_RE_TX_CNT)
#define TBFP_SIZE_OVERHEAD (TBFP_SIZE_HEADER+TBFP_SIZE_CRC)
#define TBFP_MAX_PAYLOAD (512+3+2+ 10)
#define TBFP_MAX_FRAME (TBFP_MAX_PAYLOAD+TBFP_SIZE_OVERHEAD)

#define TBFP_PREAMBLE_DFLT 0xA5
// https://docs.google.com/spreadsheets/d/1VAT3Ak7AzcufgvuRHrRVoVfC3nxugFGJR5pyzxL4W7Q/edit#gid=0
#define TBFP_INDEX_PREAMBLE 0
#define TBFP_INDEX_RETX 1
#define TBFP_INDEX_SER_NUM 2
#define TBFP_INDEX_LEN 4
#define TBFP_INDEX_PAYLOAD 6

#define TBFP_SIZE_ID 1
#define TBFP_SIZE_CRC 1

#define TBFP_OVERHEAD_SIZE (sizeof(TbfHeader_t) + TBFP_SIZE_CRC )

typedef enum xFrameId_t {
    FRAME_ID_CMD = 0x44,    /*D*/
    FRAME_ID_CHAT = 0x43,   /*C*/
    FRAME_ID_PING = 0x50,   /*P*/
    FRAME_ID_TUNNEL = 0x54, /*T matryoshka*/
    FRAME_ID_PONG = 0x90,   /**/
    FRAME_ID_RTCM3 = 0xD3,  /**/
} FrameId_t;


#ifdef __cplusplus
}
#endif

#endif /* TBFP_CONSTANTS_H */
