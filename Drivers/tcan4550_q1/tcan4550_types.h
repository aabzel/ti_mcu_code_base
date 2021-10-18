
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

//0x1018  CCCR "CC Control Register"
typedef struct xtCanRegCCctrl_t {
    union {
        uint32_t word;
        struct {
            uint8_t init      :1;//0  Initialization
            uint8_t cce       :1;//1  Configuration Change Enable
            uint8_t rom       :1;//2  Restricted Operation Mode
            uint8_t csa       :1;//3  Clock Stop Acknowledge
            uint8_t csr       :1;//4  Clock Stop Request
            uint8_t mon       :1;//5  Bus Monitoring Mode is Disabled
            uint8_t dar       :1;//6  Disable Automatic Retransmission
            uint8_t test      :1;//7  Test Mode Enable
            uint8_t fdoe      :1;//8  FD Operation Enable
            uint8_t brse      :1;//9  Bit Rate Switch Enable
            uint8_t rsvd1     :2;//10 Reserved
            uint8_t pxhd      :1;//12 Protocol Exception Handling Disable
            uint8_t efbi      :1;//13 Edge Filtering during Bus Integration
            uint8_t txp       :1;//14 Transmitter Pause
            uint8_t niso      :1;//15 Non ISO Operation
            uint16_t rsvd2     :1;//16-31 Reserved
        };
    };
} __attribute__((packed)) tCanRegCCctrl_t;


/*0x101C Nominal Bit Timing & Prescaler Register*/
typedef struct xtCanRegBitTime_t {
    union {
        uint32_t word;
        struct {
            uint8_t ntseg2 :7;//0-6   Nominal Time Segment After Sample Point
            uint8_t rsvd :1;  //7
            uint8_t ntseg1;   //8-15  Nominal Time Segment Before Sample Point
            uint16_t nbrp :9; //16-24 Nominal Bit Rate Prescaler
            uint8_t nsjw :7;  //25-31 Nominal (RE)Synchronization Jump Width
        };
    };
} __attribute__((packed)) tCanRegBitTime_t;


/*0x100C Data Bit Timing & Prescaler */
typedef struct xtCanRegDataBitTime_t {
    union {
        uint32_t word;
        struct {
            uint8_t dsjw :4;    //0 data (re)synchronization jump width
            uint8_t dtseg2 : 4 ; //4 data time segment After sample point
            uint8_t dtseg1 :5;  //8  data time segment before sample point
            uint8_t rsvd1 : 3;   //13
            uint8_t dbrp  : 5;    //16 data bit rate prescaler
            uint8_t rsvd :2;    //21
            uint8_t tdc  :1;     //23 transmitter delay compensation
            uint8_t rsvd2 ;     //24
        };
    };
} __attribute__((packed)) tCanRegDataBitTime_t;


//1080  Global Filter Configuration
typedef struct xtCanRegGloFiltCfg_t {
    union {
        uint32_t word;
        struct {
            uint8_t rrfe :1;   //0   Reject Remote Frames Extended
            uint8_t rrfs :1;   //1   Reject Remote Frames Standard
            uint8_t anfe :2;   //2-3 Accept Non-matching Frames Extended
            uint8_t anfs :2;   //4-5 Accept Non-matching Frames Standard
            uint32_t rsrvd :26;//6-31
        };
    };
} __attribute__((packed)) tCanRegGloFiltCfg_t;


//0x1084 Standard ID Filter Configuration
typedef struct xtCanRegStdIdFiltCfg_t {
    union {
        uint32_t word;
        struct {
            uint16_t flssa; //0  filter list standard start address
            uint8_t lss;    //16 list size standard
            uint8_t rsvd;   //24 reserved
        };
    };
} __attribute__((packed)) tCanRegStdIdFiltCfg_t;

//0x1088 Extended ID Filter Configuration
typedef struct xtCanRegExtIdFiltCfg_t {
    union {
        uint32_t word;
        struct {
            uint16_t flsea;    //0  filter list extended start address
            uint8_t lse :7;    //16 list size extended
            uint16_t rsvd :9;  //23 reserved
        };
    };
} __attribute__((packed)) tCanRegExtIdFiltCfg_t;

//0x10A0 Rx FIFO 0 Configuration
typedef struct xtCanRegRxFifo0Cfg_t {
    union {
        uint32_t word;
        struct {
            uint16_t f0sa ; //0-15  Rx FIFO 0 Start Address
            uint8_t f0s :7;  //16-22 Rx FIFO 0 Size
            uint8_t rsvd :1; //23
            uint8_t f0wm :7; //24-30 Rx FIFO 0 Watermark
            uint8_t f0om:1;   //31       FIFO 0 Operation Mode
        };
    };
} __attribute__((packed)) tCanRegRxFifo0Cfg_t;

//0x10B0 Rx FIFO 1 Configuration
typedef struct xtCanRegRxFifo1Cfg_t {
    union {
        uint32_t word;
        struct {
            uint16_t f1sa;  // 0   rx fifo 1 start address
            uint8_t f1s :7; // 16  rx fifo 1 size
            uint8_t rsvd :1;
            uint8_t f1wm :7; // 24  rx fifo 1 watermark
            uint8_t f10m :1; // 31  fifo 1 operation mode
        };
    };
} __attribute__((packed)) tCanRegRxFifo1Cfg_t;


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

//Interrupt Enable
typedef struct xtCanRegIntEn_t {
    union {
        uint32_t word;
        struct {
            uint8_t rf0ne :1; /*0 Rx FIFO 0 New Message                         */
            uint8_t rf0we :1; /*1 Rx FIFO 0 Watermark Reached                   */
            uint8_t rf0fe :1; /*2 Rx FIFO 0 Full                                */
            uint8_t rf0le :1; /*3 Rx FIFO 0 Message Lost                        */
            uint8_t rf1ne :1; /*4 Rx FIFO 1 New Message                         */
            uint8_t rf1we :1; /*5 Rx FIFO 1 Watermark Reached                   */
            uint8_t rf1fe :1; /*6 Rx FIFO 1 Full                                */
            uint8_t rf1le :1; /*7 Rx FIFO 1 Message Lost                        */
            uint8_t hpme :1;  /*8 High Priority Message                         */
            uint8_t tce :1;   /*9 Transmission Completed                        */
            uint8_t tcfe :1;  /*10 Transmission Cancellation Finished           */
            uint8_t tfee :1;  /*11 Tx FIFO Empty                                */
            uint8_t tefne :1; /*12 Tx Event FIFO New Entry                      */
            uint8_t tefw :1;  /*13  Tx Event FIFO Watermark Reached             */
            uint8_t teffe :1; /*14 Tx Event FIFO Full                           */
            uint8_t tefle :1; /*15 Tx Event FIFO Event Lost                     */
            uint8_t tswe :1;  /*16 Timestamp Wraparound                         */
            uint8_t mrafe :1; /*17  Message RAM Access Failure                  */
            uint8_t tooe :1;  /*18 Timeout Occurred                             */
            uint8_t drxe :1;  /*19 Message stored to Dedicated Rx Buffer        */
            uint8_t bece :1;  /*20 Bit Error Corrected                          */
            uint8_t beue :1;  /*21 Bit Error Uncorrected                        */
            uint8_t eloe :1;  /*22 Error Logging Overflow                       */
            uint8_t epe :1;   /*23 Error Passive                                */
            uint8_t ewe :1;   /*24  Warning Status                              */
            uint8_t boe :1;   /*25 Bus_Off Status                               */
            uint8_t wdie :1;  /*26 Watchdog                                     */
            uint8_t peae :1;  /*27 Protocol Error in Arbitration Phase Enable   */
            uint8_t pede :1;  /*28 Protocol Error in Data Phase Enable          */
            uint8_t arae :1;  /*29 Access to Reserved Address Enable            */
            uint8_t rsvd :2;  /*30-31                                           */
        };
    };
} __attribute__((packed)) tCanRegIntEn_t;


/*Interrupt Flags*/
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
