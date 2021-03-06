#include "param_commands.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "convert.h"
#include "data_utils.h"
#include "flash_fs.h"
#include "io_utils.h"
#include "log.h"
#include "param_ids.h"
#include "param_types.h"
#include "str_utils.h"
#include "table_utils.h"
#include "writer_config.h"

#ifdef ESP32
#define io_printf printf
#endif

bool cmd_param_get(int32_t argc, char* argv[]) {
    bool res = false;
    uint16_t param_id = 0;
    if(1 == argc) {
        res = true;
        res = try_str2uint16(argv[0], &param_id);
        if(false == res) {
            LOG_ERROR(PARAM, "Unable to extract param_id %s", argv[0]);
        }
    }
    if(res) {
        uint16_t i = 0;
        char valStr[40] = "";
        static const table_col_t cols[] = {{5, "id"}, {14, "name"}, {5, "len"}, {12, "val"}, {17, "name"}};
        table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
        for(i = 0; i < param_get_cnt(); i++) {
            if(param_id == ParamArray[i].id) {
                io_printf(TSEP);
                io_printf(" %3u " TSEP, ParamArray[i].id);
                io_printf(" %12s " TSEP, ParamArray[i].name);
                io_printf(" %3u " TSEP, ParamArray[i].len);

                uint16_t value_len = 0;
                uint8_t value[100] = {0};
                res = mm_get(ParamArray[i].id, value, ParamArray[i].len, &value_len);
                if(true == res) {
                    if(ParamArray[i].len == value_len) {
                        res = raw_val_2str(value, value_len, ParamArray[i].type, valStr, sizeof(valStr));
                    } else {
                        snprintf(valStr, sizeof(valStr), "lenErr:%u", value_len);
                    }
                } else {
                    strncpy(valStr, "lack", sizeof(valStr));
                }

                io_printf(" %10s " TSEP, valStr);
                io_printf(" %s " TSEP, param_val2str(ParamArray[i].id, value, sizeof(value)));
                io_printf(CRLF);
            }
        }
        table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    }
    return res;
}

bool param_diag(char* keyWord1, char* keyWord2) {
    bool res = false;
    char valStr[40] = "";
    memset(valStr, 0x00, sizeof(valStr));
    uint16_t i = 0, num = 1;
    static const table_col_t cols[] = {{5, "No"}, {5, "id"}, {14, "name"}, {5, "len"}, {12, "val"}, {17, "name"}};
    table_header(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));
    char line_str[500] = "";
    char suffix_str[50] = "";
    for(i = 0; i < param_get_cnt(); i++) {
        memset(line_str, 0x0, sizeof(line_str));
        strncat(line_str, TSEP, sizeof(line_str));

        snprintf(suffix_str, sizeof(suffix_str), " %3u " TSEP, ParamArray[i].id);
        strncat(line_str, suffix_str, sizeof(line_str));

        snprintf(suffix_str, sizeof(suffix_str), " %12s " TSEP, ParamArray[i].name);
        strncat(line_str, suffix_str, sizeof(line_str));

        snprintf(suffix_str, sizeof(suffix_str), " %3u " TSEP, ParamArray[i].len);
        strncat(line_str, suffix_str, sizeof(line_str));

        uint16_t value_len = 0;
        uint8_t value[100] = {0};
        res = mm_get(ParamArray[i].id, value, ParamArray[i].len, &value_len);
        if(true == res) {
            if(ParamArray[i].len == value_len) {
                res = raw_val_2str(value, value_len, ParamArray[i].type, valStr, sizeof(valStr));
            } else {
                snprintf(valStr, sizeof(valStr), "lenErr:%u", value_len);
            }
        } else {
            strncpy(valStr, "lack", sizeof(valStr));
        }

        snprintf(suffix_str, sizeof(suffix_str), " %10s " TSEP, valStr);
        strncat(line_str, suffix_str, sizeof(line_str));

        snprintf(suffix_str, sizeof(suffix_str), " %15s " TSEP, param_val2str(ParamArray[i].id, value, sizeof(value)));
        strncat(line_str, suffix_str, sizeof(line_str));

        if(is_contain(line_str, keyWord1, keyWord2)) {
            io_printf(TSEP " %3u ", num);
            io_printf("%s" CRLF, line_str);
            num++;
        }
    }
    table_row_bottom(&(curWriterPtr->s), cols, ARRAY_SIZE(cols));

    return res;
}

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
        res = param_diag(keyWord1, keyWord2);
    } else {
        LOG_ERROR(PARAM, "Usage: pg");
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
            case BOOL:
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
                res = try_str2float(argv[1], (float*)wrData);
                if(res) {
                    write_len = 4;
                }
                break;
            case DOUBLE:
                res = try_str2double(argv[1], (double*)wrData);
                if(res) {
                    write_len = 8;
                }
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
                LOG_INFO(PARAM, "Write param_id %u, %u byte OK", param_id, write_len);
            } else {
                LOG_ERROR(PARAM, "Unable to write param_id %u", param_id);
            }
        } else {
            LOG_ERROR(PARAM, "len error");
        }
    } else {
        LOG_ERROR(PARAM, "Usage: ps id data");
    }
    return res;
}
