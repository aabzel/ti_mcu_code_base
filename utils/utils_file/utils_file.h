#ifndef UTILS_FILE_H
#define UTILS_FILE_H

#include <stdbool.h>
#include <stdint.h>

bool file_load_to_array(char* file_name, uint8_t** out_buff, uint32_t* out_size);

#endif /* UTILS_FILE_H */
