#include "fifo_char.h"

#include <stdint.h>
#include <string.h>

static bool is_fifo_valid(Fifo_array_t* const instance) {
    bool res = false;
    if(NULL != instance) {
        if((true == instance->initDone) && (0u < instance->fifoState.size)) {
            if(instance->fifoState.count <= instance->fifoState.size) {
                if(NULL != instance->array) {
                    res = true;
                }
            }
        }
    }
    return res;
}

/*Array is given outside because dynamic memory prohibited*/
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
    bool res = is_fifo_valid(instance);
    if(res) {
        (void)memset(instance->array, 0x00, instance->fifoState.size);
        instance->fifoState.count = 0u;
        instance->fifoState.start = 0u;
        instance->fifoState.end = 0u;
        res = true;
    }
    return res;
}

bool fifo_clean(Fifo_array_t* instance) {
    bool res = is_fifo_valid(instance);
    fifo_index_t i = 0;
    char out_char = 0x00;
    for(i = 0; i < instance->fifoState.size; i++) {
        res = fifo_pull(instance, &out_char);
        if(false == res) {
            res = true;
            break;
        }
    }
    return res;
}

bool fifo_push(Fifo_array_t* instance, char inChar) {
    bool res = is_fifo_valid(instance);
    if(res) {
        res = false;
        if((instance->fifoState.count < instance->fifoState.size)) {
            instance->array[instance->fifoState.end] = inChar;
            (void)fifo_index_add(&(instance->fifoState));
            res = true;
        }
    }
    return res;
}

bool has_fifo_valid_data(Fifo_array_t* const instance) {
    bool res = is_fifo_valid(instance);
    if(res) {
        res = false;
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

bool fifo_pull(Fifo_array_t* instance, char* const out_char) {
    bool res = is_fifo_valid(instance);
    if(res) {
        res = false;
        if(0u < instance->fifoState.count) {
            *out_char = instance->array[instance->fifoState.start];
            instance->array[instance->fifoState.start] = (char)0x00;
            (void)fifo_index_get(&(instance->fifoState));
            res = true;
        }
    }
    return res;
}

bool fifo_peek(Fifo_array_t* instance, char* const out_char) {
    bool res = is_fifo_valid(instance);
    if(res) {
        res = false;
        if(0u < instance->fifoState.count) {
            *out_char = instance->array[instance->fifoState.start];
            res = true;
        }
    }
    return res;
}

bool fifo_push_array(Fifo_array_t* instance, char* const inArr, fifo_index_t arr_len) {
    bool res = is_fifo_valid(instance);
#ifdef DEBUG_FIFO_CHAR
    printf("\n\r%s [%s] size %d\n\r", __FUNCTION__, inArr, arr_len);
#endif
    if(res && (0u < arr_len)) {
        uint16_t i;
        for(i = 0u; i < arr_len; i++) {
            if(true == res) {
                res = fifo_push(instance, inArr[i]);
            }
        }
#ifdef DEBUG_FIFO_CHAR
        printf("\n\r%s filo len [%d]", __FUNCTION__, instance->fifoState.size);
#endif
    }
    return res;
}

bool fifo_pull_array(Fifo_array_t* instance, char* const outArr, fifo_index_t* const outLen) {
    bool res = is_fifo_valid(instance);
    if((NULL != outArr) && (NULL != outLen) && (true == res)) {
        res = false;
        bool runLoop = true;
        (*outLen) = 0u;
        while(true == runLoop) {
            if(0u < instance->fifoState.count) {
                char out_char = (char)0;
                res = fifo_pull(instance, &out_char);
                if(true == res) {
                    outArr[(*outLen)] = out_char;
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

bool fifo_peek_array(Fifo_array_t* instance, char* outArr, fifo_index_t* const outLen) {
    bool res = is_fifo_valid(instance);
    if(res) {
        res = false;
        if((NULL != outArr) && (NULL != outLen)) {
            (*outLen) = 0u;
            if(true == fifo_index_valid(&(instance->fifoState))) {
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
    return res;
}

fifo_index_t fifo_get_count(Fifo_array_t* const instance) {
    fifo_index_t ret;
    ret = instance->fifoState.count;
    return ret;
}

fifo_index_t fifo_get_size(Fifo_array_t* const instance) {
    fifo_index_t ret;
    ret = instance->fifoState.size;
    return ret;
}

bool fifo_free(Fifo_array_t* instance, fifo_index_t size) { return fifo_index_free(&instance->fifoState, size); }
