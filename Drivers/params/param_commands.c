#include "param_commands.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "convert.h"
#include "data_utils.h"
#include "flash_fs.h"
#include "log.h"
#include "param_ids.h"
#include "param_types.h"
#include "str_utils.h"
#include "table_utils.h"

bool cmd_param_diag(int32_t argc, char* argv[]) {
    bool res = false;
    char keyWord1[20] = "";
    char keyWord2[20] = "";
    if(0 <= argc) {
        strncpy(keyWord1, "", sizeof(keyWord1));
        strncpy(keyWord2, "", sizeof(keyWord2));
        res = true;
    }
    if(1 <= argc) {
        strncpy(keyWord1, argv[0], sizeof(keyWord1));
        res = true;
    }
    if(2 <= argc) {
        strncpy(keyWord2, argv[1], sizeof(keyWord2));
        res = true;
    }

    if(2 < argc) {
        LOG_ERROR(PARAM, "Usage: pg keyWord1 keyWord2");
    }

    if(res) {
        char valStr[40];
        memset(valStr, 0x00, sizeof(valStr));
        uint16_t i = 0, num = 1;
        static const table_col_t cols[] = {{5, "No"}, {5, "id"}, {12, "name"}, {5, "len"}, {11, "val"}};
        table_header(&dbg_o.s, cols, ARRAY_SIZE(cols));
        char temp_str[120] = "";
        for(i = 0; i < PARAM_CNT; i++) {
            strcpy(temp_str, TSEP);
            snprintf(temp_str, sizeof(temp_str), "%s %3u " TSEP, temp_str, ParamArray[i].id);
            snprintf(temp_str, sizeof(temp_str), "%s %10s " TSEP, temp_str, ParamArray[i].name);
            snprintf(temp_str, sizeof(temp_str), "%s %3u " TSEP, temp_str, ParamArray[i].len);
            uint16_t value_len = 0;
            uint8_t value[ParamArray[i].len];
            res = mm_get(ParamArray[i].id, value, ParamArray[i].len, &value_len);
            if(true == res) {
                if(ParamArray[i].len == value_len) {
                    res = raw_val_2str(value, value_len, ParamArray[i].type, valStr, sizeof(valStr));
                } else {
                    strncpy(valStr, "lenErr", sizeof(valStr));
                }
            } else {
                strncpy(valStr, "lack", sizeof(valStr));
            }

            snprintf(temp_str, sizeof(temp_str), "%s %9s " TSEP, temp_str, valStr);
            snprintf(temp_str, sizeof(temp_str), "%s" CRLF, temp_str);
            if(is_contain(temp_str, keyWord1, keyWord2)) {
                io_printf(TSEP " %3u ", num);
                io_printf("%s", temp_str);
                num++;
            }
        }
        table_row_bottom(&dbg_o.s, cols, ARRAY_SIZE(cols));
    }
    return res;
}

bool cmd_param_set(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint8_t wrData[100];
        memset(wrData, 0x00, sizeof(wrData));
        uint16_t write_len = 0;
        uint16_t param_id = 0;
        if(true == res) {
            res = try_str2uint16(argv[0], &param_id);
            if(false == res) {
                LOG_ERROR(PARAM, "Unable to extract param_id %s", argv[0]);
            }
        }
        // determine param type
        ParamType_t par_type = param_get_type((Id_t)param_id);
        // get data
        if(UNDEF != par_type) {
            res = false;
            uint16_t arg_len = 0;
            arg_len = (uint16_t)strlen((char*)argv[1]);
            switch(par_type) {
            case UINT8:
                res = try_str2uint8(argv[1], wrData);
                if(res) {
                    write_len = 1;
                }
                break;
            case INT8:
                res = try_str2int8(argv[1], (int8_t*)wrData);
                if(res) {
                    write_len = 1;
                }
                break;
            case UINT16:
                res = try_str2uint16(argv[1], (uint16_t*)wrData);
                if(res) {
                    write_len = 2;
                }
                break;
            case INT16:
                res = try_str2int16(argv[1], (int16_t*)wrData);
                if(res) {
                    write_len = 2;
                }
                break;
            case UINT32:
            case UINT32_HEX:
                res = try_str2uint32(argv[1], (uint32_t*)wrData);
                if(res) {
                    write_len = 4;
                }
                break;
            case INT32:
                res = try_str2int32(argv[1], (int32_t*)wrData);
                if(res) {
                    write_len = 4;
                }
                break;
            case UINT64:
                res = try_str2uint64(argv[1], (uint64_t*)wrData);
                if(res) {
                    write_len = 8;
                }
                break;
            case INT64:
                res = try_str2int64(argv[1], (int64_t*)wrData);
                if(res) {
                    write_len = 8;
                }
                break;
            case STRING:
                if(arg_len < sizeof(wrData)) {
                    strncpy((char*)wrData, argv[1], sizeof(wrData));
                    res = true;
                    write_len = arg_len + 1;
                }
                break;
            case FLOAT:
                res = try_str2float(argv[1], (float_t*)wrData);
                break;
            default:
                res = false;
                break;
            }
        } else {
            LOG_ERROR(PARAM, "Undef param_id %u", param_id);
            res = false;
        }
        if((true == res) && (0 < write_len)) {
            /* write data to flash FS*/
            res = mm_set(param_id, wrData, write_len);
            if(true == res) {
                LOG_INFO(PARAM, "Write param_id %u OK", param_id);
            } else {
                LOG_ERROR(PARAM, "Unable to write param_id %u", param_id);
            }
        }
    } else {
        LOG_ERROR(PARAM, "Usage: ps id data");
    }
    return res;
}
