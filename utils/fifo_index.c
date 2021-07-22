#include "fifo_index.h"

void fifo_index_init(fifo_index_info_t *r_init, fifo_index_t size_init) {
  r_init->size = size_init;
  r_init->count = 0U;
  r_init->start = 0U;
  r_init->end = 0U;
  r_init->errors = false;
}

bool fifo_index_valid(const fifo_index_info_t *r_valid) {
  bool valid = true;
  if (true == r_valid->errors) {
    valid = false;
  }
  if (r_valid->count > r_valid->size) {
    valid = false;
  }
  if (r_valid->start >= r_valid->size) {
    valid = false;
  }
  if (r_valid->end >= r_valid->size) {
    valid = false;
  }
  if (true == valid) {
    fifo_index_t idx_start = r_valid->start;
    fifo_index_t idx_end = r_valid->end;
    if (idx_end > idx_start) {
      if ((idx_end - idx_start) != r_valid->count) {
        valid = false;
      }
    } else {
      fifo_index_t v_count = (idx_end + r_valid->size) - idx_start;
      if ((v_count != (r_valid->count + r_valid->size))) {
        if (v_count != r_valid->count) {
          valid = false;
        }
      }
    }
  }
  return valid;
}

fifo_index_t fifo_index_add(fifo_index_info_t *r_add) {
  fifo_index_t ret_add = RING_INVALID_INDEX;
  if (r_add->count < r_add->size) {
    ret_add = r_add->end;
    r_add->end++;
    if (r_add->end >= r_add->size) {
      r_add->end = 0U;
    }
    r_add->count++;
  }
  return ret_add;
}

fifo_index_t fifo_index_get(fifo_index_info_t *r_get) {
  fifo_index_t ret_get = RING_INVALID_INDEX;
  if (0U != r_get->count) {
    ret_get = r_get->start;
    r_get->start++;
    if (r_get->start >= r_get->size) {
      r_get->start = 0U;
    }
    r_get->count--;
  }
  return ret_get;
}

void fifo_index_free(fifo_index_info_t *r_free, fifo_index_t size_free) {
  if (r_free->count >= size_free) {
    fifo_index_t s = size_free;
    fifo_index_t count_before_wrap = r_free->size - r_free->start;
    r_free->count -= s;
    if (s >= count_before_wrap) {
      s -= count_before_wrap;
      r_free->start = 0U;
    }
    if (0U != s) {
      r_free->start += s;
    }
  } else {
    r_free->errors = true;
  }
}

fifo_index_t fifo_index_continuus_used_size(const fifo_index_info_t *r_cus) {
  fifo_index_t ret_cus = r_cus->size - r_cus->start;
  if (ret_cus > r_cus->count) {
    ret_cus = r_cus->count;
  }
  return ret_cus;
}

fifo_index_t fifo_index_get_used(const fifo_index_info_t *r_used) {
  return r_used->count;
}

fifo_index_t fifo_index_get_size(const fifo_index_info_t *r_size) {
  return r_size->size;
}
