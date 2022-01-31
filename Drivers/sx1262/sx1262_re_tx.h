#ifndef SX1262_RETX_H
#define SX1262_RETX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define RETX_TRY_CNT_DFLT 2

bool is_sx1262_retx_idle(void);
bool sx1262_retx_init(void);
bool sx1262_start_retx(uint8_t* array, uint16_t len, uint8_t try_cnt);
bool sx1216_retx_proc(void);

#ifdef __cplusplus
}
#endif

#endif /* SX1262_RETX_H */
