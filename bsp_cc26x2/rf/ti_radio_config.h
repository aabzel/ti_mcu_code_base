/*
 *  ======== ti_radio_config.h ========
 *  Configured RadioConfig module definitions
 *
 *  DO NOT EDIT - This file is generated for the CC2652R1FRGZ
 *  by the SysConfig tool.
 *
 *  Radio Config module version : 1.10
 *  SmartRF Studio data version : 2.22.0
 */
#ifndef _TI_RADIO_CONFIG_H_
#define _TI_RADIO_CONFIG_H_

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_mailbox.h)
#include DeviceFamily_constructPath(driverlib/rf_common_cmd.h)
#include DeviceFamily_constructPath(driverlib/rf_prop_cmd.h)
#include <ti/drivers/rf/RF.h>

/* SmartRF Studio version that the RF data is fetched from */
#define SMARTRF_STUDIO_VERSION "2.22.0"

// *********************************************************************************
//   RF Frontend configuration
// *********************************************************************************
// RF design based on: LAUNCHXL-CC26X2R1 (CC2642EM-7ID)
#define LAUNCHXL_CC26X2R1

// RF frontend configuration
#define FRONTEND_24G_DIFF_RF
#define FRONTEND_24G_INT_BIAS

// Supported frequency bands
#define SUPPORT_FREQBAND_2400

// TX power table size definitions
#define TXPOWERTABLE_2400_PA5_SIZE 16 // 2400 MHz, 5 dBm

// TX power tables
extern RF_TxPowerTable_Entry txPowerTable_2400_pa5[]; // 2400 MHz, 5 dBm



//*********************************************************************************
//  RF Setting:   100 kbps, 50 kHz Deviation, 2-GFSK, 311 kHz RX Bandwidth (Release Candidate)
//
//  PHY:          2gfsk100kbps
//  Setting file: setting_tc900.json
//*********************************************************************************

// PA table usage
#define TX_POWER_TABLE_SIZE TXPOWERTABLE_2400_PA5_SIZE

#define txPowerTable txPowerTable_2400_pa5

// TI-RTOS RF Mode object
extern RF_Mode RF_prop;

// RF Core API commands
extern rfc_CMD_PROP_RADIO_DIV_SETUP_t RF_cmdPropRadioDivSetup;
extern rfc_CMD_FS_t RF_cmdFs;
extern rfc_CMD_PROP_TX_t RF_cmdPropTx;
extern rfc_CMD_PROP_RX_t RF_cmdPropRx;

// RF Core API overrides
extern uint32_t pOverrides[];

#endif // _TI_RADIO_CONFIG_H_
