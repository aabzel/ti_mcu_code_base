/*
 *  Configured BLE module definitions
 */

#ifndef TI_BLE_CONFIG_H
#define TI_BLE_CONFIG_H

#include <bcomdef.h>
#include <gapgattserver.h>
#include <gap_advertiser.h>
#include <gapbondmgr.h>
#include <ti_radio_config.h>

#define RF_FE_MODE_AND_BIAS            RF_FE_DIFFERENTIAL | RF_FE_INT_BIAS

// Default Tx Power Index
#define DEFAULT_TX_POWER               HCI_EXT_TX_POWER_0_DBM

extern uint8_t attDeviceName[GAP_DEVICE_NAME_LEN];

//Random Address
extern uint8_t * pRandomAddress;

// Address mode of the local device
// Note: When using the DEFAULT_ADDRESS_MODE as ADDRMODE_RANDOM or 
// ADDRMODE_RP_WITH_RANDOM_ID, GAP_DeviceInit() should be called with 
// it's last parameter set to a static random address
#define DEFAULT_ADDRESS_MODE                  ADDRMODE_PUBLIC

// How often to read current RPA (in ms)
#define READ_RPA_PERIOD                       3000

// Maximum number of BLE connections. It should be set based on the
// device GAP role. Here're some recommended values:
//      * Central:     32
//      * Peripheral:  16
//      * Observer:    0
//      * Broadcaster: 0
// Note: When the GAP role includes Peripheral and no v4.1 Controller features
//       are configured, MAX_NUM_BLE_CONNS must not be greater than 1
#define MAX_NUM_BLE_CONNS                      2

// Maximum number of BLE HCI PDUs. If the maximum number connections (above)
// is set to 0 then this number should also be set to 0.
#define MAX_NUM_PDU                   		    5

// Maximum size in bytes of the BLE HCI PDU. Valid range: 27 to 255
// The maximum ATT_MTU is MAX_PDU_SIZE - 4.
#define MAX_PDU_SIZE                  		    251

/*********************************************************************
 * Bond Manager Configuration
 */

#define GAP_BONDINGS_MAX                      10
#define GAP_CHAR_CFG_MAX                      4

extern uint8_t pairMode;
extern uint8_t mitm;
extern uint8_t ioCap;
extern uint8_t bonding;
extern uint8_t secureConnection;
extern uint8_t authenPairingOnly;
extern uint8_t autoSyncWL;
extern uint8_t eccReGenPolicy;
extern uint8_t KeySize;
extern uint8_t removeLRUBond;
extern uint8_t KeyDistList;
extern uint8_t eccDebugKeys;
extern uint8_t allowDebugKeys;
extern uint8_t eraseBondWhileInConn;

extern void setBondManagerParameters();



// Pass parameter updates to the app for it to decide.
#define DEFAULT_PARAM_UPDATE_REQ_DECISION       GAP_UPDATE_REQ_PASS_TO_APP

// Pass parameter updates to the app for it to decide.
#define DEFAULT_SEND_PARAM_UPDATE_REQ

// Delay (in ms) after connection establishment before sending a parameter update requst
#define SEND_PARAM_UPDATE_DELAY                 6000

// Minimum connection interval (units of 1.25ms) if automatic parameter update
// request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL      12

// Maximum connection interval (units of 1.25ms) if automatic parameter update
// request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL      36

// Slave latency to use if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SLAVE_LATENCY          0

// Supervision timeout value (units of 10ms) if automatic parameter update
// request is enabled
#define DEFAULT_DESIRED_CONN_TIMEOUT           200



// Advertisement Set Number 1
extern GapAdv_params_t advParams1;
extern uint8_t advData1[17];
extern uint8_t scanResData1[18];

#endif /* BLE_CONFIG_H */
