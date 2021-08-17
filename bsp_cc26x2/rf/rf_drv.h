#ifndef RF_DRV_H
#define RF_DRV_H

#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/rf/RF.h>

#define PAYLOAD_LENGTH 30
/* The Data Entries data field will contain:                                                \
 * 1 Header byte (RF_cmdPropRx.rxConf.bIncludeHdr = 0x1)                                    \
 * Max 30 payload bytes                                                                     \
 * 1 status byte (RF_cmdPropRx.rxConf.bAppendStatus = 0x1) */
#define NUM_APPENDED_BYTES 2
/* NOTE: Only two data entries supported at the moment */
#define NUM_DATA_ENTRIES 3

extern RF_Object rfObject;
extern RF_Handle rfHandle;
extern RF_Params rfParams;

extern uint32_t rf_rx_cnt;
extern uint32_t rf_rx_crc_err_cnt;
extern uint8_t rf_rx_len;
extern bool rf_proc_rx;

bool rf_rx_init(void);
bool rf_diag(void);
bool rf_tune_reception(void);
bool rf_tune_blocking_reception(void);
bool rf_init(void);
bool rf_write(uint8_t* array, uint16_t arr_len);
bool rf_read(uint8_t* out_array, uint16_t size_arr, uint16_t* out_arr_len);
bool rf_process(void);

#endif /* RF_DRV_H  */
