#ifndef FIFO_CHAR_BUFFER_H
#define FIFO_CHAR_BUFFER_H

//#include <stddef.h>

//#include "sys.h"
#include "fifo_index.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  fifo_index_info_t r;
  char *data;
} fifo_char_t;

//#define FIFO_CHAR_INIT(size, data)  { FIFO_INDEX_INIT (size), data }

bool fifo_char_add(fifo_char_t *f, char c);
const char *fifo_char_get_contiguous_block(const fifo_char_t *f,
                                           fifo_index_t *size);
void fifo_char_free(fifo_char_t *f, fifo_index_t size);
fifo_index_t fifo_char_get_used(const fifo_char_t *f);
fifo_index_t fifo_char_get_size(const fifo_char_t *f);

#ifdef __cplusplus
}
#endif

#endif /* FIFO_CHAR_BUFFER_H */
