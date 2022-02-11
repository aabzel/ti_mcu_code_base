#include "array.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifdef X86_64
#include <time.h>
#include <stdio.h>
#endif
#ifdef HAS_MCU
#include "clocks.h"
#endif
#include "data_utils.h"

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

bool array_max_cont(uint8_t* arr, size_t size, uint8_t patt, uint32_t *max_cont_patt){
    bool res = false;
    if(arr && (0<size) && max_cont_patt){
        res = true;
        uint32_t cur_cont_pat=0;
        uint32_t max_cont_pat=0;
        uint8_t prev_elem = 0xFF;
        uint32_t i = 0;
        for(i = 0; i < size; i++){
            if(patt==arr[i]){
                cur_cont_pat++;
                if(prev_elem==arr[i]){

                }else{
                    cur_cont_pat=1;
                }
            }else{
                cur_cont_pat=0;
            }
            prev_elem  = arr[i];
            max_cont_pat = max32u(max_cont_pat, cur_cont_pat);
        }
        *max_cont_patt=max_cont_pat;
    }
    return res;
}

bool array_incr(uint8_t* arr, size_t size){
    bool res = false;
    uint32_t i = 0;
    if(arr && (0<size)) {
      for(i=0;i<size;i++) {
          arr[i]=i;
      }
      res = true;
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
      res = true;
    }
    return res;
}

#ifdef HAS_LOADER
bool is_array_equal(uint8_t* array1,
                    uint8_t* array2,
                    size_t size,
                    uint32_t* match,
                    uint32_t* diff){
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
	bool res = false;
	if(array1 && array2 && match && diff) {
     	uint32_t i=0;
    	for (i=0; i<size; i++) {
    		if(array1[i]==array2[i]){
    			*match++;
    		} else {
    			*diff++;
    		}		
    	}
    	if(*match==size){
    		res = true;
    	}
	}
	return res;
}
#endif
