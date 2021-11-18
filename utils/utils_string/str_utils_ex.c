#include "str_utils_ex.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "convert.h"

#ifdef HAS_MCU
#error That code only for desktop builds
#endif

uint32_t count_substring(char* in_str, char* substr) {
    uint32_t match_cnt = 0u;
    int in_str_len = strlen(in_str);
    if(0 < in_str_len) {
        int sub_str_len = strlen(substr);
        char* cur_ptr = strstr(in_str, substr);
        while(NULL != cur_ptr) {
            match_cnt++;
            cur_ptr = strstr(&cur_ptr[sub_str_len], substr);
        }
    }

    return match_cnt;
}

bool parse_lhex_array_after_prefix(char* prefix, uint32_t prefix_num, char* in_str_array, char* out_array,
                                   uint32_t out_arr_size) {
    bool res = false;
    uint32_t i = 0;
#ifdef HAS_STR_DEBUG
    printf("\n prefix [%s] prefix_num [%u]", prefix, prefix_num);
#endif
    char* ptr = in_str_array;
    do {
        ptr = strstr(ptr, prefix);
        if(ptr) {
            if(i == prefix_num) {
#ifdef HAS_STR_DEBUG
                printf("\n spot prefix [%s] start[%s] i=%u", prefix, ptr, i);
#endif
                uint32_t out_array_len = 0;
                res = try_strl2array(ptr, 2 * out_arr_size, out_array, out_arr_size, &out_array_len);
                if(res) {
#ifdef HAS_STR_DEBUG
                    printf("\n spot array");
#endif
                    if(out_array_len == out_arr_size) {
                        res = true;
                    } else {
#ifdef HAS_STR_DEBUG
                        printf("\n[e] array len error");
#endif
                        res = false;
                    }
                } else {
#ifdef HAS_STR_DEBUG
                    printf("\n[e] parse array error");
#endif
                }
                break;
            }
            ptr++;
            i++;
        }
    } while(ptr);

    return res;
}

bool parse_text_after_prefix(char* cur_file_str, int in_str_len, char* out_text, uint16_t* text_len, char* prefix,
                             char terminator) {
    bool res = false;
    (*text_len) = 0;
    int prefix_len = strlen(prefix);
    char* ptr = strstr(cur_file_str, prefix);
    if(ptr) {
#ifdef DEBUG_GENERATE_REG_PARSER
        printf("\n spot prefix [%s]", prefix);
#endif
        res = true;
        int i = 0;
        for(i = 0; i < in_str_len; i++) {
            if(('"' != *(ptr + prefix_len + i)) && (terminator != *(ptr + prefix_len + i))) {
                out_text[i] = *(ptr + prefix_len + i);
                (*text_len)++;
                res = true;
            } else {
                break;
            }
        }
        out_text[i] = 0x00;
    } else {
        res = false;
#ifdef DEBUG_GENERATE_REG_PARSER
        printf("\n lack of prefix [%s]", prefix);
#endif
    }
    return res;
}
