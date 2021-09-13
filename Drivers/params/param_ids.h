#ifndef PARAM_IDS_H
#define PARAM_IDS_H

#include <stdbool.h>
#include <stdint.h>

#include "param_types.h"

#define PARAM_CNT 5

typedef enum eId_t {
    PAR_ID_REBOOT_CNT = 1,
    PAR_ID_LORA_FREQ = 2, /*Hz*/
    PAR_ID_LORA_SF = 3,   /*Chips / Symbol*/
    PAR_ID_LORA_CR = 4,
    PAR_ID_LORA_BW = 5, /*Hz*/
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

extern const ParamItem_t ParamArray[PARAM_CNT];

ParamType_t param_get_type(Id_t id);
bool raw_val_2str(uint8_t* value, uint16_t value_len, ParamType_t type, char* out_str, uint32_t str_size);

#endif /* PARAM_IDS_H  */
