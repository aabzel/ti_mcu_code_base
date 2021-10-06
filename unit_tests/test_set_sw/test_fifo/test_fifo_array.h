#ifndef TEST_FIFO_ARRAY_H
#define TEST_FIFO_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_fifo_array_overrun(void);
bool test_fifo_array(void);

#define TEST_SUIT_FIFO_ARRAY                                                                                            \
    {"fifo_arr_overrun", test_fifo_array_overrun}, {"fifo_array", test_fifo_array},                              \


#ifdef __cplusplus
}
#endif

#endif /* TEST_FIFO_ARRAY_H */