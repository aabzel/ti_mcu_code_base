#ifndef TEST_FIFO_ARRAY_H
#define TEST_FIFO_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_fifo_array_types(void);
bool test_fifo_array(void);
bool test_fifo_array_packing(void);

#define TEST_SUIT_FIFO_ARRAY                                                                                           \
    {"fifo_array_types", test_fifo_array_types}, {"fifo_array_pack", test_fifo_array_packing},                         \
        {"fifo_array", test_fifo_array},

#ifdef __cplusplus
}
#endif

#endif /* TEST_FIFO_ARRAY_H */
