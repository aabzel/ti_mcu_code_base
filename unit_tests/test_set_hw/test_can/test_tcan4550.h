#ifndef TEST_CAN_H
#define TEST_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_can_write(void);
bool test_can_read_word(void);
bool test_can_detect(void);
bool test_can_rate(void);
bool test_can_types(void);
bool test_can_send(void);
bool test_can_mram(void);
bool test_can_lock(void);


#define TEST_SUIT_TCAN4550                \
    {"can_types", test_can_types},        \
    {"can_detect", test_can_detect},      \
    {"can_lock", test_can_lock},          \
    {"can_rate", test_can_rate},          \
    {"can_read_word", test_can_read_word},\
    {"can_write", test_can_write},        \
    {"can_mram", test_can_mram},          \
    {"can_send", test_can_send},

#ifdef __cplusplus
}
#endif

#endif /* TEST_CAN_H */
