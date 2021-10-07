#include "fifo_array.h"

#include <string.h>

static bool is_fifo_arr_valid(FifoArray_t* const instance) {
    bool res = false;
    if((NULL != instance) && (true == instance->init_done) && (0u < instance->fifoState.size) &&
       (instance->fifoState.count <= instance->fifoState.size)) {
        res = true;
    }
    return res;
}

bool fifo_arr_init(FifoArray_t* const instance, Array_t* const heap, fifo_index_t capacity) {
    bool res = false;
    if((NULL != instance) && (0 < capacity)) {
        (void)memset(heap, 0x00, capacity * (sizeof(Array_t)));
        fifo_index_init(&(instance->fifoState), capacity);
        instance->pDataArray = heap;
        instance->init_done = true;
        res = true;
    } else {
        instance->init_done = false;
    }
    return res;
}

fifo_index_t fifo_arr_get_count(FifoArray_t* const instance) {
    fifo_index_t ret;
    ret = instance->fifoState.count;
    return ret;
}

fifo_index_t fifo_arr_get_size(FifoArray_t* const instance) {
    fifo_index_t ret;
    ret = instance->fifoState.size;
    return ret;
}

bool fifo_arr_push(FifoArray_t* const instance, Array_t node) {
    bool res = false;
    res = is_fifo_arr_valid(instance);

    if(true == res) {
        if(instance->fifoState.count < instance->fifoState.size) {
            res = true;
        } else {
            res = false;
        }
    }
    if(true == res) {
        instance->pDataArray[instance->fifoState.end] = node;
        (void)fifo_index_add(&(instance->fifoState));
        res = true;
    }
    return res;
}

bool fifo_arr_pull(FifoArray_t* const instance, Array_t* const node) {
    bool res = false;
    res = is_fifo_arr_valid(instance);

    if(true == res) {
        if(0u < instance->fifoState.count) {
            res = true;
        } else {
            res = false;
        }
    }
    if(res) {
        *node = instance->pDataArray[instance->fifoState.start];
        memset(&instance->pDataArray[instance->fifoState.start], 0x00, sizeof(Array_t));
        (void)fifo_index_get(&(instance->fifoState));
    }
    return res;
}

bool fifo_arr_peek(FifoArray_t* const instance, Array_t* const node) {
    bool res = false;
    res = is_fifo_arr_valid(instance);
    if(res) {
        if(0u < instance->fifoState.count) {
            *node = instance->pDataArray[instance->fifoState.start];
        } else {
            res = false;
        }
    }
    return res;
}
