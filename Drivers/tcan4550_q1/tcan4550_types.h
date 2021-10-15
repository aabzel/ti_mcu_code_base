
#ifndef TCAN4550_TYPES_H
#define TCAN4550_TYPES_H

#include <stdbool.h>
#include <stdint.h>

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

typedef struct xtCanRegRev_t {
    union {
        uint32_t word;
        struct {
            uint8_t minor_id;
            uint8_t major_id;
            uint8_t res;
            uint8_t spi2;
        };
    };
} __attribute__((packed)) tCanRegRev_t;

typedef struct xtCanRegStatus_t {
    union {
        uint32_t word;
        struct {
            uint8_t inter : 1;
            uint8_t spi_error_interrupt : 1;
            uint8_t internal_error_interrupt : 1;
            uint8_t internal_access_active : 1;
            uint8_t read_fifo_available : 1;
            uint8_t write_fifo_available : 1;
            uint16_t rsvd1 : 10;
            uint8_t read_underflow : 1;
            uint8_t read_overflow : 1;
            uint8_t write_underflow : 1;
            uint8_t write_overflow : 1;
            uint8_t invalid_command : 1;
            uint8_t spi_end_error : 1;
            uint8_t rsvd2 : 2;
            uint8_t write_fifo_overflow : 1;
            uint8_t read_fifo_empty : 1;
            uint8_t read_fifo_underflow : 1;
            uint8_t internal_error_log_write : 1;
            uint8_t internal_write_error : 1;
            uint8_t internal_read_error : 1;
            uint8_t rsvd3 : 2;
        };
    };
} __attribute__((packed)) tCanRegStatus_t;

// 0800 Modes of Operation and Pin Configurations
typedef struct xtCanRegModeOpPinCfg_t {
    union {
        uint32_t word;
        struct {
            uint8_t test_mode_config : 1; // 0
            uint8_t swe_dis : 1;          // 1
            uint8_t device_reset : 1;     // 2
            uint8_t wd_en : 1;            // 3
            uint8_t rsvd1 : 2;            // 4-5
            uint8_t mode_sel : 2;         // 6-7
            uint8_t nwkrq_config : 1;
            uint8_t inh_dis : 1;
            uint8_t gpio1_gpo_config : 2;
            uint8_t rsvd2 : 1;
            uint8_t fail_safe_en : 1;
            uint8_t gpio1_config : 2;
            uint8_t wd_action : 2;
            uint8_t wd_bit_set : 1;
            uint8_t nwkrq_voltage : 1;
            uint8_t rsvd3 : 1;
            uint8_t test_mode_en : 1;
            uint8_t gpo2_config : 2;
            uint8_t rsvd4 : 3;
            uint8_t clk_ref : 1;
            uint8_t wd_timer : 2;
            uint8_t wake_config : 2;
        };
    };
} __attribute__((packed)) tCanRegModeOpPinCfg_t;

typedef struct xtCanRegIntFl_t {
    union {
        uint32_t word;
        struct {
            uint8_t vtwd : 1;      // 0
            uint8_t m_can_int : 1; // 1
            uint8_t rsvd1 : 1;     // 2
            uint8_t spierr : 1;    // 3
            uint8_t rsvd2 : 1;     // 4
            uint8_t canerr : 1;    // 5
            uint8_t wkrq : 1;      // 6
            uint8_t globalerr : 1; // 7
            uint8_t candom : 1;    // 8
            uint8_t rsvd3 : 1;     // 9
            uint8_t canslnt : 1;   // 10
            uint8_t rsvd4 : 2;     // 11
            uint8_t wkerr : 1;     // 13
            uint8_t lwu : 1;       // 14
            uint8_t canint : 1;    // 15
            uint8_t eccerr : 1;    // 16
            uint8_t rsvd5 : 1;     // 17
            uint8_t wdto : 1;      // 18
            uint8_t tsd : 1;       // 19
            uint8_t pwron : 1;     // 20
            uint8_t uvio : 1;      // 21
            uint8_t uvsup : 1;     // 22
            uint8_t sms : 1;       // 23
            uint8_t rsvd6 : 7;     // 24-30
            uint8_t canbusnom : 1; // 31
        };
    };
} __attribute__((packed)) tCanRegIntFl_t;

typedef struct xTxBufCfg_t {
    union {
        uint32_t word;
        struct {
            uint16_t tx_buf_start_addr; // 0 Tx Buffers Start Address
            uint8_t ndtb : 6;           // 16-21 Number of Dedicated Transmit Buffers
            uint8_t rsvd1 : 2;          // 22-23 Reserved
            uint8_t tfqs : 6;           // 24-29 Transmit FIFO/Queue Size
            uint8_t tfqm : 1;           // 30 Tx FIFO/Queue Mode
            uint8_t rsvd2 : 1;          // 31  Reserved
        };
    };
} __attribute__((packed)) TxBufCfg_t;

typedef struct xTxBufElmSzCfg_t {
    union {
        uint32_t word;
        struct {
            uint8_t tx_buff_data_size : 3; // 0-2  Tx Buffer Data Field Size
            uint32_t rsvd : 29;            // 3-31 Reserved
        };
    };
} __attribute__((packed)) TxBufElmSzCfg_t;

/**
 *  CAN message header for transmitted messages
 */
typedef struct xTCanTxHeader_t {
    uint32_t id : 29;     // CAN ID to send
    uint8_t rtr : 1;      // Remote Transmission Request flag
    uint8_t xtd : 1;      // Extended Identifier flag
    uint8_t esi : 1;      // Error state indicator flag
    uint8_t dlc : 4;      // Data length code
    uint8_t brs : 1;      // Bit rate switch used flag
    uint8_t fdf : 1;      // CAN FD Format flag
    uint8_t reserved : 1; // Reserved
    uint8_t efc : 1;      // Event FIFO Control flag, to store tx events or not
    uint8_t mm : 8;       // Message Marker, used if @c EFC is set to 1
} tCanTxHeader_t;

// Tx Event FIFO
typedef struct xW0_t {
    union {
        uint32_t word;
        struct {
            uint32_t id : 29; // Identifier
            uint8_t rtr : 1;  // Remote Transmission Request
            uint8_t xtd : 1;  // Extended Identifier
            uint8_t esi : 1;  // Error State Indicator
        };
    };
} __attribute__((packed))W0_t;


typedef struct xTxBuffW1_t {
    union {
        uint32_t word;
        struct {
            uint16_t res1;   //0-15
            uint8_t dlc :4; //16-19 Data Length Code
            uint8_t brs :1; //20 Bit Rate Switch
            uint8_t fdf :1; //21 FD Format
            uint8_t res2 :1; //22
            uint8_t efc :1; //23 event fifo control
            uint8_t mm;     //24-31  message marker
        };
    };
} __attribute__((packed))TxBuffW1_t;

#endif /* TCAN4550_TYPES_H */
