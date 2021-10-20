
#ifndef TCAN4550_CONST_H
#define TCAN4550_CONST_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_NUM_STD_ID_FIL 128
#define MAX_NUM_EXT_ID_FIL 64
#define MAX_NUM_RX_FIFO_0_EL 64

typedef enum eClkRef_t {
    CLK_REF_20MHZ = 0,
    CLK_REF_40MHZ = 1,
    CLK_REF_UNDEF = 1,
} ClkRef_t;

typedef enum eMode_t {
    MODE_SLEEP = 0,
    MODE_STANDBY = 1,
    MODE_NORMAL = 2,
    MODE_RESERVED = 3,
    MODE_UNDEF = 4,
} DevMode_t;


typedef enum eFiFo0OpMode_t {
    FIFO_OP_MODE_BLOCKING = 0,
    FIFO_OP_MODE_OVERWRITE = 1,
} FiFo0OpMode_t;


typedef enum eStandardFilterType_t {
    //! Disabled filter. This filter will match nothing
    TCAN4x5x_SID_SFT_DISABLED           = 0x3,
    //! Classic filter with SFID1 as the ID to match, and SFID2 as the bit mask that applies to SFID1
    TCAN4x5x_SID_SFT_CLASSIC            = 0x2,
    //! Dual ID filter, where both SFID1 and SFID2 hold IDs that can match (must match exactly)
    TCAN4x5x_SID_SFT_DUALID             = 0x1,
    //! Range Filter. SFID1 holds the start address, and SFID2 holds the end address. Any address in between will match
    TCAN4x5x_SID_SFT_RANGE              = 0x0
} StandardFilterType_t;


typedef enum eStandardFilterElementConfiguration_t{
    //! Disabled filter. This filter will do nothing if it matches a packet
    TCAN4x5x_SID_SFEC_DISABLED          = 0x0,

    //! Store in RX FIFO 0 if the filter matches the incoming message
    TCAN4x5x_SID_SFEC_STORERX0          = 0x1,

    //! Store in RX FIFO 1 if the filter matches the incoming message
    TCAN4x5x_SID_SFEC_STORERX1          = 0x2,

    //! Reject the packet (do not store, do not notify MCU) if the filter matches the incoming message
    TCAN4x5x_SID_SFEC_REJECTMATCH       = 0x3,

    //! Store in default location but set a high priority message interrupt if the filter matches the incoming message
    TCAN4x5x_SID_SFEC_PRIORITY          = 0x4,

    //! Store in RX FIFO 0 and set a high priority message interrupt if the filter matches the incoming message
    TCAN4x5x_SID_SFEC_PRIORITYSTORERX0  = 0x5,

    //! Store in RX FIFO 1 and set a high priority message interrupt if the filter matches the incoming message
    TCAN4x5x_SID_SFEC_PRIORITYSTORERX1  = 0x6,

    //! Store in RX Buffer for debug if the filter matches the incoming message. SFT is ignored if this is selected.
    TCAN4x5x_SID_SFEC_STORERXBUFORDEBUG = 0x7
} TCAN4x5x_SID_SFEC_Values;


/**
 * @brief Data payload defines for the different MRAM sections, used by the @c TCAN4x5x_MRAM_Config struct
 */
typedef enum {
    MRAM_8_Byte_Data = 0,   // 8  bytes of data payload
    MRAM_12_Byte_Data = 0x1,// 12 bytes of data payload
    MRAM_16_Byte_Data = 0x2,// 16 bytes of data payload
    MRAM_20_Byte_Data = 0x3,// 20 bytes of data payload
    MRAM_24_Byte_Data = 0x4,// 24 bytes of data payload
    MRAM_32_Byte_Data = 0x5,// 32 bytes of data payload
    MRAM_48_Byte_Data = 0x6,// 48 bytes of data payload
    MRAM_64_Byte_Data = 0x7 // 64 bytes of data payload
} TCAN4x5x_MRAM_Element_Data_Size;


#endif /* TCAN4550_CONST_H */
