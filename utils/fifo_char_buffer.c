#include "fifo_char_buffer.h"

#include <stddef.h>

#include "sys.h"

bool fifo_char_add(fifo_char_t *fifo, char ch) {
  bool ret = true;
  fifo_index_t index = 0;
  index = fifo_index_add(&fifo->r);
  if (index == RING_INVALID_INDEX) {
    ret = false;
  } else {
    fifo->data[index] = ch;
  }

  return ret;
}

const char *fifo_char_get_contiguous_block(const fifo_char_t *f,
                                           fifo_index_t *size) {
  const char *retval = (char *)NULL;

  *size = fifo_index_continuus_used_size(&f->r);
  if (*size) {
    retval = f->data + f->r.start;
  } else {
    retval = (char *)NULL;
  }

  return retval;
}

void fifo_char_free(fifo_char_t *f, fifo_index_t size) {
  fifo_index_free(&f->r, size);
}

fifo_index_t fifo_char_get_used(const fifo_char_t *f) {
  fifo_index_t retval;
  retval = f->r.count;
  return retval;
}

fifo_index_t fifo_char_get_size(const fifo_char_t *f) {
  fifo_index_t retval;
  retval = f->r.size;
  return retval;
}
