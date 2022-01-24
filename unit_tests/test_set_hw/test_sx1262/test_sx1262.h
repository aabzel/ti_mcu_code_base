#ifndef TEST_SX1262_H
#define TEST_SX1262_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_sx1262_status(void);
bool test_sx1262_connected(void);
bool test_sx1262_fifo(void);
bool test_sx1262_sync_word(void);
bool test_sx1262_crc_poly(void);
bool test_sx1262_crc_init(void);
bool test_sx1262_lora_sync_word(void);
bool test_sx1262_packet_type(void); /*fails*/
bool test_sx1262_rx_addr(void);     /*fails*/
bool test_sx1262_rand(void);
bool test_sx1262_rx_gain(void);
bool test_sx1262_types(void);
bool test_sx1262_packet_type(void);

#define TEST_SUIT_SX1262                                                                                               \
    {"sx1262_connect", test_sx1262_connected},   \
    {"sx1262_lora_sync_word", test_sx1262_lora_sync_word},           \
    {"sx1262_crc_poly", test_sx1262_crc_poly},           \
    {"sx1262_crc_init", test_sx1262_crc_init},           \
    {"sx1262_rx_gain", test_sx1262_rx_gain},           \
    {"sx1262_rand", test_sx1262_rand},   \
    {"sx1262_status", test_sx1262_status},\
    {"sx1262_sync_word", test_sx1262_sync_word},           \
    {"sx1262_rx_addr", test_sx1262_rx_addr}, /*fails*/                                                             \
    {"sx1262_fifo", test_sx1262_fifo}, \
    {"sx1262_types", test_sx1262_types}, \
    {"sx1262_packet_type", test_sx1262_packet_type},

#ifdef __cplusplus
}
#endif

#endif /* TEST_SX1262_H */
