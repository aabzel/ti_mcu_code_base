#include "ostream.h"

#include <stddef.h>
#include <string.h>

void oputs(ostream_t *ostream, const char *str) {
  size_t len = strlen(str);
  ostream->f_putstr(ostream, str, len);
}

void oputs_len(ostream_t *ostream, const char *str, int32_t len) {
  ostream->f_putstr(ostream, str, len);
}

void oputc(ostream_t *ostream, char c) { ostream->f_putch(ostream, c); }
