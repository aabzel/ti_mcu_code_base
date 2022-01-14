
#ifndef CC2642R1F_H
#define CC2642R1F_H

#include "macro_utils.h"

#define RAM_SIZE (80 * K_BYTES)
#define RAM_START 0x20000000
#define RAM_END (RAM_START + RAM_SIZE)

#define ROM_SIZE ( 352 * K_BYTES)
#define ROM_START 0x00000000
#define ROM_END (ROM_START + ROM_SIZE)

#define MCU_NAME "CC2642R1F"

#endif /* CC2652R1F_H */
