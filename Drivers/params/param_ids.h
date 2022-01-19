#ifndef PARAM_IDS_H
#define PARAM_IDS_H

#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"
#include "param_types.h"

//#define PARAM_CNT 32U
#define PARAM_PERIOD_US S_2_US(3)

typedef enum eId_t {
    PAR_ID_REBOOT_CNT = 1,
    PAR_ID_LORA_FREQ = 2,       /*Hz*/
    PAR_ID_LORA_SF = 3,         /*Chips / Symbol*/
    PAR_ID_LORA_CR = 4,         /*in raw bits / total bits*/
    PAR_ID_LORA_BW = 5,         /*Hz*/
    PAR_ID_PREAMBLE_LENGTH = 6, /*byte*/
    PAR_ID_PAYLOAD_LENGTH = 7,  /*byte*/
    PAR_ID_CRC_TYPE = 8,        /*On Off*/
    PAR_ID_PACKET_TYPE = 28,    /* GFSK / LoRa*/
    PAR_ID_HEADER_TYPE = 9,     /*Variable length packet / Fixed length packet*/
    PAR_ID_APP_START = 11,      /*Flash address*/
    PAR_ID_IQ_SETUP = 10,
    PAR_ID_BOOT_CMD = 12,
    PAR_ID_BOOT_CNT = 13, /*To spot hangs on in App*/
    PAR_ID_APP_STATUS = 14,
    PAR_ID_PWR_SRC = 15,        /*power source Battary XP3(1) or external XP1(0)*/
    PAR_ID_LORA_OUT_POWER = 16, /*output power*/
    PAR_ID_TIME_ZONE = 17,      /*TimeZone*/
    PAR_ID_LORA_MAX_LINK_DIST = 18,
    PAR_ID_LORA_MAX_BIT_RATE = 19,
    PAR_ID_BASE_LOCATION = 20,
    PAR_ID_BASE_ACC = 33, /*Base Fix pos Accuracy in MM*/
    PAR_ID_RTK_MODE = 21,
    PAR_ID_BASE_ALT = 22, /* Base station altitude above sea level*/
    PAR_ID_GNSS_PERIOD = 23,
    PAR_ID_RTK_CHANNEL = 24,
    PAR_ID_SERIAL_NUM = 25,
    PAR_ID_RTK_FIX_LONG = 26, /*Max Duration in RTK fixed mode*/
    PAR_ID_SYNC_WORD = 27,
    PAR_ID_LORA_SYNC_WORD = 29,
    PAR_ID_LORA_CRC_INIT = 32,
    PAR_ID_LORA_CRC_POLY = 31,
    PAR_ID_LOW_DATA_RATE = 30,
    PAR_ID_GPS=34,
    PAR_ID_GLONASS=35,
    PAR_ID_GALILEO=36,
    PAR_ID_BEI_DOU=37,
    PAR_ID_TEST_START = 100,
    PAR_ID_TEST_END = 200,
    PAR_ID_CNT
} Id_t;

typedef struct xParamItem_t {
    Id_t id;
    uint16_t len;
    ParamType_t type;
    char* name;
    /*char* units*/
} ParamItem_t;

extern const ParamItem_t ParamArray[];

bool param_init(void);
bool param_proc(void);
bool raw_val_2str(uint8_t* value, uint32_t value_len, ParamType_t type, char* out_str, uint32_t str_size);
ParamType_t param_get_type(Id_t id);
uint32_t param_get_cnt(void);
const char* param_val2str(uint16_t id, uint8_t* value, uint32_t size);

#endif /* PARAM_IDS_H  */
