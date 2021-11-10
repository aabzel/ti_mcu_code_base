#ifndef ARRAY_API_H
#define ARRAY_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct xArray_t {
    uint32_t size;
    uint8_t *pArr;
} Array_t;
 
uint8_t *memdup(const uint8_t *mem, size_t size);
bool is_arr_pat(uint8_t* arr, size_t size, uint8_t patt) ;
bool array_rand(uint8_t* arr, size_t size);

#ifdef HAS_LOADER
bool is_array_equal(uint8_t* array1,uint8_t* array2, size_t size, uint32_t* match, uint32_t* diff);
#endif

#ifdef __cplusplus
}
#endif

#endif /* ARRAY_API_H */
