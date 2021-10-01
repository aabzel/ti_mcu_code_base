#ifndef TEST_FIFO_CHAR_H
#define TEST_FIFO_CHAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_fifo_char(void);
bool test_fifo_array1_char(void);
bool test_fifo_array_char(void);

#define TEST_SUIT_FIFO_CHAR                                                                                            \
    {"fifo_ch", test_fifo_char}, {"fifo_ch_array1", test_fifo_array1_char}, {"fifo_ch_array", test_fifo_array_char},

#ifdef __cplusplus
}
#endif

#endif /* TEST_FIFO_CHAR_H */
