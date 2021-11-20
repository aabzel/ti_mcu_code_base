#ifndef FIFO_INDEX_H
#define FIFO_INDEX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RING_INVALID_INDEX 0xFFFFU

typedef uint16_t fifo_index_t;

typedef struct {
    fifo_index_t size;
    volatile fifo_index_t start;
    volatile fifo_index_t end;
    volatile fifo_index_t count;
    volatile bool errors;
} fifo_index_info_t;

#define FIFO_INDEX_INIT(size)                                                                                          \
    { (size), 0, 0, 0, false }

bool fifo_index_init(fifo_index_info_t* r_init, fifo_index_t size_init);
bool fifo_index_valid(const fifo_index_info_t* r_valid);

fifo_index_t fifo_index_add(fifo_index_info_t* r_add);
fifo_index_t fifo_index_get(fifo_index_info_t* r_get);
fifo_index_t fifo_index_continuus_used_size(const fifo_index_info_t* r_cus);
bool fifo_index_free(fifo_index_info_t* r_free, fifo_index_t size_free);
fifo_index_t fifo_index_get_used(const fifo_index_info_t* r_used);
fifo_index_t fifo_index_get_size(const fifo_index_info_t* r_size);
fifo_index_t fifo_index_get_spare(const fifo_index_info_t* r_spare);
#ifdef __cplusplus
}
#endif

#endif /* FIFO_INDEX_H */
