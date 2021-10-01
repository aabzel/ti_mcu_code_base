#include "fifo_char.h"

#include <stdint.h>
#include <string.h>

static bool is_char_valid(char inChar);

/*Array is given outside because dynamic memory prohibeted*/
bool fifo_init(Fifo_array_t* const instance, char* const inArray, uint16_t capacity) {
    bool res = false;
    if((NULL != instance) && (0u < capacity)) {
        (void)memset(inArray, 0x00, capacity);
        fifo_index_init(&(instance->fifoState), capacity);
        instance->array = inArray;
        instance->initDone = true;
        res = true;
    } else {
        instance->initDone = false;
    }
    return res;
}

bool fifo_reset(Fifo_array_t* const instance) {
    bool res = false;
    if((NULL != instance) && (0u < instance->fifoState.size) && (true == instance->initDone)) {
        (void)memset(instance->array, 0x00, instance->fifoState.size);
        instance->fifoState.count = 0u;
        instance->fifoState.start = 0u;
        instance->fifoState.end = 0u;
        res = true;
    }
    return res;
}

bool fifo_push(Fifo_array_t* instance, char inChar) {
    bool res = false;
    if(NULL != instance) {

        if((0u < instance->fifoState.size) && (true == instance->initDone)) {
            if((instance->fifoState.count < instance->fifoState.size)) {
                if(true == is_char_valid(inChar)) {
                    instance->array[instance->fifoState.end] = inChar;
                    (void)fifo_index_add(&(instance->fifoState));
                }
                res = true;
            }
        }
    }
    return res;
}

bool has_fifo_valid_data(Fifo_array_t* const instance) {
    bool res = false;
    if((NULL != instance) && (true == instance->initDone)) {
        if(0u < instance->fifoState.size) {
            if(0u < instance->fifoState.count) {
                uint32_t i = 0u;
                char ch = (char)0;
                for(i = 0u; i < instance->fifoState.size; i++) {
                    ch = instance->array[i];
                    if((char)0 != ch) {
                        res = true;
                        break;
                    }
                }
            }
        }
    }
    return res;
}

bool fifo_pull(Fifo_array_t* instance, char* const outChar) {
    bool res = false;
    if((NULL != instance) && (true == instance->initDone)) {
        if(0u < instance->fifoState.count) {
            if(instance->fifoState.count <= instance->fifoState.size) {
                *outChar = instance->array[instance->fifoState.start];
                instance->array[instance->fifoState.start] = (char)0x00;
                (void)fifo_index_get(&(instance->fifoState));
                res = true;
            }
        }
    }
    return res;
}

bool fifo_peek(Fifo_array_t* instance, char* const outChar) {
    bool res = false;
    if((NULL != instance) && (true == instance->initDone)) {
        if(0u < instance->fifoState.size) {
            if(0u < instance->fifoState.count) {
                *outChar = instance->array[instance->fifoState.start];
                res = true;
            }
        }
    }
    return res;
}

bool fifo_push_array(Fifo_array_t* instance, char* const inArr, uint16_t arrLen) {
    bool res = true;
#if DEBUG_FIFO_CHAR
    rx_printf("\n\r%s [%s] size %d\n\r", __FUNCTION__, inArr, arrLen);
#endif
    if((NULL != instance) && (0u < arrLen) && (true == instance->initDone)) {
        uint16_t i;
        for(i = 0u; i < arrLen; i++) {
            if(res == true) {
                res = fifo_push(instance, inArr[i]);
            }
        }
#if DEBUG_FIFO_CHAR
        rx_printf("\n\r%s filo len [%d]", __FUNCTION__, instance->fifoState.size);
#endif
    }
    return res;
}

bool fifo_pull_array(Fifo_array_t* instance, char* const outArr, uint16_t* const outLen) {
    bool res = true;
    if((NULL != outArr) && (NULL != outLen) && (true == instance->initDone)) {
        bool runLoop = true;
        (*outLen) = 0u;
        while(true == runLoop) {
            if(0u < instance->fifoState.count) {
                char outChar = (char)0;
                res = fifo_pull(instance, &outChar);
                if(true == res) {
                    outArr[(*outLen)] = outChar;
                    (*outLen)++;
                } else {
                    runLoop = false;
                }
            } else {
                outArr[(*outLen)] = '\0';
                runLoop = false;
                res = true;
            }
        }
    } else {
        res = false;
    }
    return res;
}

bool fifo_peek_array(Fifo_array_t* instance, char* outArr, uint16_t* const outLen) {
    bool res = false;
    if(NULL != instance) {
        if((NULL != outArr) && (NULL != outLen)) {
            (*outLen) = 0u;
            if(true == fifo_index_valid(&(instance->fifoState))) {
                if(true == instance->initDone) {
                    if(0u < instance->fifoState.count) {
                        uint16_t i = 0u;
                        uint16_t run = instance->fifoState.start;
                        for(i = 0; i < instance->fifoState.count; i++) {
                            outArr[i] = instance->array[run];
                            if(run < instance->fifoState.size) {
                                run++;
                            } else {
                                run = 0;
                            }
                        }
                        (*outLen) = instance->fifoState.count;
                        outArr[i] = '\0';
                        res = true;
                    }
                }
            }
        }
    }
    return res;
}

static bool is_char_valid(char inChar) {
    bool res = true;
#if 0
  if ((ESCAPE_CHAR == inChar) || (ESCAPE_SQ_BR_OP_CHAR == inChar) ||
      ((char)0 == inChar)) { /*Escape [*/
    res = false;
  }
#endif
    return res;
}
