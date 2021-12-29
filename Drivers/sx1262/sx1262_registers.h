#ifndef SX1262_REGISTERS_H
#define SX1262_REGISTERS_H

/* Register Table */
/* Regiser Name     Address*/
#define SYNC_WORD_0 0x06C0
#define SYNC_WORD_1 0x06C1
#define SYNC_WORD_2 0x06C2
#define SYNC_WORD_3 0x06C3
#define SYNC_WORD_4 0x06C4
#define SYNC_WORD_5 0x06C5
#define SYNC_WORD_6 0x06C6
#define SYNC_WORD_7 0x06C7

#define RAND_NUM_GEN_0 0x0819
#define RAND_NUM_GEN_1 0x081A
#define RAND_NUM_GEN_2 0x081B
#define RAND_NUM_GEN_3 0x081C

#define LORA_SYNC_WORD_MSB 0x0740
#define LORA_SYNC_WORD_LSB 0x0741
#define RX_GAIN            0x08AC

#define OCP_CONFIGURATION 0x08E7
#define XTB_TRIM 0x0912

#endif /* SX1262_REGISTERS_H  */
