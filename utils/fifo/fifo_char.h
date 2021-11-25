#ifndef FIFO_CHAR_H
#define FIFO_CHAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fifo_index.h"
#include <stdint.h>

#define ESCAPE_CHAR ((char)0x1B)
#define ESCAPE_SQ_BR_OP_CHAR ((char)0x5B)

typedef struct xFifo_array_t {
    fifo_index_info_t fifoState;
    char* array;
    bool initDone;
    uint32_t err_cnt;
} Fifo_array_t;

fifo_index_t fifo_get_count(Fifo_array_t* const instance);
fifo_index_t fifo_get_size(Fifo_array_t* const instance);
fifo_index_t fifo_get_spare(Fifo_array_t* const instance);

bool fifo_clean(Fifo_array_t* instance);
bool fifo_free(Fifo_array_t* fifo, fifo_index_t size);
bool fifo_reset(Fifo_array_t* const instance);
bool fifo_init(Fifo_array_t* const instance, char* const inArray, fifo_index_t capacity);
bool fifo_push(Fifo_array_t* const instance, char inChar);
bool fifo_pull(Fifo_array_t* const instance, char* const outChar);
bool fifo_peek(Fifo_array_t* const instance, char* const outChar);
bool has_fifo_valid_data(Fifo_array_t* const instance);

bool fifo_push_array(Fifo_array_t* const instance, char* const inArr, fifo_index_t arrLen);
bool fifo_pull_array(Fifo_array_t* instance, char* const outArr, size_t arr_size, fifo_index_t* const outLen);
bool fifo_peek_array(Fifo_array_t* const instance, char* const outArr, fifo_index_t* const outLen);

#ifdef __cplusplus
}
#endif

#endif /* FIFO_CHAR_H */
