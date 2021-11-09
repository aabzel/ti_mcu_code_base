#ifndef WRITER_GENERIC_H
#define WRITER_GENERIC_H

#include <stdbool.h>
#include <stdint.h>

#include "fifo_index.h"
#include "oprintf.h"
#include "fifo_char.h"

struct generic_writer_s;

typedef struct generic_writer_s {
  ostream_t s;
  uint16_t in_transmit; // tx byte cnt
  int64_t lost_char_count;
  int64_t total_char_count;
  int64_t error_count;
  Fifo_array_t fifo;
  bool (*f_transmit)(struct generic_writer_s *writer);
  volatile void *instance;
} generic_writer_t;


void writer_tx_callback(generic_writer_t *s);
void writer_error_callback(generic_writer_t *s);
void writer_putc(void *_s, char ch);
void writer_puts(void *_s, const char *s, int32_t len);
bool writer_clean(const generic_writer_t *s);
bool writer_half_clean(const generic_writer_t *s);

#endif /* WRITER_GENERIC_H */
