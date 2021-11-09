#ifndef STR_UTILS_EX_H
#define STR_UTILS_EX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct xString_t {
  char *ptr;
  uint32_t len;
} string_t;


uint32_t count_substring(char* in_str, char* substr);
char *select_sub_string(char *text, char *tail);
bool parse_text_after_prefix(char *cur_file_str, int in_str_len, char *out_text,
                             uint16_t *text_len, char *prefix, char terminator);


bool parse_lhex_array_after_prefix(char *prefix,
                                   uint32_t prefix_num,
                                  char *in_str_array,
                                  char *out_array,
                                  uint32_t out_arr_size);
#ifdef __cplusplus
}
#endif

#endif /* STR_UTILS_EX_H */
