#ifndef SX1262_CONSTANTS_H
#define SX1262_CONSTANTS_H

#include <stdint.h>


#define DEV_SEL_SX1262 0
#define DEV_SEL_SX1261 1

// Table 13-41: RampTime Definition
// RampTime Value RampTime (μs)
#define SET_RAMP_10U 0x00   /*10  */
#define SET_RAMP_20U 0x01   /*20  */
#define SET_RAMP_40U 0x02   /*40  */
#define SET_RAMP_80U 0x03   /*80  */
#define SET_RAMP_200U 0x04  /*200 */
#define SET_RAMP_800U 0x05  /*800 */
#define SET_RAMP_1700U 0x06 /*1700*/
#define SET_RAMP_3400U 0x07 /*3400*/

/* OpError Bits */
#define OP_ERR_BIT_RC64K_CALIB_ERR 0 /*RC64k calibration failed */
#define OP_ERR_BIT_RC13M_CALIB_ERR 1 /*RC13M calibration failed */
#define OP_ERR_BIT_PLL_CALIB_ERR 2   /*PLL calibration failed   */
#define OP_ERR_BIT_ADC_CALIB_ERR 3   /*ADC calibration failed   */
#define OP_ERR_BIT_IMG_CALIB_ERR 4   /*IMG calibration failed   */
#define OP_ERR_BIT_XOSC_START_ERR 5  /*XOSC failed to start     */
#define OP_ERR_BIT_PLL_LOCK_ERR 6    /*PLL failed to lock       */
#define OP_ERR_BIT_PA_RAMP_ERR 8     /*PA ramping failed        */


#define IRQ_BIT_TXDONE 0           /* Packet transmission completed All   */
#define IRQ_BIT_RXDONE 1           /* Packet received All */
#define IRQ_BIT_PREAMBLEDETECTED 2 /* Preamble detected All   */
#define IRQ_BIT_SYNCWORDVALID 3    /* Valid sync word detected FSK   */
#define IRQ_BIT_HEADERVALID 4      /* Valid LoRa header received LoRa®   */
#define IRQ_BIT_HEADERERR 5        /* LoRa header CRC error LoRa®  */
#define IRQ_BIT_CRCERR 6           /* Wrong CRC received All  */
#define IRQ_BIT_CADDONE 7          /* Channel activity detection finished LoRa® */
#define IRQ_BIT_CADDETECTED 8      /* Channel activity detected LoRa®   */
#define IRQ_BIT_TIMEOUT 9          /* Rx or Tx timeout All */

#define IQR_ALL_INT MASK_10BIT

typedef enum eRegulatorMode_t{
   REG_MODE_ONLY_LDO= 0x00,  /*used for all modes*/
   REG_MODE_DC_DC_LDO= 0x01, /*used for STBY_XOSC,FS, RX and TX modes*/
}RegulatorMode_t;

/*controlling the external RX switch*/
typedef enum eDio2Mode_t{
    DIO2_FREE=0,
    DIO2_RF_SW=1,
}Dio2Mode_t;

/*Rx Gain Configuration*/
typedef enum eRxGain_t{
    RXGAIN_POWER_SAVING = 0x94,
    RXGAIN_BOOSTED = 0x96,
    RXGAIN_UNDEF = 0x00,
}RxGain_t;

/*StdbyConfig Value Description*/
typedef enum eStandbyMode_t {
    STDBY_RC = 0,   /* Device running on RC13M, set STDBY_RC mode       */
    STDBY_XOSC = 1, /* Device running on XTAL 32MHz, set STDBY_XOSC mode*/
} StandbyMode_t;


/*Table 13-70: LoRa� PacketParam6 - InvertIQ*/
typedef enum eLoRaIQSetUp_t{
    IQ_SETUP_STANDARD=0x00,
    IQ_SETUP_INVERTED=0x01,
}LoRaIQSetUp_t;

/*Status Bytes Definition*/
typedef enum eCommandStatus_t{
   COM_STAT_DATA_AVAIL= 0x2,   /*Data is available to host*/
   COM_STAT_COM_TIMEOUT= 0x3,  /*Command timeout2 */
   COM_STAT_COM_PROC_ERR= 0x4, /*Command processing error3*/
   COM_STAT_EXE_ERR =0x5,      /*Failure to execute command4*/
   COM_STAT_COM_TX_DONE= 0x6,  /*Command TX done5 */
}CommandStatus_t;

/*!
 * Represents the interruption masks available for the radio
 *
 * Note that not all these interruptions are available for all packet types
 */
typedef enum eRadioIrqMasks_t {
    IRQ_MASK_RADIO_NONE = 0x0000,
    IRQ_MASK_TX_DONE = 0x0001,
    IRQ_MASK_RX_DONE = 0x0002,
    IRQ_MASK_PREAMBLE_DETECTED = 0x0004,
    IRQ_MASK_SYNCWORD_VALID = 0x0008,
    IRQ_MASK_HEADER_VALID = 0x0010,
    IRQ_MASK_HEADER_ERROR = 0x0020,
    IRQ_MASK_CRC_ERROR = 0x0040,
    IRQ_MASK_CAD_DONE = 0x0080,
    IRQ_MASK_CAD_ACTIVITY_DETECTED = 0x0100,
    IRQ_MASK_RX_TX_TIMEOUT = 0x0200,
    IRQ_MASK_RADIO_ALL = 0xFFFF
} RadioIrqMasks_t;

/*PacketType Definition*/
typedef enum eRadioPacketTypes_t {
    PACKET_TYPE_GFSK = 0x00, /*GFSK packet type*/
    PACKET_TYPE_LORA = 0x01, /*LORA mode*/
    PACKET_TYPE_NONE = 0x0F,
    PACKET_TYPE_UNDEF = 0xFF
} RadioPacketType_t;


typedef enum eOutputPower_t { OP_14_DBM = 0, OP_17_DBM = 1, OP_20_DBM = 2, OP_22_DBM = 3, OP_UNDEF = 4 } OutputPower_t;

typedef enum eChipMode_t {
    CHP_MODE_UNDEF = 0x0,
    CHP_MODE_STBY_RC = 0x2,
    CHP_MODE_STBY_XOSC = 0x3,
    CHP_MODE_FS = 0x4,
    CHP_MODE_RX = 0x5,
    CHP_MODE_TX = 0x6
} ChipMode_t;

#endif /* SX1262_CONSTANTS_H  */
