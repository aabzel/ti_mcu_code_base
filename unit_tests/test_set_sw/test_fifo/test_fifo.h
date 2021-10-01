#ifndef TEST_FIFO_H
#define TEST_FIFO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

bool test_fifo(void);
bool test_fifo_overrun(void);

#define TEST_SUIT_FIFO_BUFF     \
                     {"fifo", test_fifo_overrun}, \
                     {"fifo_overrun", test_fifo_overrun},

#ifdef __cplusplus
}
#endif

#endif /* TEST_FIFO_H */
