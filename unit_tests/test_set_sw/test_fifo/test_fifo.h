#ifndef TEST_FIFO_H
#define TEST_FIFO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

bool test_fifo(void);

#define TEST_SUIT_FIFO {"fifo", test_fifo},

#ifdef __cplusplus
}
#endif

#endif /* TEST_FIFO_H */
