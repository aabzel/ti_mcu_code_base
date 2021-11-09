
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "macro_utils.h"
#include "utils_file.h"

uint32_t file_get_size(char* file_name) {
    uint32_t fize_size = 0;
    FILE* p_file;
    p_file = fopen(file_name, "rb");
    if(p_file) {
        printf("\n[d] Open file [%s]\n", file_name);
        fseek(p_file, 0, SEEK_END); // seek to end of file
        fize_size = ftell(p_file);  // get current file pointer
        fseek(p_file, 0, SEEK_SET); // seek back to beginning of file
        // read size of firmware
        fclose(p_file);
    } else {
        printf("\n[e] File open error [%s]\n", file_name);
    }
    return fize_size;
}

bool file_load_to_array(char* file_name, uint8_t** out_buff, uint32_t* out_size) {
    bool res = false;
    FILE* p_file;
    uint32_t file_size = file_get_size(file_name);
    if(file_size) {
        printf("\n[*] file_size %u Byte %u kByte\n", file_size, file_size / K_BYTES);
    }

    if((0 < file_size) && (out_size) && (NULL != *out_buff)) {
        *out_size = file_size;
        uint8_t* file_array = NULL;
        file_array = (uint8_t*)malloc(file_size);
        if(file_array) {
            p_file = fopen(file_name, "rb");
            if(p_file) {
                printf("\n[*] Open file [%s]\n", file_name);
                res = true;
                size_t real_read = fread(file_array, file_size, 1, p_file);
                if(real_read == file_size) {
                    printf("\n[d] file load OK %s", file_name);
                    *out_buff = file_array;
                    res = true;
                } else {
                    printf("\n[e] File load error real_read:%u\n", real_read);
                    res = false;
                }
                // read size of firmware
                fclose(p_file);
            } else {
                printf("\n[e] File open error [%s]\n", file_name);
            }
        }
    }
    return res;
}
