/*
 *  RF Driver
 https://e2e.ti.com/support/wireless-connectivity/sub-1-ghz-group/sub-1-ghz/f/sub-1-ghz-forum/818133/launchxl-cc1310-non---blocking-cc1310-radio-receive
  https://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/tirtos/2_20_00_06/exports/tirtos_full_2_20_00_06/products/tidrivers_cc13xx_cc26xx_2_20_00_08/docs/doxygen/html/_r_f_8h.html
 https://dev.ti.com/tirex/explore/node?node=AJ1y3Qt11VwjrCSQsRfxXw__pTTHBmu__LATEST
 */
#include "rf_drv.h"

#include <PowerCC26XX.h>
#include <RFCC26X2.h>
#include <rf_prop_mailbox.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ti/devices/cc13x2_cc26x2/driverlib/cpu.h>
#include <ti/devices/cc13x2_cc26x2/driverlib/ioc.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/rf/RF.h>

#include "RFQueue.h"
#include "debug_info.h"
#include "ti_radio_config.h"
#include "timer_utils.h"
/*
 * Platform-specific driver configuration
 */
const RFCC26XX_HWAttrsV2 RFCC26XX_hwAttrs = {.hwiPriority = (~0),
                                             .swiPriority = (uint8_t)0,
                                             .xoscHfAlwaysNeeded = true,
                                             .globalCallback = NULL,
                                             .globalEventMask = 0};

RF_Object rfObject;
RF_Handle rfHandle;
RF_Params rfParams;

#pragma DATA_ALIGN(rxDataEntryBuffer, 4);

static uint8_t rxDataEntryBuffer[RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(NUM_DATA_ENTRIES, PAYLOAD_LENGTH, NUM_APPENDED_BYTES)];

static uint8_t rf_tx_packet[PAYLOAD_LENGTH];
/* The length byte is stored in a separate variable */
static uint8_t rf_rx_packet[PAYLOAD_LENGTH + NUM_APPENDED_BYTES - 1];

uint32_t rf_rx_cnt = 0;
uint32_t rf_rx_crc_err_cnt = 0;
bool rf_proc_rx = false;
/* Receive dataQueue for RF Core to fill in data */
static dataQueue_t dataQueue;
static rfc_dataEntryGeneral_t* currentDataEntry;
uint8_t rf_rx_len;
static uint8_t* packetDataPointer;

void rx_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask event_mask) {
    if(RF_EventRxEntryDone == (RF_EventRxEntryDone & event_mask)) {
        /* Toggle pin to indicate RX */
        rf_rx_cnt++;

        /* Get current unhandled data entry */
        currentDataEntry = RFQueue_getDataEntry();

        /* Handle the packet data, located at &currentDataEntry->data:
         * - Length is the first byte with the current configuration
         * - Data starts from the second byte */
        rf_rx_len = *(uint8_t*)(&currentDataEntry->data);
        packetDataPointer = (uint8_t*)(&currentDataEntry->data + 1);

        /* Copy the payload + the status byte to the packet variable */
        memcpy(rf_rx_packet, packetDataPointer, (rf_rx_len + 1));

        RFQueue_nextEntry();
    }
    if(RF_EventLastCmdDone == (RF_EventLastCmdDone & event_mask)) {
        rf_rx_cnt++;
        /* Get current unhandled data entry */
        currentDataEntry = RFQueue_getDataEntry();

        /* Handle the packet data, located at &currentDataEntry->data:
         * - Length is the first byte with the current configuration
         * - Data starts from the second byte */
        rf_rx_len = *(uint8_t*)(&currentDataEntry->data);
        packetDataPointer = (uint8_t*)(&currentDataEntry->data + 1);

        /* Copy the payload + the status byte to the packet variable */
        memcpy(rf_rx_packet, packetDataPointer, (rf_rx_len + 1));

        RFQueue_nextEntry();
    }
    if(RF_EventTxCtrlAck == (RF_EventTxCtrlAck & event_mask)) {
        rf_rx_cnt++;
    }
    if(RF_EventRxOk == (RF_EventRxOk & event_mask)) {
        rf_rx_cnt++;
    }
    if(RF_EventRxNOk == (RF_EventRxNOk & event_mask)) {
        rf_rx_crc_err_cnt++;
    }
}

bool rf_read(uint8_t* out_array, uint16_t size_arr, uint16_t* out_arr_len) {
    bool res = false;
    if((NULL != out_array) && (NULL != out_arr_len) && (rf_rx_len <= size_arr)) {
        memcpy(out_array, rf_rx_packet, rf_rx_len);
        *out_arr_len = rf_rx_len;
        res = true;
    }
    return res;
}

bool rf_tune_blocking_reception(void) {
    bool res = false;
    rf_proc_rx = false;
    rf_rx_len = 0;
    memset(rf_rx_packet, 0x00, sizeof(rf_rx_packet));

    /* Modify CMD_PROP_RX command for application needs */
    /* Set the Data Entity queue for received data */
    RF_cmdPropRx.pQueue = &dataQueue;
    /* Discard ignored packets from Rx queue */
    RF_cmdPropRx.rxConf.bAutoFlushIgnored = 1;
    /* Discard packets with CRC error from Rx queue */
    RF_cmdPropRx.rxConf.bAutoFlushCrcErr = 1;
    /* Implement packet length filtering to avoid PROP_ERROR_RXBUF */
    RF_cmdPropRx.maxPktLen = PAYLOAD_LENGTH;
    RF_cmdPropRx.pktConf.bRepeatOk = 1;
    RF_cmdPropRx.pktConf.bRepeatNok = 1;

    RF_EventMask terminationReason =
        RF_runCmd(rfHandle, (RF_Op*)&RF_cmdPropRx, RF_PriorityNormal, &rx_callback, RF_EventRxEntryDone);
    switch(terminationReason) {
    case RF_EventLastCmdDone:
        // A stand-alone radio operation command or the last radio
        // operation command in a chain finished.
        res = true;
        break;
    case RF_EventCmdCancelled:
        // Command cancelled before it was started; it can be caused
        // by RF_cancelCmd() or RF_flushCmd().
        break;
    case RF_EventCmdAborted:
        // Abrupt command termination caused by RF_cancelCmd() or
        // RF_flushCmd().
        break;
    case RF_EventCmdStopped:
        // Graceful command termination caused by RF_cancelCmd() or
        // RF_flushCmd().
        break;
    default:
        // Uncaught error event
        break;
    }

    uint32_t cmdStatus = ((volatile RF_Op*)&RF_cmdPropRx)->status;
    switch(cmdStatus) {
    case PROP_DONE_OK:
        // Packet received with CRC OK
        res = true;
        break;
    case PROP_DONE_RXERR:
        // Packet received with CRC error
        break;
    case PROP_DONE_RXTIMEOUT:
        // Observed end trigger while in sync search
        break;
    case PROP_DONE_BREAK:
        // Observed end trigger while receiving packet when the command is
        // configured with endType set to 1
        break;
    case PROP_DONE_ENDED:
        // Received packet after having observed the end trigger; if the
        // command is configured with endType set to 0, the end trigger
        // will not terminate an ongoing reception
        break;
    case PROP_DONE_STOPPED:
        // received CMD_STOP after command started and, if sync found,
        // packet is received
        break;
    case PROP_DONE_ABORT:
        // Received CMD_ABORT after command started
        break;
    case PROP_ERROR_RXBUF:
        // No RX buffer large enough for the received data available at
        // the start of a packet
        break;
    case PROP_ERROR_RXFULL:
        // Out of RX buffer space during reception in a partial read
        break;
    case PROP_ERROR_PAR:
        // Observed illegal parameter
        break;
    case PROP_ERROR_NO_SETUP:
        // Command sent without setting up the radio in a supported
        // mode using CMD_PROP_RADIO_SETUP or CMD_RADIO_SETUP
        break;
    case PROP_ERROR_NO_FS:
        // Command sent without the synthesizer being programmed
        break;
    case PROP_ERROR_RXOVF:
        // RX overflow observed during operation
        break;
    default:
        // Uncaught error event - these could come from the
        // pool of states defined in rf_mailbox.h
        break;
    }
    return res;
}

bool rf_tune_reception(void) {
    bool res = false;
    rf_rx_len = 0;
    memset(rf_rx_packet, 0x00, sizeof(rf_rx_packet));
    res = true;
    rf_proc_rx = false;
    /* Modify CMD_PROP_RX command for application needs */
    /* Set the Data Entity queue for received data */
    RF_cmdPropRx.pQueue = &dataQueue;
    /* Discard ignored packets from Rx queue */
    RF_cmdPropRx.rxConf.bAutoFlushIgnored = 1;
    /* Discard packets with CRC error from Rx queue */
    RF_cmdPropRx.rxConf.bAutoFlushCrcErr = 1;
    /* Implement packet length filtering to avoid PROP_ERROR_RXBUF */
    RF_cmdPropRx.maxPktLen = PAYLOAD_LENGTH;
    RF_cmdPropRx.pktConf.bRepeatOk = 1;
    RF_cmdPropRx.pktConf.bRepeatNok = 1;

    RF_cmdPropRx.startTrigger.triggerType = TRIG_NOW;
    RF_cmdPropRx.endTrigger.triggerType = TRIG_NEVER;

    // RF_cmdPropRx.endTime = 100;
    RF_postCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

    /* Enter RX mode and stay forever in RX */
    RF_EventMask terminationReason;
    /*program will hang on here*/
    terminationReason =
        RF_postCmd(rfHandle, (RF_Op*)&RF_cmdPropRx, RF_PriorityNormal, &rx_callback, RF_EventRxEntryDone);
    if(RF_EventLastCmdDone == (RF_EventLastCmdDone & terminationReason)) {
        res = true;
    }
#if 0
    switch(terminationReason) {
    case RF_EventLastCmdDone:
        // A stand-alone radio operation command or the last radio
        // operation command in a chain finished.
        break;
    case RF_EventCmdCancelled:
        // Command cancelled before it was started; it can be caused
        // by RF_cancelCmd() or RF_flushCmd().
        break;
    case RF_EventCmdAborted:
        // Abrupt command termination caused by RF_cancelCmd() or
        // RF_flushCmd().
        break;
    case RF_EventCmdStopped:
        // Graceful command termination caused by RF_cancelCmd() or
        // RF_flushCmd().
        break;
    default:
        // Uncaught error event
        res = false;
    }
#endif

    uint32_t cmdStatus = ((volatile RF_Op*)&RF_cmdPropRx)->status;
    switch(cmdStatus) {
    case PROP_DONE_OK:
        res = true;
        // Packet received with CRC OK
        break;
    case PROP_DONE_RXERR:
        // Packet received with CRC error
        break;
    case PROP_DONE_RXTIMEOUT:
        // Observed end trigger while in sync search
        break;
    case PROP_DONE_BREAK:
        // Observed end trigger while receiving packet when the command is
        // configured with endType set to 1
        break;
    case PROP_DONE_ENDED:
        // Received packet after having observed the end trigger; if the
        // command is configured with endType set to 0, the end trigger
        // will not terminate an ongoing reception
        break;
    case PROP_DONE_STOPPED:
        // received CMD_STOP after command started and, if sync found,
        // packet is received
        break;
    case PROP_DONE_ABORT:
        // Received CMD_ABORT after command started
        break;
    case PROP_ERROR_RXBUF:
        // No RX buffer large enough for the received data available at
        // the start of a packet
        break;
    case PROP_ERROR_RXFULL:
        // Out of RX buffer space during reception in a partial read
        break;
    case PROP_ERROR_PAR:
        // Observed illegal parameter
        break;
    case PROP_ERROR_NO_SETUP:
        // Command sent without setting up the radio in a supported
        // mode using CMD_PROP_RADIO_SETUP or CMD_RADIO_SETUP
        break;
    case PROP_ERROR_NO_FS:
        // Command sent without the synthesizer being programmed
        break;
    case PROP_ERROR_RXOVF:
        // RX overflow observed during operation
        break;
    default:
        // Uncaught error event - these could come from the
        // pool of states defined in rf_mailbox.h
        //  res = false;
        break;
    }

    return res;
}

bool rf_rx_init(void) {
    bool res = false;
    rf_rx_cnt = 0;
    uint8_t ret = RFQueue_defineQueue(&dataQueue, rxDataEntryBuffer, sizeof(rxDataEntryBuffer), NUM_DATA_ENTRIES,
                                      PAYLOAD_LENGTH + NUM_APPENDED_BYTES);

    if(ret) {
        /* Failed to allocate space for all data entries */
        res = false;
    } else {
        res = true;
        res = rf_tune_reception();
    }
    return res;
}

bool rf_init(void) {
    bool res = false;
    RF_Params_init(&rfParams);

    //  rfParams.nInactivityTimeout = SEC_2_US(5);
    // rfParams.nID = 123;

    rfHandle = RF_open(&rfObject, &RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &rfParams);
    if(rfHandle) {
        res = true;
        /* Set the frequency */
        RF_postCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);
        res = rf_rx_init() && res;
    } else {
        res = false;
    }
    return res;
}

bool rf_write(uint8_t* array, uint16_t arr_len) {
    bool res = false;
    /* Send packet */
    if(arr_len < PAYLOAD_LENGTH) {

        memcpy(rf_tx_packet, array, arr_len);
        RF_cmdPropTx.pktLen = arr_len;
        RF_cmdPropTx.pPkt = rf_tx_packet;
        RF_cmdPropTx.startTrigger.triggerType = TRIG_NOW;

        RF_EventMask terminationReason = RF_runCmd(rfHandle, (RF_Op*)&RF_cmdPropTx, RF_PriorityNormal, NULL, 0);
        switch(terminationReason) {
        case RF_EventLastCmdDone:
            res = true;
            // A stand-alone radio operation command or the last radio
            // operation command in a chain finished.
            break;
        case RF_EventCmdCancelled:
            res = false;
            // Command cancelled before it was started; it can be caused
            // by RF_cancelCmd() or RF_flushCmd().
            break;
        case RF_EventCmdAborted:
            res = false;
            // Abrupt command termination caused by RF_cancelCmd() or
            // RF_flushCmd().
            break;
        case RF_EventCmdStopped:
            res = false;
            // Graceful command termination caused by RF_cancelCmd() or
            // RF_flushCmd().
            break;
        default:
            // Uncaught error event
            res = false;
        }

        uint32_t cmdStatus = ((volatile RF_Op*)&RF_cmdPropTx)->status;
        switch(cmdStatus) {
        case PROP_DONE_OK:
            res = true;
            // Packet transmitted successfully
            break;
        case PROP_DONE_STOPPED:
            // received CMD_STOP while transmitting packet and finished
            // transmitting packet
            break;
        case PROP_DONE_ABORT:
            // Received CMD_ABORT while transmitting packet
            break;
        case PROP_ERROR_PAR:
            // Observed illegal parameter
            break;
        case PROP_ERROR_NO_SETUP:
            // Command sent without setting up the radio in a supported
            // mode using CMD_PROP_RADIO_SETUP or CMD_RADIO_SETUP
            break;
        case PROP_ERROR_NO_FS:
            // Command sent without the synthesizer being programmed
            break;
        case PROP_ERROR_TXUNF:
            // TX underflow observed during operation
            break;
        default:
            // Uncaught error event - these could come from the
            // pool of states defined in rf_mailbox.h
            res = false;
        }
        /* Power down the radio */
        RF_yield(rfHandle);
    } else {
        res = false;
    }

    return res;
}

bool rf_process(void) {
    bool res = false;
    if((false == rf_proc_rx) && (0 < rf_rx_len)) {
        print_mem(rf_rx_packet, rf_rx_len, true);
        res = true;
        rf_proc_rx = true;
    }
    return res;
}
