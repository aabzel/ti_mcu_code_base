#include "fifo_char_buffer.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "fifo_index.h"
#include "sys.h"

bool fifo_char_add(fifo_char_t* fifo, char ch) {
    bool res = false;
    if(fifo) {
        fifo_index_t index = 0;
        index = fifo_index_add(&fifo->indexer);
        if(RING_INVALID_INDEX == index) {
            res = false;
        } else {
            fifo->data[index] = ch;
            res = true;
        }
    }

    return res;
}

char* fifo_char_get_contiguous_block(const fifo_char_t* fifo, fifo_index_t* size) {
    char* retval = (char*)NULL;
    *size = fifo_index_continuus_used_size(&fifo->indexer);
    if(*size) {
        retval = (char*)(fifo->data + fifo->indexer.start);
    } else {
        retval = (char*)NULL;
    }

    return retval;
}

void fifo_char_free(fifo_char_t* fifo, fifo_index_t size) { fifo_index_free(&fifo->indexer, size); }

fifo_index_t fifo_char_get_used(const fifo_char_t* fifo) {
    fifo_index_t retval;
    retval = fifo->indexer.count;
    return retval;
}

fifo_index_t fifo_char_get_size(const fifo_char_t* fifo) {
    fifo_index_t retval;
    retval = fifo->indexer.size;
    return retval;
}

bool fifo_char_init(fifo_char_t* fifo, char* fifo_mem, fifo_index_t size) {
    bool res = false;
    if((NULL != fifo_mem) && (0 < size)) {
        res = true;
        fifo->data = fifo_mem;
        fifo_index_init(&fifo->indexer, size);
    }
    return res;
}
