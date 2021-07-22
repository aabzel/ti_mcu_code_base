#include "ostream.h"

void oputs(ostream_t *ostream, const char *str) {
  ostream->f_putstr(ostream, str, -1);
}

void oputs_len(ostream_t *ostream, const char *str, int32_t len) {
  ostream->f_putstr(ostream, str, len);
}

void oputc(ostream_t *ostream, char c) { ostream->f_putch(ostream, c); }
