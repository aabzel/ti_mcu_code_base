#ifndef FIFO_CHAR_BUFFER_H
#define FIFO_CHAR_BUFFER_H

#include "fifo_index.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    fifo_index_info_t indexer;
    volatile char* data;
} fifo_char_t;

bool fifo_init(fifo_char_t* fifo, char* fifo_mem, fifo_index_t size);
bool fifo_char_add(fifo_char_t* fifo, char c);
char* fifo_char_get_contiguous_block(const fifo_char_t* fifo, fifo_index_t* size);
void fifo_char_free(fifo_char_t* fifo, fifo_index_t size);
fifo_index_t fifo_char_get_used(const fifo_char_t* fifo);
fifo_index_t fifo_char_get_size(const fifo_char_t* fifo);

#ifdef __cplusplus
}
#endif

#endif /* FIFO_CHAR_BUFFER_H */
