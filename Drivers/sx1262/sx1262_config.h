#ifndef SX1262_CONFIG_H
#define SX1262_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define FREQ_MHZ 868000000
#define SX1262_SPI_NUM 0
#define SYNC_WORD 0x1122334455667788

#define RX_BASE_ADDRESS (0x00 + (FIFO_SIZE / 2)) // 128
#define TX_BASE_ADDRESS 0x00

#define BUSY_CNT_LIMIT 1000

#ifdef __cplusplus
}
#endif

#endif /* SX1262_CONFIG_H */
