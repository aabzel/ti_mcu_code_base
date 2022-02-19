#ifndef PARAM_IDS_H
#define PARAM_IDS_H

#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"
#include "param_types.h"

#define PARAM_PERIOD_US SEC_2_US(3)

extern const ParamItem_t ParamArray[];

#ifdef HAS_LOG
#define LOAD_PARAM(COMPONENT, PAR_ID, VARIABLE, EXP_LEN, VAR_NAME, DEF_VAL, PARSER_FUNC)                               \
    do {                                                                                                               \
        uint16_t file_len = 0;                                                                                         \
        res = mm_get(PAR_ID, (uint8_t*)&VARIABLE, sizeof(VARIABLE), &file_len);                                        \
        if((true == res) && ((EXP_LEN) == file_len)) {                                                                 \
            LOG_INFO(COMPONENT, "Set" VAR_NAME "FromParams %u [%s]", VARIABLE, PARSER_FUNC(VARIABLE));                 \
        } else {                                                                                                       \
            LOG_WARNING(COMPONENT, "SetDflt" VAR_NAME " %u [%s]", VARIABLE, PARSER_FUNC(DEF_VAL));                     \
            VARIABLE = DEF_VAL;                                                                                        \
            res = false;                                                                                               \
            out_res = false;                                                                                           \
        }                                                                                                              \
    } while(0);
#else
#define LOAD_PARAM(COMPONENT, PAR_ID, VARIABLE, EXP_LEN, VAR_NAME, DEF_VAL, PARSER_FUNC)                               \
    do {                                                                                                               \
        uint16_t file_len = 0;                                                                                         \
        res = mm_get(PAR_ID, (uint8_t*)&VARIABLE, sizeof(VARIABLE), &file_len);                                        \
        if(!((true == res) && ((EXP_LEN) == file_len))) {                                                              \
            VARIABLE = DEF_VAL;                                                                                        \
            res = false;                                                                                               \
            out_res = false;                                                                                           \
        }                                                                                                              \
    } while(0);
#endif

bool param_init(void);
bool param_proc(void);
bool raw_val_2str(uint8_t* value, uint32_t value_len, ParamType_t type, char* out_str, uint32_t str_size);
ParamType_t param_get_type(Id_t id);
uint32_t param_get_cnt(void);
const char* param_val2str(uint16_t id, uint8_t* value, uint32_t size);

#endif /* PARAM_IDS_H  */
