#ifndef FIFO_ARRAY_H
#define FIFO_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "fifo_index.h"

#include "array.h"

#define FIFO_ARR_NODE_CNT 10U
typedef struct xFifoArray_t {
    fifo_index_info_t fifoState;
    Array_t* pDataArray;
    bool init_done;
} FifoArray_t;

bool fifo_arr_push(FifoArray_t* const instance, Array_t node);
bool fifo_arr_pull(FifoArray_t* const instance, Array_t* const node);
bool fifo_arr_peek(FifoArray_t* const instance, Array_t* const outChar);
bool fifo_arr_clean(FifoArray_t* instance);
fifo_index_t fifo_arr_get_count(FifoArray_t* const instance);
fifo_index_t fifo_arr_get_size(FifoArray_t* const instance);
bool fifo_arr_reset(FifoArray_t* const instance);
bool fifo_arr_init(FifoArray_t* const instance, Array_t* const heap, fifo_index_t capacity);

#ifdef __cplusplus
}
#endif

#endif /* FIFO_ARRAY_H */
