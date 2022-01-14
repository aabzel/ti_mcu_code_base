

#include <bcomdef.h>
#include <gapgattserver.h>
#include <gap_advertiser.h>
#include <gapbondmgr.h>
#include "ti_ble_config.h"




uint8_t attDeviceName[GAP_DEVICE_NAME_LEN]= "Project Zero";


uint8_t * pRandomAddress = NULL;

/*********************************************************************
 * Bond Manager Configuration
 */

uint8_t pairMode                =    GAPBOND_PAIRING_MODE_WAIT_FOR_REQ;
uint8_t mitm                    =    true;
uint8_t ioCap                   =    GAPBOND_IO_CAP_DISPLAY_ONLY;
uint8_t bonding                 =    true;
uint8_t secureConnection        =    GAPBOND_SECURE_CONNECTION_ALLOW;
uint8_t authenPairingOnly       =    false;
uint8_t autoSyncWL              =    false;
uint8_t eccReGenPolicy          =    0;
uint8_t KeySize                 =    16;
uint8_t removeLRUBond           =    false;
uint8_t KeyDistList             =    GAPBOND_KEYDIST_MENCKEY | GAPBOND_KEYDIST_MIDKEY | GAPBOND_KEYDIST_MSIGN | GAPBOND_KEYDIST_SENCKEY | GAPBOND_KEYDIST_SIDKEY | GAPBOND_KEYDIST_SSIGN;
uint8_t eccDebugKeys            =    false;
uint8_t allowDebugKeys          =    true;
uint8_t eraseBondWhileInConn    =    false;

void setBondManagerParameters()
{
    GAPBondMgr_SetParameter(GAPBOND_PAIRING_MODE, sizeof(uint8_t), &pairMode);
    GAPBondMgr_SetParameter(GAPBOND_MITM_PROTECTION, sizeof(uint8_t), &mitm);
    GAPBondMgr_SetParameter(GAPBOND_IO_CAPABILITIES, sizeof(uint8_t), &ioCap);
    GAPBondMgr_SetParameter(GAPBOND_BONDING_ENABLED, sizeof(uint8_t), &bonding);
    GAPBondMgr_SetParameter(GAPBOND_SECURE_CONNECTION, sizeof(uint8_t), &secureConnection);
    // Set Authenticated Pairing Only mode
    GAPBondMgr_SetParameter(GAPBOND_AUTHEN_PAIRING_ONLY, sizeof(uint8_t), &authenPairingOnly);
    // Set Auto Whitelist Sync
    GAPBondMgr_SetParameter(GAPBOND_AUTO_SYNC_WL, sizeof(uint8_t), &autoSyncWL);
    GAPBondMgr_SetParameter(GAPBOND_ECCKEY_REGEN_POLICY, sizeof(uint8_t), &eccReGenPolicy);
    GAPBondMgr_SetParameter(GAPBOND_KEYSIZE, sizeof(uint8_t), &KeySize);
    GAPBondMgr_SetParameter(GAPBOND_LRU_BOND_REPLACEMENT, sizeof(uint8_t), &removeLRUBond);
    GAPBondMgr_SetParameter(GAPBOND_KEY_DIST_LIST, sizeof(uint8_t), &KeyDistList);
    // Set Secure Connection Debug Keys
    GAPBondMgr_SetParameter(GAPBOND_SC_HOST_DEBUG, sizeof(uint8_t), &eccDebugKeys);
    // Set Allow Debug Keys
    GAPBondMgr_SetParameter(GAPBOND_ALLOW_DEBUG_KEYS, sizeof(uint8_t), &allowDebugKeys);
    // Set the Erase bond While in Active Connection Flag
    GAPBondMgr_SetParameter(GAPBOND_ERASE_BOND_IN_CONN, sizeof(uint8_t), &eraseBondWhileInConn);
}


GapAdv_params_t advParams1 = {
  .eventProps =   GAP_ADV_PROP_CONNECTABLE | GAP_ADV_PROP_LEGACY | GAP_ADV_PROP_SCANNABLE,
  .primIntMin =   160,
  .primIntMax =   160,
  .primChanMap =  GAP_ADV_CHAN_ALL,
  .peerAddrType = PEER_ADDRTYPE_PUBLIC_OR_PUBLIC_ID,
  .peerAddr =     { 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa },
  .filterPolicy = GAP_ADV_WL_POLICY_ANY_REQ,
  .txPower =      GAP_ADV_TX_POWER_NO_PREFERENCE,
  .primPhy =      GAP_ADV_PRIM_PHY_1_MBPS,
  .secPhy =       GAP_ADV_SEC_PHY_1_MBPS,
  .sid =          0
};

uint8_t advData1[] =
{
  0x0d,
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  'P',
  'r',
  'o',
  'j',
  'e',
  'c',
  't',
  ' ',
  'Z',
  'e',
  'r',
  'o',

  0x02,
  GAP_ADTYPE_FLAGS,
  GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED | GAP_ADTYPE_FLAGS_GENERAL,

};

uint8_t scanResData1[] =
{
  0x11,
  GAP_ADTYPE_128BIT_MORE,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0xb0,
  0x00,
  0x40,
  0x51,
  0x04,
  0x10,
  0x11,
  0x00,
  0xf0,


};

