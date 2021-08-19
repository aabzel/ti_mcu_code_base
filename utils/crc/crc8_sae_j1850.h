#ifndef CRC8_SAE_J1850_H_
#define CRC8_SAE_J1850_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t crc8_sae_j1850_calc(const void* buf, uint32_t len);
uint8_t crc8_sae_j1850_calc_continue(uint8_t crc_in, const void* buf, uint32_t len);
bool crc8_sae_j1850_is_self_test_ok(void);

#ifdef __cplusplus
}
#endif

#endif /* CRC8_SAE_J1850_H_ */
