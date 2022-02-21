#ifndef PARAM_CONSTANTS_H
#define PARAM_CONSTANTS_H

#define ID_TYPE_LINK_INFO 0x0A /*4 bit val*/

typedef enum wParamType_t {
    UINT8,
    BOOL,
    INT8,
    UINT16,
    INT16,
    UINT32,
    UINT32_HEX,
    INT32,
    UINT64,
    INT64,
    STRING,
    FLOAT,
    DOUBLE,
    STRUCT,
    UNDEF,
} ParamType_t;

typedef enum eId_t {
    PAR_ID_REBOOT_CNT = 1,
#ifdef HAS_LORA
    PAR_ID_LORA_SF = 3,         /*Chips / Symbol*/
    PAR_ID_LORA_CR = 4,         /*in raw bits / total bits*/
    PAR_ID_LORA_BW = 5,         /*Hz*/
    PAR_ID_CRC_TYPE = 8,        /*On Off*/
    PAR_ID_IQ_SETUP = 10,
    PAR_ID_LORA_HEADER_TYPE = 9,     /*0-Variable length packet / 1-Fixed length packet*/
    PAR_ID_LORA_SYNC_WORD = 29,
#endif
    PAR_ID_LORA_FREQ = 2,       /*Hz*/
    PAR_ID_PREAMBLE_LENGTH = 6, /*byte*/
    PAR_ID_PAYLOAD_LENGTH = 7,  /*byte*/
    PAR_ID_PACKET_TYPE = 28,    /* GFSK / LoRa*/
    PAR_ID_APP_START = 11,      /*Flash address*/
    PAR_ID_BOOT_CMD = 12,
    PAR_ID_BOOT_CNT = 13, /*To spot hangs on in App*/
    PAR_ID_APP_STATUS = 14,
    PAR_ID_GNSS_MODE = 21,
#ifdef HAS_GNSS_RTK
    PAR_ID_BASE_ACC = 33, /*Base Fix pos Accuracy in MM*/
    PAR_ID_BASE_LOCATION = 20,
    PAR_ID_BASE_ALT = 22, /* Base station altitude above sea level*/
    PAR_ID_RTK_CHANNEL = 24,
    PAR_ID_GPS = 34,
    PAR_ID_GLONASS = 35,
    PAR_ID_GALILEO = 36,
    PAR_ID_BEI_DOU = 37,
    PAR_ID_RTK_FIX_LONG = 26, /*Max Duration in RTK fixed mode*/
#endif
    PAR_ID_PWR_SRC = 15,        /*power source Battary XP3(1) or external XP1(0)*/
    PAR_ID_LORA_OUT_POWER = 16, /*output power*/
    PAR_ID_TIME_ZONE = 17,      /*TimeZone*/
    PAR_ID_LORA_MAX_LINK_DIST = 18,
    PAR_ID_LORA_MAX_BIT_RATE = 19,
    PAR_ID_GNSS_PERIOD = 23,
    PAR_ID_SERIAL_NUM = 25,
    PAR_ID_SYNC_WORD = 27,
    PAR_ID_LORA_CRC_INIT = 32,
    PAR_ID_LORA_CRC_POLY = 31,
    PAR_ID_LOW_DATA_RATE = 30,
    PAR_ID_RETX_CNT = 38,
    PAR_ID_TEST_START = 100,
    PAR_ID_TEST_END = 200,
#ifdef HAS_GFSK
    PAR_ID_SYNC_WORD=39,
    PAR_ID_PRE_DET_LEN=40,
    PAR_ID_ADDR_COMP=41,
    PAR_ID_NODE_ADDR=42,
    PAR_ID_BROADCAST_ADDR=43,
    PAR_ID_GFSK_CRC_TYPE=44,
    PAR_ID_WHITENING=45,
    PAR_ID_WHITENING_INIT_VALUE=46,
#endif
    PAR_ID_TX_MUTE = 47,
    PAR_ID_CNT
} Id_t;


#endif /* PARAM_CONSTANTS_H  */
