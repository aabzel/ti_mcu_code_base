#ifndef TEST_CAN_H
#define TEST_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_can_write(void) ;
bool test_can_read_word(void) ;
bool test_can_detect(void) ;

#define TEST_SUIT_TCAN4550              \
    {"can_write", test_can_write}, \
    {"can_detect", test_can_detect}, \
    {"can_read_word", test_can_read_word},

#ifdef __cplusplus
}
#endif

#endif /* TEST_CAN_H */
