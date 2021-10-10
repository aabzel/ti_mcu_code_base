#include "array.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifdef X86_64
#include<time.h>
#endif
#ifdef HAS_MCU
#include "clocks.h"
#endif
 /*
  Note: assume caller calls free().
  */
uint8_t *memdup(const uint8_t *const mem, size_t size) {
    uint8_t *out_new_mem = NULL;
    if ((0 < size) && (NULL != mem)) {
        out_new_mem = (uint8_t *)malloc(size);
        if (NULL != out_new_mem) {
            memcpy((void *)out_new_mem, (void *)mem, size);
        }
    }
    return out_new_mem;
}

bool is_arr_pat(uint8_t* arr, uint32_t size, uint8_t patt) {
    bool res = true;
    uint32_t i = 0;
    for(i = 0; i < size; i++) {
        if(patt != arr[i]) {
            res = false;
        }
    }
    return res;
}

bool array_rand(uint8_t* arr, uint32_t size){
    bool res = false;
    uint32_t i = 0;
#ifdef HAS_MCU
    srand( get_time_ms32());
#endif
#ifdef X86_64
    srand( time(0));
#endif
    if(arr && (0<size)) {
      for(i=0;i<size;i++) {
          arr[i]=(uint8_t) (rand() % 255);
      }
    }
    return res;
}
