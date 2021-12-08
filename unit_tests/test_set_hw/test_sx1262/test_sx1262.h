#ifndef TEST_SX1262_H
#define TEST_SX1262_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_sx1262(void);
bool test_sx1262_fifo(void);
bool test_sx1262_sync_word(void);
bool test_sx1262_packet_type(void); /*fails*/
bool test_sx1262_rx_addr(void);     /*fails*/
bool test_sx1262_rand(void);
bool test_sx1262_types(void);

#define TEST_SUIT_SX1262                                                                                               \
    {"sx1262", test_sx1262}, \
    {"sx1262_rand", test_sx1262_rand},\
    {"sx1262_sync_word", test_sx1262_sync_word},           \
    {"sx1262_rx_addr", test_sx1262_rx_addr}, /*fails*/                                                             \
    {"sx1262_fifo", test_sx1262_fifo}, {"sx1262_types", test_sx1262_types},
#if 0
    {"sx1262_packet_type", test_sx1262_packet_type},/*fails*/
#endif
#ifdef __cplusplus
}
#endif

#endif /* TEST_SX1262_H */
